#pragma once
#include <BlackBox/Core/MathHelper.hpp>

class CBaseShaderProgram;
struct Material;
struct IScriptObject;
class Texture;

enum Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  DOWN,
  UP
};
struct IStatObj
{
  virtual void move(Movement direction) = 0;
  virtual void rotate(float angle, Vec3 v) = 0;
  virtual void rotateX(float angle) = 0;
  virtual void rotateY(float angle) = 0;
  virtual void rotateZ(float angle) = 0;
  virtual void scale(Vec3 v) = 0;
  virtual void update(float deltatime) = 0;
  virtual void Render(SRenderParams& renderParams) = 0;

  virtual Material* getMaterial() = 0;
  virtual void setMaterial(Material* material) = 0;

  virtual void setTexture(Texture* texture, const char* type) = 0;
  virtual Mat4 getTransform() = 0;

  virtual void setVisibility(bool flag) = 0;
  virtual bool visible() = 0;

  virtual void SetScriptObject(IScriptObject* pObject) = 0;
  virtual IScriptObject* GetScriptObject() = 0;
};