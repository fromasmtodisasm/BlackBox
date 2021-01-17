#pragma once
//#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Utils/smartptr.hpp>
#include <map>
#include <string>
#include <string_view>
#include <utility>

#include <BlackBox/Renderer/VertexFormats.hpp>

struct ITexture;
class CCamera;

struct IShader;

using ShaderRef = _smart_ptr<IShader>;

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

enum EEfResTextures
{
	EFTT_DIFFUSE = 0,
	EFTT_NORMALS,
	EFTT_SPECULAR,
	EFTT_ENV,
	EFTT_DETAIL_OVERLAY,
	EFTT_SMOOTHNESS,
	EFTT_HEIGHT,
	EFTT_DECAL_OVERLAY,
	EFTT_SUBSURFACE,
	EFTT_CUSTOM,
	EFTT_CUSTOM_SECONDARY,
	EFTT_OPACITY,
	EFTT_TRANSLUCENCY,
	EFTT_EMITTANCE,

	EFTT_MAX,
	EFTT_UNKNOWN = EFTT_MAX
};



enum class ShaderLangId
{
	None,
	Glsl,
	Hlsl
};

struct IShader
{
  public:
	enum Type : int
	{
		E_VERTEX,
		E_GEOMETRY,
		E_FRAGMENT,
		E_NUM
	};
	virtual ~IShader() {}
	virtual int GetID()							= 0;
	virtual void AddRef()						= 0;
	virtual int Release()						= 0;
	virtual bool Reload()						= 0;
	virtual IShader::Type GetType()				= 0;
	virtual const char* GetName()				= 0;
	virtual eVertexFormat GetVertexFormat(void) = 0;
	virtual void Bind()							= 0;
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
