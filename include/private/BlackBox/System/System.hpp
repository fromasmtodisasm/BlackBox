#pragma once

#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Core/Stream.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/GUI/ImGuiManager.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/System/CmdLine.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/IWindowMessageHandler.h>
#include <BlackBox/System/Timer.hpp>
#include <BlackBox/System/Window.hpp>
#include <BlackBox/System/PakVars.h>

#include <BlackBox/ScriptSystem/ScriptObjectConsole.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectRenderer.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectScript.hpp>

#include <BlackBox/Core/Platform/CryLibrary.h>

#define DEFAULT_APP_NAME "BlackBox"

#define DLL_MODULE_INIT_ISYSTEM "ModuleInitISystem"

struct IWindow;

class CScriptObjectConsole;
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

struct SSystemCVars
{
	int    sys_no_crash_dialog = 0;
	int    sys_dump_aux_threads;
	int    sys_keyboard_break;
	int    sys_dump_type;
	int    sys_MaxFPS;

#ifdef USE_HTTP_WEBSOCKETS
	int sys_simple_http_base_port;
#endif

	int     sys_error_debugbreak;

	int     sys_enable_crash_handler;

	int     sys_intromoviesduringinit;
	ICVar*  sys_splashscreen;

	int     sys_filesystemCaseSensitivity;

	int sys_stop_at;

	PakVars pakVars;

#if CRY_PLATFORM_WINDOWS || CRY_PLATFORM_DURANGO
	int sys_display_threads;
#endif

#if CRY_PLATFORM_WINDOWS
	int sys_highrestimer;
#endif

	int sys_vr_support;
};
extern SSystemCVars g_cvars;

class CSystem;

IThreadManager* CreateThreadManager();
struct SubsystemWrapper : public _i_reference_target_t
{
	HMODULE m_Handle;
	char m_Name[64];

