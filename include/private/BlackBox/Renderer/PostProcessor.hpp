#pragma once
#include <BlackBox/Renderer/IPostProcessor.hpp>
#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Quad.hpp>

class PostProcessor : public IPostProcessor
{
public:
  PostProcessor(const char* shader);
  ~PostProcessor();

private:
  BaseShaderProgramRef m_ScreenShader;
  Quad m_ScreenQuad;

  // Inherited via IPostProcessor
  virtual void Do(unsigned int texture) override;
};