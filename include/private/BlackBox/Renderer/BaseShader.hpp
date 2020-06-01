#pragma once
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Utils/smartptr.hpp>
#include <string>
#include <map>
#include <memory>

struct ICVar;

class CShader;
class CBaseShaderProgram;
class CShaderProgram;

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
  char m_InfoLog[1024];
  int m_Status;
  CBaseShaderProgram* m_Program;

  ShaderProgramStatus(CBaseShaderProgram* program);
  bool get(GLenum statusType);
};

class CShader : public IShader
{
public:
  CShader(std::string text, CShader::Type type);
  ~CShader();
  // Inherited via IShader
  virtual void AddRef() override;
  virtual int Release() override;

  static CShader* load(ShaderDesc const& desc);
  static CShader* load(std::string_view source);
  static bool parseLine(std::ifstream& fin, std::string& buffer);
  static bool loadInternal(std::string const& path, std::string& buffer);
  static ShaderRef loadFromMemory(std::string text, CShader::Type type);
  
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
  IShader::Type m_Type;
  virtual IShader::Type GetType() override;
};

class CBaseShaderProgram : public IShaderProgram {
public:
  CBaseShaderProgram();
  CBaseShaderProgram(ShaderRef vs, ShaderRef fs);
  CBaseShaderProgram(const ShaderInfo& vs, const ShaderInfo& fs);
  CBaseShaderProgram(const ShaderInfo& vs, const ShaderInfo& fs, const ShaderInfo& gs);
  CBaseShaderProgram(const ShaderInfo& vs, const ShaderInfo& fs, const ShaderInfo& gs, const ShaderInfo& cs);
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
  virtual void Uniform(const int value, const char* format, ...) override;
  virtual void Uniform(const unsigned int value, const char* format, ...) override;
  virtual void Uniform(const float value, const char* format, ...) override;
  virtual void Uniform(const Vec1 value, const char* format, ...) override;
  virtual void Uniform(const Vec2 value, const char* format, ...) override;
  virtual void Uniform(const Vec3 value, const char* format, ...) override;
  virtual void Uniform(const Vec4 value, const char* format, ...) override;
  virtual void Uniform(const Mat2 value, const char* format, ...) override;
  virtual void Uniform(const Mat3 value, const char* format, ...) override;
  virtual void Uniform(const Mat4 value, const char* format, ...) override;
  virtual void Uniform(const glm::ivec4 value, const char* format, ...) override;
  virtual void Uniform(const ITexture* texture, const char* format, ...) override;

  template<typename T>
  void Uniform(const T value, std::string name) { Uniform(value, name.c_str()); }

  //virtual void Reload(ShaderRef& v, ShaderRef& f, ShaderRef& g, ShaderRef& c, const char* label) override;

  virtual void BindTexture2D(GLuint texture, GLint unit, const char* sampler) override;
  virtual void BindTextureUnit2D(GLuint texture, GLint unit) override;
  virtual uint Get() override;
  virtual void setup() override {};
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

  uint m_Program;
  char infoLog[512];
  ShaderProgramStatus m_Status;

  bool created = false;
  std::map<std::string, GLint> m_Cache;
  static char* buffer;
  std::string name;
  static ICVar* print_loc_name;
  static ICVar* use_cache;

  int m_Refs = 0;


  // Inherited via IShaderProgram
  virtual const char* GetShaderName(IShader::Type type) override;


  // Inherited via IShaderProgram
  virtual void Reload(ShaderRef& v, ShaderRef& f, ShaderRef& g, ShaderRef& c, const char* label) override;


// Inherited via IShaderProgram
  virtual int Reload() override;
};

CShader::Type str2typ(std::string type);
