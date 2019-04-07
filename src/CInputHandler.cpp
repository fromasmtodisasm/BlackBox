#include "CInputHandler.hpp"

CInputHandler::CInputHandler(IWindow * window) : m_Window(reinterpret_cast<sf::Window*>(window->getHandle()))
{
}

ICommand * CInputHandler::handleInput()
{
  sf::Event event;
  while (m_Window->pollEvent(event))
  {
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
