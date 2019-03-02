#pragma once
#include <GL/gl.h>

class CShader 
{
private:
  GLuint shader;
  char *path;
  char *text;
public:
  enum type{
    E_VERTEX,
    E_FRAGMENT
  };
  CShader(char *text, CShader::type type);
  static CShader *load(char *path, CShader::type type);
  bool create();
  bool compile();
  bool bind();
};
