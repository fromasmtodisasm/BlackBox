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
  m_Scene = scene;
  exposure = GetIEngine()->getIConsole()->CreateVariable("exp", 1.0f, 0, "exposure");
  createShader();
  shadowMapping = new ShadowMapping();
  hdrBuffer = new FrameBufferObject(FrameBufferObject::HDR_BUFFER, GetIEngine()->getIRender()->GetWidth(), GetIEngine()->getIRender()->GetHeight());
  hdrBuffer->create();

  return shadowMapping->Init(scene, hdrBuffer);
}

bool HdrTechnique::OnRenderPass(int pass)
{
  if (!shadowMapping->OnRenderPass(pass))
  {
    if (m_Scene->GetSkyBox() != nullptr)
      m_Scene->GetSkyBox()->draw(m_Scene->getCamera());
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
  auto render = GetIEngine()->getIRender();
  float
    width = render->GetWidth(),
    height = render->GetHeight();
  glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
  render->SetViewport(0, 0, width, height);
  glCheck(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
  glCheck(glClear(GL_COLOR_BUFFER_BIT));
  m_ScreenShader->use();
  m_ScreenShader->setUniformValue(exposure->GetFVal(), "exposure");
  auto proj = glm::ortho(0.0f, (float)render->GetWidth(), 0.0f, (float)render->GetHeight());
  auto transform = glm::scale(proj, glm::vec3(width, height, 1));
  m_ScreenShader->setUniformValue(transform, "transform");
  glCheck(glDisable(GL_DEPTH_TEST));
  glCheck(glActiveTexture(GL_TEXTURE0));
  glCheck(glBindTexture(GL_TEXTURE_2D, shadowMapping->GetFrame()));
  m_ScreenQuad.draw();
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
