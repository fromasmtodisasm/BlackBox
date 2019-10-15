#pragma once

#include <BlackBox/Render/ITechnique.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Render/ScreenShader.hpp>
#include <BlackBox/IPostProcessor.hpp>
#include <BlackBox/Quad.hpp>
#include <vector>

class FrameBufferObject;
struct ICVar;

class HdrTechnique : public ITechnique, public IPostProcessor
{
public:
  HdrTechnique();
  ~HdrTechnique();

  // Inherited via ITechnique
  /////////////////////////////////////////////////
  virtual bool Init(Scene* scene, FrameBufferObject* renderTarget) override;

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
	void initConsoleVariables();
	void initTest();
	int getMips(glm::vec2 resolution);
	void CreateCommands();

private:
	static const int PASSES = 4;
  ITechnique *shadowMapping;
  BaseShaderProgramRef m_ScreenShader;
  BaseShaderProgramRef m_DownsampleShader;
  BaseShaderProgramRef m_DownsampleComputeShader;
  BaseShaderProgramRef m_UpsampleShader;
  BaseShaderProgramRef m_UpsampleShaderComputeShader;
  //ScreenShader *m_ScreenShader;
	Quad m_ScreenQuad;
  FrameBufferObject* hdrBuffer;
	std::vector<FrameBufferObject*> pass0;
	std::vector<FrameBufferObject*> pass1;
  Scene* m_Scene;

  ICVar* exposure;
  ICVar* enabled;
	ICVar* bloom;
	ICVar* bloomThreshold;
	ICVar* blurOn;
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

  IRender* render;

	std::vector<glm::vec2> bloomTest;
	int testid;
	std::shared_ptr<SDispFormat> m_DispFormats;

	// Inherited via ITechnique
	virtual int SetRenderTarget(FrameBufferObject* renderTarget) override;
};