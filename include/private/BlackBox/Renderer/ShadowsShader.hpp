#pragma once
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/Shader.hpp>

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