#include "Effect.hpp"

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

void CTechnique::GetNumPasses()
{
}

bool CTechnique::CompilePass(int i)
{
	return false;
}
