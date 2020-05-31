#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/System/ISystem.hpp>

using namespace std;

CShaderProgram::CShaderProgram() : CBaseShaderProgram()
{
}

CShaderProgram::CShaderProgram(ShaderRef vs, ShaderRef fs) : CBaseShaderProgram(vs, fs)
{
}

CShaderProgram::CShaderProgram(const ShaderInfo& vs, const ShaderInfo& fs) : CBaseShaderProgram(vs, fs)
{
}

#if 0
CShaderProgram::CShaderProgram(std::string vs, std::string fs)
  :
  CBaseShaderProgram(CShader::load(vs, CShader::E_VERTEX), CShader::load(fs, CShader::E_FRAGMENT))
{
}
#endif

CShaderProgram::CShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs) : CBaseShaderProgram(vs, fs, gs)
{
}

CShaderProgram::CShaderProgram(IShaderProgram::ShaderInfo& vs, IShaderProgram::ShaderInfo& fs, IShaderProgram::ShaderInfo& gs, IShaderProgram::ShaderInfo& cs) : CBaseShaderProgram(vs, fs, gs, cs)
{
}

void CShaderProgram::setup()
{
  /*Uniform(Pipeline::instance()->model, "model");
  Uniform(Pipeline::instance()->view, "view");
  Uniform(Pipeline::instance()->projection, "projection");
  Uniform(Pipeline::instance()->view_pos, "viewPos");
  Uniform(128.0f, "material.shininess");*/
}

CShaderProgram::~CShaderProgram()
{
}
