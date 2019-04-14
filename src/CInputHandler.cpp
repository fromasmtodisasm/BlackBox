#include "CInputHandler.hpp"

CInputHandler::CInputHandler(IWindow * window) : m_Window(reinterpret_cast<sf::Window*>(window->getHandle()))
{
  Mouse.curr_pos = Mouse.curr_pos = sf::Mouse::getPosition(*m_Window);
}

ICommand * CInputHandler::handleInput()
{
  sf::Event event;
  while (m_Window->pollEvent(event))
  {
    if (event.type == sf::Event::MouseMoved)
    {
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
  return Mouse.curr_pos - Mouse.prev_pos;
}
