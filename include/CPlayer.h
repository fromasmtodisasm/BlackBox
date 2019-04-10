#ifndef CPLAYER_H
#define CPLAYER_H

#include "GameObject.hpp"
#include "HackCamera.hpp"


class CPlayer : public GameObject
{
  CCamera *m_Camera;
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
};

#endif // CPLAYER_H
