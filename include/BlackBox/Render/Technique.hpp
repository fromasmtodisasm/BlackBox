#pragma once

class Scene;

struct Technique
{
  virtual bool Init(Scene* scene) = 0;
  virtual bool OnRenderPass(int pass) = 0;
};