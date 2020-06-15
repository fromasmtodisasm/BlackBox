#pragma once
#include <BlackBox/Core/Platform/Platform.hpp> 
#include <BlackBox/Core/Version.hpp> 
#include <BlackBox/System/ILog.hpp> 
#include <BlackBox/System/IValidator.hpp> 
#include <cstdarg>

#ifdef SYSTEM_EXPORTS
  #define ISYSTEM_API DLL_EXPORT
#else
  #define ISYSTEM_API DLL_IMPORT
#endif

//! Static branch-prediction helpers
#define IF(condition, hint)    if (condition)
//! Compiler-supported type-checking helper
#define PRINTF_PARAMS(...)



struct ISystem;
struct ILog;
struct IEntitySystem;
struct IGame;
struct IShaderManager;
struct IRenderer;
struct I3DEngine;
struct ICmdLine;
struct IConsole;
struct IInput;
struct IHardwareMouse;
struct IFont;
struct IWindow;
struct IInputHandler;
struct IScriptSystem;
struct IValidator;
struct ITimer;
struct INetwork;
struct IWorld;
struct IPlatform;
struct ICryPak;

//////////////////////////////////////////////////////////////////////////
#define DEFAULT_GAME_PATH	"TestGame"
#define DATA_FOLDER "res"

//! System wide events.
enum ESystemEvent
{
	//! Changes to main window focus.
	//! wparam is not 0 is focused, 0 if not focused.
	ESYSTEM_EVENT_CHANGE_FOCUS = 10,

	//! Moves of the main window.
	//! wparam=x, lparam=y.
	ESYSTEM_EVENT_MOVE = 11,

  //! Resizes of the main window.
  //! wparam=width, lparam=height.
  ESYSTEM_EVENT_RESIZE = 12,

	//! Activation of the main window.
	//! wparam=1/0, 1=active 0=inactive.
	ESYSTEM_EVENT_ACTIVATE = 13,

  ESYSTEM_EVENT_LEVEL_LOAD_START,

  ESYSTEM_EVENT_LEVEL_GAMEPLAY_START,

  ESYSTEM_EVENT_LEVEL_POST_UNLOAD,

  ESYSTEM_EVENT_LANGUAGE_CHANGE,

	//! Toggle fullscreen.
	//! wparam is 1 means we switched to fullscreen, 0 if for windowed.
	ESYSTEM_EVENT_TOGGLE_FULLSCREEN,

	//! Sent if the window containing the running game loses focus, but application itself still has focus
	//! This is needed because some sub-systems still want to work even without focus on main application
	//! while others would prefer to suspend their operation
	ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE,
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

//! \cond INTERNAL
//! \note Can be used for LoadConfiguration().
struct ILoadConfigurationEntrySink
{
	// <interfuscator:shuffle>
	virtual ~ILoadConfigurationEntrySink(){}
	virtual void OnLoadConfigurationEntry(const char* szKey, const char* szValue, const char* szGroup) = 0;
	virtual void OnLoadConfigurationEntry_End() {}
	// </interfuscator:shuffle>
};
//! \endcond

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
	bool bMinimal;                        //!< Don't load banks.
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

struct SSystemGlobalEnvironment
{
  INetwork* pNetwork = nullptr;
  I3DEngine* p3DEngine = nullptr;
  IScriptSystem* pScriptSystem = nullptr;
  IInput* pInput = nullptr;
  ICryPak* pCryPak = nullptr;
  ITimer* pTimer = nullptr;
  IConsole* pConsole = nullptr;
  ISystem* pSystem = nullptr;
  ILog* pLog = nullptr;
  IRenderer* pRenderer = nullptr;
  IHardwareMouse* pHardwareMouse = nullptr;
  IPlatform* pPlatform;

	ILINE void SetIsDedicated(bool isDedicated)
	{
	#if defined(DEDICATED_SERVER)
		bDedicated = true;
	#else
		bDedicated = isDedicated;
	#endif
	}

