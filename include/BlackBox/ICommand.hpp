#pragma once
#include <SFML/Window/Event.hpp>

struct ICommand
{
  virtual void execute(sf::Event &event) = 0;
};
