#pragma once

#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Core/Stream.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include "CmdLine.hpp"
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/FrameProfiler.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/IWindowMessageHandler.h>
#include "Timer.hpp"
#include "Window.hpp"
#include "PakVars.h"
#include <BlackBox/System/IProcess.hpp>

#include "ScriptBinds/ScriptObjectConsole.hpp"
#include "ScriptBinds/ScriptObjectRenderer.hpp"
#include "ScriptBinds/ScriptObjectScript.hpp"
#include "ScriptBinds/ScriptObjectSound.hpp"

#include <Font/NullFont.h>

#include <BlackBox/Core/Platform/CryLibrary.h>

#include <BlackBox/System/File/ICryPak.hpp>
#include <BlackBox/Core/Path.hpp>

#include <Legacy\System.h>

#define DEFAULT_APP_NAME        "BlackBox"

#define DLL_MODULE_INIT_ISYSTEM "ModuleInitISystem"

struct IWindow;

class CScriptObjectConsole;
class CScriptObjectSystem;
class CScriptObjectScript;
class CScriptObjectRenderer;

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
	#define USE_DEDICATED_SERVER_CONSOLE
#endif

#if !BB_PLATFORM_LINUX && !BB_PLATFORM_ANDROID && !BB_PLATFORM_DURANGO && !BB_PLATFORM_APPLE && !BB_PLATFORM_ORBIS
	#define DOWNLOAD_MANAGER
#endif

#ifdef DOWNLOAD_MANAGER
	#include "DownloadManager.h"
#endif //DOWNLOAD_MANAGER
#include <BlackBox/System/ZLib/IZlibDecompressor.h>

typedef void* WIN_HMODULE;

using stack_string = string;

struct SSystemCVars
{
	int sys_no_crash_dialog = 0;
	int sys_dump_aux_threads;
	int sys_keyboard_break;
	int sys_dump_type;
	int sys_MaxFPS;

#ifdef USE_HTTP_WEBSOCKETS
	int sys_simple_http_base_port;
#endif

	int     sys_error_debugbreak;

	int     sys_enable_crash_handler;

	int     sys_intromoviesduringinit;
	ICVar*  sys_splashscreen;

	int     sys_filesystemCaseSensitivity;

	int     sys_stop_at;

	PakVars pakVars;

#if BB_PLATFORM_WINDOWS || BB_PLATFORM_DURANGO
	int sys_display_threads;
#endif

#if CRY_PLATFORM_WINDOWS
	int sys_highrestimer;
#endif

	int    sys_vr_support;

	ICVar* sys_build_folder;
};
extern SSystemCVars g_cvars;

class CSystem;
class CDownloadManager;

IThreadManager* CreateThreadManager();
struct SubsystemWrapper : public _i_reference_target_t
{
	HMODULE m_Handle;
	char    m_Name[64];

	SubsystemWrapper(HMODULE handle, const char* name)
	    : m_Handle(handle)
	{
		strncpy(m_Name, name, std::min(strlen(name) + 1, size_t(64)));
	}

	~SubsystemWrapper()
	{
		CryFreeLibrary(m_Handle);
	}
};

/*
   ===========================================
   The System interface Class
   ===========================================
 */
class CSystem final : public ISystem, public IInputEventListener, public IConsoleVarSink, public ISystemEventListener, public IWindowMessageHandler, public ILoadConfigurationEntrySink, IRendererCallbackClient
{
public:
	CSystem(SSystemInitParams& initParams);
	~CSystem();
	void              PreprocessCommandLine();
	void              ProcessCommandLine();

	virtual void      SetGCFrequency(const float fRate) override;
	virtual void      SetIProcess(IProcess* process) override;
	virtual IProcess* GetIProcess() override;
#if (defined(WIN32) || defined(PS2)) && defined(CHANGE_DEFINE)
	virtual IRenderer* CreateRenderer(bool fullscreen, void* hinst, void* hWndAttach = 0) = 0;
#endif

