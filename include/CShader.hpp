#pragma once
#include <glad/glad.h>
#include <string>

class CShader; 
class CShaderProgram; 

struct ShaderStatus
{
 GLchar infoLog[512]; 
 GLint m_Status;
 CShader *m_Shader;

 ShaderStatus(CShader *shader);
 bool get();
};

struct ShaderProgramStatus
{
 GLchar infoLog[512]; 
 GLint m_Status;
 CShaderProgram *m_Program;

 ShaderProgramStatus(CShaderProgram *program);
 bool get();
};

class CShader 
{
private:
	GLuint m_Shader;
	std::string m_Path;
	std::string m_Text;
  ShaderStatus m_Status;
  int m_Type; 
public:
  enum type : int{
    E_VERTEX = GL_VERTEX_SHADER,
    E_FRAGMENT = GL_FRAGMENT_SHADER
  };
  CShader(std::string text, CShader::type type);
  ~CShader();
  static CShader *load(std::string path, CShader::type type);
  bool create();
  bool compile();
  bool bind();
  void print();
  std::string getName();
  GLuint get();
};

class CShaderProgram {
private:
  CShader *m_Vertex;
  CShader *m_Fragment;
  GLuint m_Program;
  GLchar infoLog[512];
  ShaderProgramStatus m_Status;

  bool status();
public:
  CShaderProgram(std::string vertex, std::string fragment);
  ~CShaderProgram();

  bool create();
  bool attach(CShader &shader);
  bool link();
  void use();
  GLuint get();
};
