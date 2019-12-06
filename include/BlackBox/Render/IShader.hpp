#pragma once
#include <BlackBox/Render/IRender.hpp>
#include <string>
#include <map>

//////////////////////////////////////////////////////////////////////
struct ShaderDesc
{
  ShaderDesc() {}
  ShaderDesc(std::string name) : name(name) {}
  ShaderDesc(std::string name, std::string type) : type(type), name(name) {}

  using Macro = std::map<std::string, std::string>;
  std::string type;
  std::string name;
  Macro macro;

  static std::string root;
};
struct ProgramDesc
{
  std::string name;
  ShaderDesc vs;
  ShaderDesc fs;
  ShaderDesc gs;
  ShaderDesc cs;
};

struct IShader
{
  enum type : int {
    E_VERTEX,
    E_FRAGMENT,
    E_GEOMETRY,
    E_COMPUTE,
    E_UNKNOWN = -1
  };
  //static std::shared_ptr<CShader> loadFromMemory(std::string text, CShader::type type);
  virtual void AddRef() = 0;
  virtual int Release() = 0;

  virtual bool Create() = 0;
  virtual bool Compile() = 0;
  virtual bool Bind() = 0;
  virtual bool Empty() = 0;
  virtual IShader::type GetType() = 0;
  virtual void print() = 0;
  virtual const char* typeToStr() = 0;
  virtual const char* getName() = 0;
  virtual uint get() = 0;
};



class IShaderProgram {
public:
  struct ShaderInfo
  {
    ShaderInfo()
      :
      shader(nullptr),
      attached(false),
      used(false)
    {
    }
    ShaderInfo(IShader* shader, std::string& name)
      :
      shader(shader),
      name(name),
      attached(false),
      used(true)
    {
    }

    IShader* shader;
    std::string name;
    bool attached;
    bool used;
  };

  virtual void AddRef() = 0;
  virtual int Release() = 0;

  virtual bool Create(const char* label) = 0;
  virtual void Attach(ShaderInfo& shader) = 0;
  virtual ShaderInfo& attachInternal(ShaderInfo& src, ShaderInfo& dst) = 0;
  virtual void Detach(ShaderInfo& shader) = 0;
  virtual bool Dispatch(int x, int y, int z, uint barriers) = 0;
  virtual bool DispatchInderect() = 0;
  virtual bool Link() = 0;
  virtual void Use() = 0;
  virtual void Unuse() = 0;
  virtual void DeleteProgram() = 0;
  virtual int GetUniformLocation(const char* format, ...) = 0;
  virtual int GetUniformLocation(std::string& name) = 0;
  //UniformValue GetUniformValue(const char* name);
  void Uniform(bool value, const char* format, ...) { Uniform((int)value, format); }
  virtual void Uniform(int value, const char* format, ...) = 0;
  virtual void Uniform(unsigned int value, const char* format, ...) = 0;
  virtual void Uniform(float value, const char* format, ...) = 0;
  virtual void Uniform(Vec1 value, const char* format, ...) = 0;
  virtual void Uniform(Vec2 value, const char* format, ...) = 0;
  virtual void Uniform(Vec3 value, const char* format, ...) = 0;
  virtual void Uniform(Vec4 value, const char* format, ...) = 0;
  virtual void Uniform(Mat2 value, const char* format, ...) = 0;
  virtual void Uniform(Mat3 value, const char* format, ...) = 0;
  virtual void Uniform(Mat4 value, const char* format, ...) = 0;
  virtual void Uniform(glm::ivec4 value, const char* format, ...) = 0;

  template<typename T>
  void Uniform(T value, std::string name) { Uniform(value, name.c_str()); }

  void Reload(IShader *v, IShader *f, IShader *g, IShader *c, const char* label);

  virtual void BindTexture2D(uint texture, int unit, const char* sampler) = 0;
  virtual void BindTextureUnit2D(uint texture, int unit) = 0;
  virtual uint Get() = 0;
  virtual void setup() = 0;
  virtual void Dump() = 0;
};