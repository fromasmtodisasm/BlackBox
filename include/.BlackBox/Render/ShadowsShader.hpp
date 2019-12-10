#pragma once
#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>

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