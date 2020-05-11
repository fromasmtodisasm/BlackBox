#include <BlackBox/Renderer/PostProcessor.hpp>
#include <BlackBox/Renderer/MaterialManager.hpp>

PostProcessor::PostProcessor(const char* shader)
{
  m_ScreenShader = gEnv->pRenderer->Sh_Load("ScreenQuad", 0);
  m_ScreenShader->Use();
  m_ScreenShader->Uniform(0, "screenTexture");
  m_ScreenShader->Unuse();
}

PostProcessor::~PostProcessor()
{
}

void PostProcessor::Do(unsigned int texture)
{
	#if 0
  gl::BindFramebuffer(0);
  m_ScreenShader->Use();
  gl::Disable(GL_DEPTH_TEST);
  gl::ActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  m_ScreenQuad.draw();;
	#endif
}