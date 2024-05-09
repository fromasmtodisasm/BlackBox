#include "pch.hpp"
#include "Effect.hpp"
//#include <BlackBox/Renderer/IRender.hpp>
//#include <BlackBox/Renderer/BaseShader.hpp>

IShader* FxEffect::GetShader(const char* name)
{
	return nullptr;
}

int FxEffect::GetNumTechniques()
{
	return m_Techniques.size();
}

ITechnique* FxEffect::GetTechnique(int i)
{
	assert(i >= 0 && i < m_Techniques.size());
	return &m_Techniques[i];
}

ITechnique* FxEffect::GetTechnique(const char* name, size_t size)
{
	for (size_t i = 0; i < m_Techniques.size(); i++)
	{
		if (std::string_view(m_Techniques[i].Name) == std::string_view(name, size))
		{
			return &m_Techniques[i];
		}
	}
	return nullptr;
}

const char* FxEffect::GetName()
{
	return m_name.data();
}

ShaderLangId FxEffect::GetLangId()
{
	return m_LangId;
}

const char* FxEffect::GetCode()
{
	return m_Code.data();
}

int CTechnique::GetNumPasses()
{
	return Passes.size();
}

bool CTechnique::CompilePass(int i)
{
	return false;
}

SPass* CTechnique::GetPass(int i)
{
	return &Passes[i];
}

int CTechnique::GetId()
{
	return Id;
}

const char* CTechnique::GetName()
{
	return Name.data();
}
