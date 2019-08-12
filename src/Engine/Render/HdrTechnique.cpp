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
  m_Scene = scene;
  exposure = GetIEngine()->getIConsole()->CreateVariable("exp", 1.0f, 0, "exposure");
  enabled = GetIEngine()->getIConsole()->CreateVariable("hdr", 1, 0, "Enable/disable HDR");
  createShader();
  shadowMapping = new ShadowMapping();
  hdrBuffer = new FrameBufferObject(FrameBufferObject::HDR_BUFFER, GetIEngine()->getIRender()->GetWidth(), GetIEngine()->getIRender()->GetHeight());
  hdrBuffer->create();

  inited = true;
  return shadowMapping->Init(scene, hdrBuffer);
}

bool HdrTechnique::OnRenderPass(int pass)
{
  if (!shadowMapping->OnRenderPass(pass))
  {
    if (m_Scene->GetSkyBox() != nullptr)
      m_Scene->GetSkyBox()->draw(m_Scene->getCurrentCamera());
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
    m_Scene->setPostProcessor(this);
  else
    m_Scene->setPostProcessor(nullptr);

  return false;
}

void HdrTechnique::createShader()
{
  m_ScreenShader =new CShaderProgram(
    CShader::load("res/shaders/screenshader.vs", CShader::E_VERTEX), 
    CShader::load("res/shaders/hdrshader.frag", CShader::E_FRAGMENT)
  );
	m_ScreenShader->create();
	m_ScreenShader->use();
	m_ScreenShader->setUniformValue(0,"screenTexture");
	m_ScreenShader->unuse();
}

void HdrTechnique::Do(unsigned int texture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ScreenShader->use();
  m_ScreenShader->setUniformValue(exposure->GetFVal(), "exposure");
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_ScreenQuad.draw();
}