	SubsystemWrapper(HMODULE handle, const char* name) : m_Handle(handle)
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
class CSystem final : public ISystem
	, public IInputEventListener
	, public IConsoleVarSink
	, public ISystemEventListener
	, public IWindowMessageHandler
	, public ILoadConfigurationEntrySink
{
  public:
	CSystem(SSystemInitParams& initParams);
	~CSystem();
	void PreprocessCommandLine();
	void ProcessCommandLine();

	// Inherited via ISystem
	virtual bool Init() override;
	bool InitLog();
	virtual void Start() override;
	virtual bool Update(int updateFlags = 0, int nPauseMode = 0) override;
	virtual void RenderBegin() override;
	virtual void Render() override;
	virtual void RenderEnd() override;
	virtual void Release() override;
	virtual SSystemGlobalEnvironment* GetGlobalEnvironment() override
	{
		return &m_env;
	}
	virtual ISystemUserCallback*         GetUserCallback() const override { return m_pUserCallback; }

	virtual IRenderer* GetIRenderer() override;
	virtual ILog* GetILog() override;
	virtual ICmdLine* GetICmdLine() override
	{
		return m_pCmdLine;
	};
	virtual ITimer* GetITimer() override;
	virtual IConsole* GetIConsole() override;
	virtual IInput* GetIInput() override;
	virtual IGame* GetIGame() override;
	virtual IFont* GetIFont() override;
	virtual IWindow* GetIWindow() override;
	IValidator* GetIValidator() override
	{
		return m_pValidator;
	};
	virtual IScriptSystem* GetIScriptSystem() override;
	virtual ISystemEventDispatcher* GetISystemEventDispatcher() override
	{
		return m_pSystemEventDispatcher;
	}
	virtual INetwork* GetINetwork() override;
	virtual ICryPak* GetIPak() override;
	virtual IHardwareMouse* GetIHardwareMouse() override
	{
		return m_env.pHardwareMouse;
	};
	virtual IEntitySystem* GetIEntitySystem() override;
	virtual IStreamEngine* GetStreamEngine() override;
	IRemoteConsole* GetIRemoteConsole() override;
	virtual ITextModeConsole* GetITextModeConsole() override;
	virtual IProjectManager* GetIProjectManager() override; 

	virtual IGame* CreateGame(IGame* game) override;

	virtual void Quit() override;

	virtual void ShowMessage(const char* message, const char* caption, MessageType messageType) override;
	virtual void Log(const char* message) override;

	virtual bool OnInputEvent(const SInputEvent& event) override;

	bool ConfigLoad(const char* file);

	virtual bool IsDevMode() override;
	virtual void Error(const char* message) override;
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;

	// Inherited via IConsoleVarSink
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;

	virtual float GetDeltaTime() override;

	virtual const SFileVersion& GetFileVersion() override;
	virtual const SFileVersion& GetProductVersion() override;
	virtual const char* GetRootFolder() const override;

	// Унаследовано через ISystem

	void SetViewCamera(CCamera& Camera) override
	{
		m_ViewCamera = Camera;
	}
	CCamera& GetViewCamera() override
	{
		return m_ViewCamera;
	}

  public:
	void RunMainLoop();
	bool DoFrame(int updateFlags = 0) override;

  private:
	bool InitConsole();
	bool InitRender();
	bool InitInput();
	bool InitScriptSystem();
	bool InitEntitySystem();
	bool InitNetwork();
	bool InitGUI();
	bool Init3DEngine();
	bool InitSubSystem();
	bool OpenRenderLibrary(std::string_view render);

	void ParseCMD();
	void LoadScreen();
	bool InitScripts();
	void ReleaseScripts();
	bool InitFileSystem(/*const IGameStartup* pGameStartup*/);
	void SetWorkingDirectory(const std::string& path) const;
	void LogCommandLine() const;
	void Tests();
	void PollEvents();

	//////////////////////////////////////////////////////////////////////////
	// Threading functions.
	//////////////////////////////////////////////////////////////////////////
	void InitThreadSystem();
	void ShutDownThreadSystem();
	//////////////////////////////////////////////////////////////////////////
	// Helper functions.
	//////////////////////////////////////////////////////////////////////////
	void CreateRendererVars(const SSystemInitParams& startupParams);
	void CreateSystemVars();

	void ShutDown();

	void UnloadSubsystems();
	virtual void EnableGui(bool enable) override;
	virtual void SaveConfiguration() override;
	virtual void              LoadConfiguration(const char* sFilename, ILoadConfigurationEntrySink* pSink = 0, ELoadConfigurationType configType = eLoadConfigDefault,
	                                            ELoadConfigurationFlags flags = ELoadConfigurationFlags::None) override;
	virtual void OnLoadConfigurationEntry(const char* szKey, const char* szValue, const char* szGroup) override;

	virtual void Relaunch(bool bRelaunch) override;
	virtual bool IsQuitting() override;
	virtual void Error(const char* sFormat, ...) override;
	virtual void Warning(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, ...) override;
	void         WarningV(EValidatorModule module, EValidatorSeverity severity, int flags, const char* file, const char* format, va_list args) override;
	virtual bool CheckLogVerbosity(int verbosity) override;

	virtual bool WriteCompressedFile(char* filename, void* data, unsigned int bitlen) override;
	virtual unsigned int ReadCompressedFile(char* filename, void* data, unsigned int maxbitlen) override;
	virtual unsigned int GetCompressedFileSize(char* filename) override;

	virtual void OnAfterVarChange(ICVar* pVar) override;
	virtual void OnVarUnregister(ICVar* pVar) override;

	virtual void RenderStatistics() override;
	virtual const char* GetUserName() override;


// Inherited via ISystem
	virtual void FatalError(const char* sFormat, ...);

	// Inherited via ISystem
	virtual void ExecuteCommandLine() override;

	// Inherited via ISystem
	virtual bool IsCVarWhitelisted(const char* szName, bool silent) const override;

	template<typename L, typename P>
	inline P GetProcedure(L lib, const char* name)
	{
		return reinterpret_cast<P>(CryGetProcAddress(lib, name));
	}

	template<typename Proc>
	inline bool LoadSubsystem(const char* lib_name, const char* proc_name, std::function<bool(Proc proc)> f)
	{
		//gEnv->pSystem->Log("Loading...");
		string msg;
		msg = "Loading Module ";
		msg += lib_name;
		msg += "...";

		if (gEnv->pSystem->GetUserCallback())
		{
			gEnv->pSystem->GetUserCallback()->OnInitProgress(msg.c_str());
		}

		auto L = CryLoadLibrary(lib_name);
		if (L)
		{
			auto P = GetProcedure<decltype(L), Proc>(L, proc_name);
			if (P)
			{
				typedef void* (*PtrFunc_ModuleInitISystem)(ISystem * pSystem, const char* moduleName);
				PtrFunc_ModuleInitISystem pfnModuleInitISystem = (PtrFunc_ModuleInitISystem)CryGetProcAddress(L, DLL_MODULE_INIT_ISYSTEM);
				if (pfnModuleInitISystem)
				{
					pfnModuleInitISystem(gEnv->pSystem, lib_name);
				}
				auto subsystem = _smart_ptr(new SubsystemWrapper(L, lib_name));
				m_Subsystems.push_back(subsystem);
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
				CryFatalError("Error loading dynamic library: %s, error :  %s\n", lib_name, dlerror());
                fprintf(stderr, "dlopen failed: %s\n", dlerror());
	#else
				//CryFatalError("Error loading dynamic library: %s, error code %d", modulePath.c_str(), GetLastError());
				CryFatalError("Error loading dynamic library: %s, error code %d", lib_name, GetLastError());
	#endif

				gEnv->pSystem->Quit();
			}

		return false;
			gEnv->pSystem->Log("Library not found");
		}
		return false;
	}

	template<class T>
	struct Sizer
	{
		typedef void (T::*Func)(ICrySizer*);
	};

	void GetMemoryStatistics(ICrySizer* pSizer)
	{
		pSizer->AddObject(this, sizeof(*this));
	}

	enum class Align
	{
		Left,
		Right,
		Center,
		Bottom,
		Up,
	};

	void PrintRightAlignedText(float posY, const char* szText)
	{
		SDrawTextInfo info;
		float		  rightMargin = 20;
		info.font	= m_pFont;
		#if 0
		auto& color = info.color;
		color[0]	= 1.0; //green
		color[1]	= 1.0;
		color[2]	= 1.0; //alpha
		color[3]	= 0.0; //red
		#endif
		m_env.pRenderer->Draw2dText(m_env.pRenderer->GetWidth() - m_pFont->TextWidth(szText) - rightMargin, posY, szText, info);
	}
	
	template<class T>
	void PrintMemoryUsageForName(const char* name, typename Sizer<T>::Func fn, T* This, float px, float py)
	{

		static char	 stats[256];
		CrySizerImpl sizer;
		SIZER_COMPONENT_NAME(&sizer, name);
		(This->*fn)(&sizer);
		auto len   = sprintf(stats, "%s memory usage: %d", name, sizer.GetTotalSize());
		stats[len] = 0;

		PrintRightAlignedText(py, stats);
	}



  protected:
	std::vector<_smart_ptr<SubsystemWrapper>> m_Subsystems;
	CCmdLine* m_pCmdLine = nullptr;

  private:
	// System environment.
#if defined(SYS_ENV_AS_STRUCT)
	//since gEnv is a global var, this should just be a reference for code consistency
	SSystemGlobalEnvironment& m_env;
#else
	SSystemGlobalEnvironment m_env;
#endif
	CTimer		  m_Time;		  //!<
	CCamera		  m_ViewCamera;	  //!<
	volatile bool m_bQuit{false}; //!< if is true the system is quitting. Volatile as it may be polled from multiple threads.

	//IInput* m_pInput;
	ICryPak*		  m_pCryPak = nullptr;
	IGame*			  m_pGame	= nullptr;
	IFont*			  m_pFont	= nullptr;
	IWindow*		  m_pWindow = nullptr;
	IValidator*		  m_pValidator; //!< Pointer to validator interface.
	IEntitySystem*	  m_pEntitySystem	 = nullptr;
	INetwork*		  m_pNetwork		 = nullptr;
	ITextModeConsole* m_pTextModeConsole = nullptr;
	//! system event dispatcher
	ISystemEventDispatcher* m_pSystemEventDispatcher = nullptr;

	CScriptObjectConsole*  m_ScriptObjectConsole  = nullptr;
	CScriptObjectScript*   m_ScriptObjectScript	  = nullptr;
	CScriptObjectRenderer* m_ScriptObjectRenderer = nullptr;

  private:
	ICVar* cvGameName = nullptr;
	//////////////////////////////////////////////////////////////////////////
	//! User define callback for system events.
	ISystemUserCallback* m_pUserCallback = nullptr;

	//SSystemInitParams& m_startupParams;
	SSystemInitParams m_startupParams;
	SFileVersion	  m_FileVersion;
	SFileVersion	  m_ProductVersion;

	uint64_t NOW;
	uint64_t LAST;
	double	 m_DeltaTime = 0.0;

#if ENABLE_DEBUG_GUI
	IImGuiManager* m_GuiManager = nullptr;
#endif

	//! global root folder
	string m_root;
	int	   m_iApplicationInstance;

	//! to hold the values stored in system.cfg
	//! because editor uses it's own values,
	//! and then saves them to file, overwriting the user's resolution.
	int m_iHeight	 = 0;
	int m_iWidth	 = 0;
	int m_iColorBits = 0;

	// System console variables.
	//////////////////////////////////////////////////////////////////////////
	float m_rIntialWindowSizeRatio;
	int	  m_rWidth;
	int	  m_rHeight;
	int	  m_rColorBits;
	int	  m_rDepthBits;
	int	  m_rStencilBits;
	int	  m_rFullscreen;
	//ICVar* m_rFullsceenNativeRes;
	//ICVar* m_rWindowState;
	ICVar* m_rDriver;
	int	   m_rDisplayInfo;
	int	   m_rDebug;
	int	   m_rTonemap;
	int	   m_rSkipShaderCache;

	bool m_bIsActive = true;
	
	string m_RootFolder;

	int sys_dump_memstats = false;

};

void AddInternalCommands(ISystem* pSystem);
