#include <iostream>

#include <BlackBox/Game/Player.h>
#include <BlackBox/Primitives.hpp>
#include <BlackBox/Game/Game.hpp>
#include <BlackBox/Resources/ObjectManager.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Resources/SceneManager.hpp>
#include <BlackBox/Profiler/Profiler.h>

CPlayer::CPlayer() : GameObject(ObjectManager::instance()->getObject("pengium.obj"))
{
  //getShaderProgram()->Uniform("color", glm::vec3(1,0,0));
  mouseState = FREE;
  setMaterial(defaultMaterial);
	GetISystem()->GetIScriptSystem()->GetGlobalValue("player", m_pScript);
}

CPlayer::CPlayer(Object *obj) : GameObject(obj), impulse(0.0f, 3.0f, 0.0f)
{
	m_pScript = GetISystem()->GetIScriptSystem()->CreateEmptyObject();
	GetISystem()->GetIScriptSystem()->GetGlobalValue("player", m_pScript);
}

bool CPlayer::OnInputEvent(const SInputEvent& event)
{
	bool mousePressed = event.deviceType == eIDT_Mouse && event.state == eIS_Pressed;
	bool mouseMoved = event.deviceType == eIDT_Mouse && (event.keyId == eKI_MouseX || event.keyId == eKI_MouseY);
	////////////////////////
	bool keyPressed = event.deviceType == eIDT_Keyboard && eIS_Down;
	bool control = event.modifiers == eMM_Ctrl;
	bool shift = event.modifiers == eMM_Shift;
	bool alt = event.modifiers == eMM_Alt;
	////////////////////////
  if (mousePressed)
  {
    Object *obj = SceneManager::instance()->currentScene()->getObject("MyPlayer");
    GameObject *go = new GameObject(obj);
    go->setMaterial(obj->getMaterial());
    go->m_transform.position = m_Camera->getPosition();// + glm::vec3(0,0,5);
    go->velocity = 48.0f*m_Camera->Front;
    Game->getWorld()->getActiveScene()->addObject("bullet", go);

    return true;
  }
	else if (mouseMoved)
	{
		// TODO: GET DELTA  MOUSE
    //delta = p_gIGame->getInputHandler()->getDeltaMouse();
    m_Camera->ProcessMouseMovement(static_cast<GLfloat>(delta.x), -static_cast<GLfloat>(delta.y));
    return true;
  }
	else if (keyPressed)
	{
		if (event.state == eIS_Down)
			return OnKeyPress(event.keyId);
		else if (event.state == eIS_Released)
			return OnKeyReleas(event.keyId);
	}
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

void CPlayer::draw(void *camera)
{
  Object::draw(camera);
}

void CPlayer::attachCamera(CCamera *camera)
{
  m_Camera = camera;
  GameObject::m_Camera = camera;
}

CCamera* CPlayer::getCurrentCamera()
{
  return m_Camera;
}

glm::vec3 CPlayer::getPos()
{
  return m_transform.position;
}

void CPlayer::setGame(CGame *game)
{
  Game = game;
}

CGame* CPlayer::getGame()
{
  return Game;
}

void CPlayer::update(float deltatime)
{
  //ImGui
  float speed = deltatime*MOVE_SPEED;
  float rotSpeed = deltatime*5.f;//m_rotAngle;
  for (auto& key : m_keys)
  {
    switch (key)
    {
    case eKI_J:
      velocity += impulse;
      break;
    case eKI_K:
      velocity -= impulse;
      break;
    case eKI_W:
      m_Camera->ProcessKeyboard(Movement::FORWARD, deltatime);
      break;
    case eKI_S:
      m_Camera->ProcessKeyboard(Movement::BACKWARD, deltatime);
      break;
    case eKI_A:
      m_Camera->ProcessKeyboard(Movement::LEFT, deltatime);
      break;
    case eKI_D:
      m_Camera->ProcessKeyboard(Movement::RIGHT, deltatime);
      break;
    default:
      ;//GameObject::update(deltatime);
    }
  }
#if 0
  if (m_transform.position.y < 0)
    velocity.y = - velocity.y*friction;
  m_transform.position += velocity * deltatime;
#endif
  //m_Camera->m_target = m_transform.position;
	PROFILER_PUSH_CPU_MARKER("CALL SCRIPT", Utils::COLOR_DARK_GREEN);
	GetISystem()->GetIScriptSystem()->BeginCall("player", "Update");
	GetISystem()->GetIScriptSystem()->PushFuncParam(m_pScript);
	GetISystem()->GetIScriptSystem()->PushFuncParam(deltatime);
	GetISystem()->GetIScriptSystem()->EndCall();
	PROFILER_POP_CPU_MARKER();
}

CPlayer *CPlayer::operator=(Object *obj)
{
  return nullptr;
}
