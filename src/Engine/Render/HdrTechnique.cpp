#include <BlackBox/Render/HdrTechnique.hpp>
#include <BlackBox/Render/ShadowMapTechnique.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Scene.hpp>
#include <BlackBox/Render/SkyBox.hpp>
#include <BlackBox/IConsole.hpp>

HdrTechnique::HdrTechnique()
{
}

HdrTechnique::~HdrTechnique()
{
}

bool HdrTechnique::Init(Scene* scene, FrameBufferObject* renderTarget)
{
  if (inited)
    return true;
  render = GetIEngine()->getIRender();
  m_Scene = scene;
  exposure = GetIEngine()->getIConsole()->CreateVariable("exp", 1.0f, 0, "exposure");
  enabled = GetIEngine()->getIConsole()->CreateVariable("hdr", 1, 0, "Enable/disable HDR");
  bloom = GetIEngine()->getIConsole()->CreateVariable("bloom", 1, 0, "Enable/disable HDR");
  bloomThreshold = GetIEngine()->getIConsole()->CreateVariable("bt", 2.0f, 0, "Bloom threshold");
  createShader();
  shadowMapping = new ShadowMapping();
  hdrBuffer =  FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, GetIEngine()->getIRender()->GetWidth(), GetIEngine()->getIRender()->GetHeight(), 2);
  pingPongBuffer[0] =  FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, GetIEngine()->getIRender()->GetWidth(), GetIEngine()->getIRender()->GetHeight(), 2);
  pingPongBuffer[1] =  FrameBufferObject::create(FrameBufferObject::HDR_BUFFER, GetIEngine()->getIRender()->GetWidth(), GetIEngine()->getIRender()->GetHeight(), 2);

  inited = true;
  return shadowMapping->Init(scene, hdrBuffer);
}

bool HdrTechnique::OnRenderPass(int pass)
{
  if (!shadowMapping->OnRenderPass(pass))
  {
		if (m_Scene->GetSkyBox() != nullptr)
			;// m_Scene->GetSkyBox()->draw(m_Scene->getCurrentCamera());
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
		BloomPass();
	}
  else
    m_Scene->setPostProcessor(nullptr);

  return false;
}

void HdrTechnique::BloomPass()
{
	// 2. blur bright fragments with two-pass Gaussian Blur 
				 // --------------------------------------------------
	bool horizontal = true, first_iteration = true;
	unsigned int amount = 10;
	m_BlurShader->use();
	glCheck(glDisable(GL_DEPTH_TEST));
	glActiveTexture(GL_TEXTURE0);
	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pingPongBuffer[horizontal]->id);
		m_BlurShader->setUniformValue(horizontal, "horizontal");
		glBindTexture(GL_TEXTURE_2D, first_iteration ? hdrBuffer->texture[1] : pingPongBuffer[!horizontal]->texture[0]);  // bind texture of other framebuffer (or scene if first iteration)
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

  m_BlurShader =new CShaderProgram(
    CShader::load("res/shaders/screenshader.vs", CShader::E_VERTEX), 
    CShader::load("res/shaders/blur.frag", CShader::E_FRAGMENT)
  );
	m_BlurShader->create();
	m_BlurShader->use();
	m_BlurShader->setUniformValue(0,"image");
	m_BlurShader->unuse();


}

bool HdrTechnique::PreRenderPass()
{
  return false;
}

void HdrTechnique::PostRenderPass()
{
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
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrBuffer->texture[0]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, pingPongBuffer[pingpong]->texture[0]);
	m_ScreenShader->setUniformValue(bloom->GetIVal(), "bloom");
	//shaderBloomFinal.setFloat("exposure", exposure);

	glCheck(glViewport(0, 0, GetIEngine()->getIRender()->GetWidth(), GetIEngine()->getIRender()->GetHeight()));
	m_ScreenQuad.draw();
}
