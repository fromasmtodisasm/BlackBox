#pragma once
#if 0
#include <BlackBox/ICommand.hpp>
#include <SFML/Window/Event.hpp>

struct IInputEventListener
{
  virtual bool OnInputEvent(sf::Event& event) = 0;
};

struct IInputHandler
{
  virtual ~IInputHandler() {};
  virtual ICommand* handleInput(bool bPause) = 0;
  virtual void AddEventListener(IInputEventListener* pListener) = 0;

  virtual sf::Vector2i getDeltaMouse() = 0;
  virtual void mouseLock(bool lock) = 0;
};
#endif