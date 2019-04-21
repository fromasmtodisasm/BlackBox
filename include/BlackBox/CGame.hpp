#pragma once
#include "IGame.hpp"
#include "IEngine.hpp"
#include <BlackBox/IWindow.hpp>
#include <BlackBox/CInputHandler.hpp>
#include <BlackBox/CShader.hpp>
#include <BlackBox/Triangle.hpp>
#include <BlackBox/World.hpp>
#include <BlackBox/CPlayer.h>
#include <BlackBox/CameraController.hpp>
#include <BlackBox/MusicList.hpp>
#include <BlackBox/common.h>

#include <map>
#include <string>
#include <vector>
#include <memory>
#include <stack>


using string = std::string;
class EventListener; 

class CGame : public IGame, public IInputEventListener{
  class GameState;
  class EventListener;
private:
  IEngine *m_pSystem;
  IWindow *m_Window;
  IInputHandler *m_inputHandler;
  World *m_World;
  CCamera *m_camera1, *m_camera2, *m_active_camera;
  CPlayer *m_player;
	CameraController *camControl;
  bool isWireFrame = false;

  MusicList m_PlayList;
  bool m_isMusicPlaying = false;
	bool m_isPaused = false;
	bool m_InMenu = false;
	bool m_InGame = false;

  std::string m_Title;
  bool m_running = true;
  float m_deltaTime;
  float m_lastTime;
  sf::Clock deltaClock;
  EventListener *listener;
	bool isDrawingGui = false;
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
  CGame(std::string title);
  ~CGame() = default;
  bool init(IEngine *pSystem);
  bool update();
  bool run();
  void input();

  bool init_opbject();
  void setRenderState();
  void render();
	void guiControls();

  // IInputEventListener interface
public:
  virtual bool OnInputEvent(sf::Event &event);

  // IGame interface
public:
  virtual IInputHandler *getInputHandler() override;
private:
	void gotoMenu();
	void gotoGame();
};


class CGame::GameState
{
  EventListener *m_eventListener;
public:
};
