#include "CInputHandler.hpp"
#include <iostream>

using namespace std;

CInputHandler::CInputHandler(IWindow * window) : m_Window(reinterpret_cast<sf::Window*>(window->getHandle()))
{
  Mouse.curr_pos = Mouse.curr_pos = sf::Mouse::getPosition(*m_Window);
  Mouse.x_wraped = Mouse.y_wraped = false;
}

ICommand * CInputHandler::handleInput()
{
  sf::Event event;
  while (m_Window->pollEvent(event))
  {
    if (event.type == sf::Event::MouseMoved)
    {
      if (event.mouseMove.x >= m_Window->getSize().x - 1)
			{
        sf::Mouse::setPosition(Mouse.curr_pos = sf::Vector2i(0,event.mouseMove.y), *m_Window);
				//Mouse.x_wraped = true;
			}
      else if (event.mouseMove.x <= 0)
			{
        sf::Mouse::setPosition(Mouse.curr_pos = sf::Vector2i(m_Window->getSize().x - 1, event.mouseMove.y));
				//Mouse.x_wraped = false;
			}
      if (event.mouseMove.y >= m_Window->getSize().y - 1)
			{
        sf::Mouse::setPosition(Mouse.curr_pos = sf::Vector2i(event.mouseMove.x,0), *m_Window);
				//Mouse.y_wraped = true;
			}
      else if (event.mouseMove.y <= 0)
			{
        sf::Mouse::setPosition(Mouse.curr_pos = sf::Vector2i(event.mouseMove.x,m_Window->getSize().y - 1));
				//Mouse.y_wraped = false;
			}

      Mouse.prev_pos = Mouse.curr_pos;
      Mouse.curr_pos = sf::Vector2i(event.mouseMove.x,event.mouseMove.y);
    }
    for (const auto &listener : listeners)
    {
      listener->OnInputEvent(event);
    }
  }
  return nullptr;
}

void CInputHandler::AddEventListener(IInputEventListener * pListener)
{
  listeners.push_back(pListener);
}

sf::Vector2i CInputHandler::getDeltaMouse()
{
	sf::Vector2i delta = (Mouse.curr_pos - Mouse.prev_pos);
	if (Mouse.x_wraped) {
		delta.x = delta.x % m_Window->getSize().x;
		cout << "x wraped" << endl;
	}
	if (Mouse.y_wraped) {
		delta.y = delta.y % m_Window->getSize().y;
		cout << "y wraped" << endl;
	}
	cout << "delta.x = " << delta.x << "; delta.y = " << delta.y <<";" << endl;
	return delta;
}
