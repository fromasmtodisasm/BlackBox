#include <BlackBox/Render/HdrTechnique.hpp>
#include <BlackBox/Render/ShadowMapTechnique.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Scene.hpp>
#include <BlackBox/Render/SkyBox.hpp>
#include <BlackBox/IConsole.hpp>
#include <BlackBox/Profiler/Profiler.h>

#define NBLOOM
#define PREVIOS 0
#define CURRENT 1

HdrTechnique::HdrTechnique()
{
}

HdrTechnique::~HdrTechnique()
{
}

bool HdrTechnique::Init(Scene* pScene, FrameBufferObject* renderTarget)
{
  if (inited)
    return true;
  render = GetIEngine()->getIRender();
  m_Scene = pScene;
  exposure = GetIEngine()->getIConsole()->CreateVariable("exp", 1.0f, 0, "exposure");
  enabled = GetIEngine()->getIConsole()->CreateVariable("hdr", 1, 0, "Enable/disable HDR");
  bloom = GetIEngine()->getIConsole()->CreateVariable("bloom", 1, 0, "Enable/disable HDR");
  bloomThreshold = GetIEngine()->getIConsole()->CreateVariable("bt", 2.0f, 0, "Bloom threshold");
  blurOn = GetIEngine()->getIConsole()->CreateVariable("blur", 1, 0, "Enable/disable blur for bloom");
  bloom_exposure = GetIEngine()->getIConsole()->CreateVariable("bexp", 0.007f, 0, "Enable/disable blur for bloom");
  offset = GetIEngine()->getIConsole()->CreateVariable("offset", -3.0f, 0, "Enable/disable blur for bloom");
  useBoxFilter = GetIEngine()->getIConsole()->CreateVariable("bf", 0, 0, "Enable/disable BoxFilter in bloom");
  defaultFilter = GetIEngine()->getIConsole()->CreateVariable("df", 1, 0, "Enable/disable default filtering in bloom");
  createShader();
  shadowMapping = new ShadowMapping();
	float m = 1;
	glm::vec2 size_m2 = glm::vec2(GetIEngine()->getIRender()->GetWidth()*m, GetIEngine()->getIRender()->GetHeight()*m) / 1.f;
  hdrBuffer =  FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, size_m2.x, size_m2.y, 2, false);
  //pingPongBuffer[0] =  FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, size_m2.x, size_m2.y, 1, false);
  //pingPongBuffer[1] =  FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, size_m2.x, size_m2.y, 1, false);

	int mip_cnt = std::log2(std::max(size_m2.x, size_m2.y)) + 1;
	pass0.resize(mip_cnt);
	pass1.resize(mip_cnt);
	for (int i = 0, width = size_m2.x, height = size_m2.y; i < mip_cnt; i++)
	{
		pass0[i] = FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, width, height, 1, false);
		width >>= 1;
		if (width <= 0) 
			width = 1;
		height >>= 1;
		if (height <= 0) 
			height = 1;
	}
	for (int i = 0, width = size_m2.x, height = size_m2.y; i < mip_cnt; i++)
	{
		pass1[i] = FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, width, height, 1, false);
		width >>= 1;
		if (width <= 0) 
			width = 1;
		height >>= 1;
		if (height <= 0) 
			height = 1;
	}

  inited = true;
  return shadowMapping->Init(pScene, hdrBuffer);
}

bool HdrTechnique::OnRenderPass(int pass)
{
  if (!shadowMapping->OnRenderPass(pass))
  {
		if (m_Scene->GetSkyBox() != nullptr)
			;// m_Scene->GetSkyBox()->draw(m_Scene->getCurrentCamera());
		{
			Object *water;
			if ((water = m_Scene->getObject("water")) != nullptr)
			{
				water->m_transparent = true;
				reinterpret_cast<ShadowMapping*>(shadowMapping)->RenderTransparent(water);

			}
		}
    return HdrPass();
  }
  return true;
}

int HdrTechnique::GetFrame()
{
  return shadowMapping->GetFrame();
}

bool HdrTechnique::HdrPass()
{
	if (enabled->GetIVal())
	{
    m_Scene->setPostProcessor(this);
		PROFILER_PUSH_GPU_MARKER("BLOOM PASS", Utils::COLOR_DARK_GREEN);
		BloomPass();
		PROFILER_POP_GPU_MARKER();
	}
  else
    m_Scene->setPostProcessor(nullptr);

  return false;
}

void HdrTechnique::BloomPass()
{
	PROFILER_PUSH_CPU_MARKER("DOWNSAMPLING", Utils::COLOR_BLACK);
	downsampling();
	PROFILER_POP_CPU_MARKER();
	PROFILER_PUSH_CPU_MARKER("UPSAMPLING", Utils::COLOR_RED);
	upsampling();
	PROFILER_POP_CPU_MARKER();
}

