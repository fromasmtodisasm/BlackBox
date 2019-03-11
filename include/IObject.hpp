#pragma once

enum OBJType {
  TCAM,
  TPRIMITIVE //, // triangle, circle
  // ...
};

struct IObject
{
  virtual void move() = 0;
  virtual void rotate() = 0;
  virtual void draw() = 0;
  virtual OBJType getType() = 0;
};
