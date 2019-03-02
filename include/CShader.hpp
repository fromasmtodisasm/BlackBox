#pragma once
#include <GL/glew.h>
#include <string>

class CShader 
{
private:
  GLuint m_shader;
  std::string path;
  std::string text;
public:
  enum type{
    E_VERTEX,
    E_FRAGMENT
  };
  CShader(std::string text, CShader::type type);
  static CShader *load(std::string path, CShader::type type);
  bool create();
  bool compile();
  bool bind();
  GLuint get();
};

class CShaderProgram {
private:
  CShader *m_vertex;
  CShader *m_fragment;
  GLuint m_program;
  GLuint m_status;
  GLchar infoLog[512];

  bool status();
public:
  CShaderProgram();
  ~CShaderProgram();

  bool create();
  bool attach(CShader &shader);
  bool link();
  bool status(CShader &shader);
};
