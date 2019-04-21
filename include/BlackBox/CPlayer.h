#ifndef CPLAYER_H
#define CPLAYER_H

#include "GameObject.hpp"
#include "HackCamera.hpp"
#include "IGame.hpp"


class CPlayer : public GameObject
{
  CCamera *m_Camera;
  const float SCROLL_SPEED = 2.0f;
  const float MOUSE_SPEED = 1.5f;
  const float MOUSE_SENSIVITY = 0.05f;
public:
  CPlayer();

  // IInputEventListener interface
public:
  virtual bool OnInputEvent(sf::Event &event) override;

  // IDrawable interface
public:
  virtual void draw() override;

public:
  void attachCamera(CCamera *camera);
  glm::vec3 getPos();

  // IObject interface
public:
  virtual void update(float deltatime) override;
};

#endif // CPLAYER_H