	void                              OnRenderer_BeforeEndFrame() override;

	// Inherited via ISystem
	virtual bool                      Init() override;
	bool                              InitLog();
	virtual void                      Start() override;
	virtual bool                      Update(int updateFlags = 0, int nPauseMode = 0) override;
	virtual void                      UpdateScriptSink() override;
	virtual void                      RenderBegin() override;
	virtual void                      Render() override;
	virtual void                      RenderEnd() override;
	virtual void                      Release() override;
	virtual SSystemGlobalEnvironment* GetGlobalEnvironment() override
	{
		return &m_env;
	}
	virtual ISystemUserCallback* GetUserCallback() const override { return m_pUserCallback; }

	virtual I3DEngine*           GetI3DEngine() override { return m_env.p3DEngine; }
	virtual IAISystem*           GetAISystem() override { return m_env.pAISystem; }
	virtual ICmdLine*            GetICmdLine() override { return m_pCmdLine; };
	virtual IConsole*            GetIConsole() override
	{
		return m_env.pConsole;
	}
	IZLibDecompressor*           GetIZLibDecompressor() { return m_pIZLibDecompressor; }
	virtual ICryCharManager*     GetIAnimationSystem() { NOT_IMPLEMENTED_V; }
	virtual ICryFont*            GetICryFont() { return &m_Font; }
	virtual ICryPak*             GetIPak() override;
	virtual IEntitySystem*       GetIEntitySystem() override;
	virtual IFont*               GetIFont() override;
	virtual IFrameProfileSystem* GetIProfileSystem() { return m_env.pFrameProfileSystem; }
	virtual IGame*               GetIGame() override
	{
		return m_pGame;
	}
	virtual IHardwareMouse* GetIHardwareMouse() override { return m_env.pHardwareMouse; };
	virtual IInput*         GetIInput() override
	{
		return m_env.pInput;
	}
	virtual ILog* GetILog() override
	{
		return m_env.pLog;
	}
	virtual IMovieSystem*    GetIMovieSystem() { return m_env.pMovieSystem; };
	virtual IMusicSystem*    GetIMusicSystem() { return m_env.pMusicSystem; };
	virtual INetwork*        GetINetwork() override;
	virtual IPhysicalWorld*  GetIPhysicalWorld() { return m_env.pPhysicalWorld; }
	virtual IProjectManager* GetIProjectManager() override;
	virtual IRemoteConsole*  GetIRemoteConsole() override;
	virtual IRenderer*       GetIRenderer() override
	{
		return m_env.pRenderer;
	}
	virtual IScriptSystem*          GetIScriptSystem() override;
	virtual ISoundSystem*           GetISoundSystem() { return m_env.pSoundSystem; }
	virtual IStreamEngine*          GetStreamEngine() override;
	virtual ISystemEventDispatcher* GetISystemEventDispatcher() override { return m_pSystemEventDispatcher; }
	virtual ITextModeConsole*       GetITextModeConsole() override;
	virtual ITimer*                 GetITimer() override;
	virtual IValidator*             GetIValidator() override { return m_pValidator; };
	virtual IWindow*                GetIWindow() override;

	virtual int                     GetCPUFlags() override;
	virtual double                  GetSecondsPerCycle() override;
	virtual void                    DumpMemoryUsageStatistics() override;
	virtual IGame*                  CreateGame(IGame* game) override;
	virtual void                    Quit() override;
	virtual void                    ShowMessage(const char* message, const char* caption, MessageType messageType) override;
	virtual void                    Log(const char* message) override;
	virtual bool                    OnInputEvent(const SInputEvent& event) override;
	bool                            ConfigLoad(const char* file);
	//////////////////////////////////////////////////////////////////////////
	virtual void                    SetForceNonDevMode(const bool bValue) override;
	virtual bool                    GetForceNonDevMode() const override;
	virtual bool                    WasInDevMode() const override;
	virtual bool                    IsDevMode() const override;
	virtual void                    Error(const char* message) override;
	virtual void                    OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	virtual bool                    OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;
	virtual float                   GetDeltaTime() override;
	virtual const SFileVersion&     GetFileVersion() override;
	virtual const SFileVersion&     GetProductVersion() override;
	const SFileVersion&             GetBuildVersion();
	void                            SystemVersionChanged(ICVar* pCVar);
	void                            SetVersionInfo(const char* const szVersion);
	void                            QueryVersionInfo();
	virtual const char*             GetRootFolder() const override;
	void                            SetViewCamera(CCamera& Camera) override { m_ViewCamera = Camera; }
	CCamera&                        GetViewCamera() override { return m_ViewCamera; }
	virtual void										CreateEntityScriptBinding(IEntity* ent) override;

