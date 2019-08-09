#pragma once

class Scene;
class FrameBufferObject;

struct ITechnique
{
  virtual bool Init(Scene* scene, FrameBufferObject *renderTarget) = 0;
  virtual bool OnRenderPass(int pass) = 0;
  virtual int  GetFrame() = 0;
};