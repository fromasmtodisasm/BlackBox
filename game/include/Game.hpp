#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////
// Version of the game
#define GAME_MAIN_VERSION						1						//!< [0..255]
#define GAME_SUB_VERSION						4						//!< [0..255] patch version number, shown in menu
#define GAME_PATCH_VERSION					0						//!< [0..256*256[
#define SERVERINFO_FORMAT_VERSION		88				  //!< [0..255] bump if server info format changes (old version won't show up any more)
#define NETWORK_FORMAT_VERSION			5						//!< [0..2^32] bump if netcode stream format changes

#define SAVEVERSION									23					// [Petar] Do not bump this value anymore it shows the release version of the savegame - it will always be supported
#define PATCH1_SAVEVERSION					24					// [Kirill] Do not bump this value anymore it shows the Patch 1 version of the savegame - it will always be supported
#define PATCH2_SAVEVERSION					36					//!< bump this if the savegame format changes and we are still working on patch 2

#define GAME_VERSION_SUFIX					"F"				//!< A - alpha, B - beta, RC - release candidate, F - final


#define MAKE_GAME_VERSION(m,s,p)		(((m)<<24)|((s)<<16)|(p))
#define GAME_VERSION								MAKE_GAME_VERSION(GAME_MAIN_VERSION,GAME_SUB_VERSION,GAME_PATCH_VERSION)

#define ENTITYTYPE_PLAYER				0x00000001
#define ENTITYTYPE_WAYPOINT			0x00000002
#define ENTITYTYPE_OWNTEAM			0x00000004

#define SAVEMAGIC "CRYLEVELSAVE"

// game states
enum { CGS_INPROGRESS = 0, CGS_COUNTDOWN = 1, CGS_PREWAR = 2, CGS_INTERMISSION = 3 };

//#include "BitStream_Base.h"						// CBitStream_Base
//#include "BitStream_Compressed.h"			// CBitStream_Compressed


#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/IGame.hpp>
#include <BlackBox/Resources/ObjectManager.hpp>
#include <BlackBox/World/IWorld.hpp>
//#include <BlackBox/CameraController.hpp>
//#include <BlackBox/MusicList.hpp>
#include <BlackBox/System/ILog.hpp>
//#include <BlackBox/Renderer/PostProcessor.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Input/IInput.hpp>

#include <Network/XNetwork.hpp>
#include <ScriptObjects/ScriptObjectServer.hpp>
#include <ScriptObjects/ScriptObjectClient.hpp>
#include <ScriptObjects/ScriptObjectGame.hpp>
#include "Player.h"



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
class IScene;
class SceneManager;
class CTagPoint;

class CXClient;
class CXServer;

enum ActionType { ACTIONTYPE_MOVEMENT = 1, ACTIONTYPE_COMBAT, ACTIONTYPE_GAME, ACTIONTYPE_MULTIPLAYER, ACTIONTYPE_DEBUG };

//////////////////////////////////////////////////////////////////////
typedef std::queue<string> StringQueue;
typedef std::multimap<string, CTagPoint*> TagPointMap;

typedef std::vector<string> Vec2Str;
typedef Vec2Str::iterator Vec2StrIt;
typedef std::list<CPlayer*> ListOfPlayers;

typedef std::map<CIPAddress, SXServerInfos>	ServerInfosMap;
typedef ServerInfosMap::iterator ServerInfosVecItor;

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


