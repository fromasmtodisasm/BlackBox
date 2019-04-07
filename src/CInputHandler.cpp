#include "CInputHandler.hpp"

CInputHandler::CInputHandler(CWindow * window) //: m_Window(window)
{
}

ICommand * CInputHandler::handleInput()
{
  sf::Event event;
  while (m_Window->pollEvent(event))
  {
    // Close window: exit
    if (event.type == sf::Event::Closed)
      //m_bClose = true;      

    // Escape key: exit
    if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
      m_Window->close();

    if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up))
      ;//key_up->execute();
    if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down))
      ;//key_down->execute();
    if ((event.type == sf::Event::JoystickButtonPressed))
      ;//key_down->execute();
    // Resize event: adjust the viewport
    if (event.type == sf::Event::Resized)
    {
      glViewport(0, 0, event.size.width, event.size.height);
    }
  }
  return nullptr;
}

void CInputHandler::AddEventListener(IInputEventListener * pListener)
{
}
