#pragma once

#include <BlackBox/Render/ITechnique.hpp>
#include <BlackBox/Scene.hpp>


class ShadowMapping 
  :
  public ITechnique, 
  public ForEachObjectSink,
  public ForEachDirectionLightSink,
  public ForEachPointLightSink,
  public ForEachSpotLightSink
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
  virtual bool Init(Scene* scene, FrameBufferObject *renderTarget) override;
  virtual bool PreRenderPass() override;
  virtual bool OnRenderPass(int pass) override;
  virtual void PostRenderPass() override;
  virtual int GetFrame() override;

  // Inherited via ForEachObjectSink
  virtual bool OnObjectFound(Object* object) override;

  // Inherited via ForEachDirectionLightSink
  virtual bool OnLightFound(DirectionLight* light) override;
  // Inherited via ForEachPointLightSink
  virtual bool OnLightFound(PointLight* light) override;
  // Inherited via ForEachSpotLightSink
  virtual bool OnLightFound(SpotLight* light) override;

private:
  void DepthPass();
  void RenderPass();
  void RenderDepth(Object* object);
  void RenderOpaque(Object* object);
  void RenderTransparent(Object* object);

  void OnDepthPass();
  void OnRenderPass();
  void SetupLights(Object *object);
  inline void SetupDirectionLights();
  inline void SetupPointLights();
  inline void SetupSpotLights();
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
	ICVar* lighting = nullptr;

  bool bLighting = true;

  Pass renderPass;
  Stage renderStage;
  //////////////////////////
  int currentLight = 0;
};