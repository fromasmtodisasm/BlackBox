#pragma once

#include <BlackBox/Render/Shader.hpp>
#include <BlackBox/Render/OpenglDebug.hpp>
#include <BlackBox/Render/Pipeline.hpp>

class ShadowMapShader : public CBaseShaderProgram
{
  static const char* vs;
  static const char* fs;
public:
	ShadowMapShader();
	~ShadowMapShader();


private:
	ISystem* m_Engine;

	// Inherited via CBaseShaderProgram
	virtual void setup() override;
};