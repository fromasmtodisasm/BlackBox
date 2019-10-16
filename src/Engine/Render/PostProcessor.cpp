#include <BlackBox/Render/PostProcessor.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>

PostProcessor::PostProcessor(const char *shader)
{
	ProgramDesc desc = {
		shader,
		"screenshader.vs",
		std::string(shader) + ".frag"
	};

	MaterialManager::instance()->loadProgram(desc, false);
	m_ScreenShader = MaterialManager::instance()->getProgram(shader);
	m_ScreenShader->use();
	m_ScreenShader->setUniformValue(0,"screenTexture");
	m_ScreenShader->unuse();
}

PostProcessor::~PostProcessor()
{
}

void PostProcessor::Do(unsigned int texture)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	m_ScreenShader->use();
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_ScreenQuad.draw();;
}