	virtual bool                    DoFrame(int updateFlags = 0) override;
	virtual void                    EnableGui(bool enable) override;
	virtual void                    SaveConfiguration() override;

	virtual bool                    IsTestMode() const override;
	virtual void                    ShowDebugger(const char* pszSourceFile, int iLine, const char* pszReason) override;
	virtual void                    SetFrameProfiler(bool on, bool display, char* prefix) override;
	virtual void                    StartProfilerSection(CFrameProfilerSection* pProfileSection) override;
	virtual void                    EndProfilerSection(CFrameProfilerSection* pProfileSection) override;

	virtual void                    LoadConfiguration(const char* sFilename, ILoadConfigurationEntrySink* pSink = 0, ELoadConfigurationType configType = eLoadConfigDefault,
	                                                  ELoadConfigurationFlags flags = ELoadConfigurationFlags::None) override;
	virtual void                    OnLoadConfigurationEntry(const char* szKey, const char* szValue, const char* szGroup) override;

	virtual void                    Relaunch(bool bRelaunch) override;
	virtual bool                    IsQuitting() override;
	virtual void                    Error(const char* sFormat, ...) override;
	virtual void                    Warning(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, ...) override;
	void                            WarningV(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, va_list args) override;
	virtual bool                    CheckLogVerbosity(int verbosity) override;

	virtual bool                    WriteCompressedFile(char* filename, void* data, unsigned int bitlen) override;
	virtual unsigned int            ReadCompressedFile(char* filename, void* data, unsigned int maxbitlen) override;
	virtual unsigned int            GetCompressedFileSize(char* filename) override;

	virtual void                    OnAfterVarChange(ICVar* pVar) override;
	virtual void                    OnVarUnregister(ICVar* pVar) override;

	virtual void                    RenderStatistics() override;
	virtual const char*             GetUserName() override;

	virtual void                    FatalError(const char* sFormat, ...);
	virtual void                    ExecuteCommandLine() override;
	virtual bool                    IsCVarWhitelisted(const char* szName, bool silent) const override;

public:
	void              RunMainLoop();
	void              SleepIfNeeded();

	void              InitResourceCacheFolder();

	//virtual ICryFactory* LoadModuleWithFactory(const char* dllName, const CryInterfaceID& moduleInterfaceId) override;
	/*virtual */ bool UnloadEngineModule(const char* dllName);
	WIN_HMODULE       LoadDynamicLibrary(const char* dllName, bool bQuitIfNotFound = true, bool bLogLoadingInfo = false);
	bool              UnloadDynamicLibrary(const char* dllName);
	void              GetLoadedDynamicLibraries(std::vector<string>& moduleNames) const;

public:
	void         OpenBasicPaks(bool bLoadGamePaks);

	void				 InitLocalization();
	void				 OpenLanguagePak(const char* sLanguage);

	virtual bool IsMODValid(const char* szMODName) const
	{
		if (!szMODName || strstr(szMODName, ".") || strstr(szMODName, "\\") || stricmp(szMODName, PathUtil::GetGameFolder().c_str()) == 0)
			return (false);
		return (true);
	}

private:
	//! @name Initialization routines
	//@{

