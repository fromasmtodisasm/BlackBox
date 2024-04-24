#pragma once
#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Version.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/IValidator.hpp>
#include <cstdarg>

#ifdef SYSTEM_EXPORTS
	#define ISYSTEM_API DLL_EXPORT
#elif defined(IS_MONOLITHIC_BUILD) && defined(CRY_IS_APPLICATION)
	#define ISYSTEM_API
#else
	#define ISYSTEM_API DLL_IMPORT
#endif

//! Static branch-prediction helpers
#define IF(condition, hint) if (condition)
//! Compiler-supported type-checking helper
#define PRINTF_PARAMS(...)

////////////////////////////////////////////////////////////////////////////////////////////////
// Forward declarations
////////////////////////////////////////////////////////////////////////////////////////////////
#include <IXMLDOM.h>
#include <IXml.h>

struct I3DEngine;
struct IAISystem;
struct ICmdLine;
struct IConsole;
struct ICryCharManager;
struct ICryFont;
struct ICryPak;
struct IEntitySystem;
struct IFont;
struct IFrameProfileSystem;
struct IGame;
struct IHardwareMouse;
struct IInput;
struct IInputHandler;
struct ILog;
struct IMovieSystem;
struct IMusicSystem;
struct INetwork;
struct IOutputPrintSink;
struct IPhysicalWorld;
struct IPlatform;
struct IProcess;
struct IProjectManager;
struct IRemoteConsole;
struct IRenderAuxGeom;
struct IRenderer;
struct IScriptSystem;
struct IShaderManager;
struct ISoundSystem;
struct IStreamEngine;
struct ISystem;
struct ITextModeConsole;
struct IThreadManager;
struct ITimer;
struct IValidator;
struct IVisArea;
struct IWindow;
struct IWorld;
struct IGamePlatform;

class CFrameProfilerSection;

//////////////////////////////////////////////////////////////////////////
#define DEFAULT_GAME_PATH     "FarCry"
#define DATA_FOLDER           "FCData"

#define PROC_MENU             1
#define PROC_3DENGINE         2

//ID for script userdata typing (maybe they should be moved into the game.dll)
#define USER_DATA_SOUND       1
#define USER_DATA_TEXTURE     2
#define USER_DATA_OBJECT      3
#define USER_DATA_LIGHT       4
#define USER_DATA_BONEHANDLER 5
#define USER_DATA_POINTER     6

//////////////////////////////////////////////////////////////////////////
enum ESystemUpdateFlags
{
	ESYSUPDATE_IGNORE_AI      = 0x0001,
	ESYSUPDATE_IGNORE_PHYSICS = 0x0002,
	// Special update mode for editor.
	ESYSUPDATE_EDITOR         = 0x0004,
	ESYSUPDATE_MULTIPLAYER    = 0x0008
};

//////////////////////////////////////////////////////////////////////////
enum ESystemConfigSpec
{
	CONFIG_LOW_SPEC      = 0,
	CONFIG_MEDIUM_SPEC   = 1,
	CONFIG_HIGH_SPEC     = 2,
	CONFIG_VERYHIGH_SPEC = 3,
};

#if BB_PLATFORM_ANDROID
	#define USE_ANDROIDCONSOLE
#elif BB_PLATFORM_LINUX || BB_PLATFORM_MAC
	#define USE_UNIXCONSOLE
#elif BB_PLATFORM_IOS
	#define USE_IOSCONSOLE
#elif BB_PLATFORM_WINDOWS
	#define USE_WINDOWSCONSOLE
#endif

#if defined(USE_UNIXCONSOLE) || defined(USE_ANDROIDCONSOLE) || defined(USE_WINDOWSCONSOLE) || defined(USE_IOSCONSOLE)
//#	define USE_DEDICATED_SERVER_CONSOLE
#endif

//! System wide events.
enum ESystemEvent : uint
{
	//! Changes to main window focus.
	//! wparam is not 0 is focused, 0 if not focused.
	ESYSTEM_EVENT_CHANGE_FOCUS = 10,

