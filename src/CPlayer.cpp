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

CPlayer::CPlayer(Object *obj) : GameObject(obj)
{

}

bool CPlayer::OnInputEvent(sf::Event &event)
{
  float dt = Game->getDeltaTime();
  //delta = {0,0};
  switch (event.type) {
  case sf::Event::MouseButtonPressed:
  {
    /*if(mouseState == LOCKED) {
      Game->m_Window->mouseUnlock();
      mouseState = FREE;
    }*/
    if (event.mouseButton.button == sf::Mouse::Left && mouseState != LOCKED)
      Game->m_Window->mouseLock(sf::Vector2i(event.mouseButton.x, event.mouseButton.y)), mouseState = LOCKED;
    return true;
  }
  case sf::Event::MouseButtonReleased:
  {
    if (event.mouseButton.button == sf::Mouse::Left && mouseState == LOCKED)
      Game->m_Window->mouseUnlock(), mouseState = FREE;

    return true;
  }
  case sf::Event::MouseMoved:
	{
    std::cout << "mouse moved ( " << event.mouseMove.x << ", " << event.mouseMove.y << " )" << std::endl;
    if (mouseState == LOCKED)
    {
      std::cout << "move game mouse" << std::endl;
      delta = p_gIGame->getInputHandler()->getDeltaMouse();
      m_Camera->ProcessMouseMovement(delta.x, -delta.y);
/*      sf::Vector2i pos = sf::Mouse::getPosition();
      sf::Mouse::setPosition(pos + delta);*/
      return true;
    }
    return false;
  }
  case sf::Event::KeyPressed:
    m_keys.insert(event.key.code);
    return true;
  case sf::Event::KeyReleased:
    m_keys.erase(event.key.code);
    return true;
  default:
    return GameObject::OnInputEvent(event);
  }
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

glm::vec3 CPlayer::getPos()
{
  return m_transform.position;
}

void CPlayer::setGame(CGame *game)
{
  Game = game;
}

void CPlayer::update(float deltatime)
{

  //ImGui
  float speed = deltatime*MOVE_SPEED;
  float rotSpeed = deltatime*5.f;//m_rotAngle;

  glm::vec3 impulse(0,9.0,0);
  for (const auto &key : m_keys)
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
