#pragma once
//#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Utils/smartptr.hpp>
#include <map>
#include <string>
#include <string_view>
#include <utility>

struct ITexture;
class CCamera;

struct IShader;
struct IBaseShaderProgram;
struct IShaderProgram;

using BaseShaderProgramRef = _smart_ptr<IShaderProgram>;
using ShaderProgramRef	   = _smart_ptr<IShaderProgram>;
using ShaderRef			   = _smart_ptr<IShader>;

struct IIncluder
{
	virtual ~IIncluder()									  = default;
	virtual std::string_view OnInclude(std::string_view file) = 0;
};

class BasicIncluder final : public IIncluder
{
	std::string_view OnInclude(std::string_view file) override
	{
		return "";
	}
};

// SShader::m_Flags
// Different useful flags
#define EF_RELOAD 1 // Shader needs tangent vectors array.
#define EF_FORCE_RELOAD 2
#define EF_RELOADED 4

enum class ShaderLangId
{
	None,
	Glsl,
	Hlsl
};

struct IShader
{
  protected:
	~IShader() = default;

  public:
	enum Type : int
	{
		E_VERTEX,
		E_TESSELATION_CONTROL,
		E_TESSELATION_EVALUATION,
		E_GEOMETRY,
		E_FRAGMENT,
		E_COMPUTE,
		E_UNKNOWN = -1
	};
	//static std::shared_ptr<CShader> loadFromMemory(std::string text, CShader::type type);
	virtual void AddRef() = 0;
	virtual int Release() = 0;

	virtual bool Create()								= 0;
	virtual bool Compile(std::vector<std::string> code) = 0;
	virtual bool Bind()									= 0;
	virtual bool Empty()								= 0;
	virtual IShader::Type GetType()						= 0;
	virtual void Print()								= 0;
	virtual const char* TypeToStr()						= 0;
	virtual const char* GetName();
	virtual uint Get() = 0;
};

struct IShaderProgram
{
	struct ShaderInfo
	{
		ShaderInfo()
			: shader(nullptr),
			  attached(false),
			  used(false)
		{
		}
		ShaderInfo(IShader* shader, std::string name)
			: shader(shader),
			  name(std::move(name)),
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
	virtual int Reload()  = 0;

	virtual bool Create(const char* label)								 = 0;
	virtual void Attach(ShaderInfo& shader)								 = 0;
	virtual ShaderInfo& AttachInternal(ShaderInfo& src, ShaderInfo& dst) = 0;
	virtual void Detach(ShaderInfo& shader)								 = 0;
	virtual bool Dispatch(int x, int y, int z, uint barriers)			 = 0;
	virtual bool DispatchIndirect()										 = 0;
	virtual bool Link()													 = 0;
	virtual void Use()													 = 0;
	virtual void Unuse()												 = 0;
	virtual void DeleteProgram()										 = 0;
	virtual int GetUniformLocation(const char* format, ...)				 = 0;
	virtual int GetUniformLocation(string& name)						 = 0;
	virtual const char* GetShaderName(IShader::Type type)				 = 0;
	//UniformValue GetUniformValue(const char* name);
	virtual void Uniform(bool value, const char* format, ...)
	{
		Uniform((int)value, format);
	}
	virtual void Uniform(const int value, const char* format, ...)			= 0;
	virtual void Uniform(const unsigned int value, const char* format, ...) = 0;
	virtual void Uniform(const float value, const char* format, ...)		= 0;
	virtual void Uniform(const Vec1 value, const char* format, ...)			= 0;
	virtual void Uniform(const Vec2 value, const char* format, ...)			= 0;
	virtual void Uniform(const Vec3 value, const char* format, ...)			= 0;
	virtual void Uniform(const Vec4 value, const char* format, ...)			= 0;
	virtual void Uniform(const Mat2 value, const char* format, ...)			= 0;
	virtual void Uniform(const Mat3 value, const char* format, ...)			= 0;
	virtual void Uniform(const Mat4 value, const char* format, ...)			= 0;
	virtual void Uniform(const glm::ivec4 value, const char* format, ...)	= 0;
	virtual void Uniform(const ITexture* texture, const char* format, ...)	= 0;

	template<typename T>
	void Uniform(const T value, string name)
	{
		Uniform(value, name.c_str());
	}

	virtual void Reload(ShaderRef& v, ShaderRef& f, ShaderRef& g, ShaderRef& c, const char* label) = 0;

	virtual void BindTexture2D(uint texture, int unit, const char* sampler) = 0;
	virtual void BindTextureUnit2D(uint texture, int unit)					= 0;
	virtual uint Get()														= 0;
	virtual void setup()													= 0;
	virtual void Dump()														= 0;
};

//////////////////////////////////////////////////////////////////////
struct ShaderDesc
{
	ShaderDesc()
	{
	}
	ShaderDesc(std::string name)
		: name(std::move(name))
	{
	}

	ShaderDesc(std::string name, IShader::Type type);

	inline void AddMacro(const std::string& key, const std::string& value)
	{
		macro.insert(Macro::value_type(key, value));
	}

	using Macro = std::map<std::string, std::string>;
	std::string name;
	IShader::Type type;
	Macro macro;

	const std::string root = "res/shaders/";
};
struct ProgramDesc
{
	std::string name;
	ShaderDesc vs;
	ShaderDesc fs;
	ShaderDesc gs;
	ShaderDesc cs;
};

IShaderProgram* GetGLSLSandBoxShader();
