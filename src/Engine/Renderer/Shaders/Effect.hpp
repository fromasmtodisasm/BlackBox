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
		pass.EntryPoints[type] = name;
	}
	bool SetLang(ShaderLangId id)
	{
		if (m_LangId != ShaderLangId::None)
			return false;
		m_LangId = id;
		return true;
	}

public:
	std::string m_name;
	std::vector<ShaderInfo> m_shaders;
	std::vector<CTechnique> m_Techniques;
	ShaderLangId m_LangId = ShaderLangId::None;

	// Inherited via IEffect
	virtual const char* GetName() override;

	// Inherited via IEffect
	virtual ShaderLangId GetLangId() override;
};