	bool   InitConsole();
	ICVar* attachVariable(const char* szVarName, int* pContainer, const char* szComment, int dwFlags = 0);
	bool   InitRender();
	bool   InitInput();
	bool   InitScriptSystem();
	bool   InitEntitySystem();
	bool   InitNetwork();
	bool   InitGUI();
	bool   Init3DEngine();
	bool   InitSoundSystem();
	bool   InitSubSystem();
	bool   InitPhysics();
	bool	 InitGamePlatform();
	bool   LoadCrynetwork();
	bool   OpenRenderLibrary(std::string_view render);
	bool   CloseRenderLibrary(std::string_view render);

	bool   InitFileSystem_LoadEngineFolders();

	void   ParseCMD();
	void   LoadScreen();
	bool   InitScripts();
	void   ReleaseScripts();
	bool   InitFileSystem(/*const IGameStartup* pGameStartup*/);
	void   SetWorkingDirectory(const std::string& path) const;
	void   LogCommandLine() const;
	void   Tests();
	void   PollEvents();

	//////////////////////////////////////////////////////////////////////////
	// Threading functions.
	//////////////////////////////////////////////////////////////////////////
	void   InitThreadSystem();
	void   ShutDownThreadSystem();
	//////////////////////////////////////////////////////////////////////////
	// Helper functions.
	//////////////////////////////////////////////////////////////////////////
	void   CreateRendererVars(const SSystemInitParams& startupParams);
	void   CreateSystemVars();

	void   AddCVarGroupDirectory(const string& sPath);

	void   ShutDown();

	void   LogVersion();

	void   UnloadSubsystems();

	template<typename L, typename P>
	inline P GetProcedure(L lib, const char* name)
	{
		return reinterpret_cast<P>(CryGetProcAddress(lib, name));
	}

public:
	//! Update screen during loading.
	void UpdateLoadingScreen()
	{
	}

	template<typename Proc>
	inline bool InitStatic(const char* proc_name, std::function<bool(Proc proc)> f)
	{
		auto P = GetProcedure<HANDLE, Proc>(NULL, proc_name);
		if (P)
		{
			typedef void* (*PtrFunc_ModuleInitISystem)(ISystem * pSystem, const char* moduleName);
			PtrFunc_ModuleInitISystem pfnModuleInitISystem = (PtrFunc_ModuleInitISystem)CryGetProcAddress(NULL, DLL_MODULE_INIT_ISYSTEM);
			if (pfnModuleInitISystem)
			{
				pfnModuleInitISystem(Env::System(), "");
			}
			return f(P);
		}
		else
		{
			CryError("Entrypoint %s not found", proc_name);
		}
		return false;
	}

	template<typename Proc>
	inline bool LoadSubsystem(const char* szModulePath, const char* proc_name, std::function<bool(Proc proc)> f)
	{
		auto initStatic = InitStatic(proc_name, f);
		if (initStatic)
			return true;
		//Env::System()->Log("Loading...");
		stack_string modulePath = szModulePath;
		modulePath              = CrySharedLibraryPrefix + PathUtil::ReplaceExtension(modulePath.c_str(), CrySharedLibraryExtension);

		stack_string msg;
		msg = "Loading Module ";
		msg += modulePath;
		msg += "...";

		if (Env::System()->GetUserCallback())
		{
			Env::System()->GetUserCallback()->OnInitProgress(msg.c_str());
		}

		auto L = CryLoadLibrary(modulePath.c_str());
		if (L)
		{
			auto P = GetProcedure<decltype(L), Proc>(L, proc_name);
			if (P)
			{
				typedef void* (*PtrFunc_ModuleInitISystem)(ISystem * pSystem, const char* moduleName);
				PtrFunc_ModuleInitISystem pfnModuleInitISystem = (PtrFunc_ModuleInitISystem)CryGetProcAddress(L, DLL_MODULE_INIT_ISYSTEM);
				if (pfnModuleInitISystem)
				{
					pfnModuleInitISystem(Env::System(), modulePath.c_str());
				}
				auto subsystem = _smart_ptr(new SubsystemWrapper(L, modulePath.c_str()));
				m_Subsystems.push_back(subsystem);
				m_moduleDLLHandles[modulePath.c_str()] = L;
				return f(P);
			}
			else
			{
				CryError("Entrypoint %s not found", proc_name);
			}
			return false;
		}
		else
		{
#if 0
			if (bQuitIfNotFound)
#endif
			{
#if BB_PLATFORM_LINUX || BB_PLATFORM_ANDROID || BB_PLATFORM_APPLE
				CryFatalError("Error loading dynamic library: %s, error :  %s\n", modulePath, dlerror());
				fprintf(stderr, "dlopen failed: %s\n", dlerror());
#else
				CryFatalError("Error loading dynamic library: %s, error code %d", modulePath.c_str(), GetLastError());
#endif

				Env::System()->Quit();
			}

			return false;
			Env::System()->Log("Library not found");
		}
		return false;
	}

