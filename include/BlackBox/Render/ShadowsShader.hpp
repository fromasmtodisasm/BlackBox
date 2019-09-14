#pragma once
#include <BlackBox/Render/CShader.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/Pipeline.hpp>

class ShadowsShader : public CBaseShaderProgram
{
public:
	ShadowsShader();
	~ShadowsShader();


private:
	ISystem* m_Engine;

	// Inherited via CBaseShaderProgram
	virtual void setup() override;
};
