#ifndef CPLAYER_H
#define CPLAYER_H

#include <BlackBox/CCamera.hpp>
#include <BlackBox/GameObject.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

class CGame;


class CPlayer : public GameObject
{
  friend class GameGUI;
  CCamera *m_Camera;
  const float SCROLL_SPEED = 2.0f;
  const float MOUSE_SPEED = 1.5f;
  const float MOUSE_SENSIVITY = 0.05f;
  CGame *Game;
  std::set<sf::Keyboard::Key> m_keys;
  friend class CGame;
  enum MouseState
  {
    LOCKED,
    FREE
  }mouseState;

public:
  CPlayer();
  CPlayer(Object *obj);

  // IInputEventListener interface
public:
  virtual bool OnInputEvent(sf::Event &event) override;

  // IDrawable interface
public:
  virtual void draw(CCamera *camera) override;

public:
  void attachCamera(CCamera *camera);
  glm::vec3 getPos();
  void setGame(CGame *game);

  // IObject interface
public:
  virtual void update(float deltatime) override;
  CPlayer *operator=(Object *obj);
};

#endif // CPLAYER_H
