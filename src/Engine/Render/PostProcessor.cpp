#include <BlackBox/Render/PostProcessor.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>

PostProcessor::PostProcessor(const char *shader)
{
	ProgramDesc desc = {
		shader,
		ShaderDesc("screenshader.vs"),
		ShaderDesc(std::string(shader) + ".frag")
	};

	MaterialManager::instance()->loadProgram(desc, false);
	m_ScreenShader = MaterialManager::instance()->getProgram(shader);
	m_ScreenShader->Use();
	m_ScreenShader->Uniform(0,"screenTexture");
	m_ScreenShader->Unuse();
}

PostProcessor::~PostProcessor()
{
}

void PostProcessor::Do(unsigned int texture)
{
	gl::BindFramebuffer(0);
	m_ScreenShader->Use();
	gl::Disable(GL_DEPTH_TEST);
	gl::ActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	m_ScreenQuad.draw();;
}
