#pragma once
#if 0
#include <BlackBox/InputHandler.hpp>
#include <BlackBox/Window.hpp>
#include <SFML/System.hpp>

#include <map>
#include <list>

class CInputHandler : public IInputHandler
{
  sf::Window *m_Window;
  std::list<IInputEventListener*> listeners; 
  struct
  {
    sf::Vector2i prev_pos;
    sf::Vector2i curr_pos;
		bool x_wraped;
		bool y_wraped;
    bool locked;
  }Mouse;

public:
  CInputHandler(IWindow *window);
  virtual ICommand *handleInput() override;

  virtual void AddEventListener(IInputEventListener * pListener) override;

  // IInputHandler interface
public:
  virtual sf::Vector2i getDeltaMouse() override;
  virtual sf::Vector2i mouseLock(bool lock) override;
};

#endif
