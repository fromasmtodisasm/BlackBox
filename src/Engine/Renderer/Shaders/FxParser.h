#pragma once
#include <array>
struct IShader;
struct IPass
{
	
};

struct ITechnique : public _i_reference_target_t
{
	virtual void GetNumPasses() = 0;
	virtual bool CompilePass(int i)	= 0;
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
	virtual int GetNumShaders() = 0;
	virtual ShaderInfo GetShader(int i) = 0;
	virtual IShader* GetShader(const char* name) = 0;
};

typedef struct IEffect* PEffect;

struct FxParser
{
	FxParser();
	bool Parse(const std::string& f, PEffect* pEffect);
};

