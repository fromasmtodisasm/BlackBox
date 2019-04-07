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
  CInputHandler(IWindow *window);
  virtual ICommand *handleInput() override;

  // ������������ ����� IInputHandler
  virtual void AddEventListener(IInputEventListener * pListener) override;
};
