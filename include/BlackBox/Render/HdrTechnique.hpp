#pragma once

#include <BlackBox/Render/ITechnique.hpp>
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

  virtual bool OnRenderPass(int pass) override;

  virtual int GetFrame() override;

  virtual bool PreRenderPass() override;

  virtual void PostRenderPass() override;

	void downsampling();
	void upsampling();
  /////////////////////////////////////////////////
  virtual void Do(unsigned int texture) override;
private:
  bool HdrPass();
	void BloomPass();
  void createShader();
	void initConsoleVariables();
	void initTest();
	int getMips(glm::vec2 resolution);

private:
	static const int PASSES = 4;
  ITechnique *shadowMapping;
  BaseShadeProgramrRef m_ScreenShader;
  BaseShadeProgramrRef m_DownsampleShader;
  BaseShadeProgramrRef m_UpsampleShader;
  //ScreenShader *m_ScreenShader;
	Quad m_ScreenQuad;
  FrameBufferObject* hdrBuffer;
	FrameBufferObject* scene;
	std::vector<FrameBufferObject*> pass0;
	std::vector<FrameBufferObject*> pass1;
	FrameBufferObject* callOfDutySample;
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

  bool inited = false;
	bool pingpong = false;

	GLuint timer_queries[3];

  IRender* render;

	std::vector<glm::vec2> bloomTest;
	int testid;
};