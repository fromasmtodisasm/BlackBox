#pragma once
#include <BlackBox/IGame.hpp>
#include <BlackBox/ISystem.hpp>
//#include <BlackBox/IWindow.hpp>
#include <BlackBox/Window.hpp>
#include <BlackBox/InputHandler.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/Triangle.hpp>
#include <BlackBox/World.hpp>
#include <BlackBox/Game/Player.h>
#include <BlackBox/CameraController.hpp>
#include <BlackBox/MusicList.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/Render/PostProcessor.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/Console.hpp>
#include <BlackBox/ScriptObjectConsole.hpp>
#include <BlackBox/ScriptObjectGame.hpp>

#include <BlackBox/common.h>


#include <map>
#include <string>
#include <vector>
#include <memory>
#include <stack>

//forward declarations
//////////////////////////////////////////////////////////////////////
using string = std::string;
class EventListener; 
class GameGUI;
class Scene;
class SceneManager;
class CTagPoint;

typedef std::multimap<string, CTagPoint*> TagPointMap;

class CGame : public IGame, public IInputEventListener, public IPostRenderCallback, public IPreRenderCallback
{
  class GameState;
  class EventListener;
  friend class GameGUI;
  friend class CPlayer;
private:
  ISystem *m_pSystem;
  IScriptSystem *m_pScriptSystem;
  IRender *m_pRender;
  IWindow *m_Window;
  IInputHandler *m_inputHandler;
  World *m_World;
  CPlayer *m_player = nullptr;
	CameraController *camControl;
  Scene *m_scene;
  SceneManager *m_sceneManager;
  ILog *m_Log;
  bool isWireFrame = false;
  bool isFullScreen = false;

  MusicList m_PlayList;
  bool m_isMusicPlaying = false;

  std::string m_Title;
  bool m_running = true;
  float m_lastTime;
	float m_time = 0.0f;
  sf::Clock deltaClock;
  EventListener *listener;
	bool isDrawingGui = false;
  class GameState; 
  //GUI
#ifdef GUI
  GameGUI *gui;
#endif // GUI

	CShaderProgram *m_ScreenShader;
	FreeTypeFont* m_Font;
	//EDIT MODE
	//==========
	IConsole* m_Console;
	// Render states
	bool culling = true;
	glm::vec2 viewPort = glm::vec2(1366.0f,768.0f);
  //
  bool openShadowMap = true;

  //
  ShaderManager *shaderManager;
	std::vector<IPostProcessor*> postProcessors;
	int currPP = 0;
	//=======================
  ICVar* g_scene;
  ICVar* r_displayinfo;
	ICVar* r_profile;
	ICVar* r_cap_profile;

  TagPointMap							m_mapTagPoints;					//!< Map of tag points by name
	CScriptObjectConsole* m_ScriptObjectConsole;
	CScriptObjectGame* m_ScriptObjectGame;
	IScriptObject* m_playerObject;

  enum Mode
  {
    FPS,
    MENU,
    FLY,
		EDIT
    
  }m_Mode = FPS;
  std::stack<GameState*> states;
	float fps = 0.0;

public:
  float m_deltaTime;
public:
  CGame(std::string title);
  CGame() = default;
  ~CGame() = default;
  bool init(ISystem *pSystem) override;
  bool update() override;
	void execScripts();
	void drawHud(float fps);
  void DisplayInfo(float fps);
  bool run() override;
  void input();

  bool loadScene();
  void setRenderState();
  void render();
  void setPlayer(CPlayer *player);
  void setCamera(CCamera *camera);

  // IInputEventListener interface
public:
  virtual bool OnInputEvent(sf::Event &event) override;

  // IGame interface
public:
  virtual IInputHandler *getInputHandler() override;
  void Stop();
	void gotoMenu();
	void gotoFullscreen();
	void gotoGame();
	void gotoFly();
	void gotoEdit();
  void showMenu();
	IWindow* getWindow();
private:

  bool initPlayer();

  bool FpsInputEvent(sf::Event& event);
  bool FlyInputEvent(sf::Event& event);
  bool MenuInputEvent(sf::Event& event);
  bool DefaultInputEvent(sf::Event& event);
  bool EditInputEvent(sf::Event& event);
  bool OnInputEventProxy(sf::Event& event);

	bool ShouldHandleEvent(sf::Event& event, bool& retflag);

  // IGame interface
public:
  virtual float getDeltaTime() override;
  virtual float getFPS() override;

  // Унаследовано через IPostRenderCallback
  virtual void PostRender() override;
  World *getWorld() const;

	void drawHud();
	void initCommands();
	void initVariables();

	// Inherited via IPreRenderCallback
	virtual void PreRender() override;

	// Inherited via IGame
	virtual float getTime() override;

  // tagpoint management functions
  ITagPoint* CreateTagPoint(const string& name, const Vec3& pos, const Vec3& angles);
  ITagPoint* GetTagPoint(const string& name);
  void RemoveTagPoint(ITagPoint* pPoint);
  bool RenameTagPoint(const string& oldname, const string& newname);

	//
	bool InitScripts();

	bool TestScriptSystem(bool& retflag);

};



class CGame::GameState
{
  EventListener *m_eventListener;
public:
};
