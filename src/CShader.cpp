#include "CShader.hpp"
#include <fstream>
#include <string>

using namespace std;

CShader::CShader(string text, CShader::type type)  {

}

bool CShader::create() {
  int type;
  switch(type) {
  case E_VERTEX: type = GL_VERTEX_SHADER; break;
  case E_FRAGMENT: type = GL_FRAGMENT_SHADER; break;
  default:
    return false;
  }

  m_shader = glCreateShader(type);
  return true;
}


CShader *CShader::load(string path, CShader::type type) {
  ifstream fin(path);
  string buff;
  string text;

  if (!fin.is_open()) return nullptr;
  
  while (getline(fin, buff)) {
    text += buff;
    text += '\n';
  }

  CShader *shader = new CShader(text, type);

  return shader;
}
