#include "CPlayer.h"
#include "Primitives.hpp"

CPlayer::CPlayer() : GameObject(*Primitive::create(Primitive::CUBE, "vertex.glsl", "basecolor.frag"))
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
