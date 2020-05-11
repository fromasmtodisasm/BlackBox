#pragma once

#include <BlackBox/Renderer/ITechnique.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/ScreenShader.hpp>
#include <BlackBox/Renderer/IPostProcessor.hpp>
#include <BlackBox/Renderer/Quad.hpp>
#include <vector>

class FrameBufferObject;
struct ICVar;
struct ILog;

class HdrTechnique : public ITechnique, public IPostProcessor
{
public:
  HdrTechnique();
  ~HdrTechnique();

  // Inherited via ITechnique
  /////////////////////////////////////////////////
  virtual bool Init(IScene* scene, FrameBufferObject* renderTarget) override;

  void CreateFrameBuffers(SDispFormat* format);
  void DeleteFrameBuffers();

  virtual bool OnRenderPass(int pass) override;

  virtual int GetFrame() override;

  virtual bool PreRenderPass() override;

  virtual void PostRenderPass() override;

  void downsampling();
  void downsamplingStandard();
  void downsamplingCompute();
  void upsampling();
  /////////////////////////////////////////////////
  virtual void Do(unsigned int texture) override;
  void SetMode(int n);
private:
  bool HdrPass();
  void BloomPass();
  void createShader();
  void InitConsoleVariables();
  void initTest();
  int getMips(glm::vec2 resolution);
  void CreateCommands();

private:
  static const int PASSES = 4;
  ITechnique* shadowMapping;
  BaseShaderProgramRef m_ScreenShader;
  BaseShaderProgramRef m_DownsampleShader;
  BaseShaderProgramRef m_DownsampleComputeShader;
  BaseShaderProgramRef m_UpsampleShader;
  BaseShaderProgramRef m_UpsampleShaderComputeShader;
  //Quad m_ScreenQuad;
  FrameBufferObject* m_HdrBuffer;
  std::vector<FrameBufferObject*> m_DownsampleBuffer;
  std::vector<FrameBufferObject*> m_UpsampleBuffer;
  IScene* m_Scene;

  ICVar* exposure;
  ICVar* enabled;
  ICVar* bloom;
  ICVar* bloomThreshold;
  ICVar* blurOn;
  ICVar* blurOnly;
  ICVar* useBoxFilter;
  ICVar* defaultFilter;
  ICVar* bloom_exposure;
  ICVar* offset;
  ICVar* bloomTime;
  ICVar* upsampleTime;
  ICVar* downsampleTime;
  ICVar* averageBloomTime;
  ICVar* downsampleType;
  ICVar* cam_width;
  ICVar* cam_height;

  bool inited = false;
  bool pingpong = false;

  GLuint timer_queries[3];

  IRenderer* render;
  ILog* log;

  std::vector<glm::vec2> bloomTest;
  int testid;
  std::shared_ptr<SDispFormat> m_DispFormats;

  GLuint quadCornersVBO;
  glm::vec2 quadCorners;

  int show_all_fb = true;
  int draw_sky = false;

  // Inherited via ITechnique
  virtual int SetRenderTarget(FrameBufferObject* renderTarget) override;
};