	//! Moves of the main window.
	//! wparam=x, lparam=y.
	ESYSTEM_EVENT_MOVE         = 11,

	//! Resizes of the main window.
	//! wparam=width, lparam=height.
	ESYSTEM_EVENT_RESIZE       = 12,

	//! Activation of the main window.
	//! wparam=1/0, 1=active 0=inactive.
	ESYSTEM_EVENT_ACTIVATE     = 13,

	ESYSTEM_EVENT_LEVEL_LOAD_START,

	ESYSTEM_EVENT_LEVEL_GAMEPLAY_START,

	ESYSTEM_EVENT_LEVEL_POST_UNLOAD,

	ESYSTEM_EVENT_LANGUAGE_CHANGE,

	//! Sent when the system is doing a full shutdown.
	ESYSTEM_EVENT_FULL_SHUTDOWN,

	//! Sent when the system is doing a fast shutdown.
	ESYSTEM_EVENT_FAST_SHUTDOWN,

	//! Toggle fullscreen.
	//! wparam is 1 means we switched to fullscreen, 0 if for windowed.
	ESYSTEM_EVENT_TOGGLE_FULLSCREEN,

	//! Sent if the System module initialized successfully.
	ESYSTEM_EVENT_SYSTEM_INIT_DONE,

	//! Sent before initializing the renderer.
	ESYSTEM_EVENT_PRE_RENDERER_INIT,

	//! Sent if the window containing the running game loses focus, but application itself still has focus
	//! This is needed because some sub-systems still want to work even without focus on main application
	//! while others would prefer to suspend their operation
	ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE,

	//! Sent once the Editor finished initialization.
	ESYSTEM_EVENT_EDITOR_ON_INIT,
};

//! User defined callback, which can be passed to ISystem.
struct ISystemUserCallback
{
	// <interfuscator:shuffle>
	virtual ~ISystemUserCallback()
	{
	}

	//! This method is called at the earliest point the ISystem pointer can be used the log might not be yet there.
	virtual void OnSystemConnect(ISystem* pSystem)
	{
	}

	//! If working in Editor environment notify user that engine want to Save current document.
	//! This happens if critical error have occurred and engine gives a user way to save data and not lose it due to crash.
#if BB_PLATFORM_WINDOWS
	virtual bool OnSaveDocument() = 0;
#endif

	//! Notifies user that system wants to switch out of current process.
	//! Example: Called when pressing ESC in game mode to go to Menu.
	virtual void OnProcessSwitch()                        = 0;

	//! Notifies user, usually editor, about initialization progress in system.
	virtual void OnInitProgress(const char* sProgressMsg) = 0;

	//! Initialization callback.
	//! This is called early in CSystem::Init(), before any of the other callback methods is called.
	virtual void OnInit(ISystem*)
	{
	}

	//! Shutdown callback.
	virtual void OnShutdown()
	{
	}

	//! Quit callback.
	virtual void OnQuit()
	{
	}

	//! Notify user of an update iteration. Called in the update loop.
	virtual void OnUpdate()
	{
	}

#if 1
	//! Show message by provider.
	virtual EQuestionResult ShowMessage(const char* text, const char* caption, EMessageBox uType) { return eQR_None; }
#endif

	//! Collects the memory information in the user program/application.
	virtual void GetMemoryUsage(class ICrySizer* pSizer) = 0;
	// </interfuscator:shuffle>
};

//! Interface used for getting notified when a system event occurs.
struct ISystemEventListener
{
	// <interfuscator:shuffle>
	virtual ~ISystemEventListener()
	{
	}
	virtual void OnSystemEventAnyThread(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
	{
	}
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) = 0;
	// </interfuscator:shuffle>
};

