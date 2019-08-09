#pragma once

#include <BlackBox/Render/Technique.hpp>
#include <BlackBox/Scene.hpp>


class ShadowMapping : public Technique, public ForEachObjectSink
{
  enum Pass
  {
    RENDER_DEPTH,
    RENDER_SHADOWS
  };
  enum Stage
  {
    RENDER_DEPTH,
    RENDER_OPAQUE,
    RENDER_TRANSPARENT
  };
public:
  ShadowMapping();
  ~ShadowMapping();

  // Inherited via Technique
  virtual bool Init(Scene* scene) override;
  virtual bool OnRenderPass(int pass) override;

private:
  void DepthPass();
  void RenderPass();
  void RenderDepth(Object* object);
  void RenderOpaque(Object* object);
  void RenderTransparent(Object* object);

  // Inherited via ForEachObjectSink
  virtual bool OnObjectFound(Object* object) override;

private:
  Scene* m_Scene;
  FrameBufferObject* m_DepthBuffer;
  CBaseShaderProgram* m_ShadowMapShader;
  glm::mat4 lightSpaceMatrix;
  const int width = 1024;
  const int height = 1024;
  //====================
	ICVar* s_divider = nullptr;
	ICVar* lightPosX = nullptr;
	ICVar* lightPosY = nullptr;
	ICVar* lightPosZ = nullptr;

  Pass renderPass;
  Stage renderStage;
};