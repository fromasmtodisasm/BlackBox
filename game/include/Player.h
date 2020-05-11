#ifndef CPLAYER_H
#define CPLAYER_H

#include <BlackBox/Renderer/Camera.hpp>
#include <GameObject.hpp>
#include <BlackBox/System/ICommand.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Core/MathHelper.hpp>

#include <map>
#include <queue>

class CGame;
typedef std::queue<ICommand*> CommandQueue;


class CPlayer //: public GameObject
{
#if 0
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
  Vec3 impulse = Vec3(0.f, 3.f, 0.f);
  float floor = 1.f;

public:
  CPlayer(CGame *game);
  CPlayer(Object *obj);

  void init();

  // IInputEventListener interface
public:
  //virtual bool OnInputEvent(const SInputEvent& event) override;
  bool OnKeyPress(EKeyId key);
  bool OnKeyReleas(EKeyId key);

  // IDrawable interface
public:
  //virtual void draw(SRenderParams& renderParams) override;

public:
  void attachCamera(CCamera *camera);
  CCamera *getCurrentCamera();
  glm::vec3 getPos();
  void setGame(CGame *game);
  CGame *getGame();

  // IStatObj interface
public:
  //virtual void update(float deltatime) override;
  CPlayer *operator=(Object *obj);
#endif
};

#endif // CPLAYER_H
