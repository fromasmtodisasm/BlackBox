#include <BlackBox/Renderer/ShadowsShader.hpp>

ShadowsShader::ShadowsShader() : m_Engine(GetIEngine()), CBaseShaderProgram(CShader::load("res/shaders/reflect.vs", CShader::E_VERTEX), CShader::load("res/shaders/reflect.frag", CShader::E_FRAGMENT))
{
	
}

ShadowsShader::~ShadowsShader()
{
}

void ShadowsShader::setup()
{
		glCheck(glActiveTexture(GL_TEXTURE0));
		glCheck(glBindTexture(GL_TEXTURE_2D, 0));
		glCheck(glActiveTexture(GL_TEXTURE0));
		glCheck(glBindTexture(GL_TEXTURE_CUBE_MAP, Pipeline::instance()->skyBox->id));
		setUniformValue(0, "skybox");
}