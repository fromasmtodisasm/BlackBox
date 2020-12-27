#pragma once
#include "FxParser.h"
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IShader.hpp>

class CTechnique : public ITechnique
{
public:
	// Inherited via ITechnique
	virtual int GetNumPasses() override;

	// Inherited via ITechnique
	virtual bool CompilePass(int i) override;
	virtual SPass* GetPass(int i) override;

	std::string Name;
	//std::vector<std::string_view> CommonCode;
	std::vector<SPass> Passes;
};

class CEffect: public IEffect
{
public:
	CEffect(std::string_view name) : m_name(name) {}
	virtual int GetNumShaders() override;
	virtual ShaderInfo GetShader(int i) override;
	virtual IShader* GetShader(const char* name) override;

	virtual int GetNumTechniques() override;
	virtual ITechnique* GetTechnique(int i) override;


	void shader_assignment(IShader::Type type, const string& name)
	{
		auto &tech = m_Techniques.back();
		auto &pass = tech.Passes.back();
		auto &sh_name = name;
		for (int i = 0; i < m_shaders.size(); i++)
		{
			if (m_shaders[i].name == sh_name)
			{
				pass.Shaders[type] = m_shaders[i].data;
				break;
			}
		}
		CryLog("ident for shader_type: %s", name.data());
	}

public:
	std::string m_name;
	std::vector<ShaderInfo> m_shaders;
	std::vector<CTechnique> m_Techniques;

	// Inherited via IEffect
	virtual const char* GetName() override;
};
