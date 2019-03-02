#include "CShader.hpp"

CShader::CShader(char *text, CShader::type type)  {

}

bool CShader::create() {
  return true;
}


CShader *CShader::load(char *path, CShader::type type) {
  CShader *shader = new CShader(path, type);

  return shader;
}
