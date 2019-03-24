#pragma once
#include <glm/fwd.hpp>

enum OBJType {
  TCAM,
  TPRIMITIVE //, // triangle, circle
  // ...
};

struct IObject
{
  virtual void move(glm::vec3 v) = 0;
  virtual void rotate(float angle, glm::vec3 v) = 0;
  virtual void scale(glm::vec3 v) = 0;
  virtual void draw() = 0;
  virtual OBJType getType() = 0;
};
