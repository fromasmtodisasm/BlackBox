#pragma once
#include <BlackBox/ISystem.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/World.hpp>
#include <BlackBox/Game/Player.h>
#include <BlackBox/CameraController.hpp>
#include <BlackBox/MusicList.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/Render/PostProcessor.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/Console.hpp>
#include <BlackBox/ScriptObjectGame.hpp>
#include <BlackBox/IInput.hpp>

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

enum ActionType { ACTIONTYPE_MOVEMENT = 1, ACTIONTYPE_COMBAT, ACTIONTYPE_GAME, ACTIONTYPE_MULTIPLAYER, ACTIONTYPE_DEBUG };

//////////////////////////////////////////////////////////////////////
typedef std::queue<string> StringQueue;
typedef std::multimap<string, CTagPoint*> TagPointMap;

typedef std::vector<string> Vec2Str;
typedef Vec2Str::iterator Vec2StrIt;
typedef std::list<CPlayer*> ListOfPlayers;

struct ActionInfo
{
	int nId;
	string sDesc;
	bool bConfigurable;
	XActionActivationMode ActivationMode;
	Vec2Str vecSetToActionMap;	// if it is configured via "BindActionMultipleMaps" it will set the key-bindings to all action-maps in this array and leaves the others untouched
	int nType;
};

typedef std::map<string, ActionInfo> ActionsEnumMap;
typedef ActionsEnumMap::iterator ActionsEnumMapItor;
typedef std::multimap<string, CTagPoint*> TagPointMap;


class CGame : public IGame, public IInputEventListener, public IPostRenderCallback, public IPreRenderCallback
{
  class GameState;
  class EventListener;
  friend class GameGUI;
  friend class CPlayer;

public:
  CGame(std::string title);
  CGame() = default;
  ~CGame() = default;
  bool Init(ISystem *pSystem) override;
  bool Update() override;
	void execScripts();
	void drawHud(float fps);
  void DisplayInfo(float fps);
  bool Run(bool& bRelaunch) override;

  bool loadScene();
  void setRenderState();
  void render();
  void setPlayer(CPlayer *player);
  void setCamera(CCamera *camera);

  // IInputEventListener interface
public:
  virtual bool OnInputEvent(const SInputEvent& event) override;

	void PersistentHandler(const SInputEvent& event);

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
	virtual void SendMessage(const char* str) override {
		m_qMessages.push(str);
	}
	IWindow* getWindow();
private:

  bool initPlayer();

  bool FpsInputEvent(const SInputEvent& event);
  bool FlyInputEvent(const SInputEvent& event);
  bool MenuInputEvent(const SInputEvent& event);
  bool DefaultInputEvent(const SInputEvent& event);
  bool EditInputEvent(const SInputEvent& event);
  bool OnInputEventProxy(const SInputEvent& event);

	bool ShouldHandleEvent(const SInputEvent& event, bool& retflag);
	void ProcessPMessages(const char* szMsg);
	bool IsInPause();


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

	ISystem* GetSystem() { return m_pSystem; }
	bool	IsDevModeEnable();
	//////////////////////////////////////////////////////////////////////////
	// DevMode.
	//////////////////////////////////////////////////////////////////////////
	void DevModeInit();
	void DevModeUpdate();
	void DevMode_SavePlayerPos(int index, const char* sTagName = NULL, const char* sDescription = NULL);
	void DevMode_LoadPlayerPos(int index, const char* sTagName = NULL);
	//////////////////////////////////////////////////////////////////////////
	void  ResetInputMap();
	string GetLevelsFolder() const;

protected:
	void SetConfigToActionMap(const char* pszActionName, ...);
	//bool LoadMaterials(string sFolder);
	void	InitInputMap();
	void	InitConsoleCommands();
	void	InitConsoleVars();
	//set the common key bindings for the specified action map.
	//it reduces code redundancy and makes things more clear.
	void SetCommonKeyBindings(IActionMap* pActionMap);
public:
  float m_deltaTime;
public:
		std::string										m_currentLevel;						//!< Name of current level.
		std::string										m_currentMission;					//!< Name of current mission.
		std::string										m_currentLevelFolder;			//!< Folder of the current level.

private:
  ISystem *m_pSystem;
  IScriptSystem *m_pScriptSystem;
  IRender *m_pRender;
	IInput* m_pInput;
  IWindow *m_Window;
  IInputHandler *m_inputHandler;
  World *m_World;
  CPlayer *m_player = nullptr;
	CameraController *camControl;
  Scene *m_scene;
  SceneManager *m_sceneManager;
  ILog *m_Log;
	StringQueue							m_qMessages;
  bool isWireFrame = false;
  bool isFullScreen = false;

#ifdef ENABLE_MUSIC_LIST
  MusicList m_PlayList;
  bool m_isMusicPlaying = false;
#endif // ENABLE_MUSIC_LIST


  std::string m_Title;
  bool m_bUpdateRet = true;
  bool m_bRelaunch = true;
  float m_lastTime;
	float m_time = 0.0f;
#ifdef CLOCK_FIXED
	//TODO: FIX CLOCK
  sf::Clock deltaClock;
#endif // CLOCK_FIXED

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
	ICVar* m_pCVarCheatMode;

  TagPointMap							m_mapTagPoints;					//!< Map of tag points by name
	CScriptObjectGame* m_ScriptObjectGame;
	IScriptObject* m_playerObject;

	// other
	bool canDragViewPortWidth = false;
	bool canDragViewPortHeight = false;
	bool mousePressed = false;
	sf::Vector2i mouseDelta;
	sf::Vector2i mousePrev;

  enum Mode
  {
    FPS,
    MENU,
    FLY,
		EDIT
    
  }m_Mode = Mode::FPS;
  std::stack<GameState*> states;
	float fps = 0.0;

	ActionsEnumMap					m_mapActionsEnum;				//!< Input Stuff(is for the client only but must be here)
	struct IActionMapManager* m_pIActionMapManager;			//!<
	bool m_bInPause = false;

	//other
	bool can_drag_vp = true; // can drag view port ?


													 // Inherited via IGame


													 // Inherited via IGame
	virtual void Release() override;

};



class CGame::GameState
{
  EventListener *m_eventListener;
public:
};
