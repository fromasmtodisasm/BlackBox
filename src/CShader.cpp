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

bool CShader::compile() {
  glShaderSource(m_shader, 1, reinterpret_cast<const GLchar* const*>(text.c_str()), nullptr);
  glCompileShader(m_shader);
  return true;
}

GLuint CShader::get() {
  return m_shader;
}

CShaderProgram::CShaderProgram() {

}

CShaderProgram::~CShaderProgram() {

}

bool CShaderProgram::create() {
  m_program = glCreateProgram();
}

bool CShaderProgram::attach(CShader &shader) {
  glAttachShader(m_program, shader.get());
}

bool CShaderProgram::link() {
  glLinkProgram(m_program);
}

bool CShaderProgram::status(CShader &shader) {
  //glGetShaderiv(shader.get(), GL_COMPILE_STATUS, &m_status);
}
