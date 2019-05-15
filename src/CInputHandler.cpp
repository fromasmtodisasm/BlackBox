#include <BlackBox/CSFMLWindow.hpp>
#include <iostream>

using namespace std;

ICommand * CSFMLWindow::handleInput()
{
	int offset = 5;
  sf::Event event;
  while (m_Window->pollEvent(event))
  {
    if (event.type == sf::Event::MouseMoved)
    {
      Mouse.prev_pos = Mouse.curr_pos;
      Mouse.curr_pos = sf::Vector2i(event.mouseMove.x,event.mouseMove.y);
    }
    this->OnInputEvent(event);
    if (viewPort.contains(sf::Mouse::getPosition(*m_Window)))
      for (const auto &listener : listeners)
      {
        listener->OnInputEvent(event);
      }
    else {
      continue;
    }
  }
  return nullptr;
}

void CSFMLWindow::AddEventListener(IInputEventListener * pListener)
{
  listeners.push_back(pListener);
}

sf::Vector2i CSFMLWindow::getDeltaMouse()
{
  sf::Vector2i delta = Mouse.curr_pos - Mouse.lockedPos;
  sf::Mouse::setPosition(Mouse.lockedPos, *m_Window);
  return delta;
}

void CSFMLWindow::mouseLock(sf::Vector2i pos)
{
  Mouse.locked = true;
  Mouse.lockedPos = pos;
  m_Window->setMouseCursorVisible(false);
}

void CSFMLWindow::mouseUnlock()
{
  Mouse.locked = false;
  m_Window->setMouseCursorVisible(true);
}
