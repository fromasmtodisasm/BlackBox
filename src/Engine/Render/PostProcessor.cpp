#include <BlackBox/Render/PostProcessor.hpp>

PostProcessor::PostProcessor(const char *shader)
	:
 m_ScreenShader(new CShaderProgram(CShader::load("res/shaders/screenshader.vs", CShader::E_VERTEX), CShader::load("res/shaders/" + std::string(shader) + ".frag", CShader::E_FRAGMENT)))
{
	m_ScreenShader->create();
	m_ScreenShader->setUniformValue(0,"screenTexture");
}

PostProcessor::~PostProcessor()
{
}

void PostProcessor::Do(unsigned int texture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_ScreenShader->use();
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_ScreenQuad.draw();;
}