  ILINE bool IsEditor() { return false; }
	ILINE const bool IsEditing() const
	{
#if BB_PLATFORM_DESKTOP
		return bEditor && !bEditorGameMode;
#else
		return false;
#endif
	}
	ILINE const bool IsDedicated() const
	{
#if !BB_PLATFORM_DESKTOP
		return false;
#elif defined(DEDICATED_SERVER)
		return true;
#else
		return bDedicated;
#endif
	}

#if BB_PLATFORM_DESKTOP
	bool bDedicatedArbitrator;

private:
	bool bClient;
	bool bEditor;          //!< Engine is running under editor.
	bool bEditorGameMode;  //!< Engine is in editor game mode.
	bool bDedicated;       //!< Engine is in dedicated.
#endif
};

struct ISystem
{
  enum MessageType {
    M_ERROR,
    M_WARNING
  };

	// Begin rendering frame.
	virtual void	RenderBegin() = 0;
	// Render subsystems.
	virtual void	Render() = 0;
	// End rendering frame and swap back buffer.
	virtual void	RenderEnd() = 0;


  virtual bool Init() = 0;
  virtual void Start() = 0;
  virtual bool Update(int updateFlags = 0, int nPauseMode = 0) = 0;
  virtual void Release() = 0;
	//! Returns pointer to the global environment structure.
	virtual SSystemGlobalEnvironment* GetGlobalEnvironment() = 0;
  virtual IGame* CreateGame(IGame* game) = 0;

  virtual IRenderer*              GetIRenderer() = 0;
  virtual ILog*                   GetILog() = 0;
	virtual ICmdLine*               GetICmdLine() = 0;
  virtual IConsole*               GetIConsole() = 0;
  virtual IInput*                 GetIInput() = 0;
  virtual IGame*                  GetIGame() = 0;
  virtual IFont*                  GetIFont() = 0;
  virtual INetwork*               GetINetwork() = 0;
  virtual IWindow*                GetIWindow() = 0;
	virtual IEntitySystem		*       GetIEntitySystem() = 0;
  virtual ICryPak*                GetIPak() = 0;
  virtual IHardwareMouse*         GetIHardwareMouse() = 0;
#if 0
  virtual IInputHandler* GetIInputHandler() = 0;
#endif
  virtual IScriptSystem* GetIScriptSystem() = 0;
  virtual ISystemEventDispatcher* GetISystemEventDispatcher() = 0;

  virtual ITimer* GetITimer() = 0;

  
	// Quit the appliacation
	virtual void	Quit() = 0;
	// Tells the system if it is relaunching or not
	virtual void	Relaunch(bool bRelaunch) = 0;
	// return true if the application is in the shutdown phase
	virtual bool	IsQuitting() = 0;

	// Display error message.
	// Logs it to console and file and error message box.
	// Then terminates execution.
	virtual void Error( const char *sFormat,... ) = 0;
	
	//DOC-IGNORE-BEGIN
	//[Timur] DEPRECATED! Use Validator Warning instead.
	// Display warning message.
	// Logs it to console and file and display a warning message box.
	// Not terminates execution.
	//__declspec(deprecated) virtual void Warning( const char *sFormat,... ) = 0;
	//DOC-IGNORE-END

	// Report warning to current Validator object.
	// Not terminates execution.
	virtual void Warning( EValidatorModule module,EValidatorSeverity severity,int flags,const char *file,const char *format,... ) = 0;
	// Compare specified verbosity level to the one currently set.
	virtual bool CheckLogVerbosity( int verbosity ) = 0;

	// returns true if this is dedicated server application
	virtual bool IsDedicated() {return false;}

  virtual void Log(const char* message) = 0;
  virtual void Error(const char* message) = 0;

  virtual void ShowMessage(const char* message, const char* caption, MessageType messageType) = 0;
  virtual bool IsDevMode() = 0;

  virtual float GetDeltaTime() = 0;

	virtual void SetViewCamera(class CCamera &Camera) = 0;
	virtual CCamera& GetViewCamera() = 0;