//! Structure used for getting notified when a system event occurs.
struct ISystemEventDispatcher
{
	// <interfuscator:shuffle>
	virtual ~ISystemEventDispatcher()
	{
	}
	virtual bool RegisterListener(ISystemEventListener* pListener, const char* szName)                         = 0;
	virtual bool RemoveListener(ISystemEventListener* pListener)                                               = 0;

	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam, bool force_queue = false) = 0;
	virtual void Update()                                                                                      = 0;

	virtual uint RegisterEvent(const string& EventName)                                                        = 0;

	//virtual void OnLocaleChange() = 0;
	// </interfuscator:shuffle>
};

//! \cond INTERNAL
//! \note Can be used for LoadConfiguration().
struct ILoadConfigurationEntrySink
{
	// <interfuscator:shuffle>
	virtual ~ILoadConfigurationEntrySink()
	{
	}
	virtual void OnLoadConfigurationEntry(const char* szKey, const char* szValue, const char* szGroup) = 0;
	virtual void OnLoadConfigurationEntry_End()
	{
	}
	// </interfuscator:shuffle>
};
//! \endcond
enum ELoadConfigurationType
{
	eLoadConfigDefault,
	eLoadConfigInit,
	eLoadConfigSystemSpec,
	eLoadConfigGame,
	eLoadConfigLevel
};

enum class ELoadConfigurationFlags : uint32
{
	None                          = 0,
	SuppressConfigNotFoundWarning = BIT(0)
};

//////////////////////////////////////////////////////////////////////////
// Structure passed to Init method of ISystem interface.
struct SSystemInitParams
{
	void*                hInstance;            //
	void*                hWnd;                 //
	char                 szSystemCmdLine[512]; // command line, used to execute the early commands e.g. -DEVMODE "g_gametype ASSAULT"
	ISystemUserCallback* pUserCallback;        //
	ILog*                pLog;                 // You can specify your own ILog to be used by System.
	IValidator*          pValidator;           // You can specify different validator object to use by System.
	IOutputPrintSink*    pPrintSync;           //!< Print Sync which can be used to catch all output from engine.
	const char*          sLogFileName;         // File name to use for log.

	bool                 bEditor;          // When runing in Editor mode.
	bool                 bPreview;         // When runing in Preview mode (Minimal initialization).
	bool                 bTestMode;        // When runing in Automated testing mode.
	bool                 bDedicatedServer; // When runing a dedicated server.
	bool                 bMinimal;         //!< Don't load banks.
	bool                 bShaderCacheGen;  //!< When running in shadercache gen mode.
	bool                 bManualEngineLoop;
	bool                 bExecuteCommandLine;

	ISystem*             pSystem; // Pointer to existing ISystem interface, it will be reused if not NULL.

#if defined(LINUX)
	void (*pCheckFunc)(void*); // authentication function (must be set).
#else
	void* pCheckFunc; // authentication function (must be set).
#endif

	// Initialization defaults.
	SSystemInitParams()
	{
		hInstance = 0;
		hWnd      = 0;
		memset(szSystemCmdLine, 0, sizeof(szSystemCmdLine));
		pLog                = nullptr;
		pValidator          = nullptr;
		pPrintSync          = nullptr;
		pUserCallback       = nullptr;
		sLogFileName        = nullptr;
		bEditor             = false;
		bPreview            = false;
		bTestMode           = false;
		bDedicatedServer    = false;
		bManualEngineLoop   = true;
		bExecuteCommandLine = true;
		pSystem             = nullptr;
		pCheckFunc          = nullptr;
	}
};

