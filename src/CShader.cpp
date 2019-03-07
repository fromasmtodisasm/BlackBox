#include "CShader.hpp"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

ShaderStatus::ShaderStatus(CShader *shader) :
  m_Shader(shader)
{

}

bool ShaderStatus::get() {
  glGetShaderiv(m_Shader->get(), GL_COMPILE_STATUS, &m_Status);
  if(m_Status != GL_TRUE)
  {
    glGetShaderInfoLog(m_Shader->get(), 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::" << m_Shader->getName() <<"\n" << infoLog << std::endl;
    return false;
  }
  return true;
 
}

ShaderProgramStatus::ShaderProgramStatus(CShaderProgram *program) :
  m_Program(program)
{

}

bool ShaderProgramStatus::get() {
  glGetShaderiv(m_Program->get(), GL_LINK_STATUS, &m_Status);
  if(m_Status != GL_TRUE)
  {
    glGetShaderInfoLog(m_Program->get(), 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM " <<"\n" << infoLog << std::endl;
    return false;
  }
  return true;
 
}
CShader::CShader(string text, CShader::type type) :
  m_Text(text), m_Type(type), m_Status(this) 
{
  
}

CShader::~CShader() {
  glDeleteShader(m_Shader);
}

bool CShader::create() {
  cout << __func__ << endl;
  cout << glCreateShader << endl;
  m_Shader = glCreateShader(m_Type);
  return m_Status.get();
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
  shader->create();
  shader->compile();

  return shader;
}

bool CShader::compile() {
  cout << __func__ << endl;
  cout  << "shader: " << m_Shader << endl;
  const char *text = m_Text.c_str();
  glShaderSource(m_Shader, 1, &text, nullptr);
  cout << "test" << endl;
  glCompileShader(m_Shader);
  return m_Status.get();
}

bool CShader::bind() {
 return true; 
}

void CShader::print() {
  cout << m_Text << endl; 
}

string CShader::getName() {
  return m_Path;
}

GLuint CShader::get() {
  return m_Shader;
}

CShaderProgram::CShaderProgram(
  string vertex, string fragment) :
  m_Status(this)
{
  m_Vertex = CShader::load(vertex, CShader::E_VERTEX);
  m_Fragment = CShader::load(fragment, CShader::E_FRAGMENT);
  if (m_Vertex == nullptr || m_Fragment == nullptr)
    cout << "Error of load shader" << endl;
  else
    cout << "Shaders loaded" << endl;
}

CShaderProgram::~CShaderProgram() {

}

bool CShaderProgram::create() {
  m_Program = glCreateProgram();
  attach(*m_Vertex);
  attach(*m_Fragment);
  link();
	return true;
}

bool CShaderProgram::attach(CShader &shader) {
  glAttachShader(m_Program, shader.get());
	return m_Status.get();
}

bool CShaderProgram::link() {
  glLinkProgram(m_Program);
  delete m_Vertex;
  delete m_Fragment;
	return m_Status.get();
}

void CShaderProgram::use() {
  glUseProgram(m_Program);
}

GLuint CShaderProgram::get() {
  return m_Program;
}
