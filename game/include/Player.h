#ifndef CPLAYER_H
#define CPLAYER_H

#include <BlackBox/Camera.hpp>
#include <GameObject.hpp>
#include <BlackBox/ICommand.hpp>
#include <BlackBox/IInput.hpp>
#include <BlackBox/MathHelper.hpp>
#ifdef GUI
#include <imgui-SFML.h>
#include <imgui.h>
#endif

#include <map>
#include <queue>

class CGame;
typedef std::queue<ICommand*> CommandQueue;


class CPlayer : public GameObject
{
  friend class GameGUI;
  CCamera *m_Camera;
  const float SCROLL_SPEED = 2.0f;
  const float MOUSE_SPEED = 1.5f;
  const float MOUSE_SENSIVITY = 0.05f;
  CGame *Game;
  CommandQueue m_commandQueue;
  std::set<EKeyId> m_keys;
  friend class CGame;
  enum MouseState
  {
    LOCKED,
    FREE
  }mouseState;
  Vec2 delta;
  Vec3 impulse;

public:
  CPlayer(CGame *game);
  CPlayer(Object *obj);

  // IInputEventListener interface
public:
  virtual bool OnInputEvent(const SInputEvent& event) override;
  bool OnKeyPress(EKeyId key);
  bool OnKeyReleas(EKeyId key);

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