struct SSystemGlobalEnvironment
{
	INetwork*            pNetwork            = nullptr;
	I3DEngine*           p3DEngine           = nullptr;
	IScriptSystem*       pScriptSystem       = nullptr;
	IInput*              pInput              = nullptr;
	ICryPak*             pCryPak             = nullptr;
	ITimer*              pTimer              = nullptr;
	IEntitySystem*       pEntitySystem       = nullptr;
	IConsole*            pConsole            = nullptr;
	ISystem*             pSystem             = nullptr;
	ILog*                pLog                = nullptr;
	IRenderer*           pRenderer           = nullptr;
	IRenderAuxGeom*      pAuxGeomRenderer    = nullptr;
	IHardwareMouse*      pHardwareMouse      = nullptr;
	IPlatform*           pPlatform           = nullptr;
	IThreadManager*      pThreadManager      = nullptr;
	IProjectManager*     pProjectManager     = nullptr;
	IFrameProfileSystem* pFrameProfileSystem = nullptr;
	IPhysicalWorld*      pPhysicalWorld      = nullptr;
	IMusicSystem*        pMusicSystem        = nullptr;
	ISoundSystem*        pSoundSystem        = nullptr;
	IMovieSystem*        pMovieSystem        = nullptr;
	IAISystem*           pAISystem           = nullptr;
	ICryFont*            pCryFont            = nullptr;
	IGamePlatform* pGamePlatform = nullptr;

	ILINE void           SetIsDedicated(bool isDedicated)
	{
#if defined(DEDICATED_SERVER)
		bDedicated = true;
#else
		bDedicated = isDedicated;
#endif
	}

	ILINE bool IsEditor()
	{
		return false;
	}
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
#if defined(SYS_ENV_AS_STRUCT)
	//! Remove pointer indirection.
	ILINE SSystemGlobalEnvironment* operator->()
	{
		return this;
	}

	ILINE SSystemGlobalEnvironment& operator*()
	{
		return *this;
	}

	ILINE const bool operator!() const
	{
		return false;
	}

	ILINE operator bool() const
	{
		return true;
	}
#endif

#if BB_PLATFORM_DESKTOP
	bool bDedicatedArbitrator;

private:
	bool bClient;
	bool bEditor;         //!< Engine is running under editor.
	bool bEditorGameMode; //!< Engine is in editor game mode.
	bool bDedicated;      //!< Engine is in dedicated.
#endif
};

struct ISystem
{
	enum MessageType
	{
		M_ERROR,
		M_WARNING
	};

	// Begin rendering frame.
	virtual void                      RenderBegin()                                                                                                                 = 0;
	// Render subsystems.
	virtual void                      Render()                                                                                                                      = 0;
	// End rendering frame and swap back buffer.
	virtual void                      RenderEnd()                                                                                                                   = 0;

	// update _time, _frametime (useful after loading level to apply the time value)
	virtual void                      UpdateScriptSink()                                                                                                            = 0;

	// Renders the statistics; this is called from RenderEnd, but if the
	// Host application (Editor) doesn't employ the Render cycle in ISystem,
	// it may call this method to render the essencial statistics
	virtual void                      RenderStatistics()                                                                                                            = 0;

	// Retrieve the name of the user currently logged in to the computer
	virtual const char*               GetUserName()                                                                                                                 = 0;

	virtual bool                      Init()                                                                                                                        = 0;
	virtual void                      Start()                                                                                                                       = 0;
	virtual bool                      Update(int updateFlags = 0, int nPauseMode = 0)                                                                               = 0;
	virtual void                      Release()                                                                                                                     = 0;
	//! Returns pointer to the global environment structure.
	virtual SSystemGlobalEnvironment* GetGlobalEnvironment()                                                                                                        = 0;
	virtual bool                      IsCVarWhitelisted(const char* szName, bool silent) const                                                                      = 0;
	virtual ISystemUserCallback*      GetUserCallback() const                                                                                                       = 0;
	virtual IGame*                    CreateGame(IGame* game)                                                                                                       = 0;