	template<class T>
	struct Sizer
	{
		typedef void (T::*Func)(ICrySizer*) const;
	};

	void GetMemoryUsage(ICrySizer* pSizer) const
	{
		pSizer->AddObject(this, sizeof(*this));
		pSizer->AddObject(m_env.pConsole);
	}

	enum class Align
	{
		Left,
		Right,
		Center,
		Bottom,
		Up,
	};

#if 0
	template<class T>
	void PrintMemoryUsageForName(const char* name, typename Sizer<T>::Func fn, T* This, float px, float py)
	{
		static char  stats[256];
		CrySizerImpl sizer;
		SIZER_COMPONENT_NAME(&sizer, name);
		(This->*fn)(&sizer);
		auto len   = sprintf(stats, "%s memory usage: %d", name, sizer.GetTotalSize());
		stats[len] = 0;

		PrintRightAlignedText(py, stats);
	}
#endif

	void RenderStats();
#if BB_PLATFORM_WINDOWS
	bool GetWinGameFolder(char* szMyDocumentsPath, int maxPathSize);
#endif

protected:
	std::vector<_smart_ptr<SubsystemWrapper>> m_Subsystems;
	CCmdLine*                                 m_pCmdLine = nullptr;

private:
	// System environment.
#if defined(SYS_ENV_AS_STRUCT)
	//since gEnv is a global var, this should just be a reference for code consistency
	SSystemGlobalEnvironment& m_env;
#else
	SSystemGlobalEnvironment m_env;
#endif
	CTimer                  m_Time;         //!<
	CCamera                 m_ViewCamera;   //!<
	volatile bool           m_bQuit{false}; //!< if is true the system is quitting. Volatile as it may be polled from multiple threads.

	//IInput* m_pInput;
	ICryPak*                m_pCryPak       = nullptr;
	IGame*                  m_pGame         = nullptr;
	IFont*                  m_pFont         = nullptr;
	IFont*                  m_pBlackBoxFont = nullptr;
	IWindow*                m_pWindow       = nullptr;
	IValidator*             m_pValidator; //!< Pointer to validator interface.
	//IEntitySystem*	  m_pEntitySystem	 = nullptr;
	INetwork*               m_pNetwork               = nullptr;
	ITextModeConsole*       m_pTextModeConsole       = nullptr;
	//! system event dispatcher
	ISystemEventDispatcher* m_pSystemEventDispatcher = nullptr;

	//! DLLs handles.
	struct SDllHandles
	{
		WIN_HMODULE hRenderer;
		WIN_HMODULE hInput;
		WIN_HMODULE hSound;
		WIN_HMODULE hEntitySystem;
		WIN_HMODULE hNetwork;
		WIN_HMODULE hPhysics;
		WIN_HMODULE hFont;
		WIN_HMODULE hScript;
		WIN_HMODULE h3DEngine;
		WIN_HMODULE hGame;
		WIN_HMODULE hLiveCreate;
		WIN_HMODULE hInterface;
	};
	SDllHandles                                                            m_dll = {0};

