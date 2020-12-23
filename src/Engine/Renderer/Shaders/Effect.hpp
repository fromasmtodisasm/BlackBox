#pragma once
#include "FxParser.h"

struct SPass
{
	std::string Name;
	std::string CommonCode;
	std::array<std::string, 6> Shaders;
};

class CTechnique : public ITechnique
{
public:
	// Inherited via ITechnique
	virtual void GetNumPasses() override;

	// Inherited via ITechnique
	virtual bool CompilePass(int i) override;

	std::string Name;
	std::vector<SPass> Passes;
};

class CEffect: public IEffect
{
public:
	// Inherited via IEffect
	virtual int GetNumShaders() override;
	virtual ShaderInfo GetShader(int i) override;
	virtual IShader* GetShader(const char* name) override;

public:
	std::vector<ShaderInfo> m_shaders;
	std::vector<CTechnique> m_Techniques;
};
