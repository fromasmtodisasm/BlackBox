#pragma once
#include <BlackBox/Core/Primitives.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <set>

#if 0
class GameObject : /*public Object, */public IInputEventListener
{
protected:
  float MOVE_SPEED = 0.5f;
  CCamera *m_Camera;

  GameObject(Primitive::Type type);
public:
  GameObject(const Object *obj);
  virtual bool OnInputEvent(const SInputEvent& event) override;

  // IStatObj interface
public:
  //virtual void update(float deltatime) override;

  // IStatObj interface
public:
  //virtual glm::mat4 getTransform() override;
};
#endif
