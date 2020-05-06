#include <GameObject.hpp>

#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ISystem.hpp>

#if 0
void GameObject::update(float deltatime)
{
  float speed = deltatime * MOVE_SPEED;
  float rotSpeed = deltatime * 5.f;//m_rotAngle;
  /*
  for (const auto &key : m_keys)
  {
    switch (key)
    {
      ;
    }
  }
  */
  /*
  if (m_transform.position.y < 0)
    velocity.y = -velocity.y * friction;
  m_transform.position += velocity * deltatime;
  */
}

/*
glm::mat4 GameObject::getTransform()
{
  return glm::translate(m_Camera->getModelMatrix(), glm::vec3(0, -1.0f,0));
}
*/

GameObject::GameObject(const Object* obj) : Object(obj), m_Camera(nullptr)
{
  GetISystem()->GetIConsole()->Register("object_speed", &MOVE_SPEED, 200, 0, "object speed");
}

bool GameObject::OnInputEvent(const SInputEvent& event)
{
  if (event.deviceType == eIDT_Keyboard)
    this->getTransform();
  return false;
}
#endif