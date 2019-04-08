#include "GameObject.hpp"

GameObject *GameObject::create(Primitive::Type type)
{
	Object *obj =  Primitive::create(type);
	GameObject *go = new GameObject();
	go->m_obj = obj;
	return go;
}

bool GameObject::OnInputEvent(sf::Event &event)
{

  switch (event.type)
  {
  case sf::Event::KeyPressed:
    switch (event.key.code)
    {
    case sf::Keyboard::K:
      m_obj->move(glm::vec3(0,1,0)*(MOVE_SPEED));
      return true;
    case sf::Keyboard::J:
      m_obj->move(glm::vec3(0,1,0)*(-MOVE_SPEED));
      return true;
    case sf::Keyboard::H:
      m_obj->move(glm::vec3(1,0,0)*(-MOVE_SPEED));
      return true;
    case sf::Keyboard::L:
      m_obj->move(glm::vec3(1,0,0)*(MOVE_SPEED));
      return true;
    }
    break;
  case sf::Event::MouseLeft:
    break;
  }
}
