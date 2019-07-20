#pragma once
#include <BlackBox/Render/CShader.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>

class ReflectShader : public CShaderProgram
{
public:
	ReflectShader();
	~ReflectShader();


private:
	IEngine* m_Engine;
};