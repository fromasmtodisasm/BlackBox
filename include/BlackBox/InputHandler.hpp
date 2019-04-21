#pragma once
#include <BlackBox/ICommand.hpp>
#include <SFML/Window/Event.hpp>

struct IInputEventListener
{
  virtual bool OnInputEvent(sf::Event &event) = 0;
};

struct IInputHandler
{
  virtual ICommand *handleInput() = 0;
  virtual void AddEventListener(IInputEventListener *pListener) = 0;

  virtual sf::Vector2i getDeltaMouse() = 0;
};