	// return the related subsystem interface
	virtual I3DEngine*                GetI3DEngine()                                                                                                                = 0;
	virtual IAISystem*                GetAISystem()                                                                                                                 = 0;
	virtual ICmdLine*                 GetICmdLine()                                                                                                                 = 0;
	virtual IConsole*                 GetIConsole()                                                                                                                 = 0;
	virtual ICryCharManager*          GetIAnimationSystem()                                                                                                         = 0;
	virtual ICryFont*                 GetICryFont()                                                                                                                 = 0;
	virtual ICryPak*                  GetIPak()                                                                                                                     = 0;
	virtual IEntitySystem*            GetIEntitySystem()                                                                                                            = 0;
	// new
	virtual IFont*                    GetIFont()                                                                                                                    = 0;
	virtual IFrameProfileSystem*      GetIProfileSystem()                                                                                                           = 0;
	virtual IGame*                    GetIGame()                                                                                                                    = 0;
	// new
	virtual IHardwareMouse*           GetIHardwareMouse()                                                                                                           = 0;
	virtual IInput*                   GetIInput()                                                                                                                   = 0;
	virtual ILog*                     GetILog()                                                                                                                     = 0;
	//FarCry
	//virtual IMemoryManager*			GetIMemoryManager()			= 0;
	virtual IMovieSystem*             GetIMovieSystem()                                                                                                             = 0;
	virtual IMusicSystem*             GetIMusicSystem()                                                                                                             = 0;
	virtual INetwork*                 GetINetwork()                                                                                                                 = 0;
	virtual IPhysicalWorld*           GetIPhysicalWorld()                                                                                                           = 0;
	//new
	virtual IProjectManager*          GetIProjectManager()                                                                                                          = 0;
	//new
	virtual IRemoteConsole*           GetIRemoteConsole()                                                                                                           = 0;
	virtual IRenderer*                GetIRenderer()                                                                                                                = 0;
	virtual IScriptSystem*            GetIScriptSystem()                                                                                                            = 0;
	virtual ISoundSystem*             GetISoundSystem()                                                                                                             = 0;
	virtual IStreamEngine*            GetStreamEngine()                                                                                                             = 0;

	//! Changes current user sub path, the path is always relative to the user documents folder.
	//! Example: "My Games\Crysis"
	virtual void                      ChangeUserPath(const char* sUserPath)                                                                                         = 0;
	//new
	virtual ISystemEventDispatcher*   GetISystemEventDispatcher()                                                                                                   = 0;
	//new
	virtual ITextModeConsole*         GetITextModeConsole()                                                                                                         = 0;
	virtual ITimer*                   GetITimer()                                                                                                                   = 0;
	virtual IValidator*               GetIValidator()                                                                                                               = 0;
	//my
	virtual IWindow*                  GetIWindow()                                                                                                                  = 0;

	// Gets current supported CPU features flags. (CPUF_SSE, CPUF_SSE2, CPUF_3DNOW, CPUF_MMX)
	virtual int                       GetCPUFlags()                                                                                                                 = 0;

	// Get seconds per processor tick
	virtual double                    GetSecondsPerCycle()                                                                                                          = 0;

	// dumps the memory usage statistics to the log
	virtual void                      DumpMemoryUsageStatistics()                                                                                                   = 0;

	// Quit the appliacation
	virtual void                      Quit()                                                                                                                        = 0;
	// Tells the system if it is relaunching or not
	virtual void                      Relaunch(bool bRelaunch)                                                                                                      = 0;
	// return true if the application is in the shutdown phase
	virtual bool                      IsQuitting()                                                                                                                  = 0;

	//! Displays error message.
	//! Logs it to console and file and error message box then terminates execution.
	virtual void                      FatalError(const char* sFormat, ...) PRINTF_PARAMS(2, 3)                                                                      = 0;

	// Display error message.
	// Logs it to console and file and error message box.
	// Then terminates execution.
	virtual void                      Error(const char* sFormat, ...)                                                                                               = 0;

	virtual void                      WarningV(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, va_list args) = 0;
	//DOC-IGNORE-BEGIN
	//[Timur] DEPRECATED! Use Validator Warning instead.
	// Display warning message.
	// Logs it to console and file and display a warning message box.
	// Not terminates execution.
	//__declspec(deprecated) virtual void Warning( const char *sFormat,... ) = 0;
	//DOC-IGNORE-END

