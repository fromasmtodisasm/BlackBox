#pragma once
#include <BlackBox/Opengl.hpp>
#include <glm/fwd.hpp>
#include <string>
#include <memory>

class CShader; 
class CShaderProgram;

extern CShaderProgram *defaultProgram;

struct ShaderStatus
{
 GLchar infoLog[512]; 
 GLint m_Status;
 CShader *m_Shader;

 ShaderStatus(CShader *shader);
 bool get(GLenum statusType);
};

struct ShaderProgramStatus
{
 GLchar infoLog[512]; 
 GLint m_Status;
 CShaderProgram *m_Program;

 ShaderProgramStatus(CShaderProgram *program);
 bool get(GLenum statusType);
};

class CShader 
{
private:
	GLuint m_Shader;
	std::string m_Path;
	std::string m_Text;
  ShaderStatus m_Status;
public:
  std::shared_ptr<std::string> m_path;
  int m_Type;
  enum type : int{
    E_VERTEX = GL_VERTEX_SHADER,
    E_FRAGMENT = GL_FRAGMENT_SHADER,
    E_UNKNOWN = -1
  };
  CShader(std::string text, CShader::type type);
  ~CShader();
  static CShader *load(std::string path, CShader::type type);
  bool create();
  bool compile();
  bool bind();
  void print();
  std::string typeToStr();
  std::string getName();
  GLuint get();
};

template<class T>
class UniformValue {
private:
  GLint m_location;
  T m_value;
};

class CShaderProgram {
private:
  CShader *m_Vertex = nullptr;
  CShader *m_Fragment = nullptr;
  GLuint m_Program;
  GLchar infoLog[512];
  ShaderProgramStatus m_Status;
  bool attached = false;
  bool created = false;

  bool status();
public:
  CShaderProgram();
  CShaderProgram(CShader *vertex, CShader *fragment);
  ~CShaderProgram();

  bool create();
  void attach(CShader *shader);
  bool link();
  void use();
  void unuse();
  void setUniformValue(const char *name, float value);
  void setUniformValue(const char *name, glm::vec1 value);
  void setUniformValue(const char *name, glm::vec2 value);
  void setUniformValue(const char *name, glm::vec3 value);
  void setUniformValue(const char *name, glm::vec4 value);
  void setUniformValue(const char *name, glm::mat2 value);
  void setUniformValue(const char *name, glm::mat3 value);
  void setUniformValue(const char *name, glm::mat4 value);
  GLuint get();
};
