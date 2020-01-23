#pragma once

#include <BlackBox/Renderer/ITechnique.hpp>
#include <BlackBox/Renderer/ScreenShader.hpp>
#include <BlackBox/IPostProcessor.hpp>
#include <BlackBox/Renderer/Quad.hpp>

class FrameBufferObject;
struct ICVar;

class BloomTechnique : public ITechnique, public IPostProcessor
{
public:
  BloomTechnique();
  ~BloomTechnique();

  // Inherited via ITechnique
  virtual bool Init(Scene* scene, FrameBufferObject* renderTarget) override;

  virtual bool OnRenderPass(int pass) override;

  virtual int GetFrame() override;
private:
  bool BloomPass();
  void createShader();

private:
  ITechnique* shadowMapping;
  CBaseShaderProgram* m_ScreenShader;
  //ScreenShader *m_ScreenShader;
  Quad m_ScreenQuad;
  FrameBufferObject* m_HdrBuffer;
  Scene* m_Scene;

  ICVar* exposure;
  ICVar* enabled;

  bool inited = false;

  // Inherited via IPostProcessor
  virtual void Do(unsigned int texture) override;
};