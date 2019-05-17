#pragma once
//#include <SFML/Window/Event.hpp>

struct ICommand
{
  virtual void execute() = 0;
};
