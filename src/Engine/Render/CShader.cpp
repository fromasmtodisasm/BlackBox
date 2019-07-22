#include <BlackBox/Render/CShader.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/Render/Pipeline.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

CShaderProgram::CShaderProgram() : CBaseShaderProgram()
{

}

CShaderProgram::CShaderProgram(CShader* vs, CShader* fs) : CBaseShaderProgram(vs, fs)
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
