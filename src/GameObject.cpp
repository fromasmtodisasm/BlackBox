#include <BlackBox/GameObject.hpp>
#include "glm/gtc/matrix_transform.hpp"

GameObject *GameObject::create(Primitive::Type type)
{
  Object *obj =  Primitive::create(type, ShaderManager::instance()->getProgram("vertex.glsl", "fragment.glsl"));
  GameObject *go = new GameObject(obj);
  return go;
}

void GameObject::update(float deltatime)
{
  float speed = deltatime*MOVE_SPEED;
  float rotSpeed = deltatime*5.f;//m_rotAngle;
  /*
  for (const auto &key : m_keys)
  {
    switch (key)
    {
      ;
    }
  }
  */
  if (m_transform.position.y < 0)
    velocity.y = - velocity.y*friction;
  m_transform.position += velocity * deltatime;
}

/*
glm::mat4 GameObject::getTransform()
{
  return glm::translate(m_Camera->getModelMatrix(), glm::vec3(0, -1.0f,0));
}
*/

GameObject::GameObject(const Object *obj) : Object(obj)
{
}

bool GameObject::OnInputEvent(sf::Event &event)
{
  switch (event.type)
  {
  /*
  case sf::Event::KeyPressed:
    m_keys.insert(event.key.code);
    return true;
  case sf::Event::KeyReleased:
    m_keys.erase(event.key.code);
    return true;
  */
  default:
    break;
  }
  return false;
}
