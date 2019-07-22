#pragma once
#include <glm/fwd.hpp>
#include <BlackBox/Render/Texture.hpp>

class CBaseShaderProgram;
struct Material;

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

  virtual CBaseShaderProgram *getShaderProgram() = 0;
  virtual void setShaderProgram(CBaseShaderProgram* shader) = 0;
  virtual Material *getMaterial() = 0;
  virtual void setMaterial(Material *material) = 0;

  virtual void setTexture(Texture* texture, const char *type) = 0;
  virtual glm::mat4 getTransform() = 0;
};
