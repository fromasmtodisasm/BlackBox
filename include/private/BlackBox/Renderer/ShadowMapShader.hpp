#pragma once

#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Renderer/OpenGL/Debug.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>

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