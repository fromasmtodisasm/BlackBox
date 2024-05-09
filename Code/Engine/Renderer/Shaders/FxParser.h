#pragma once #include <BlackBox/Renderer/IShader.hpp>
#include <array>
#ifdef DX_11
#include <d3d11.h>
#endif
struct IShader;
namespace fx
{
//%token ZENABLE
//%token ZWRITEENABLE
//%token ZFUNC
//%token CULLFACE
//%token CULLFACE_MODE
//%token BLEND
//%token BLEND_SRC
//%token BLEND_DST
//%token BLEND_EQUATION
//%token BLEND_SRC_ALPHA
//%token BLEND_DST_ALPHA
//%token BLEND_EQUATION_ALPHA
//%token BLEND_COLOR
enum class ERenderState
{
	ALPHABLENDENABLE,
	ALPHABLEND_COLOR,
	ALPHABLEND_DST,
	ALPHABLEND_DST_ALPHA,
	ALPHABLEND_EQUATION,
	ALPHABLEND_EQUATION_ALPHA,
	ALPHABLEND_SRC,
	ALPHABLEND_SRC_ALPHA,
	ALPHAFUNC,
	ALPHAREF,
	BLENDENABLE,
	BLENDOP,
	CULLMODE,
	FILLMODE,
	DESTBLEND,
	SRCBLEND,
	STENCILENABLE,
	STENCILFAIL,
	STENCILFUNC,
	STENCILMASK,
	STENCILPASS,
	STENCILZFAIL,
	ZENABLE,
	ZFUNC,
	ZWRITEENABLE,
	ZWRITEMASK,
	COLORWRITEENABLE,

	FRONTCOUNTERCLOCKWISE,
	DEPTHBIAS,
	DEPTHBIASCLAMP,
	SLOPESCALEDDEPTHBIAS,
	DEPTHCLIPENABLE,
	SCISSORENABLE,
	MULTISAMPLEENABLE,
	ANTIALIASEDLINEENABLE,


};
} // namespace fx

struct SRenderStateValue
{
	fx::ERenderState Type;
	union
	{
		int		i;
		float	f;
		float f4[4];
		float f3[3];
		bool b;
	};
};

namespace nvFX {
/*************************************************************************/ /**
** \brief IUniform Parameter interface
**/ /*************************************************************************/ 
class IUniform 
{
protected:
	IUniform() {}
public:
	virtual ~IUniform() {}
	enum Type {
		TUndefined = 0,
		TInt, TInt2, TInt3, TInt4,
		//UInt,
		TBool, TBool2, TBool3, TBool4,
		TFloat, TVec2, TVec3, TVec4,
		TMat2, TMat2x4, TMat3, TMat34,TMat4, 
		TUBO, // Uniform Buffer Object
		TCB,  // Constant Buffer (D3D)
		TUniform,
		TTexture, // a generic texture : for example when the app created this uniform to bind a texture to a sampler
		// Note: texture==sampler in this type list. D3D uses texture objects, OpenGL will use samplers...
		// Those typed texture/GL-samplers are when the parser encoutered an external declaration of uniform : it needs to know the type
		TTexture1D,
		TTexture2D,
		TTexture2DShadow,
		TTexture2DRect,
		TTexture3D,
		TTextureCube,
		TTexUnit, //TODO: check about this... used for the Cg case (can't assign texunit)
		// TODO : add missing cases
#ifndef OGLES2
		TSubroutineUniform,
		TSubroutineUniformByID,
#endif
	};
	enum PrecisionType /// precision is only used for OpenGL ES
	{
		PHighP = 0,
		PMediumP = 1,
		PLowP = 2,
	};
};
inline std::string toString(IUniform::Type type)
{
	switch(type)
	{
		case nvFX::IUniform::TFloat: return "float";
		case nvFX::IUniform::TVec2: return "vec2";
		case nvFX::IUniform::TVec3: return  "vec3";
		case nvFX::IUniform::TVec4: return  "vec4";
		case nvFX::IUniform::TInt: return  "int";
		case nvFX::IUniform::TInt2: return  "ivec2";
		case nvFX::IUniform::TInt3: return  "ivec3";
		case nvFX::IUniform::TInt4: return  "ivec4";
		case nvFX::IUniform::TBool: return  "bool";
		case nvFX::IUniform::TBool2: return  "bvec2";
		case nvFX::IUniform::TBool3: return  "bvec3";
		case nvFX::IUniform::TBool4: return  "bvec4";
		case nvFX::IUniform::TMat2: return  "mat2";
		case nvFX::IUniform::TMat3: return  "mat3";
		case nvFX::IUniform::TMat4: return  "mat4";
		default: assert(0); return "unknown_type";

	}

}
}

struct DirectDeclarator
{
	string Name;
};

using StringList = std::vector<std::string>;

enum class StorageClass
{
	Static,
	Extern,
	Auto,
	Register,
	Typedef
};

struct SObjectTypeInfo
{
	StorageClass Storage;
	std::string Name;
	nvFX::IUniform::Type Type;
};