class CGame : 
	public IGame, 
	public IInputEventListener, 
	public IPostRenderCallback, 
	public IPreRenderCallback,
	public IServerSnooperSink,
	public INETServerSnooperSink,
	public LoadObjectSink
{
  class EventListener;
  friend class GameGUI;
  friend class CPlayer;

public:	
	enum HostType
	{
		CLIENT,
		SERVER,
		NOT_CONECTED
	};

  CGame(std::string title);
  CGame() = default;
  ~CGame() = default;
  bool Init(ISystem *pSystem) override;
  bool Update() override;
	void ExecScripts();
	void DrawHud(float fps);
  void DisplayInfo(float fps);
  bool Run(bool& bRelaunch) override;


  bool loadScene(std::string name);
  void unloadScene(std::string name);
  void saveScene(std::string name, std::string as);
  void SetRenderState();
  void setPlayer(CPlayer *player);
  void setCamera(CCamera *camera);

  // IInputEventListener interface
public:
  virtual bool OnInputEvent(const SInputEvent& event) override;

	void PersistentHandler(const SInputEvent& event);

  // IGame interface
public:
	virtual void SaveConfiguration(const char* sSystemCfg, const char* sGameCfg, const char* sProfile) override;
	virtual void ReloadScripts() override;
	virtual bool GetModuleState(EGameCapability eCap) override;
	virtual void UpdateDuringLoading() override;
	virtual IXAreaMgr* GetAreaManager() override;
	virtual ITagPointManager* GetTagPointManager() override;
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
	virtual void Release() override;
	IWindow* getWindow();
private:

  bool initPlayer();
	void initTechniques();

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

  // Унаследовано через IPostRenderCallback
  virtual void PostRender() override;
  IWorld *getWorld() const;

	void drawHud();
	void initCommands();
	void initVariables();

	// Inherited via IPreRenderCallback
	virtual void PreRender() override;

  // tagpoint management functions
  ITagPoint* CreateTagPoint(const string& name, const Vec3& pos, const Vec3& angles);
  ITagPoint* GetTagPoint(const string& name);
  void RemoveTagPoint(ITagPoint* pPoint);
  bool RenameTagPoint(const string& oldname, const string& newname);

	IScriptSystem* GetScriptSystem() { return m_pScriptSystem; }
	IClient* CreateClient(IClientSink* pSink, bool bLocal = false)
	{
		return m_pNetwork->CreateClient(pSink, bLocal);
	}
	IActionMapManager* GetActionMapManager() { return m_pIActionMapManager; }

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

	// Network -------------------------------------------------------------
	bool StartupServer(bool listen, const char* szName);
	void ShutdownServer();
	bool StartupClient();
	bool StartupLocalClient();
	void ShutdownClient();
	bool IsClient();
	void MarkClientForDestruct();
	void OnServerFound(CIPAddress& ip, const string& szServerInfoString, int ping);
	void OnNETServerFound(const CIPAddress& ip, const string& szServerInfoString, int ping);
	void OnNETServerTimeout(const CIPAddress& ip);
	void RefreshServerList();
	//////////////////////////////////////////////////////////////////////////
protected:
	void SetConfigToActionMap(const char* pszActionName, ...);
	//bool LoadMaterials(string sFolder);
	void	InitInputMap();
	void	InitConsoleCommands();
	void	InitConsoleVars();
	//set the common key bindings for the specified action map.
	//it reduces code redundancy and makes things more clear.
	void SetCommonKeyBindings(IActionMap* pActionMap);

	virtual Object* OnLoad(Object* object, std::string type) override;
public:
  float m_deltaTime;

public:
	ISystem *											m_pSystem;								//!< The system interface
	CXServer *										m_pServer;								//!< The server of this computer
	CXClient *										m_pClient;								//!< The client of this computer
  IScriptSystem *								m_pScriptSystem;
  IRenderer *										m_pRender;
	IInput *											m_pInput;
  IWindow *											m_Window;
  IInputHandler *								m_inputHandler;
  IWorld *											m_World;
  CPlayer *											m_player = nullptr;
	//CameraController *camControl;
  IScene *											m_scene;
  SceneManager *								m_sceneManager;
  ILog *												m_pLog;
	INetwork*											m_pNetwork;
	StringQueue										m_qMessages;
  bool													isWireFrame = false;
  bool													isFullScreen = false;

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
  //GUI
#ifdef GUI
  GameGUI *gui;
#endif // GUI

	//CShaderProgram *m_ScreenShader;
	IFont* m_Font;
	//EDIT MODE
	//==========
	IConsole* m_Console;
	// Render states
	bool culling = true;
	glm::vec2 viewPort = glm::vec2(1366.0f,768.0f);
  //
  bool openShadowMap = true;

  //
  //ShaderManager *shaderManager;
	//std::vector<IPostProcessor*> postProcessors;
	int currPP = 0;

public:
	//!	The dummy client of this computer, required to get the list of servers if
	//! theres not a real client actually connected and playing

	IServerSnooper*								m_pServerSnooper;					//!< used for LAN Multiplayer, to remove control servers
	INETServerSnooper*						m_pNETServerSnooper;			//!< used for Internet Multiplayer, to remove control servers
	IRConSystem*									m_pRConSystem;						//!< used for Multiplayer, to remote control servers
	std::string										m_szLastAddress;
	bool													m_bLastDoLateSwitch;
	bool													m_bLastCDAuthentication;

	//CUIHud* m_pUIHud;									//!< Hud
	//CUIHud* m_pCurrentUI;							//!< for the current ui

	std::string										m_currentLevel;						//!< Name of current level.
	std::string										m_currentMission;					//!< Name of current mission.
	std::string										m_currentLevelFolder;			//!< Folder of the current level.


	// console variables -----------------------------------------------------------
	//=======================
  ICVar* g_scene;
  ICVar* r_displayinfo;
	ICVar* r_profile;
	ICVar* r_cap_profile;
	ICVar* m_pCVarCheatMode;

	ICVar* g_LevelName;
	ICVar* g_StartMission;

	ICVar* sv_port;
	ICVar* sv_mapcyclefile;
	ICVar* sv_cheater_kick;
	ICVar* sv_cheater_ban;

	ICVar* sv_timeout;
	ICVar* cl_timeout;
	ICVar* cl_loadtimeout;
	ICVar* cl_snooptimeout;
	ICVar* cl_snoopretries;
	ICVar* cl_snoopcount;

	ServerInfosMap						m_ServersInfos;							//!< Infos about the avaible servers
	std::string								m_strLastSaveGame;
	bool											m_bEditor;
	//tPlayerPersistentData			m_tPlayerPersistentData;


  TagPointMap													m_mapTagPoints;					//!< Map of tag points by name
	CScriptObjectGame*									m_pScriptObjectGame;
	IScriptObject*											m_playerObject;

	// other
	bool canDragViewPortWidth = false;
	bool canDragViewPortHeight = false;
	bool mousePressed = false;
	//sf::Vector2i mouseDelta;
	//sf::Vector2i mousePrev;

  enum Mode
  {
    FPS,
    MENU,
    FLY,
		EDIT
    
  }m_Mode = Mode::FPS;
	float fps = 0.0;

	ActionsEnumMap					m_mapActionsEnum;				//!< Input Stuff(is for the client only but must be here)
	struct IActionMapManager* m_pIActionMapManager;			//!<
	bool m_bInPause = false;

	//other
	bool can_drag_vp = true; // can drag view port ?

	HostType m_HostType = NOT_CONECTED;

	//CScriptObjectClient* m_pScriptClient = nullptr;
	//CScriptObjectServer* m_pScriptServer = nullptr;

	bool m_SceneRendered = false;

};


