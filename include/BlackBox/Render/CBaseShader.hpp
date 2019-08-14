#pragma once
#include <BlackBox/Render/Opengl.hpp>
#include <BlackBox/IEngine.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <map>
#include <memory>

class CShader; 
class CBaseShaderProgram;
class CShader;

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
 CBaseShaderProgram *m_Program;

 ShaderProgramStatus(CBaseShaderProgram *program);
 bool get(GLenum statusType);
};

class CShader 
{
private:
	GLuint m_Shader;
	std::string m_Text;
  ShaderStatus m_Status;
public:
	std::string m_Path;
  int m_Type;
  enum type : int{
    E_VERTEX = GL_VERTEX_SHADER,
    E_FRAGMENT = GL_FRAGMENT_SHADER,
    E_UNKNOWN = -1
  };
  CShader(std::string text, CShader::type type);
  ~CShader();
  static std::shared_ptr<CShader> load(std::string path, CShader::type type);
  static bool parseLine(std::ifstream &fin, std::string &buffer);
  static bool loadInternal(std::string &path, std::string& buffer);
  static std::shared_ptr<CShader> loadFromMemory(std::string text, CShader::type type);
  bool create();
  bool compile();
  bool bind();
  void print();
  std::string typeToStr();
  std::string getName();
  GLuint get();
};

class UniformValue {
public:
  inline void Set(int value)
  {
    glCheck(glUniform1i(location, value));
  }
  inline void Set(float value)
  {
    glCheck(glUniform1f(location, value));
  }
  inline void Set(glm::vec1 value)
  {
    glCheck(glUniform1fv(location, 1, glm::value_ptr(value)));
  }
  inline void Set(glm::vec2 value)
  {
    glCheck(glUniform2fv(location, 1, glm::value_ptr(value)));
  }
  inline void Set(glm::vec3 value)
  {
    glCheck(glUniform3fv(location, 1, glm::value_ptr(value)));
  }
  inline void Set(glm::vec4 value)
  {
    glCheck(glUniform4fv(location, 1, glm::value_ptr(value)));
  }
  inline void Set(glm::mat2 value)
  {
    glCheck(glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value)));
  }
  inline void Set(glm::mat3 value)
  {
    glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
  }
  inline void Set(glm::mat4 value)
  {
    glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
  }
public:
  GLint location;
};

class CBaseShaderProgram {
public:
  const size_t BUFFER_SIZE = 1024;

  std::shared_ptr<CShader> m_Vertex = nullptr;
	std::string vertex_name;
  std::shared_ptr<CShader> m_Fragment = nullptr;
	std::string fragment_name;
  GLuint m_Program;
  GLchar infoLog[512];
  ShaderProgramStatus m_Status;
  bool attached = false;
  bool created = false;
  std::map<std::string, GLint> m_Cache;
  static char* buffer;

  bool status();
public:
  CBaseShaderProgram();
  CBaseShaderProgram(std::shared_ptr<CShader> vs, std::shared_ptr<CShader> fs);
  ~CBaseShaderProgram();

  bool create();
  void attach(std::shared_ptr<CShader> shader);
  bool link();
  void use();
  void unuse();
  GLint getUniformLocation(const char* name);
  GLint getUniformLocation(std::string &name);
  UniformValue getUniformValue(const char* name);
  void setUniformValue(int value, const char *format, ...);
  void setUniformValue(float value, const char *format, ...);
  void setUniformValue(glm::vec1 value, const char *format, ...);
  void setUniformValue(glm::vec2 value, const char *format, ...);
  void setUniformValue(glm::vec3 value, const char *format, ...);
  void setUniformValue(glm::vec4 value, const char *format, ...);
  void setUniformValue(glm::mat2 value, const char *format, ...);
  void setUniformValue(glm::mat3 value, const char *format, ...);
  void setUniformValue(glm::mat4 value, const char *format, ...);
  GLuint get();
	virtual void setup() = 0;
private:
  const char* buildName(const char* format, va_list args);
};

//typedef std::shared_ptr<CShader> std::shared_ptr<CShader>;
