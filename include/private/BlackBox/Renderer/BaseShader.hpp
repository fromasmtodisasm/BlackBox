#pragma once
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Utils/smartptr.hpp>
#include <string>
#include <map>
#include <memory>

class CShader;
class CBaseShaderProgram;
class CShaderProgram;
struct ICVar;

using BaseShaderProgramRef = _smart_ptr<CBaseShaderProgram>;
using ShaderProgramRef = _smart_ptr<CShaderProgram>;
using ShaderRef = _smart_ptr<CShader>;

struct ShaderStatus
{
  char m_InfoLog[512] = {0};
  int m_Status = GL_FALSE;
  CShader* m_Shader = nullptr;

  ShaderStatus(CShader* shader);
  bool get(GLenum statusType);
};

struct ShaderProgramStatus
{
  char m_InfoLog[512];
  int m_Status;
  CBaseShaderProgram* m_Program;

  ShaderProgramStatus(CBaseShaderProgram* program);
  bool get(GLenum statusType);
};

class CShader : public IShader
{
public:
  CShader(std::string text, CShader::type type);
  ~CShader();
  // Inherited via IShader
  virtual void AddRef() override;
  virtual int Release() override;

  static CShader* load(ShaderDesc const& desc);
  static bool parseLine(std::ifstream& fin, std::string& buffer);
  static bool loadInternal(std::string const& path, std::string& buffer);
  static ShaderRef loadFromMemory(std::string text, CShader::type type);
  
  virtual bool Create() override;
  virtual bool Compile() override;
  virtual bool Bind() override;
  virtual bool Empty() override;
  virtual void print() override;
  virtual const char* typeToStr() override;
  virtual const char* getName() override;
  virtual uint get() override;

private:
  GLuint m_Shader;
  std::string m_Text;
  ShaderStatus m_Status;
  bool m_Empty;
  int m_Refs = 0;
public:
  std::string m_Path;
  IShader::type m_Type;
  // Inherited via IShader
  virtual IShader::type GetType() override;
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
    gl::Uniform(location, value);
  }
  inline void Set(float value)
  {
    dirty = true;
    gl::Uniform(location, value);
  }
  inline void Set(glm::vec1 value)
  {
    dirty = true;
    gl::Uniform(location, value);
  }
  inline void Set(glm::vec2 value)
  {
    dirty = true;
    gl::Uniform(location, value);
  }
  inline void Set(glm::vec3 value)
  {
    dirty = true;
    gl::Uniform(location, value);
  }
  inline void Set(glm::vec4 value)
  {
    dirty = true;
    gl::Uniform(location, value);
  }
  inline void Set(glm::mat2 value)
  {
    dirty = true;
    gl::Uniform(location, value);
  }
  inline void Set(glm::mat3 value)
  {
    dirty = true;
    gl::Uniform(location, value);
  }
  inline void Set(glm::mat4 value)
  {
    dirty = true;
    gl::Uniform(location, value);
  }

  inline void* Get(UniformValue::Type type, void* res)
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
  inline void Get(int& value)
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
  inline void Get(float& value)
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
  inline void Get(glm::vec1& value)
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
  inline void Get(glm::vec2& value)
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
  inline void Get(glm::vec3& value)
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
  inline void Get(glm::vec4& value)
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
  inline void Get(glm::mat2& value)
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
  inline void Get(glm::mat3& value)
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
  inline void Get(glm::mat4& value)
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

class CBaseShaderProgram : public IShaderProgram {
public:
  CBaseShaderProgram();
  CBaseShaderProgram(ShaderRef vs, ShaderRef fs);
  CBaseShaderProgram(ShaderInfo& vs, ShaderInfo& fs);
  CBaseShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs);
  CBaseShaderProgram(ShaderInfo& vs, ShaderInfo& fs, ShaderInfo& gs, ShaderInfo& cs);
  ~CBaseShaderProgram();

  virtual void AddRef() override;
  virtual int Release() override;

  virtual bool Create(const char* label) override;
  virtual void Attach(ShaderInfo& shader) override;
  virtual ShaderInfo& attachInternal(ShaderInfo& src, ShaderInfo& dst) override;
  virtual void Detach(ShaderInfo& shader) override;
  virtual bool Dispatch(int x, int y, int z, GLbitfield barriers) override;
  virtual bool DispatchInderect() override;
  virtual bool Link() override;
  virtual void Use() override;
  virtual void Unuse() override;
  virtual void DeleteProgram() override;
  virtual GLint GetUniformLocation(const char* format, ...) override;
  virtual GLint GetUniformLocation(std::string& name) override;
  UniformValue GetUniformValue(const char* name);
  void Uniform(bool value, const char* format, ...) { Uniform((int)value, format);}
  virtual void Uniform(int value, const char* format, ...) override;
  virtual void Uniform(unsigned int value, const char* format, ...) override;
  virtual void Uniform(float value, const char* format, ...) override;
  virtual void Uniform(Vec1 value, const char* format, ...) override;
  virtual void Uniform(Vec2 value, const char* format, ...) override;
  virtual void Uniform(Vec3 value, const char* format, ...) override;
  virtual void Uniform(Vec4 value, const char* format, ...) override;
  virtual void Uniform(Mat2 value, const char* format, ...) override;
  virtual void Uniform(Mat3 value, const char* format, ...) override;
  virtual void Uniform(Mat4 value, const char* format, ...) override;
  virtual void Uniform(glm::ivec4 value, const char* format, ...) override;

  template<typename T>
  void Uniform(T value, std::string name) { Uniform(value, name.c_str()); }

  void Reload(ShaderRef v, ShaderRef f, ShaderRef g, ShaderRef c, const char* label);

  virtual void BindTexture2D(GLuint texture, GLint unit, const char* sampler) override;
  virtual void BindTextureUnit2D(GLuint texture, GLint unit) override;
  virtual GLuint Get() override;
  virtual void setup() = 0;
  virtual void Dump() override;
private:
  void reset(ShaderInfo src);
  const char* buildName(const char* format, va_list args);
public:
  const size_t BUFFER_SIZE = 1024;
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
  static ICVar* print_loc_name;
  static ICVar* use_cache;

private:
  int m_Refs = 0;

};

CShader::type str2typ(std::string type);
