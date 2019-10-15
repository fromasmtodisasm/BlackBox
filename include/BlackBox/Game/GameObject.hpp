#pragma once
#include <BlackBox/Primitives.hpp>
#include <BlackBox/InputHandler.hpp>
#include <BlackBox/Camera.hpp>
#include <set>

class GameObject : public Object, public IInputEventListener
{
protected:
  const float MOVE_SPEED = 0.5f;
  CCamera *m_Camera;

  GameObject(Primitive::Type type);
public:
  GameObject(const Object *obj);
  virtual bool OnInputEvent(sf::Event &event) override;

  // IObject interface
public:
  virtual void update(float deltatime) override;

  // IObject interface
public:
  //virtual glm::mat4 getTransform() override;
};
