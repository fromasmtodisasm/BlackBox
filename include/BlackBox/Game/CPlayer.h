#ifndef CPLAYER_H
#define CPLAYER_H

#include <BlackBox/CCamera.hpp>
#include <BlackBox/Game/GameObject.hpp>
#include <BlackBox/ICommand.hpp>
#ifdef GUI
#include <imgui-SFML.h>
#include <imgui.h>
#endif

#include <map>
#include <queue>

class CGame;
typedef std::map<sf::Keyboard::Key, ICommand*> KeyBindings;
typedef std::queue<ICommand*> CommandQueue;


class CPlayer : public GameObject
{
  friend class GameGUI;
  CCamera *m_Camera;
  const float SCROLL_SPEED = 2.0f;
  const float MOUSE_SPEED = 1.5f;
  const float MOUSE_SENSIVITY = 0.05f;
  CGame *Game;
  KeyBindings m_keyBindings;
  CommandQueue m_commandQueue;
  std::set<sf::Keyboard::Key> m_keys;
  friend class CGame;
  enum MouseState
  {
    LOCKED,
    FREE
  }mouseState;
  sf::Vector2i delta;
  glm::vec3 impulse;

public:
  CPlayer();
  CPlayer(Object *obj);

  // IInputEventListener interface
public:
  virtual bool OnInputEvent(sf::Event &event) override;
  bool OnKeyPress(sf::Event& event);
  bool OnKeyReleas(sf::Event& event);

  // IDrawable interface
public:
  virtual void draw(void *camera) override;

public:
  void attachCamera(CCamera *camera);
  CCamera *getCurrentCamera();
  glm::vec3 getPos();
  void setGame(CGame *game);
  CGame *getGame();

  // IObject interface
public:
  virtual void update(float deltatime) override;
  CPlayer *operator=(Object *obj);
};

#endif // CPLAYER_H
