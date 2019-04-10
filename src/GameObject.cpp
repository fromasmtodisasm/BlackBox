#include "GameObject.hpp"

GameObject *GameObject::create(Primitive::Type type)
{
  Object *obj =  Primitive::create(type, "vertex.glsl", "fragment.glsl");
	GameObject *go = new GameObject(*obj);
	return go;
}

GameObject::GameObject(const Object &obj) : Object(obj)
{
}

bool GameObject::OnInputEvent(sf::Event &event)
{
  switch (event.type)
  {
  case sf::Event::KeyPressed:
    switch (event.key.code)
    {
    case sf::Keyboard::K:
      move(glm::vec3(0,1.0f,0)*(MOVE_SPEED));
      return true;
    case sf::Keyboard::J:
      move(glm::vec3(0,1,0)*(-MOVE_SPEED));
      return true;
    case sf::Keyboard::H:
      move(glm::vec3(1,0,0)*(-MOVE_SPEED));
      return true;
    case sf::Keyboard::L:
      move(glm::vec3(1,0,0)*(MOVE_SPEED));
      return true;
    case sf::Keyboard::U:
      move(glm::vec3(0,0,1)*(-MOVE_SPEED));
      return true;
    case sf::Keyboard::I:
      move(glm::vec3(0,0,1)*(MOVE_SPEED));
      return true;
    }
    break;
  case sf::Event::MouseLeft:
    break;
  }
}