	std::unordered_map<string, WIN_HMODULE /*, stl::hash_strcmp<string>*/> m_moduleDLLHandles;

	CTimeValue                                                             m_lastTickTime;

	CScriptObjectConsole*                                                  m_ScriptObjectConsole  = nullptr;
	CScriptObjectSystem*                                                   m_ScriptObjectSystem   = nullptr;
	CScriptObjectSound*                                                    m_ScriptObjectSound    = nullptr;
	CScriptObjectScript*                                                   m_ScriptObjectScript   = nullptr;
	CScriptObjectRenderer*                                                 m_ScriptObjectRenderer = nullptr;

public:
	//! Pointer to the download manager
	CDownloadManager* m_pDownloadManager;

private:
	ICVar*               cvGameName      = nullptr;
	//////////////////////////////////////////////////////////////////////////
	//! User define callback for system events.
	ISystemUserCallback* m_pUserCallback = nullptr;

	//SSystemInitParams& m_startupParams;
	SSystemInitParams    m_startupParams;

	SFileVersion         m_FileVersion;
	SFileVersion         m_ProductVersion;
	SFileVersion         m_BuildVersion;

	//! System to access zlib decompressor
	IZLibDecompressor*   m_pIZLibDecompressor;

	uint64_t             NOW;
	uint64_t             LAST;
	double               m_DeltaTime = 0.0;

	//! global root folder
	string               m_root;
	int                  m_iApplicationInstance;

	//! game path folder
	char	m_szGameMOD[MAX_PATH];

	//! to hold the values stored in system.cfg
	//! because editor uses it's own values,
	//! and then saves them to file, overwriting the user's resolution.
	int                  m_iHeight    = 0;
	int                  m_iWidth     = 0;
	int                  m_iColorBits = 0;

	// System console variables.
	//////////////////////////////////////////////////////////////////////////
	float                m_rIntialWindowSizeRatio;
	int                  m_rWidth;
	int                  m_rHeight;
	int                  m_rColorBits;
	int                  m_rDepthBits;
	int                  m_rStencilBits;
	int                  m_rFullscreen;
	//ICVar* m_rFullsceenNativeRes;
	//ICVar* m_rWindowState;
	ICVar*               m_rDriver;
	ICVar*               m_sysNoUpdate{};

	// DLL names
	ICVar* m_sys_dll_ai;
	ICVar* m_sys_dll_response_system;
	ICVar* m_sys_user_folder;
#if !defined(_RELEASE)
	ICVar* m_sys_resource_cache_folder;
#endif
	ICVar* m_sys_spec;


	int              m_rDisplayInfo;
	int              m_rDebug;
	int              m_rTonemap;
	int              m_rSkipShaderCache;

	bool             m_bIsActive = true;

	string           m_RootFolder;

	int              sys_dump_memstats = false;

	IProcess*        m_pProcess{};

	INetwork*        m_pNetworkLegacy;
	legacy::ISystem* m_pSystemLegacy;
	CCryNullFont     m_Font;

	bool             m_bCanSwitch = false;
	// Pause mode.
	bool             m_bPaused;
	uint8            m_PlatformOSCreateFlags;
	bool             m_bNoUpdate;

	uint64           m_nUpdateCounter;

	// ISystem interface
public:
	virtual XDOM::IXMLDOMDocument* CreateXMLDocument() override;
	virtual XmlNodeRef             CreateXmlNode(const char* sNodeName) override;
	virtual XmlNodeRef             LoadXmlFile(const char* sFilename) override;
	virtual XmlNodeRef             LoadXmlFromString(const char* sXmlString) override;

	// Inherited via ISystem
	virtual void                   ChangeUserPath(const char* sUserPath) override;
	int                            GetApplicationInstance();
};

void AddInternalCommands(ISystem* pSystem);
