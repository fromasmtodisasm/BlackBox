#pragma once
#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/Pipeline.hpp>

class ReflectShader : public CBaseShaderProgram
{
public:
	ReflectShader();
	~ReflectShader();


private:
	ISystem* m_Engine;

	// Inherited via CBaseShaderProgram
	virtual void setup() override;
};