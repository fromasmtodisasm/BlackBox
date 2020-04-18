#pragma once
#include <glm/glm.hpp>
#include <BlackBox/Renderer/BaseTexture.hpp>
#include <BlackBox/Renderer/Shader.hpp>

class Object;

class Pipeline
{
public:
  static Pipeline* instance();
  void bindProgram(ShaderProgramRef program);
  void bindProgram(const char* name);
private:
  Pipeline() = default;
public:
  BaseTexture* skyBox;
  IShaderProgram* shader;
  Object* object;

private:
  static Pipeline* m_instance;
};