#pragma once
//#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Utils/smartptr.hpp>
#include <map>
#include <string>
#include <string_view>

struct ITexture;
class CCamera;

struct IShader;
struct IBaseShaderProgram;
struct IShaderProgram;

using BaseShaderProgramRef = _smart_ptr<IShaderProgram>;
using ShaderProgramRef	 = _smart_ptr<IShaderProgram>;
using ShaderRef			   = _smart_ptr<IShader>;

//////////////////////////////////////////////////////////////////////
struct UniformValue
{
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
		M4_VAL,
		Samp_VAL,
		CAMERA_VAL
	};
	union value {
		int i;
		float f;
		glm::vec1 v1;
		glm::vec2 v2;
		glm::vec3 v3;
		glm::vec4 v4;

		glm::mat2 m2;
		glm::mat3 m3;
		glm::mat4 m4;
		ITexture* t;
		CCamera* c;
	} val;
	UniformValue() = default;
	UniformValue(std::string_view name, Type type)
		: name(name), type(type)
	{
	}
	template<class T>
	inline void Set(T value)
	{
		dirty = true;
		Set(value);
	}
	inline void Set(int value)
	{
		val.i = value;
	}
	inline void Set(float value)
	{
		val.f = value;
	}
	inline void Set(glm::vec1 value)
	{
		val.v1 = value;
	}
	inline void Set(glm::vec2 value)
	{
		val.v2 = value;
	}
	inline void Set(glm::vec3 value)
	{
		val.v3 = value;
	}
	inline void Set(glm::vec4 value)
	{
		val.v4 = value;
	}
	inline void Set(glm::mat2 value)
	{
		val.m2 = value;
	}
	inline void Set(glm::mat3 value)
	{
		val.m3 = value;
	}
	inline void Set(glm::mat4 value)
	{
		val.m4 = value;
	}

	inline void Set(ITexture* value)
	{
		val.t = value;
	}

	inline void Set(CCamera* value)
	{
		val.c = value;
	}

	inline void Set(value val)
	{
		int len = 0;
		switch (type)
		{
		case UniformValue::Type::INT_VAL:
			len = sizeof(int);
			Set(val.i);
			break;
		case UniformValue::Type::FLOAT_VAL:
			len = sizeof(float);
			Set(val.f);
			break;
		case UniformValue::Type::V1_VAL:
			len = sizeof(glm::vec1);
			Set(val.v1);
			break;
		case UniformValue::Type::V2_VAL:
			len = sizeof(glm::vec2);
			Set(val.v2);
			break;
		case UniformValue::Type::V3_VAL:
			len = sizeof(glm::vec3);
			Set(val.v3);
			break;
		case UniformValue::Type::V4_VAL:
			len = sizeof(glm::vec4);
			Set(val.v4);
			break;
		case UniformValue::Type::M2_VAL:
			len = sizeof(glm::mat2);
			Set(val.m2);
			break;
		case UniformValue::Type::M3_VAL:
			len = sizeof(glm::mat3);
			Set(val.m3);
			break;
		case UniformValue::Type::M4_VAL:
			len = sizeof(glm::mat4);
			Set(val.m4);
			break;
		default:
			break;
		}
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
#if 0
	template<class T>
	inline void Get(T& value)
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
#endif
	inline void Get(int& value)
	{
		value = val.i;
	}
	inline void Get(float& value)
	{
		value = val.f;
	}
	inline void Get(glm::vec1& value)
	{
		value = val.v1;
	}
	inline void Get(glm::vec2& value)
	{
		value = val.v2;
	}
	inline void Get(glm::vec3& value)
	{
		value = val.v3;
	}
	inline void Get(glm::vec4& value)
	{
		value = val.v4;
	}
	inline void Get(glm::mat2& value)
	{
		value = val.m2;
	}
	inline void Get(glm::mat3& value)
	{
		value = val.m3;
	}
	inline void Get(glm::mat4& value)
	{
		value = val.m4;
	}
	int location = -1;
	std::string_view name;
	Type type;
	bool dirty;
};

struct IIncluder
{
	virtual std::string_view OnInclude(std::string_view file) = 0;
};

class BasicIncluder : public IIncluder
{
	std::string_view OnInclude(std::string_view file) override
	{
        return "";
	}
};

struct IShader
{
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

	virtual bool Create()			= 0;
	virtual bool Compile()			= 0;
	virtual bool Bind()				= 0;
	virtual bool Empty()			= 0;
	virtual IShader::Type GetType() = 0;
	virtual void print()			= 0;
	virtual const char* typeToStr() = 0;
	virtual const char* getName()   = 0;
	virtual uint get()				= 0;
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
		ShaderInfo(IShader* shader, const std::string& name)
			: shader(shader),
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
	virtual int Reload() = 0;

	virtual bool Create(const char* label)								 = 0;
	virtual void Attach(ShaderInfo& shader)								 = 0;
	virtual ShaderInfo& attachInternal(ShaderInfo& src, ShaderInfo& dst) = 0;
	virtual void Detach(ShaderInfo& shader)								 = 0;
	virtual bool Dispatch(int x, int y, int z, uint barriers)			 = 0;
	virtual bool DispatchInderect()										 = 0;
	virtual bool Link()													 = 0;
	virtual void Use()													 = 0;
	virtual void Unuse()												 = 0;
	virtual void DeleteProgram()										 = 0;
	virtual int GetUniformLocation(const char* format, ...)				 = 0;
	virtual int GetUniformLocation(std::string& name)					 = 0;
	virtual const char* GetShaderName(IShader::Type type)				 = 0;
	//UniformValue GetUniformValue(const char* name);
	void Uniform(bool value, const char* format, ...)
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
	virtual void Uniform(const glm::ivec4 value, const char* format, ...)   = 0;
	virtual void Uniform(const ITexture* texture, const char* format, ...)  = 0;

	template<typename T>
	void Uniform(const T value, std::string name)
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
		: name(name)
	{
	}
	ShaderDesc(std::string name, IShader::Type type)
		: type(type), name(name)
	{
	}

	inline void AddMacro(const std::string& key, const std::string& value)
	{
		macro.insert(Macro::value_type(key, value));
	}

	using Macro = std::map<std::string, std::string>;
	std::string name;
	IShader::Type type;
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

IShaderProgram* GetGLSLSandBoxShader();

