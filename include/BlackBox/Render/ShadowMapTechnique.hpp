#pragma once

#include <BlackBox/Render/ITechnique.hpp>
#include <BlackBox/Scene.hpp>


class ShadowMapping : public ITechnique, public ForEachObjectSink
{
  enum Pass
  {
    RENDER_DEPTHMAP,
    RENDER_SHADOWS
  };
  enum Stage
  {
    RENDER_DEPTH,
    RENDER_OPAQUE,
    RENDER_TRANSPARENT,
    RENDER_COMPLETE
  };
public:
  ShadowMapping();
  ~ShadowMapping();

  // Inherited via ITechnique
  virtual bool Init(Scene* scene) override;
  virtual bool OnRenderPass(int pass) override;

private:
  void DepthPass();
  void RenderPass();
  void RenderDepth(Object* object);
  void RenderOpaque(Object* object);
  void RenderTransparent(Object* object);

  void OnDepthPass();
  void OnRenderPass();

  // Inherited via ForEachObjectSink
  virtual bool OnObjectFound(Object* object) override;

private:
  Scene* m_Scene;
  FrameBufferObject* m_DepthBuffer;
  FrameBufferObject* m_RenderedScene;
  CBaseShaderProgram* m_ShadowMapShader;
  glm::mat4 lightSpaceMatrix;
  glm::vec3 lightPos;
  const int width = 1024;
  const int height = 1024;
  //====================
	ICVar* s_divider = nullptr;
	ICVar* lightPosX = nullptr;
	ICVar* lightPosY = nullptr;
	ICVar* lightPosZ = nullptr;

  Pass renderPass;
  Stage renderStage;

  // Inherited via ITechnique
  virtual int GetFrame() override;
};