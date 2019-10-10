#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/Render/Pipeline.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

CShaderProgram::CShaderProgram() : CBaseShaderProgram()
{

}

CShaderProgram::CShaderProgram(ShaderRef vs, ShaderRef fs) : CBaseShaderProgram(vs, fs)
{

}

CShaderProgram::CShaderProgram(ShaderInfo& vs, ShaderInfo& fs) : CBaseShaderProgram(vs, fs)
{
}

CShaderProgram::CShaderProgram(std::string vs, std::string fs) 
	:
	CBaseShaderProgram(CShader::load(vs, CShader::E_VERTEX), CShader::load(fs, CShader::E_FRAGMENT))
{
}

CShaderProgram::CShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs) : CBaseShaderProgram(vs, fs, gs)
{
}

CShaderProgram::CShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs, ShaderInfo& cs) : CBaseShaderProgram(vs, fs, gs, cs)
{

}

void CShaderProgram::setup()
{
  setUniformValue( Pipeline::instance()->model,"model");
  setUniformValue( Pipeline::instance()->view,"view");
  setUniformValue( Pipeline::instance()->projection,"projection");
  setUniformValue( Pipeline::instance()->view_pos,"viewPos");
  setUniformValue( 128.0f,"material.shininess");
}

CShaderProgram::~CShaderProgram()
{
}
