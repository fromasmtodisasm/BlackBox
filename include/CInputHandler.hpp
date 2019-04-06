#pragma once
#include "InputHandler.hpp"
#include "CWindow.hpp"
#include <map>
#include <list>

class CInputHandler : public IInputHandler
{
  sf::Window *m_Window;
  std::list<IInputEventListener*> listeners; 
public:
  CInputHandler(CWindow *window);
  virtual ICommand *handleInput() override;

  // Унаследовано через IInputHandler
  virtual void AddEventListener(IInputEventListener * pListener) override;
};
