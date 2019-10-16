#pragma once
#include <BlackBox/Render/Opengl.hpp>
#include <BlackBox/ISystem.hpp>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <map>
#include <memory>

class CShader; 
class CBaseShaderProgram;
class CShaderProgram;

using BaseShaderProgramRef = std::shared_ptr<CBaseShaderProgram>;
using ShaderProgramRef = std::shared_ptr<CShaderProgram>;
using ShaderRef = std::shared_ptr<CShader>;

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
	bool m_Empty;
public:
	std::string m_Path;
  int m_Type;
  enum type : int{
    E_VERTEX = GL_VERTEX_SHADER,
    E_FRAGMENT = GL_FRAGMENT_SHADER,
    E_GEOMETRY = GL_GEOMETRY_SHADER,
    E_COMPUTE = GL_COMPUTE_SHADER,
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
	bool empty();
  void print();
  std::string typeToStr();
  std::string getName();
  GLuint get();
};

class UniformValue {
  enum class Type
  {
    INT_VAL,
    FLOAT_VAL,
    V1_VAL,
    V2_VAL,
    V3_VAL,
    V4_VAL,
    M1_VAL,
    M2_VAL,
    M3_VAL,
    M4_VAL
  };
public:
  inline void Set(int value)
  {
    dirty = true;
    glCheck(glUniform1i(location, value));
  }
  inline void Set(float value)
  {
    dirty = true;
    glCheck(glUniform1f(location, value));
  }
  inline void Set(glm::vec1 value)
  {
    dirty = true;
    glCheck(glUniform1fv(location, 1, glm::value_ptr(value)));
  }
  inline void Set(glm::vec2 value)
  {
    dirty = true;
    glCheck(glUniform2fv(location, 1, glm::value_ptr(value)));
  }
  inline void Set(glm::vec3 value)
  {
    dirty = true;
    glCheck(glUniform3fv(location, 1, glm::value_ptr(value)));
  }
  inline void Set(glm::vec4 value)
  {
    dirty = true;
    glCheck(glUniform4fv(location, 1, glm::value_ptr(value)));
  }
  inline void Set(glm::mat2 value)
  {
    dirty = true;
    glCheck(glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(value)));
  }
  inline void Set(glm::mat3 value)
  {
    dirty = true;
    glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
  }
  inline void Set(glm::mat4 value)
  {
    dirty = true;
    glCheck(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value)));
  }
  

  inline void* Get(UniformValue::Type type, void *res)
  {
    value val;
    int len = 0;
    switch (type)
    {
    case UniformValue::Type::INT_VAL:
      len = sizeof(int);
      Get(val.i);
      break;
    case UniformValue::Type::FLOAT_VAL:
      len = sizeof(float);
      Get(val.f);
      break;
    case UniformValue::Type::V1_VAL:
      len = sizeof(glm::vec1);
      Get(val.v1);
      break;
    case UniformValue::Type::V2_VAL:
      len = sizeof(glm::vec2);
      Get(val.v2);
      break;
    case UniformValue::Type::V3_VAL:
      len = sizeof(glm::vec3);
      Get(val.v3);
      break;
    case UniformValue::Type::V4_VAL:
      len = sizeof(glm::vec4);
      Get(val.v4);
      break;
    case UniformValue::Type::M2_VAL:
      len = sizeof(glm::mat2);
      Get(val.m2);
      break;
    case UniformValue::Type::M3_VAL:
      len = sizeof(glm::mat3);
      Get(val.m3);
      break;
    case UniformValue::Type::M4_VAL:
      len = sizeof(glm::mat4);
      Get(val.m4);
      break;
    default:
      break;
    }
    memcpy(res, &val, len);
    return res;
  }
  inline void Get(int &value)
  {
    if (!dirty)
    {
      value = val.i;
    }
    else
    {
      glGetUniformiv(program, location, &value);
    }
  }
  inline void Get(float &value)
  {
    if (!dirty)
    {
      value = val.f;
    }
    else
    {
      glGetUniformfv(program, location, &value);
    }
  }
  inline void Get(glm::vec1 &value)
  {
    if (!dirty)
    {
      value = val.v1;
    }
    else
    {
      glCheck(glGetUniformfv(program, location, const_cast<float*>(glm::value_ptr(value))));
    }
 }
  inline void Get(glm::vec2 &value)
  {
    if (!dirty)
    {
      value = val.v2;
    }
    else
    {
      glCheck(glGetUniformfv(program, location, const_cast<float*>(glm::value_ptr(value))));
    }
  }
  inline void Get(glm::vec3 &value)
  {
    if (!dirty)
    {
      value = val.v3;
    }
    else
    {
      glCheck(glGetUniformfv(program, location, const_cast<float*>(glm::value_ptr(value))));
    }
  }
  inline void Get(glm::vec4 &value)
  {
    if (!dirty)
    {
      value = val.v4;
    }
    else
    {
      glCheck(glGetUniformfv(program, location, const_cast<float*>(glm::value_ptr(value))));
    }
  }
  inline void Get(glm::mat2 &value)
  {
    if (!dirty)
    {
      value = val.m2;
    }
    else
    {
      glCheck(glGetUniformfv(program, location, const_cast<float*>(glm::value_ptr(value))));
    }
  }
  inline void Get(glm::mat3 &value)
  {
    if (!dirty)
    {
      value = val.m3;
    }
    else
    {
      glCheck(glGetUniformfv(program, location, const_cast<float*>(glm::value_ptr(value))));
    }
  }
  inline void Get(glm::mat4 &value)
  {
    if (!dirty)
    {
      value = val.m4;
    }
    else
    {
      glCheck(glGetUniformfv(program, location, const_cast<float*>(glm::value_ptr(value))));
    }
  }
