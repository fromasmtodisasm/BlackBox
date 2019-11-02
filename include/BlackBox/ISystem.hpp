#pragma once
#include "Exports.hpp"
#include "Platform.hpp"
#include <cstdarg>

struct ISystem;
struct IGame;
struct IShaderManager;
struct IRender;
struct ILog;
struct IConsole;
struct IInput;
struct IFont;
struct IWindow;
struct IInputHandler;
struct IScriptSystem;
struct IValidator;


//! System wide events.
enum ESystemEvent
{
	//! Resizes of the main window.
	//! wparam=width, lparam=height.
	ESYSTEM_EVENT_RESIZE = 12,

	ESYSTEM_EVENT_CHANGE_FOCUS ,

	ESYSTEM_EVENT_LEVEL_LOAD_START ,

	ESYSTEM_EVENT_LEVEL_GAMEPLAY_START,

	ESYSTEM_EVENT_LEVEL_POST_UNLOAD,

	ESYSTEM_EVENT_LANGUAGE_CHANGE,


};

//////////////////////////////////////////////////////////////////////////
// User defined callback, which can be passed to ISystem.
struct ISystemUserCallback
{
	/** Signals to User that engine error occured.
			@return true to Halt execution or false to ignore this error.
	*/
	virtual bool OnError(const char* szErrorString) = 0;
	/** If working in Editor environment notify user that engine want to Save current document.
			This happens if critical error have occured and engine gives a user way to save data and not lose it
			due to crash.
	*/
	virtual void OnSaveDocument() = 0;

	/** Notify user that system wants to switch out of current process.
			(For ex. Called when pressing ESC in game mode to go to Menu).
	*/
	virtual void OnProcessSwitch() = 0;
};

//! Interface used for getting notified when a system event occurs.
struct ISystemEventListener
{
	// <interfuscator:shuffle>
	virtual ~ISystemEventListener() {}
	virtual void OnSystemEventAnyThread(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) {}
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) = 0;
	// </interfuscator:shuffle>
};

//! Structure used for getting notified when a system event occurs.
struct ISystemEventDispatcher
{
	// <interfuscator:shuffle>
	virtual ~ISystemEventDispatcher() {}
	virtual bool RegisterListener(ISystemEventListener* pListener, const char* szName) = 0;
	virtual bool RemoveListener(ISystemEventListener* pListener) = 0;

	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam, bool force_queue = false) = 0;
	virtual void Update() = 0;

	//virtual void OnLocaleChange() = 0;
	// </interfuscator:shuffle>
};

//////////////////////////////////////////////////////////////////////////
// Structure passed to Init method of ISystem interface.
struct SSystemInitParams
{
	void* hInstance;											//
	void* hWnd;														//
	char szSystemCmdLine[512];						// command line, used to execute the early commands e.g. -DEVMODE "g_gametype ASSAULT"
	ISystemUserCallback* pUserCallback;		//
	ILog* pLog;														// You can specify your own ILog to be used by System.
	IValidator* pValidator;								// You can specify different validator object to use by System.
	const char* sLogFileName;							// File name to use for log.
	bool bEditor;													// When runing in Editor mode.
	bool bPreview;												// When runing in Preview mode (Minimal initialization).
	bool bTestMode;												// When runing in Automated testing mode.
	bool bDedicatedServer;								// When runing a dedicated server.
	ISystem* pSystem;											// Pointer to existing ISystem interface, it will be reused if not NULL.

#if defined(LINUX)
	void (*pCheckFunc)(void*);							// authentication function (must be set).
#else
	void* pCheckFunc;											// authentication function (must be set).
#endif

	// Initialization defaults.
	SSystemInitParams()
	{
		hInstance = 0;
		hWnd = 0;
		memset(szSystemCmdLine, 0, sizeof(szSystemCmdLine));
		pLog = 0;
		pValidator = 0;
		pUserCallback = 0;
		sLogFileName = 0;
		bEditor = false;
		bPreview = false;
		bTestMode = false;
		bDedicatedServer = false;
		pSystem = 0;
		pCheckFunc = 0;
	}
};

struct ISystem
{
	enum MessageType {
		M_ERROR,
		M_WARNING
	};

  virtual bool Init() = 0;
  virtual void Start() = 0;
	virtual bool Update(int updateFlags = 0, int nPauseMode = 0) = 0;
  virtual void BeginFrame() = 0;
  virtual void EndFrame() = 0;
  virtual void Release() = 0;
  virtual IGame *CreateGame(IGame* game) = 0;

  
  virtual IShaderManager*						GetShaderManager() = 0;
  virtual IRender*									GetIRender() = 0;
  virtual ILog*											GetILog() = 0;
  virtual IConsole*									GetIConsole() = 0;
  virtual IInput*										GetIInput() = 0;
  virtual IGame*										GetIGame() = 0;
	virtual IFont*										GetIFont() = 0;
	virtual IWindow*									GetIWindow() = 0;
#if 0
	virtual IInputHandler*						GetIInputHandler() = 0;
#endif
	virtual IScriptSystem*						GetIScriptSystem() = 0;
	virtual ISystemEventDispatcher*		GetISystemEventDispatcher() = 0;
	//! Quits the application.
	virtual void Quit() = 0;

	virtual void Log(const char* message) = 0;
	virtual void Error(const char* message) = 0;

	virtual void ShowMessage(const char *message, const char *caption, MessageType messageType) = 0;
	virtual bool IsDevMode() = 0;

	virtual float GetDeltaTime() = 0;
};

// Get the system interface (must be defined locally in each module)
extern ISystem *GetISystem();

// interface of the DLL
extern "C"
{
  BLACKBOX_EXPORT ISystem* CreateSystemInterface(SSystemInitParams& initParams);
}

#define FatalError(...) void(0)
#define CryFatalError(...) FatalError(__VA_ARGS__)


#define Warning(...)
#define CryWarning(...) Warning(__VA_ARGS__)
#define ScriptWarning(...) Warning(__VA_ARGS__)

#define CryError(...) void(0)

//! Simple logs of data with low verbosity.
//void        CryLog(const char*, ...) PRINTF_PARAMS(1, 2);
#define CryLog(format, ...) GetISystem()->GetIConsole()->PrintLine(format, __VA_ARGS__)

