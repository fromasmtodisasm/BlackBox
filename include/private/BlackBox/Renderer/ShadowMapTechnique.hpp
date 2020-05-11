#pragma once

#include <BlackBox/Renderer/ITechnique.hpp>
#include <BlackBox/Scene/Scene.hpp>

class ShadowMapping
  :
  public ITechnique,
  public ForEachObjectSink
  //public ForEachDirectionLightSink,
  //public ForEachPointLightSink,
  //public ForEachSpotLightSink
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
  struct BaseLightValues
  {
    /*UniformValue ambient;
    UniformValue diffuse;
    UniformValue specular;*/
  };

  struct PointLightValues : public BaseLightValues
  {
    /*UniformValue position;
    UniformValue constant;
    UniformValue linear;
    UniformValue quadratic;*/
  };

  struct SpotLightValues : public PointLightValues
  {
    /*UniformValue direction;
    UniformValue cutOff;
    UniformValue outerCutOff;*/
  };

public:
  ShadowMapping();
  ~ShadowMapping();

  // Inherited via ITechnique
  virtual bool Init(IScene* scene, FrameBufferObject* renderTarget) override;
  virtual bool PreRenderPass() override;
  virtual bool OnRenderPass(int pass) override;
  virtual void PostRenderPass() override;
  virtual int GetFrame() override;

  // Inherited via ForEachObjectSink
  //virtual bool OnObjectFound(CStatObj* object) override;

  // Inherited via ForEachDirectionLightSink
  virtual bool OnLightFound(const DirectionLight* light, SRenderParams& renderParams);
  // Inherited via ForEachPointLightSink
  virtual bool OnLightFound(const PointLight* light, SRenderParams& renderParams) ;
  // Inherited via ForEachSpotLightSink
  virtual bool OnLightFound(const SpotLight* light, SRenderParams& renderParams) ;

  void RenderOpaque(CStatObj* object);
  void RenderTransparent(CStatObj* object);
private:
  void DepthPass();
  void RenderPass();
  void RenderDepth(CStatObj* object);

  void OnDepthPass();
  void OnRenderPass();
  void SetupLights(SRenderParams& renderParams);
  inline void SetupDirectionLights(SRenderParams& renderParams);
  inline void SetupPointLights(SRenderParams& renderParams);
  inline void SetupSpotLights(SRenderParams& renderParams);
  void InitLights();
private:
  IRenderer* m_pRender = nullptr;
  Scene* m_Scene;
  FrameBufferObject* m_DepthBuffer;
	ITexture* m_DepthTexture;
  FrameBufferObject* m_RenderedScene;
  BaseShaderProgramRef m_ShadowMapShader;
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

  ICVar* cam_width = nullptr;
  ICVar* cam_height = nullptr;

  bool bLighting = true;

  Pass renderPass;
  Stage renderStage;
  //////////////////////////
  int currentLight = 0;

  //UniformValue m_DirectionLight;
  std::vector<PointLightValues> m_PointLights;
  std::vector<SpotLightValues> m_SpotLights;

  int frame_id = 0;

  // Inherited via ITechnique
  virtual int SetRenderTarget(FrameBufferObject* renderTarget) override;
};