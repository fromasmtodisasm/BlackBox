#pragma once
class CCamera;
struct IDrawable
{
  virtual void draw(CCamera *camera) = 0;
};
