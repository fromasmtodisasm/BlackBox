#ifndef CPLAYER_H
#define CPLAYER_H

#include "GameObject.hpp"


class CPlayer : public GameObject
{
public:
  CPlayer();

  // IInputEventListener interface
public:
  virtual bool OnInputEvent(sf::Event &event) override;

  // IDrawable interface
public:
  virtual void draw() override;
};

#endif // CPLAYER_H
