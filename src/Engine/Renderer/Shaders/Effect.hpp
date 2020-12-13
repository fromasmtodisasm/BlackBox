#pragma once
#include "FxParser.h"
class CEffect: public IEffect
{
public:
	// Inherited via IEffect
	virtual int GetNumShaders() override;
	virtual ShaderInfo GetShader(int i) override;
	virtual IShader* GetShader(const char* name) override;

public:
	std::vector<ShaderInfo> m_shaders;
};