	// Report warning to current Validator object.
	// Not terminates execution.
	virtual void                      Warning(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, ...)           = 0;
	// Compare specified verbosity level to the one currently set.
	virtual bool                      CheckLogVerbosity(int verbosity)                                                                                              = 0;

	// returns true if this is dedicated server application
	virtual bool                      IsDedicated()
	{
		return false;
	}

	virtual void                   Log(const char* message)                                                       = 0;
	virtual void                   Error(const char* message)                                                     = 0;

	virtual void                   ShowMessage(const char* message, const char* caption, MessageType messageType) = 0;

	//////////////////////////////////////////////////////////////////////////
	// @param bValue set to true when running on a cheat protected server or a client that is connected to it (not used in singlplayer)
	virtual void                   SetForceNonDevMode(const bool bValue)                                          = 0;
	// @return is true when running on a cheat protected server or a client that is connected to it (not used in singlplayer)
	virtual bool                   GetForceNonDevMode() const                                                     = 0;
	virtual bool                   WasInDevMode() const                                                           = 0;
	virtual bool                   IsDevMode() const                                                              = 0;

	//////////////////////////////////////////////////////////////////////////

	virtual XDOM::IXMLDOMDocument* CreateXMLDocument()                                                            = 0;

	//////////////////////////////////////////////////////////////////////////
	// IXmlNode interface.
	//////////////////////////////////////////////////////////////////////////

	// Creates new xml node.
	virtual XmlNodeRef             CreateXmlNode(const char* sNodeName = "")                                      = 0;
	// Load xml file, return 0 if load failed.
	virtual XmlNodeRef             LoadXmlFile(const char* sFilename)                                             = 0;
	// Load xml from string, return 0 if load failed.
	virtual XmlNodeRef             LoadXmlFromString(const char* sXmlString)                                      = 0;

	// Set rate of Garbage Collection for script system.
	// @param fRate in seconds
	virtual void                   SetGCFrequency(const float fRate)                                              = 0;

	/* Set the active process
		@param process a pointer to a class that implement the IProcess interface
	*/
	virtual void                   SetIProcess(IProcess* process)                                                 = 0;
	/* Get the active process
		@return a pointer to the current active process
	*/
	virtual IProcess*              GetIProcess()                                                                  = 0;

#if (defined(WIN32) || defined(PS2)) && defined(CHANGE_DEFINE)
	virtual IRenderer* CreateRenderer(bool fullscreen, void* hinst, void* hWndAttach = 0) = 0;
#endif

	// Returns true if system running in Test mode.
	virtual bool     IsTestMode() const                                                        = 0;

	virtual void     ShowDebugger(const char* pszSourceFile, int iLine, const char* pszReason) = 0;

	//////////////////////////////////////////////////////////////////////////
	// Frame profiler functions
	virtual void     SetFrameProfiler(bool on, bool display, char* prefix)                     = 0;

	// Starts section profiling.
	virtual void     StartProfilerSection(CFrameProfilerSection* pProfileSection)              = 0;
	// Stops section profiling.
	virtual void     EndProfilerSection(CFrameProfilerSection* pProfileSection)                = 0;

	virtual float    GetDeltaTime()                                                            = 0;

	virtual void     ExecuteCommandLine()                                                      = 0;

	virtual void     SetViewCamera(class CCamera& Camera)                                      = 0;
	virtual CCamera& GetViewCamera()                                                           = 0;
	//////////////////////////////////////////////////////////////////////////

	virtual void     Deltree(const char* szFolder, bool bRecurse)
	{
		Error("%s not implemented", __FUNCTION__);
	}

	//////////////////////////////////////////////////////////////////////////
	// File version.
	//////////////////////////////////////////////////////////////////////////

	virtual const SFileVersion& GetFileVersion()                                                                 = 0;
	virtual const SFileVersion& GetProductVersion()                                                              = 0;

	virtual void                EnableGui(bool enable)                                                           = 0;

