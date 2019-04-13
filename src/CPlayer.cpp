#include "CPlayer.h"
#include "Primitives.hpp"

CPlayer::CPlayer() : GameObject(*Primitive::create(Primitive::CUBE, "vertex.glsl", "fragment.glsl"))
{
  m_type = OBJType::TPRIMITIVE;
  getShaderProgram()->setUniformValue("color", glm::vec3(1,0,0));
  move({0,0,0});
}

bool CPlayer::OnInputEvent(sf::Event &event)
{
  GameObject::OnInputEvent(event);
  return true;
}

void CPlayer::draw()
{
  Object::draw();
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

void CPlayer::update(float deltatime)
{

  float speed = deltatime*MOVE_SPEED;
  float rotSpeed = deltatime*5.f;//m_rotAngle;
  for (const auto &key : m_keys)
  {
    switch (key)
    {
    case sf::Keyboard::J:
      move(glm::vec3(0,1,0)*speed);
      break;
    case sf::Keyboard::K:
      move(glm::vec3(0,1,0)*-speed);
      break;
    default:
      GameObject::update(deltatime);
    }
  }
  //m_Camera->m_target = m_transform.position;
}
