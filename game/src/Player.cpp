#include <Player.h>
#include <Game.hpp>

#include <BlackBox/Core/Primitives.hpp>
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Renderer/ISceneManager.hpp>
#include <BlackBox/Scene/IScene.hpp>

#include <iostream>

#if 0
CPlayer::CPlayer(CGame* game) //: GameObject(ObjectManager::instance()->getObject("pengium.obj", "player", game)), impulse(0.0f, 3.0f, 0.0f)
{
  //getShaderProgram()->Uniform("color", glm::vec3(1,0,0));
  mouseState = FREE;
  //setMaterial(defaultMaterial);
  init();
}

#if 0
CPlayer::CPlayer(Object* obj) : GameObject(obj), impulse(0.0f, 3.0f, 0.0f)
{
  m_pScript = GetISystem()->GetIScriptSystem()->CreateEmptyObject();
  init();
}
#endif

void CPlayer::init()
{
  //GetISystem()->GetIScriptSystem()->GetGlobalValue("player", m_pScript);
  GetISystem()->GetIConsole()->Register("floor", &floor, floor, 0, "floor height");
}

#if 0
bool CPlayer::OnInputEvent(const SInputEvent& event)
{
  bool mousePressed = event.deviceType == eIDT_Mouse && event.state == eIS_Pressed;
  bool rotated = false;
  //if (event.pSymbol != nullptr)
  rotated = event.keyId == eKI_MouseX || event.keyId == eKI_MouseY;// || event.pSymbol->type == SInputSymbol::EType::Axis;

////////////////////////
  bool keyPressed = event.deviceType == eIDT_Keyboard && event.state == eIS_Pressed;
  bool keyReleased = event.deviceType == eIDT_Keyboard && event.state == eIS_Released;
  bool control = event.modifiers & eMM_Ctrl;
  bool shift = event.modifiers & eMM_Shift;
  bool alt = event.modifiers & eMM_Alt;
  ////////////////////////
  if (false)
  {
    auto obj = SceneManager::instance()->currentScene()->getObject("MyPlayer");
    GameObject* go = new GameObject(obj);
    go->setMaterial(obj->getMaterial());
    go->m_transform.position = m_Camera->GetPos();// + glm::vec3(0,0,5);
    go->velocity = 48.0f * m_Camera->Front;
    Game->getWorld()->GetActiveScene()->addObject("bullet", go);

    return true;
  }
  else if (rotated)
  {
    // TODO: GET DELTA  MOUSE
    delta = Vec2(0);
    if (event.keyId == eKI_MouseX || event.keyId == eKI_XI_ThumbRX || event.keyId == eKI_XI_ThumbLX)
      delta.x = event.value;
    else
      delta.y = event.value;

    m_Camera->ProcessMouseMovement(static_cast<float>(delta.x), -static_cast<float>(delta.y));
    return true;
  }
  else if (keyPressed)
  {
    return OnKeyPress(event.keyId);
  }
  else if (keyReleased)
    return OnKeyReleas(event.keyId);
  else
    return GameObject::OnInputEvent(event);
}

bool CPlayer::OnKeyPress(EKeyId key)
{
  m_keys.insert(key);
  return true;
}

bool CPlayer::OnKeyReleas(EKeyId key)
{
  m_keys.erase(key);
  return false;
}

void CPlayer::draw(SRenderParams& renderParams)
{
  Object::draw(renderParams);
}
#endif

void CPlayer::attachCamera(CCamera* camera)
{
  m_Camera = camera;
}

#if 0
CCamera* CPlayer::getCurrentCamera()
{
  return m_Camera;
}

glm::vec3 CPlayer::getPos()
{
  return m_transform.position;
}
#endif
void CPlayer::setGame(CGame* game)
{
  Game = game;
}

#if 0
CGame* CPlayer::getGame()
{
  return Game;
}

void CPlayer::update(float deltatime)
{
	if (gEnv->IsDedicated())
  {
    return;
  }

  //ImGui
  float mult = m_keys.find(eKI_LShift) != m_keys.end() ? 3.f : 1.f;
  float rotation_speed = deltatime * MOVE_SPEED * mult;
  //float rotSpeed = deltatime * 5.f;//m_rotAngle;
  static Vec3 impulse = Vec3(0.f, 10.f, 0.f);
  float move_speed = deltatime * mult;
  for (auto& key : m_keys)
  {
    switch (key)
    {
    case eKI_Space:
      velocity += impulse;
      break;
    case eKI_W:
      m_Camera->ProcessKeyboard(Movement::FORWARD, move_speed);
      break;
    case eKI_S:
      m_Camera->ProcessKeyboard(Movement::BACKWARD, move_speed);
      break;
    case eKI_A:
      m_Camera->ProcessKeyboard(Movement::LEFT, move_speed);
      break;
    case eKI_D:
      m_Camera->ProcessKeyboard(Movement::RIGHT, move_speed);
      break;
    case eKI_Up:
      m_Camera->ProcessMouseMovement(0.f, rotation_speed);
      break;
    case eKI_Down:
      m_Camera->ProcessMouseMovement(0.f, -rotation_speed);
      break;
    case eKI_Left:
      m_Camera->ProcessMouseMovement(-rotation_speed, 0.f);
      break;
    case eKI_Right:
      m_Camera->ProcessMouseMovement(rotation_speed, 0.f);
      break;
    default:
      ;//GameObject::update(deltatime);
    }
  }
//#if 0
  if (m_Camera->mode == CCamera::Mode::FPS)
  {
    if (m_Camera->transform.position.y < floor)
    {
      velocity.y = 0.f;
      m_Camera->transform.position.y = floor;
    }
    //m_transform.position += velocity * deltatime;
    m_Camera->transform.position -= velocity * deltatime;
  }

//#endif
  //m_Camera->m_target = m_transform.position;
  PROFILER_PUSH_CPU_MARKER("CALL SCRIPT", Utils::COLOR_DARK_GREEN);
  #if 0
  GetISystem()->GetIScriptSystem()->BeginCall("Player", "Update");
  GetISystem()->GetIScriptSystem()->PushFuncParam(m_pScript);
  GetISystem()->GetIScriptSystem()->PushFuncParam(deltatime);
  GetISystem()->GetIScriptSystem()->EndCall();
  #endif
  PROFILER_POP_CPU_MARKER();
}

CPlayer* CPlayer::operator=(Object* obj)
{
  return nullptr;
}
#endif

#endif
