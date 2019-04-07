#pragma once
#include "IGame.hpp"
#include "ISystem.hpp"
#include "IWindow.hpp"
#include "CInputHandler.hpp"
#include "CShader.hpp"
#include "Triangle.hpp"
#include "World.hpp"

#include <common.h>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include <stack>

using string = std::string;
class EventListener; 

class CGame : public IGame {
  class GameState;
  class EventListener;
private:
  ISystem *m_pSystem;
  IWindow *m_Window;
  IInputHandler *inputHandler;
  World *m_World;

  char *m_Title;
  bool m_running = true;
  EventListener *listener;
  class GameState; 
  class GameState;
  enum State
  {
    INIT,
    MENU,
    RUN,
    RELASE
  };
  std::stack<GameState*> states;

public:
  CGame(char *);
  ~CGame() = default;
  bool init(ISystem *pSystem);
  bool update();
  bool run();
  void input();

  bool init_opbject();
};


class CGame::EventListener : public IInputEventListener 
{
  CGame *m_Game;
  EventListener(CGame *game);
  virtual bool OnInputEvent(sf::Event &event);
};

class CGame::GameState
{
  EventListener *m_eventListener;
public:
};
