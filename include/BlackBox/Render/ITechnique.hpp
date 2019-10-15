#pragma once

class Scene;
class FrameBufferObject;

struct ITechnique
{
  virtual bool Init(Scene* scene, FrameBufferObject *renderTarget) = 0;
  virtual bool PreRenderPass() = 0;
  virtual bool OnRenderPass(int pass) = 0;
  virtual void PostRenderPass() = 0;
  virtual int  GetFrame() = 0;
  virtual int  SetRenderTarget(FrameBufferObject *renderTarget) = 0;
	virtual const char* GetName() { return "Unnamed technique"; };
};