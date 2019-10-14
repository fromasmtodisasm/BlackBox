#include <BlackBox/SFMLWindow.hpp>
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
      if (Mouse.x_wraped || Mouse.y_wraped)
      {
        Mouse.x_wraped = Mouse.y_wraped = false;
        continue;
      }
      Mouse.prev_pos = Mouse.curr_pos;
      Mouse.curr_pos = nextMousePos(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
    }
    this->OnInputEvent(event);
    //if (viewPort.contains(sf::Mouse::getPosition(*m_Window)))
      for (const auto &listener : listeners)
      {
				if (listener->OnInputEvent(event))
					break;
      }
    //else {
    //  continue;
    //}
  }
  return nullptr;
}

void CSFMLWindow::AddEventListener(IInputEventListener * pListener)
{
  listeners.push_back(pListener);
}

sf::Vector2i CSFMLWindow::getDeltaMouse()
{
  sf::Vector2i delta = Mouse.curr_pos - Mouse.prev_pos;
  return delta;
}

void CSFMLWindow::mouseLock(bool lock)
{
  sf::Vector2i center = (sf::Vector2i)m_Window->getSize() / 2;
  Mouse.locked = lock;
  Mouse.prev_pos = center;
  Mouse.curr_pos = center;

  sf::Mouse::setPosition(center, *m_Window);
  m_Window->setMouseCursorGrabbed(lock);
	m_Window->setMouseCursorVisible(!lock);
}

Rect &CSFMLWindow::getViewPort()
{
	return viewPort;
}

bool CSFMLWindow::create(Params params)
{
	auto settings = reinterpret_cast<sf::ContextSettings*>(params);
  m_contextSettings = sf::ContextSettings(settings->depthBits, settings->stencilBits, 0, settings->majorVersion, settings->minorVersion, settings->attributeFlags);
	return true;
}

Point CSFMLWindow::getCursorPos()
{
	return Point(sf::Mouse::getPosition().x,sf::Mouse::getPosition().y);
}

void CSFMLWindow::changeSize(int w, int h)
{
	m_Window->setSize(sf::Vector2u(std::max(0, w),std::max(0, h)));
}

void CSFMLWindow::ToogleFullScreen(int w, int h)
{
	Create(w, h, !m_bFullScreen, 32);
}