	// Compressed file read & write
	virtual bool                WriteCompressedFile(char* filename, void* data, unsigned int bitlen)             = 0;
	virtual unsigned int        ReadCompressedFile(char* filename, void* data, unsigned int maxbitlen)           = 0;
	virtual unsigned int        GetCompressedFileSize(char* filename)                                            = 0;

	//////////////////////////////////////////////////////////////////////////
	// Configuration.
	//////////////////////////////////////////////////////////////////////////
	// Saves system configuration.
	virtual void                SaveConfiguration()                                                              = 0;
	// Loads system configuration

	//! Loads system configuration
	//! \param pCallback 0 means normal LoadConfigVar behavior is used.
	//! \param bQuiet when set to true will suppress warning message if config file is not found.
	virtual void                LoadConfiguration(const char* sFilename, ILoadConfigurationEntrySink* pSink = 0, ELoadConfigurationType configType = eLoadConfigDefault,
	                                              ELoadConfigurationFlags flags = ELoadConfigurationFlags::None) = 0;

	virtual const char*         GetRootFolder() const                                                            = 0;
	//! Starts a new frame, updates engine systems, game logic and finally renders.
	//! \return Returns true if the engine should continue running, false to quit.
	virtual bool                DoFrame(int updateFlags = 0)                                                     = 0;
};

namespace Env
{
// Global environment variable.
#if defined(SYS_ENV_AS_STRUCT)
	extern SSystemGlobalEnvironment gEnv;
	inline auto                     Set(const SSystemGlobalEnvironment& Env) { gEnv = Env; }
	inline SSystemGlobalEnvironment* Get() { return &gEnv; }
#else
	extern SSystemGlobalEnvironment* gEnv;
	inline auto                      Set(SSystemGlobalEnvironment* Env) { gEnv = Env; }
	inline SSystemGlobalEnvironment* Get() { return gEnv; }
#endif


	//////////////////////////////////////////////////
	inline auto System() { return gEnv->pSystem; }
	//////////////////////////////////////////////////
	inline auto AuxGeomRenderer() { return gEnv->pAuxGeomRenderer; }
	inline I3DEngine* I3DEngine() { return gEnv->p3DEngine; }
	inline auto AISystem() { return nullptr; }
	inline auto CmdLine() { return nullptr; }
	inline auto Console() { return gEnv->pConsole; }
	inline auto AnimationSystem() { return nullptr; }
	inline auto CryFont() { return gEnv->pCryFont; }
	inline auto CryPak() { return gEnv->pCryPak; }
	inline auto EntitySystem() { return gEnv->pEntitySystem; }
	inline auto Font() { return gEnv->pCryFont; }
	inline auto ProfileSystem() { return System()->GetIProfileSystem(); }
	inline auto Game() { return nullptr; }
	inline auto HardwareMouse() { return gEnv->pHardwareMouse; }
	inline auto Input() { return gEnv->pInput; }
	inline auto Log() { return gEnv->pLog; }
	inline auto MemoryManager() { return nullptr; }
	inline auto MovieSystem() { return nullptr; }
	inline auto MusicSystem() { return nullptr; }
	inline auto Network() { return gEnv->pNetwork; }
	inline auto PhysicalWorld() { return gEnv->pPhysicalWorld; }
	inline auto ProjectManager() { return gEnv->pProjectManager; }
	inline auto RemoteConsole() { return nullptr; }
	inline auto Renderer() { return gEnv->pRenderer; }
	inline auto ScriptSystem() { return gEnv->pScriptSystem; }
	inline auto SoundSystem() { return System()->GetISoundSystem(); }
	inline auto StreamEngine() { return nullptr; }
	inline auto SystemEventDispatcher() { return Env::System()->GetISystemEventDispatcher(); }
	inline auto TextModeConsole() { return Env::System()->GetITextModeConsole(); }
	inline auto Timer() { return gEnv->pTimer; }
	inline auto Validator() { return System()->GetIValidator(); }
	inline auto Window() { return System()->GetIWindow(); }
	inline auto GamePlatform() { return gEnv->pGamePlatform; }

} // namespace Env