public:
  GLint location;
  union value
  {
    int i;
    float f;
    glm::vec1 v1;
    glm::vec2 v2;
    glm::vec3 v3;
    glm::vec4 v4;

    glm::mat2 m2;
    glm::mat3 m3;
    glm::mat4 m4;
  }val;
  bool dirty;
  GLint program;
};

class CBaseShaderProgram {
public:
  const size_t BUFFER_SIZE = 1024;
	struct ShaderInfo
	{
		ShaderInfo()
			:
			shader(nullptr),
			attached(false),
			used(false)
		{
		}
		ShaderInfo(ShaderRef shader, std::string &name)
			:
			shader(shader),
			name(name),
			attached(false),
			used(true)
		{
		}

		ShaderRef shader;
		std::string name;
		bool attached;
		bool used;
	};

  ShaderInfo m_Vertex;
  ShaderInfo m_Fragment;
  ShaderInfo m_Geometry;
  ShaderInfo m_Compute;

  GLuint m_Program;
  GLchar infoLog[512];
  ShaderProgramStatus m_Status;

  bool created = false;
  std::map<std::string, GLint> m_Cache;
  static char* buffer;
	std::string name;

  bool status();
public:
  CBaseShaderProgram();
  CBaseShaderProgram(ShaderRef vs, ShaderRef fs);
  CBaseShaderProgram(ShaderInfo& vs, ShaderInfo& fs);
  CBaseShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs);
  CBaseShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs, ShaderInfo& cs);
  ~CBaseShaderProgram();

  bool create(const char *label);
  void attach(ShaderInfo& shader);
  ShaderInfo& attachInternal(ShaderInfo& src, ShaderInfo& dst);
  void detach(ShaderInfo& shader);
	bool dispatch(int x, int y, int z, GLbitfield barriers);
	bool dispatchInderect();
  bool link();
  void use();
  void unuse();
	void deleteProgram();
  GLint getUniformLocation(const char* format, ...);
  GLint getUniformLocation(std::string &name);
  UniformValue getUniformValue(const char* name);
	void setUniformValue(bool value, const char* format, ...) { setUniformValue((int)value, format); }
  void setUniformValue(int value, const char *format, ...);
  void setUniformValue(unsigned int value, const char *format, ...);
  void setUniformValue(float value, const char *format, ...);
  void setUniformValue(glm::vec1 value, const char *format, ...);
  void setUniformValue(glm::vec2 value, const char *format, ...);
  void setUniformValue(glm::vec3 value, const char *format, ...);
  void setUniformValue(glm::vec4 value, const char *format, ...);
  void setUniformValue(glm::ivec4 value, const char *format, ...);
  void setUniformValue(glm::mat2 value, const char *format, ...);
  void setUniformValue(glm::mat3 value, const char *format, ...);
  void setUniformValue(glm::mat4 value, const char *format, ...);

	template<typename T>
	void setUniformValue(T value, std::string name) { setUniformValue(value, name.c_str()); }

	void reload(ShaderRef v, ShaderRef f, ShaderRef g, ShaderRef c, const char* label);

	void bindTexture2D(GLuint texture, GLint unit, const char* sampler);
	void bindTextureUnit2D(GLuint texture, GLint unit);
  GLuint get();
	virtual void setup() = 0;
	void dump();
private:
	void reset(ShaderInfo src);
  const char* buildName(const char* format, va_list args);
};

//typedef std::shared_ptr<CShader> std::shared_ptr<CShader>;
