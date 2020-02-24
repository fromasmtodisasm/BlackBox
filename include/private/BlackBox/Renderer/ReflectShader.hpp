#pragma once
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/Shader.hpp>

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