//! Gets the system interface.
inline ISystem* GetISystem()
{
	return Env::System();
};

//! This function must be called once by each module at the beginning, to setup global pointers.
extern "C" DLL_EXPORT void ModuleInitISystem(ISystem* pSystem, const char* moduleName);

// System DLL Exports.
typedef ISystem* (*PFNCREATESYSTEMINTERFACE)(SSystemInitParams& initParams /*, bool bManualEngineLoop*/);

// interface of the DLL
extern "C"
{
	ISYSTEM_API ISystem* CreateSystemInterface(SSystemInitParams& initParams);
}

//! Displays error message, logs it to console and file and error message box, tThen terminates execution.
void        CryFatalError(const char*, ...) PRINTF_PARAMS(1, 2);
inline void CryFatalError(const char* format, ...)
{
	if (!Env::Get() || !Env::System())
		return;

	va_list ArgList;
	char    szBuffer[MAX_WARNING_LENGTH];
	va_start(ArgList, format);
	vsprintf(szBuffer, format, ArgList);
	va_end(ArgList);

	Env::System()->FatalError("%s", szBuffer);
}
//////////////////////////////////////////////////////////////////////////
// Display error message.
// Logs it to console and file and error message box.
// Then terminates execution.
inline void CryError(const char* format, ...)
{
	if (!GetISystem())
		return;

	va_list ArgList;
	char    szBuffer[MAX_WARNING_LENGTH];
	va_start(ArgList, format);
	vsprintf(szBuffer, format, ArgList);
	va_end(ArgList);

	GetISystem()->Error("%s", szBuffer);
}

//////////////////////////////////////////////////////////////////////////
// Display warning message.
// Logs it to console and file and display a warning message box.
// Not terminates execution.
inline void CryWarning(EValidatorModule module, EValidatorSeverity severity, const char* format, ...)
{
	if (!GetISystem() || !format)
		return;
	va_list ArgList;
	char    szBuffer[MAX_WARNING_LENGTH];
	va_start(ArgList, format);
	vsprintf(szBuffer, format, ArgList);
	va_end(ArgList);
	GetISystem()->Warning(module, severity, 0, 0, szBuffer);
}

//////////////////////////////////////////////////////////////////////////
// Simple log of data with low verbosity.
inline void CryLog(const char* format, ...)
{
	if (GetISystem() && GetISystem()->CheckLogVerbosity(8))
	{
		va_list args;
		va_start(args, format);
		GetISystem()->GetILog()->LogV(ILog::eMessage, format, args);
		va_end(args);
	}
}

//! Very rarely used log comment.
void        CryComment(const char*, ...) PRINTF_PARAMS(1, 2);
inline void CryComment(const char* format, ...)
{
	// Fran: we need these guards for the testing framework to work
	if (Env::Get() && Env::System() && Env::Log())
	{
		va_list args;
		va_start(args, format);
		Env::Log()->LogV(ILog::eComment, format, args);
		va_end(args);
	}
}

//////////////////////////////////////////////////////////////////////////
// Very rarely used log comment.
inline void CryLogComment(const char* format, ...)
{
	if (GetISystem() && GetISystem()->CheckLogVerbosity(9))
	{
		va_list args;
		va_start(args, format);
		GetISystem()->GetILog()->LogV(ILog::eMessage, format, args);
		va_end(args);
	}
}

//////////////////////////////////////////////////////////////////////////
// Logs important data that must be printed regardless verbosity.
inline void CryLogAlways(const char* format, ...)
{
	if (GetISystem())
	{
		va_list args;
		va_start(args, format);
		GetISystem()->GetILog()->LogV(ILog::eAlways, format, args);
		va_end(args);
	}
}

//////////////////////////////////////////////////////////////////////////
// Additional headers.
//////////////////////////////////////////////////////////////////////////
//#include <BlackBox/System/FrameProfiler.hpp>