void HdrTechnique::createShader()
{
  m_ScreenShader =new CShaderProgram(
    CShader::load("res/shaders/screenshader.vs", CShader::E_VERTEX), 
    CShader::load("res/shaders/hdrshader.frag", CShader::E_FRAGMENT)
  );
	m_ScreenShader->create();
	m_ScreenShader->use();
	m_ScreenShader->setUniformValue(0,"scene");
	m_ScreenShader->setUniformValue(1,"bloomBlur");
	m_ScreenShader->unuse();

  m_DownsampleShader =new CShaderProgram(
    CShader::load("res/shaders/screenshader.vs", CShader::E_VERTEX), 
    CShader::load("res/shaders/downsampling.frag", CShader::E_FRAGMENT)
  );
	m_DownsampleShader->create();
	m_DownsampleShader->use();
	m_DownsampleShader->setUniformValue(0,"image");
	m_DownsampleShader->unuse();

  m_UpsampleShader =new CShaderProgram(
    CShader::load("res/shaders/screenshader.vs", CShader::E_VERTEX), 
    CShader::load("res/shaders/upsampling.frag", CShader::E_FRAGMENT)
  );
	m_UpsampleShader->create();

}

bool HdrTechnique::PreRenderPass()
{
  return false;
}

void HdrTechnique::PostRenderPass()
{
}

void HdrTechnique::downsampling()
{
	// 2. blur bright fragments with two-pass Gaussian Blur 
				 // --------------------------------------------------
	bool horizontal = true, first_iteration = true;
	unsigned int amount = PASSES;
	m_DownsampleShader->use();
  m_DownsampleShader->setUniformValue(useBoxFilter->GetIVal(), "use_box_filter");
  m_DownsampleShader->setUniformValue(defaultFilter->GetIVal(), "default_filter");
  m_DownsampleShader->setUniformValue(offset->GetFVal(), "offset");
	glCheck(glDisable(GL_DEPTH_TEST));
	glActiveTexture(GL_TEXTURE0);
	if (useBoxFilter->GetIVal())
		amount = 1;
	else
		amount = std::log2(std::max(pass0[0]->viewPort.z, pass0[0]->viewPort.w)) + 1;
	for (unsigned int i = 0; i < amount - 1; i++)
	{
		pass0[i + 1]->bind();
		//m_DownsampleShader->setUniformValue(horizontal, "horizontal");
		glBindTexture(GL_TEXTURE_2D, first_iteration ? hdrBuffer->texture[1] : pass0[i]->texture[0]);  // bind texture of other framebuffer (or scene if first iteration)
		//renderQuad();
		m_ScreenQuad.draw();
		horizontal = !horizontal;
		if (first_iteration)
			first_iteration = false;
	}
	pingpong = !horizontal;
	glCheck(glEnable(GL_DEPTH_TEST));
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HdrTechnique::upsampling()
{
	m_UpsampleShader->use();
	m_UpsampleShader->setUniformValue(blurOn->GetIVal(), "blurOn");
	
	uint32_t amount;
	bool first_iteration = true;
	glCheck(glDisable(GL_DEPTH_TEST));

	amount = std::log2(std::max(pass0[0]->viewPort.z, pass0[0]->viewPort.w)) + 1;
	for (unsigned int i = amount - 1; i > 0; i--)
	{
		pass1[i - 1]->bind();
		m_UpsampleShader->bindTexture2D(first_iteration ? pass0[amount - 1]->texture[0] : pass1[i]->texture[0], PREVIOS, "previos");
		m_UpsampleShader->bindTexture2D(first_iteration ? pass0[amount - 1]->texture[0] : pass0[i - 1]->texture[0], CURRENT, "current");
		m_ScreenQuad.draw();
		if (first_iteration)
			first_iteration = false;
	}

}

void HdrTechnique::Do(unsigned int texture)
{
	glm::vec3 fog = glm::vec3(
		GetIEngine()->getIConsole()->GetCVar("fogR")->GetFVal(),
		GetIEngine()->getIConsole()->GetCVar("fogG")->GetFVal(),
		GetIEngine()->getIConsole()->GetCVar("fogB")->GetFVal());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(fog.r, fog.g, fog.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ScreenShader->use();
  m_ScreenShader->setUniformValue(exposure->GetFVal(), "exposure");
  m_ScreenShader->setUniformValue(bloom_exposure->GetFVal(), "bloom_exposure");
	glDisable(GL_DEPTH_TEST);

	m_ScreenShader->bindTexture2D(hdrBuffer->texture[0], 0, "scene");
	m_ScreenShader->bindTexture2D(pass1[0]->texture[0], 1, "bloomBlur");
	m_ScreenShader->setUniformValue(bloom->GetIVal(), "bloom");

	glCheck(glViewport(0, 0, GetIEngine()->getIRender()->GetWidth(), GetIEngine()->getIRender()->GetHeight()));
	m_ScreenQuad.draw();
}