	//////////////////////////////////////////////////////////////////////////
	// File version.
	//////////////////////////////////////////////////////////////////////////
	
	virtual const SFileVersion& GetFileVersion() = 0;
	virtual const SFileVersion& GetProductVersion() = 0;

  virtual void EnableGui(bool enable) = 0;

	// Compressed file read & write
	virtual bool WriteCompressedFile(char *filename, void *data, unsigned int bitlen) = 0;
	virtual unsigned int ReadCompressedFile(char *filename, void *data, unsigned int maxbitlen) = 0;
	virtual unsigned int GetCompressedFileSize(char *filename)=0;

	//////////////////////////////////////////////////////////////////////////
	// Configuration.
	//////////////////////////////////////////////////////////////////////////
	// Saves system configuration.
	virtual void SaveConfiguration() = 0;
	// Loads system configuration
	virtual void LoadConfiguration(const string &sFilename)=0;

};

// Global environment variable.
#if defined(SYS_ENV_AS_STRUCT)
  extern SSystemGlobalEnvironment gEnv;
#else
  extern SSystemGlobalEnvironment* gEnv;
#endif

//! Gets the system interface.
inline ISystem* GetISystem()
{
	return gEnv->pSystem;
};

// interface of the DLL
extern "C"
{
  ISYSTEM_API ISystem* CreateSystemInterface(SSystemInitParams& initParams);
}

#define FatalError(...) void(0)
#define CryFatalError(...) FatalError(__VA_ARGS__)

//////////////////////////////////////////////////////////////////////////
// Display error message.
// Logs it to console and file and error message box.
// Then terminates execution.
inline void CryError( const char *format,... )
{ 
	if (!GetISystem())
		return;

	va_list	ArgList;
	char szBuffer[MAX_WARNING_LENGTH];
	va_start(ArgList, format);
	vsprintf(szBuffer, format, ArgList);
	va_end(ArgList);

	GetISystem()->Error( "%s",szBuffer );
}


//////////////////////////////////////////////////////////////////////////
// Display warning message.
// Logs it to console and file and display a warning message box.
// Not terminates execution.
inline void CryWarning( EValidatorModule module,EValidatorSeverity severity,const char *format,... )
{
	if (!GetISystem() || !format)
		return;
	va_list	ArgList;
	char		szBuffer[MAX_WARNING_LENGTH];
	va_start(ArgList, format);
	vsprintf(szBuffer, format, ArgList);
	va_end(ArgList);
	GetISystem()->Warning( module,severity,0,0,szBuffer );
}

//////////////////////////////////////////////////////////////////////////
// Simple log of data with low verbosity.
inline void CryLog( const char *format,... )
{
	if (GetISystem() && GetISystem()->CheckLogVerbosity(8))
	{
		va_list args;
		va_start(args,format);
		GetISystem()->GetILog()->LogV( ILog::eMessage,format,args );
		va_end(args);
	}
}

//! Very rarely used log comment.
void        CryComment(const char*, ...) PRINTF_PARAMS(1, 2);
inline void CryComment(const char* format, ...)
{
	// Fran: we need these guards for the testing framework to work
	if (gEnv && gEnv->pSystem && gEnv->pLog)
	{
		va_list args;
		va_start(args, format);
		gEnv->pLog->LogV(ILog::eComment, format, args);
		va_end(args);
	}
}

//////////////////////////////////////////////////////////////////////////
// Very rarely used log comment.
inline void CryLogComment( const char *format,... )
{
	if (GetISystem() && GetISystem()->CheckLogVerbosity(9))
	{
		va_list args;
		va_start(args,format);
		GetISystem()->GetILog()->LogV( ILog::eMessage,format,args );
		va_end(args);
	}
}

//////////////////////////////////////////////////////////////////////////
// Logs important data that must be printed regardless verbosity.
inline void CryLogAlways( const char *format,... )
{
	if (GetISystem())
	{
		va_list args;
		va_start(args,format);
		GetISystem()->GetILog()->LogV( ILog::eAlways,format,args );
		va_end(args);
	}
}
