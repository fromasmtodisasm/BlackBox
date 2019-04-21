#include <BlackBox/CPlayer.h>
#include <BlackBox/Primitives.hpp>

CPlayer::CPlayer() : GameObject(*Object::load("pengium.obj"))
{
  m_type = OBJType::TPRIMITIVE;
  //getShaderProgram()->setUniformValue("color", glm::vec3(1,0,0));
  move({0,0,0});
}

bool CPlayer::OnInputEvent(sf::Event &event)
{
  switch (event.type) {
  case sf::Event::MouseWheelScrolled:
    if (event.mouseWheelScroll.delta > 0)
    {
      m_Camera->moveForward(SCROLL_SPEED);
    }
    else {
      m_Camera->moveBackward(SCROLL_SPEED);
    }
    switch(event.mouseWheelScroll.wheel){
    case sf::Mouse::VerticalWheel:
    case sf::Mouse::HorizontalWheel:
      m_Camera->rotateX(p_gIGame->getInputHandler()->getDeltaMouse().y);
      m_Camera->rotateX(p_gIGame->getInputHandler()->getDeltaMouse().x);
    }
    return true;
  case sf::Event::MouseMoved:
	{
		sf::Vector2i delta = p_gIGame->getInputHandler()->getDeltaMouse();
		m_Camera->rotateX(-delta.y*MOUSE_SENSIVITY);
		m_Camera->rotateY(delta.x*MOUSE_SENSIVITY);
		return true;
	}
  default:
    return GameObject::OnInputEvent(event);
  }
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
