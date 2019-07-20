#pragma once
class CCamera;
struct IDrawable
{
  virtual void draw(void *data) = 0;
};
