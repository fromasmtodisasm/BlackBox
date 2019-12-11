#pragma once

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Renderer/IObject.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/IGeometry.hpp>
#include <BlackBox/Renderer/IDrawable.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <memory>

class Material;
using MeshList = std::shared_ptr<std::vector<Mesh>>;

struct Transform
{
  Vec3 position;
  Vec3 rotation;
  Vec3 scale;

  Transform();
  Transform(Vec3 position, Vec3 rotation, Vec3 scale) :
    position(position), rotation(rotation), scale(scale) {}
};

class Object : public IObject, public IDrawable {
  friend class ObjectManager;
protected:
  _smart_ptr<IShaderProgram> m_Shader;
  Object();
  Object(MeshList mesh);
  Object(const Object* obj);
public:
  static Object* getEmpty()
  {
    return new Object();
  }
  //=============================
  // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
  // Default camera values
  const float YAW = -90.0f;
  const float PITCH = 0.0f;
  const float SPEED = 0.1f;
  const float SENSITIVTY = 0.25f;
  const float ZOOM = 45.0f;
  //*****************************
    // Camera Attributes
  Vec3 Front = Vec3(0.f, 0.f, -1.0f);
  Vec3 Up = Vec3(0.f, 1.f, 0.0f);
  Vec3 Right = Vec3(0.f, 0.f, 0.f);
  Vec3 WorldUp = Vec3(0.f, 1.f, 0.0f);
  // Eular Angles
  float Yaw = YAW;
  float Pitch = PITCH;
  // Camera options
  float MovementSpeed = 2;
  //=============================

  int m_RenderMode = 0;
  MeshList m_Mesh;
  std::string type;
  Material* m_Material = nullptr;
  static int refs;
  std::string m_path;
  float friction = 0.99f;
  Vec3 velocity;
  float gravity = 0.f;// 9.8f;
  bool m_transparent = false;
  bool m_visible = true;

  Transform m_transform;

  Mat4 uvMatrix;// = Mat4(1.0);

  IScriptObject* m_pScript = nullptr;

  static Object* load(std::string path);
  virtual void move(Movement direction) override;
  void move(Vec3 v);
  virtual void moveTo(Vec3 v);
  virtual void rotate(float angle, Vec3 v) override;
  virtual void scale(Vec3 v) override;
  virtual void draw(void* camera) override;
  virtual Mat4 getTransform() override;
  void updateVectors();

  bool visible();
  void setVisibility(bool v);

public:
  virtual void update(float deltatime) override;

public:
  virtual void setTexture(Texture* texture, const char* type) override;

  Object operator=(Object& that);
  Object* clone();
public:
  virtual Material* getMaterial() override;
  virtual void setMaterial(Material* material) override;

  void setRenderMode(int mode);
  int getRenderMode();
  virtual void rotateX(float angle) override;
  virtual void rotateY(float angle) override;
  virtual void rotateZ(float angle) override;
  virtual void SetScriptObject(IScriptObject* pObject) override;
  virtual IScriptObject* GetScriptObject() override;
};