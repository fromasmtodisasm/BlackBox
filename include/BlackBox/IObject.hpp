#pragma once
#include <glm/fwd.hpp>
#include <BlackBox/Texture.hpp>

class CShaderProgram;

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
  virtual void update(float deltatime) = 0;
  virtual OBJType getType() = 0;
  virtual void setType(OBJType) = 0;

  virtual CShaderProgram *getShaderProgram() = 0;
  virtual void setShaderProgram(CShaderProgram* shader) = 0;
  virtual void setTexture(Texture* texture) = 0;
  virtual glm::mat4 getTransform() = 0;
};
