#include "Shader.hpp"

int CShader::GetID()
{
	return 0;
}

void CShader::AddRef()
{
}

int CShader::Release()
{
	return 0;
}

IShader::Type CShader::GetType()
{
	return IShader::Type();
}

const char* CShader::GetName()
{
	return nullptr;
}

eVertexFormat CShader::GetVertexFormat(void)
{
	return eVertexFormat();
}
