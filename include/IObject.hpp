#pragma once

struct IObject
{
  virtual void move() = 0;
  virtual void rotate() = 0;
  virtual void draw() = 0;
};
