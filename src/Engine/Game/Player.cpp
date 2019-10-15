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
  m_type = OBJType::TPRIMITIVE;
  //getShaderProgram()->setUniformValue("color", glm::vec3(1,0,0));
  mouseState = FREE;
  setMaterial(defaultMaterial);
	GetISystem()->GetIScriptSystem()->GetGlobalValue("player", m_pScript);
}

CPlayer::CPlayer(Object *obj) : GameObject(obj), impulse(0.0f, 3.0f, 0.0f)
{
	m_pScript = GetISystem()->GetIScriptSystem()->CreateEmptyObject();
	GetISystem()->GetIScriptSystem()->GetGlobalValue("player", m_pScript);
}

bool CPlayer::OnInputEvent(sf::Event &event)
{
  switch (event.type) {
  case sf::Event::MouseButtonPressed:
  {
    Object *obj = SceneManager::instance()->currentScene()->getObject("MyPlayer");
    GameObject *go = new GameObject(obj);
    go->setMaterial(obj->getMaterial());
    go->m_transform.position = m_Camera->getPosition();// + glm::vec3(0,0,5);
    go->velocity = 48.0f*m_Camera->Front;
    Game->getWorld()->getActiveScene()->addObject("bullet", go);

    return true;
  }
  case sf::Event::MouseMoved:
	{
    delta = p_gIGame->getInputHandler()->getDeltaMouse();
    m_Camera->ProcessMouseMovement(static_cast<GLfloat>(delta.x), -static_cast<GLfloat>(delta.y));
    return true;
  }
  case sf::Event::KeyPressed:
    return OnKeyPress(event);
  case sf::Event::KeyReleased:
    return OnKeyReleas(event);
  default:
    return GameObject::OnInputEvent(event);
  }
}

bool CPlayer::OnKeyPress(sf::Event& event)
{
  m_keys.insert(event.key.code);
  return true;
}

bool CPlayer::OnKeyReleas(sf::Event& event)
{
  m_keys.erase(event.key.code);
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
    case sf::Keyboard::J:
      velocity += impulse;
      break;
    case sf::Keyboard::K:
      velocity -= impulse;
      break;
    case sf::Keyboard::W:
      m_Camera->ProcessKeyboard(Movement::FORWARD, deltatime);
      break;
    case sf::Keyboard::S:
      m_Camera->ProcessKeyboard(Movement::BACKWARD, deltatime);
      break;
    case sf::Keyboard::A:
      m_Camera->ProcessKeyboard(Movement::LEFT, deltatime);
      break;
    case sf::Keyboard::D:
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
