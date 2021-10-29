#include "Effect.hpp"
//#include <BlackBox/Renderer/IRender.hpp>
//#include <BlackBox/Renderer/BaseShader.hpp>

int CEffect::GetNumShaders()
{
	return m_shaders.size();
}
IEffect::ShaderInfo CEffect::GetShader(int i)
{
	return m_shaders[i];
}
IShader* CEffect::GetShader(const char* name)
{
	return nullptr;
}

int CEffect::GetNumTechniques()
{
	return m_Techniques.size();
}

ITechnique* CEffect::GetTechnique(int i)
{
	assert(i >= 0 && i < m_Techniques.size());
	return &m_Techniques[i];
}

ITechnique* CEffect::GetTechnique(const char* name, size_t size)
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

const char* CEffect::GetName()
{
	return m_name.data();
}

ShaderLangId CEffect::GetLangId()
{
	return m_LangId;
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
