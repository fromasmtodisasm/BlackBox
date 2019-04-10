#include "GameObject.hpp"
#include "glm/gtc/matrix_transform.hpp"

GameObject *GameObject::create(Primitive::Type type)
{
  Object *obj =  Primitive::create(type, "vertex.glsl", "fragment.glsl");
	GameObject *go = new GameObject(*obj);
  return go;
}

void GameObject::update(float deltatime)
{
  float speed = deltatime*MOVE_SPEED;
  float rotSpeed = deltatime*0.7;//m_rotAngle;
  for (const auto &key : m_keys)
  {
    switch (key)
    {
    case sf::Keyboard::W:
      m_Camera->moveForward(speed);
      break;
    case sf::Keyboard::S:
      m_Camera->moveBackward(speed);
      break;
    case sf::Keyboard::A:
      m_Camera->strafeRight(speed);
      break;
    case sf::Keyboard::D:
      m_Camera->strafeLeft(speed);
      break;
    case sf::Keyboard::Up:
      m_Camera->moveUp(speed);
      break;
    case sf::Keyboard::Down:
      m_Camera->moveDown(speed);
      break;
    case sf::Keyboard::Left:
      m_Camera->rotateY(rotSpeed);
      break;
    case sf::Keyboard::Right:
      m_Camera->rotateY(-rotSpeed);
      break;
    case sf::Keyboard::N:
      m_Camera->rotateZ(rotSpeed);
      break;
    case sf::Keyboard::M:
      m_Camera->rotateZ(-rotSpeed);
      break;
    case sf::Keyboard::F:
      m_Camera->rotateX(rotSpeed);
      break;
    case sf::Keyboard::G:
      m_Camera->rotateX(-rotSpeed);
      break;
    case sf::Keyboard::R:
      m_Camera->reset();
      break;
    case sf::Keyboard::T:
      m_Camera->rotateAroundTarget(rotSpeed);
      break;
    case sf::Keyboard::Num1:
      m_Camera->rotateY(180.0f);
      break;
    }
  }
}

/*
glm::mat4 GameObject::getTransform()
{
  return glm::translate(m_Camera->getModelMatrix(), glm::vec3(0, -1.0f,0));
}
*/

GameObject::GameObject(const Object &obj) : Object(obj)
{
}

bool GameObject::OnInputEvent(sf::Event &event)
{
  switch (event.type)
  {
  case sf::Event::KeyPressed:
    m_keys.insert(event.key.code);
    return true;
  case sf::Event::KeyReleased:
    m_keys.erase(event.key.code);
    return true;
  case sf::Event::Resized:    return true;
  }
}
