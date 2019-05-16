#include <iostream>

#include <BlackBox/CPlayer.h>
#include <BlackBox/Primitives.hpp>
#include <BlackBox/CGame.hpp>
#include <BlackBox/ObjectManager.hpp>
#include <BlackBox/MaterialManager.hpp>

CPlayer::CPlayer() : GameObject(ObjectManager::instance()->getObject("pengium.obj"))
{
  m_type = OBJType::TPRIMITIVE;
  //getShaderProgram()->setUniformValue("color", glm::vec3(1,0,0));
  mouseState = FREE;
  setMaterial(defaultMaterial);
  move({0,0,0});
}

CPlayer::CPlayer(Object *obj) : GameObject(obj), impulse(0.0f, 3.0f, 0.0f)
{

}

bool CPlayer::OnInputEvent(sf::Event &event)
{
  switch (event.type) {
  case sf::Event::MouseButtonPressed:
  {
    return false;
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

void CPlayer::draw(CCamera *camera)
{
  Object::draw(camera);
}

void CPlayer::attachCamera(CCamera *camera)
{
  m_Camera = camera;
  GameObject::m_Camera = camera;
}

CCamera* CPlayer::getCamera()
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
      m_Camera->ProcessKeyboard(Camera_Movement::FORWARD, deltatime);
      break;
    case sf::Keyboard::S:
      m_Camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltatime);
      break;
    case sf::Keyboard::A:
      m_Camera->ProcessKeyboard(Camera_Movement::LEFT, deltatime);
      break;
    case sf::Keyboard::D:
      m_Camera->ProcessKeyboard(Camera_Movement::RIGHT, deltatime);
      break;
    default:
      ;//GameObject::update(deltatime);
    }
  }
  if (m_transform.position.y < 0)
    velocity.y = - velocity.y*friction;
  m_transform.position += velocity * deltatime;
  //m_Camera->m_target = m_transform.position;
}

CPlayer *CPlayer::operator=(Object *obj)
{
  return nullptr;
}
