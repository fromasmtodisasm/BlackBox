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
	return &m_Techniques[i];
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
