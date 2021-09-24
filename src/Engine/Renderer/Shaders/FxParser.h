#pragma once
#include <array>
#include <BlackBox/Renderer/IShader.hpp>
struct IShader;
struct IPass
{
	
};

struct SPass
{
	std::string Name;
	std::vector<std::string> InputLayout;
	std::vector<std::string> Code;
	std::array<std::string, 6> EntryPoints;
};


struct ITechnique : public _i_reference_target_t
{
	virtual int GetNumPasses() = 0;
	virtual bool CompilePass(int i)	= 0;
	virtual SPass* GetPass(int i)		= 0;
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
	virtual ~IEffect() = default;
	virtual const char* GetName()				 = 0;
	virtual int GetNumShaders()					 = 0;
	virtual ShaderInfo GetShader(int i)			 = 0;
	virtual IShader* GetShader(const char* name) = 0;
	virtual int GetNumTechniques()				 = 0;
	virtual ITechnique* GetTechnique(int i)		 = 0;
	virtual ShaderLangId GetLangId()			 = 0;
};

typedef struct IEffect* PEffect;

struct FxParser
{
	FxParser();
	bool Parse(const std::string& f, PEffect* pEffect);
};