struct SAnnotation
{
	std::string Name;
	std::string Value;
};

struct SVariable
{
	std::string Name;
	std::string Spec;
	SObjectTypeInfo Type;
	std::string Value;
	std::string Semantic;
	std::vector<SAnnotation> Annotations;
};

using SVariableList = std::vector<SVariable>;

struct SFunction
{
	std::string Name;
	SObjectTypeInfo ReturnType;
	SVariableList Arguments;
	std::string Semantic;
	//SVariableList LocalVariables;
	//std::vector<std::string> Body;

	void Dump()
	{
		CryLog("Function: %s", Name.c_str());
		CryLog("Return type: %s", ReturnType.Name.c_str());
		for (auto& arg : Arguments)
		{
			CryLog("Arg: %s %s %s", arg.Type.Name.c_str(), arg.Name.c_str(), arg.Value.c_str());
		}
	}
};

using SFunctionList = std::vector<SFunction>;
struct IPass
{
};

struct SimpleValue
{
	nvFX::IUniform::Type type;
	union
	{
		int		i;
		float	f;
		bool b;
	};
};

// declare render state structs
struct SRenderState
{
	fx::ERenderState Type;
	SimpleValue value;
};


struct SPass
{
	std::string				   Name;
	std::vector<std::string>   InputLayout;
	std::array<std::string, 6> EntryPoints;
	std::vector<SRenderStateValue>  RenderStates;
};

struct ITechnique : public _i_reference_target_t
{
	virtual int			GetNumPasses()	   = 0;
	virtual bool		CompilePass(int i) = 0;
	virtual SPass*		GetPass(int i)	   = 0;
	virtual int			GetId()			   = 0;
	virtual const char* GetName()		   = 0;
};

struct STechnique
{
};

struct IEffect : public _i_reference_target_t
{
	struct ShaderInfo
	{
		string name;
		string data;
	};
	virtual ~IEffect()												 = default;
	virtual const char*	 GetCode()									 = 0;
	virtual const char*	 GetName()									 = 0;
	virtual IShader*	 GetShader(const char* name)				 = 0;
	virtual int			 GetNumTechniques()							 = 0;
	virtual ITechnique*	 GetTechnique(int i)						 = 0;
	virtual ITechnique*	 GetTechnique(const char* name, size_t size) = 0;
	virtual ShaderLangId GetLangId()								 = 0;
};


class CTechnique : public ITechnique
{
  public:
		CTechnique() = default;
	CTechnique(int id, std::string name)
		: Name(name)
		, Id(id)
	{
	}
	CTechnique(std::string name)
		: Name(name)
		, Id(-1)
	{
	}
	// Inherited via ITechnique
	virtual int			GetNumPasses() override;
	virtual bool		CompilePass(int i) override;
	virtual SPass*		GetPass(int i) override;
	virtual int			GetId() override;
	virtual const char* GetName() override;;

	std::string Name;
	int			Id;
	//std::vector<std::string_view> CommonCode;
	std::vector<SPass> Passes;
};

class FxEffect : public IEffect
{
  public:
	FxEffect() = default;
	FxEffect(std::string_view name)
		: m_name(name)
	{
	}
	virtual IShader*   GetShader(const char* name) override;

	virtual int			GetNumTechniques() override;
	virtual ITechnique* GetTechnique(int i) override;
	virtual ITechnique* GetTechnique(const char* name, size_t size) override;

	void AddTechnique(CTechnique&& tech);

	void AddVariable(const SVariable& var)
	{
		m_Variables.push_back(std::move(var));
	}

	void AddFunction(const SFunction& func)
	{
		m_Functions.push_back(std::move(func));
	}

	void AddPass(const SPass& pass)
	{
		auto& tech = m_Techniques.back();
		tech.Passes.push_back(pass);
	}

	auto& GetCurrentPass()
	{
		auto& tech			   = m_Techniques.back();
		auto& pass			   = tech.Passes.back();
		return pass;
	}

	void shader_assignment(IShader::Type type, const string& name)
	{
		auto& pass = GetCurrentPass();
		pass.EntryPoints[type] = name;
	}

	void render_state_assignment(const SRenderStateValue& state)
	{
		auto& pass = GetCurrentPass();
		pass.RenderStates.push_back(state);
	}


	bool SetLang(ShaderLangId id)
	{
		if (m_LangId != ShaderLangId::None)
			return false;
		m_LangId = id;
		return true;
	}

  public:
	std::string				m_name;
	//std::vector<ShaderInfo> m_shaders;
	SVariableList			m_Variables;
	SFunctionList			m_Functions;
	std::vector<CTechnique> m_Techniques;
	ShaderLangId			m_LangId = ShaderLangId::None;
	std::string				m_Code;

	// Inherited via IEffect
	virtual const char* GetName() override;

	// Inherited via IEffect
	virtual ShaderLangId GetLangId() override;

	// Inherited via IEffect
	virtual const char* GetCode() override;
};

struct FxParser
{
	FxParser();
	~FxParser();
	bool Parse(const std::string& f, FxEffect** pEffect);
};
