#include "ProjectManager/ProjectManager.hpp"
#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/System/IWindow.hpp>
#include <BlackBox/System/NullLog.hpp>
#include <BlackBox/System/System.hpp>
#include <WindowsConsole.h>

#include "Profiling\ProfilingSystem.hpp"

#include <BlackBox/Core/Path.hpp>
#include <BlackBox/Core/Platform/CryLibrary.h>

using stack_string = string;

//#undef USE_DEDICATED_SERVER_CONSOLE
//////////////////////////////////////////////////////////////////////////
#define DEFAULT_LOG_FILENAME "Log.txt"
//////////////////////////////////////////////////////////////////////////
// Where possible, these are defaults used to initialize cvars
// System.cfg can then be used to override them
// This includes the Game DLL, although it is loaded elsewhere
#define DLL_AUDIOSYSTEM   "AudioSystem"
#define DLL_NETWORK       "Network"
#define DLL_ENTITYSYSTEM  "EntitySystem"
#define DLL_SCRIPTSYSTEM  "ScriptSystem"
#define DLL_INPUT         "Input"
#define DLL_PHYSICS       "Physics"
#define DLL_MOVIE         "Movie"
#define DLL_AI            "AISystem"
#define DLL_ANIMATION     "Animation"
#define DLL_FONT          "Font"
#define DLL_3DENGINE      "3DEngine"
//#define DLL_RENDERER_DX11 "RenderD3D11"
#define DLL_RENDERER_DX11 "RendererDX"
#define DLL_RENDERER_DX12 "RenderD3D12"
#define DLL_RENDERER_VK   "RenderVulkan"
#define DLL_RENDERER_GNM  "RenderGNM"
#define DLL_LIVECREATE    "LiveCreate"
#define DLL_MONO_BRIDGE   "MonoBridge"
#define DLL_UDR           "UDR"
#define DLL_SCALEFORM     "ScaleformHelper"

//////////////////////////////////////////////////////////////////////////
static inline void InlineInitializationProcessing(const char* sDescription)
{
	if (gEnv->pLog)
		gEnv->pLog->UpdateLoadingScreen(0);
}

void CSystem::UnloadSubsystems()
{
	for (size_t i = 0; i < m_Subsystems.size(); i++)
	{
		auto CleanupModuleCVars = (void (*)())CryGetProcAddress(m_Subsystems[i]->m_Handle, "CleanupModuleCVars");
		if (CleanupModuleCVars)
		{
			CleanupModuleCVars();
		}
	}
	m_Subsystems.clear();
	// CVars should be unregistered earlier than owning objects/modules are destroyed.
}
#
class CNULLConsole : public IOutputPrintSink
	, public ISystemUserCallback
	, public ITextModeConsole
{
  public:
	CNULLConsole(bool isDaemonMode);

	///////////////////////////////////////////////////////////////////////////////////////
	// IOutputPrintSink
	///////////////////////////////////////////////////////////////////////////////////////
	virtual void Print(const char* inszText);

	///////////////////////////////////////////////////////////////////////////////////////
	// ISystemUserCallback
	///////////////////////////////////////////////////////////////////////////////////////
	/** this method is called at the earliest point the ISystem pointer can be used
	   the log might not be yet there
	 */
	virtual void OnSystemConnect(ISystem* pSystem){};

	/** If working in Editor environment notify user that engine want to Save current document.
	   This happens if critical error have occured and engine gives a user way to save data and not lose it
	   due to crash.
	 */
	virtual bool OnSaveDocument() { return false; }

	/** Notify user that system wants to switch out of current process.
	   (For ex. Called when pressing ESC in game mode to go to Menu).
	 */
	virtual void OnProcessSwitch(){};

	// Notify user, usually editor about initialization progress in system.
	virtual void OnInitProgress(const char* sProgressMsg){};

	// Initialization callback.  This is called early in CSystem::Init(), before
	// any of the other callback methods is called.
	virtual void OnInit(ISystem*);

	// Shutdown callback.
	virtual void OnShutdown(){};

	// Notify user of an update iteration.  Called in the update loop.
	virtual void OnUpdate();

	// to collect the memory information in the user program/application
	virtual void GetMemoryUsage(ICrySizer* pSizer){};

	///////////////////////////////////////////////////////////////////////////////////////
	// ITextModeConsole
	///////////////////////////////////////////////////////////////////////////////////////
	virtual glm::ivec2 BeginDraw() { return glm::ivec2(0, 0); };
	virtual void	   PutText(int x, int y, const char* msg);
	virtual void	   EndDraw(){};

	void SetRequireDedicatedServer(bool)
	{
		// Does nothing
	}
	virtual void SetHeader(const char*)
	{
		//Does nothing
	}

  private:
#if CRY_PLATFORM_WINDOWS
	HANDLE m_hOut;
#endif
	bool m_isDaemon;
	//CSyslogStats m_syslogStats;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// simple light-weight console implementation
//
///////////////////////////////////////////////////////////////////////////////////////
CNULLConsole::CNULLConsole(bool isDaemonMode)
	: m_isDaemon(isDaemonMode)
{
}

void CNULLConsole::Print(const char* inszText)
{
	if (m_isDaemon)
		return;

#if CRY_PLATFORM_WINDOWS
	DWORD written;
	char  buf[1024];
	cry_sprintf(buf, "%s\n", inszText);
	WriteConsole(m_hOut, buf, strlen(buf), &written, NULL);
#elif BB_PLATFORM_LINUX || BB_PLATFORM_ANDROID || BB_PLATFORM_MAC
	printf("%s\n", inszText);
#endif
}

void CNULLConsole::OnInit(ISystem* pSystem)
{
	//m_syslogStats.Init();

	if (m_isDaemon)
		return;

	IConsole* pConsole = pSystem->GetIConsole();
	pConsole->AddOutputPrintSink(this);

#if CRY_PLATFORM_WINDOWS
	AllocConsole();
	m_hOut = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
}

void CNULLConsole::OnUpdate()
{
	int	  numPlayers = 0;
	float srvRate	 = 0;

	if (ITimer* pTimer = gEnv->pSystem->GetITimer())
		srvRate = pTimer->GetFrameRate();

	//m_syslogStats.Update(srvRate, numPlayers, netNub);
}

void CNULLConsole::PutText(int x, int y, const char* msg)
{
}

namespace
{
} // namespace

void CSystem::PreprocessCommandLine()
{
	if (const auto ca = m_pCmdLine->FindArg(eCLAT_Pre, "myproto"); ca)
	{
		std::string output;
		const auto	input = ca->GetValue();
		output.resize(strlen(input) + 1);
		urldecode2(output.data(), input);
		printf("Decoded string: %s\n", output.data());
		/////////////////////////////////////////////////////////////////
		delete m_pCmdLine;
		/////////////////////////////////////////////////////////////////
		output			 = output.substr(strlen("myproto://"));
		std::string left = std::string(m_startupParams.szSystemCmdLine);
		auto		p	 = left.find("-myproto");
		left.resize(p);
		output = left + " " + output;
		std::cout << "output:::: " << output << std::endl;
		system("pause");
		m_pCmdLine = new CCmdLine(
			output.data());
		/////////////////////////////////////////////////////////////////
	}
}

void CSystem::ProcessCommandLine()
{
	if (m_pCmdLine->FindArg(eCLAT_Pre, "dedicated"))
	{
		m_env.SetIsDedicated(true);
	}

	if (const auto ca = m_pCmdLine->FindArg(eCLAT_Post, "wd"); ca)
	{
		SetWorkingDirectory(ca->GetValue());
	}
}

bool CSystem::Init()
{
	m_env.SetIsDedicated(m_startupParams.bDedicatedServer);
	/////////////////////////////////////////////
	m_pCmdLine = new CCmdLine(m_startupParams.szSystemCmdLine);
	PreprocessCommandLine();
	ProcessCommandLine();
#ifdef ENABLE_PROFILER
	initTimer();
#endif
	//====================================================
#if BB_PLATFORM_DESKTOP
#	if !defined(_RELEASE)
	bool isDaemonMode = (m_pCmdLine->FindArg(eCLAT_Pre, "daemon") != 0);
#	else
	bool isDaemonMode = false;
#	endif // !defined(_RELEASE)

	m_env.pFrameProfileSystem = new ProfilingSystem;

#	if defined(USE_DEDICATED_SERVER_CONSOLE)

#		if !defined(_RELEASE)
	bool isSimpleConsole = (m_pCmdLine->FindArg(eCLAT_Pre, "simple_console") != 0);
	if (!(isDaemonMode || isSimpleConsole))
#		endif // !defined(_RELEASE)
	{
		string headerName;
#		if defined(USE_UNIXCONSOLE)
		//CUNIXConsole* pConsole = pUnixConsole = new CUNIXConsole();
		CNULLConsole* pConsole = new CNULLConsole(false);
		headerName			   = "Unix ";
#		elif defined(USE_IOSCONSOLE)
		CIOSConsole* pConsole = new CIOSConsole();
		headerName			  = "iOS ";
#		elif defined(USE_WINDOWSCONSOLE)
		CWindowsConsole* pConsole = new CWindowsConsole();
#		elif defined(USE_ANDROIDCONSOLE)
		CAndroidConsole* pConsole = new CAndroidConsole();
		headerName				  = "Android "
#		else
		CNULLConsole* pConsole = new CNULLConsole(false);
#		endif
		m_pTextModeConsole = static_cast<ITextModeConsole*>(pConsole);

		if (m_pUserCallback == NULL)
		{
			auto getProductVersion = [this]
			{
				char version[64];
				GetProductVersion().ToString(version);
				return string(version);
			};

			if (m_env.IsDedicated())
			{
				m_pUserCallback = pConsole;
				pConsole->SetRequireDedicatedServer(true);
				headerName.append("Dedicated Server");
				if (gEnv->bDedicatedArbitrator)
				{
					headerName.append(" Arbitrator");
				}
				headerName.append(" - Version ");
				headerName.append(getProductVersion());
				pConsole->SetHeader(headerName.c_str());
			}
#		if !defined(RELEASE) || defined(ENABLE_DEVELOPER_CONSOLE_IN_RELEASE)
			else if (m_pCmdLine->FindArg(eCLAT_Pre, "console"))
			{
				m_pUserCallback = pConsole;
				pConsole->SetRequireDedicatedServer(false);
				headerName.append("Client - Version ");
				headerName.append(getProductVersion());
				pConsole->SetHeader(headerName.c_str());
			}
#		endif
		}
	}
#		if !defined(_RELEASE)
	else
#		endif
#	endif

#	if !(defined(USE_DEDICATED_SERVER_CONSOLE) && defined(_RELEASE))
	{
		CNULLConsole* pConsole = new CNULLConsole(isDaemonMode);
		m_pTextModeConsole	   = pConsole;

		if (m_pUserCallback == NULL && m_env.IsDedicated())
			m_pUserCallback = pConsole;
	}
#	endif
	//////////////////////////////////////////////////////////////////////////
	// LOAD GAME PROJECT CONFIGURATION
	//////////////////////////////////////////////////////////////////////////
	InlineInitializationProcessing("CSystem::Init Load project configuration");

	// Load project directory early, since it relies on overriding current working folder
	m_env.pProjectManager = new CProjectManager();
	{
		//////////////////////////////////////////////////////////////////////////
		// File system, must be very early
		//////////////////////////////////////////////////////////////////////////
		InitFileSystem(/*startupParams*/);
		//////////////////////////////////////////////////////////////////////////
		InlineInitializationProcessing("CSystem::Init InitFileSystem");

		//here we should be good to ask Crypak to do something

		//#define GEN_PAK_CDR_CRC
#	ifdef GEN_PAK_CDR_CRC

		const char* filename = m_pCmdLine->GetArg(1)->GetName();
		gEnv->pCryPak->OpenPack(filename);

		int crc = gEnv->pCryPak->ComputeCachedPakCDR_CRC(filename, false);

		exit(crc);
#	endif
		GetIRemoteConsole()->RegisterConsoleVariables();
		GetIRemoteConsole()->Update();
		CreateSystemVars();
	}
	LogCommandLine();

#endif // CRY_PLATFORM_DESKTOP
	if (m_pUserCallback)
		m_pUserCallback->OnInit(this);
	//====================================================
	std::string prompt = "Initializing System";
	if (m_env.IsDedicated())
		prompt += " on dedicated server";
	CryLog(prompt.c_str());
	//====================================================
	CryLog("Initializing Console");
	//====================================================
	CryLog("Loading config");
	if (!ConfigLoad("system.cfg"))
		return false;
	CreateRendererVars(m_startupParams);
	if (auto ovr = m_pCmdLine->FindArg(eCLAT_Pre, "override"); ovr)
	{
		LoadConfiguration(ovr->GetValue());
	}
	//====================================================
	if (!OpenRenderLibrary(gEnv->pConsole->GetCVar("r_Driver")->GetString()))
	{
		return false;
	}
	//====================================================
	if (!InitInput())
	{
		return false;
	}
	//====================================================
	Log("Initialize Render");
	m_pSystemEventDispatcher->OnSystemEvent(ESYSTEM_EVENT_PRE_RENDERER_INIT, 0, 0);

	if (!m_env.IsDedicated())
	{
		if (!InitRender())
			return false;
		auto font_size = vector2f(14, 14);
		m_pFont		   = m_env.pRenderer->GetIFont();
		m_pFont->Init("VeraMono.ttf", (uint)font_size.x, (uint)font_size.y);

		m_pBlackBoxFont = m_env.pRenderer->GetIFont();
		m_pBlackBoxFont->Init("VeraMono.ttf", 24, 24);

		m_Font.ms_nullFont.m_pFont = m_pFont;
		m_Font.ms_nullFont.m_Size  = font_size;

		auto splash = gEnv->pRenderer->LoadTexture("fcsplash.bmp", 0, 0);
		for (int i = 0; i < 3; i++)
		{
			RenderBegin();
			//gEnv->pRenderer->DrawFullScreenImage(splash->getId());
			gEnv->pRenderer->DrawImage(
#if 0
				gEnv->pRenderer->GetWidth() / 2.f - splash->getWidth() / 2,
				gEnv->pRenderer->GetHeight() / 2.f - splash->getHeight() / 2,
				(float)splash->getWidth(),
				(float)splash->getHeight(),
				splash->getBindlesId(),
#else
				0,
				0,
				(float)gEnv->pRenderer->GetWidth(),
				(float)gEnv->pRenderer->GetHeight(),
				splash,
				0, 0, 1, 1, 1, 1, 1, 1);
#endif
			RenderEnd();
		}
	}
	if (m_env.pConsole != nullptr)
	{
		static_cast<CXConsole*>(m_env.pConsole)->PostRendererInit();
	}
	if (!InitSoundSystem())
		return false;
	if (!Init3DEngine())
		return false;
	if (!InitPhysics())
		return false;

	//////////////////////////////////////////////////////////////////////////
	// Input Post Initialise - enables input threads to be created after thread init
	//////////////////////////////////////////////////////////////////////////
	if (m_env.pInput)
	{
		m_env.pInput->PostInit();
	}

	//////////////////////////////////////////////////////////////////////////
	// Hardware mouse
	//////////////////////////////////////////////////////////////////////////
	// - Dedicated server is in console mode by default (Hardware Mouse is always shown when console is)
	// - Mouse is always visible by default in Editor (we never start directly in Game Mode)
	// - Mouse has to be enabled manually by the Game (this is typically done in the main menu)
#ifdef DEDICATED_SERVER
	m_env.pHardwareMouse = NULL;
#else
	if (!m_env.IsDedicated())
		m_env.pHardwareMouse = new CHardwareMouse(true);
	else
		m_env.pHardwareMouse = NULL;
#endif
#ifdef NEED_VC
	m_pWindow->setTitle(cvGameName == nullptr ? DEFAULT_APP_NAME : (std::string(cvGameName->GetString()) + " -- branch[" + GitBranch + "]; hash[" + Hash + "]; " + GitIsDirty + "; Message: [" + Message + "]").c_str());
#endif
	//====================================================
	if (m_env.pHardwareMouse)
		m_env.pHardwareMouse->OnPostInitInput();
		//====================================================
		// Initialize the 2D drawer
#ifdef ENABLE_PROFILER
	Log("Initialize Profiling");
	if (!drawer2D.init(m_env.pRenderer->GetWidth(), m_env.pRenderer->GetHeight()))
	{
		fprintf(stderr, "*** FAILED initializing the Drawer2D\n");
		return EXIT_FAILURE;
	}
#endif
	//====================================================
	//TODO: IMPLEMENT THIS
#if 0
  PROFILER_INIT(m_env.pRenderer->GetWidth(), m_env.pRenderer->GetHeight(), window->getCursorPos().x, window->getCursorPos().y);
#endif
	//====================================================
	//m_env.pLog->Log("[OK] Window susbsystem inited\n");
	//====================================================
	if (!InitScriptSystem())
	{
		return false;
	}
	if (!InitEntitySystem())
	{
		return false;
	}
	//====================================================
	m_env.pConsole->AddConsoleVarSink(this);
	ParseCMD();
	LoadScreen();
	//====================================================
	InitScripts();
	//====================================================
	//====================================================
	if (!m_env.IsDedicated())
	{
		m_env.pInput->AddEventListener(this);
		m_env.pInput->AddEventListener(static_cast<CXConsole*>(m_env.pConsole));
	}

	if (CreateGame(nullptr) == nullptr)
		return false;
	//====================================================

	//====================================================
	if (!InitNetwork())
		return false;
	//LoadCrynetwork();
	//====================================================
	Log("Initialize Game");
	if (!m_pGame->Init(this, m_env.IsDedicated(), m_startupParams.bEditor, "FunCry"))
	{
		return false;
	}
	if (IsDevMode())
	{
		CryLog("DEVMODE is Enabled");
		auto ok = m_env.pScriptSystem->ExecuteFile("DevMode.lua");
		CryLog("\tLoading DevMode.lua: %s!", ok ? "Ok" : "Failed");
	}

	ExecuteCommandLine();
	Tests();

	m_env.pRenderer->RegisterCallbackClient(this);

	if (m_startupParams.bShaderCacheGen)
	{
		GetIConsole()->ExecuteString("r_PrecacheShaderList");
	}

	gEnv->pLog->Log("Main thread : %d", std::this_thread::get_id());
	return true;
}

bool CSystem::InitLog()
{
	if (m_startupParams.pLog == nullptr)
	{
		m_env.pLog			= new CLog(this);
		string sLogFileName = m_startupParams.sLogFileName != nullptr ? m_startupParams.sLogFileName : DEFAULT_LOG_FILENAME;

		const ICmdLineArg* logfile = m_pCmdLine->FindArg(eCLAT_Pre, "logfile");
		if (logfile && strlen(logfile->GetValue()) > 0)
		{
			sLogFileName = logfile->GetValue();
		}

		m_env.pLog->SetFileName(m_startupParams.sLogFileName);
		m_env.pLog = m_env.pLog;
	}
	else
	{
		m_env.pLog = m_env.pLog = m_startupParams.pLog;
	}
	return true;
}

bool CSystem::InitConsole()
{
	if (!static_cast<CXConsole*>(m_env.pConsole)->Init(this))
		return false;
	return true;
}

bool CSystem::InitRender()
{
	if (m_env.IsDedicated())
		return true;
	// In release mode it failed!!!
	// TODO: Fix it

	if (m_env.pRenderer)
	{
		int width  = m_rWidth;
		int height = m_rHeight;
		if (gEnv->IsEditor())
		{
			// In Editor base default Display Context is not really used, so it is allocated with the minimal resolution.
			width  = 32;
			height = 32;
		}

		if (!(m_pWindow = m_env.pRenderer->Init(
				  0, 0, width, height,
				  m_rColorBits, m_rDepthBits, m_rStencilBits,
				  m_rFullscreen, m_pWindow)))
		{
			return false;
		}
		return true;
	}
	return false;
}

bool CSystem::InitInput()
{
	Log("Creating Input");
	return LoadSubsystem<PTRCREATEINPUTFUNC>("Input", "CreateInput", [&](PTRCREATEINPUTFUNC p)
											 {
												 if (!m_env.IsDedicated())
												 {
													 m_env.pInput = p(this, m_pWindow->getHandle());
													 return m_env.pInput->Init();
												 }
												 else
												 {
													 if (m_env.IsDedicated())
														 return true;
													 return false;
												 }
											 });
}

bool CSystem::InitScriptSystem()
{
	Log("Creating ScriptSystem");
	return LoadSubsystem<CREATESCRIPTSYSTEM_FNCPTR>("ScriptSystem", "CreateScriptSystem", [&](CREATESCRIPTSYSTEM_FNCPTR p)
													{
														m_env.pScriptSystem = p(this, true);
														return m_env.pScriptSystem != nullptr;
													});
}

bool CSystem::InitEntitySystem()
{
	Log("Creating EntitySystem");
	return LoadSubsystem<PFNCREATEENTITYSYSTEM>("EntitySystem", "CreateEntitySystem", [&](PFNCREATEENTITYSYSTEM p)
												{
													m_env.pEntitySystem = p(this);
													return m_env.pEntitySystem != nullptr;
												});
}

bool CSystem::InitNetwork()
{
	Log("Creating Network");
	return LoadSubsystem<PFNCREATENETWORK>("Network", "CreateNetwork", [&](PFNCREATENETWORK p)
										   {
											   m_env.pLog->Log("Creating Network");
											   m_pNetwork = p(this);
											   if (m_pNetwork == nullptr)
												   return false;
											   return true;
										   });
}

#if ENABLE_DEBUG_GUI
bool CSystem::InitGUI()
{
	Log("Creating GUI");
	if (LoadSubsystem<PFNCREATEGUI>("GUI", "CreateGUI", [&](PFNCREATEGUI p)
									{
										m_GuiManager = p(this);
										if (m_GuiManager == nullptr)
											return false;
										return true;
									}))
	{
		return m_GuiManager->Init();
	}
	return false;
}
#endif

bool CSystem::Init3DEngine()
{
	Log("Creating 3DEngine");
	return LoadSubsystem<PFNCREATE3DENGINE>("3DEngine", "Create3DEngine", [&](PFNCREATE3DENGINE p)
											{
												m_env.p3DEngine = p(this, "0.0.0");
												if (m_env.p3DEngine == nullptr)
													return false;
												if (!m_env.p3DEngine->Init())
												{
													return false;
												}
												m_pProcess = m_env.p3DEngine;
												m_pProcess->SetFlags(PROC_3DENGINE);
												return true;
											});
}

bool CSystem::InitSoundSystem()
{
	Log("Creating SoundSystem");
	return LoadSubsystem<PFNCREATESOUNDSYSTEM>("Sound", "CreateSoundSystem", [&](PFNCREATESOUNDSYSTEM p)
											   {
												   m_env.pSoundSystem = p(this, "0.0.0");
												   if (m_env.pSoundSystem == nullptr)
													   return false;
												   return true;
											   });
}

bool CSystem::InitSubSystem()
{
	return false;
}

bool CSystem::OpenRenderLibrary(std::string_view render)
{
	Log("Open Render Library");
	if (m_env.IsDedicated())
		return true;
	//====================================================
	if (!LoadSubsystem<PFNCREATEWINDOW>("Window", "CreateIWindow", [&](PFNCREATEWINDOW p)
										{
											Log("Load Window Library");
											m_pWindow = p();
											if (m_pWindow == nullptr)
												return false;
											return true;
										}))
	{
		return false;
	}
	//====================================================

	const char* RenderLibrary = "RendererGL";
	if (render == "DX11")
	{
		RenderLibrary = "RendererDX";
	}
	else if (render == "VK")
	{
		RenderLibrary = "RendererVK";
	}
	return LoadSubsystem<PFNCREATERENDERERINTERFACE>(RenderLibrary, "CreateIRender", [&](PFNCREATERENDERERINTERFACE p)
													 {
														 Log("Load Render Library");
														 m_env.pRenderer = m_env.pRenderer = p(this);
														 if (m_env.pRenderer == nullptr)
															 return false;
														 else
															 return true;
													 });

#if 0
	CryFatalError("Unknown renderer type: %s", t_rend);
	return false;
#endif
}
//////////////////////////////////////////////////////////////////////////
bool CSystem::CloseRenderLibrary(std::string_view render)
{
	const char* t_rend = render.data();
	//CRY_PROFILE_FUNCTION(PROFILE_LOADING_ONLY);

	if (gEnv->IsDedicated())
		return true;

	if (stricmp(t_rend, STR_DX11_RENDERER) == 0)
		return UnloadEngineModule(DLL_RENDERER_DX11);
	#if 0
	else if (stricmp(t_rend, STR_DX12_RENDERER) == 0)
		return UnloadEngineModule(DLL_RENDERER_DX12);
	else if (stricmp(t_rend, STR_VK_RENDERER) == 0)
		return UnloadEngineModule(DLL_RENDERER_VK);
	#endif

	CryFatalError("Unknown renderer type: %s", t_rend);
	return false;
}
bool CSystem::UnloadEngineModule(const char* dllName)
{
	return UnloadDynamicLibrary(dllName);
}
bool CSystem::UnloadDynamicLibrary(const char* szDllName)
{
	stack_string modulePath = szDllName;
	modulePath				= CrySharedLibraryPrefix + PathUtil::ReplaceExtension(modulePath.c_str(), CrySharedLibraryExtension);

	auto moduleIt = m_moduleDLLHandles.find(modulePath);
	if (moduleIt != m_moduleDLLHandles.end())
	{
		#if 0
		string msg = string().Format("Unloading %s...", modulePath.c_str());
		#else
		string msg(' ', 64);
		msg[sprintf(msg.data(), "Unloading %s...", modulePath.c_str()) - 1] = 0;
		//= string().Format("Unloading %s...", modulePath.c_str());
		#endif

		CryLog("%s", msg.c_str());

#if BB_PLATFORM_WINDOWS || BB_PLATFORM_DURANGO || BB_PLATFORM_LINUX || BB_PLATFORM_ANDROID || BB_PLATFORM_APPLE
		WIN_HMODULE hModule = moduleIt->second;
#endif

		// CVars should be unregistered earlier than owning objects/modules are destroyed.
		auto CleanupModuleCVars = (void (*)())CryGetProcAddress(hModule, "CleanupModuleCVars");
		if (CleanupModuleCVars)
		{
			CleanupModuleCVars();
		}

		#if 0
		auto			 GetHeadToRegFactories = (PtrFunc_GetHeadToRegFactories)CryGetProcAddress(hModule, "GetHeadToRegFactories");
		SRegFactoryNode* pFactoryNode		   = GetHeadToRegFactories();

		if (pFactoryNode)
		{
			ICryFactoryRegistryImpl* const pReg = static_cast<ICryFactoryRegistryImpl*>(GetCryFactoryRegistry());

			pReg->UnregisterFactories(pFactoryNode);
		}
		#endif

		CryFreeLibrary(hModule);
		m_moduleDLLHandles.erase(moduleIt);

		return true;
	}

	return false;

	return false;
}

/////////////////////////////////////////////////////////////////////////////////
bool CSystem::InitFileSystem()
{
	if (m_pUserCallback)
		m_pUserCallback->OnInitProgress("Initializing File System...");
#if 0
	m_RootFolder = string(fs::current_path().u8string().c_str());
#endif

	bool bLvlRes = false; // true: all assets since executable start are recorded, false otherwise

#if !defined(_RELEASE)
	const ICmdLineArg* pArg = m_pCmdLine->FindArg(eCLAT_Pre, "LvlRes"); // -LvlRes command line option

	if (pArg)
		bLvlRes = true;
#endif // !defined(_RELEASE)

	CCryPak* pCryPak;
	pCryPak = new CCryPak(m_env.pLog, &g_cvars.pakVars, bLvlRes);
//TODO:
#if 0
	pCryPak->SetGameFolderWritable(m_bGameFolderWritable);
#endif
	m_env.pCryPak = pCryPak;

	// Check if root folder is overridden by command-line
	const ICmdLineArg* root = m_pCmdLine->FindArg(eCLAT_Pre, "root");
	if (root)
	{
		string temp = PathUtil::ToUnixPath(PathUtil::AddSlash(root->GetValue()));
#if 0
		if (pCryPak->MakeDir(temp.c_str()))
			m_root = temp;
#else
		fs::current_path(fs::path(temp.c_str()));
#endif
	}
#if 0
	if (m_bEditor || bLvlRes)
		m_env.pCryPak->RecordFileOpen(ICryPak::RFOM_EngineStartup);

	{
		char szEngineRootDir[_MAX_PATH];
		CryFindEngineRootFolder(CRY_ARRAY_COUNT(szEngineRootDir), szEngineRootDir);
		string engineRootDir = PathUtil::RemoveSlash(szEngineRootDir);
		m_env.pCryPak->SetAlias("%ENGINEROOT%", engineRootDir.c_str(), true);

		const CryPathString engineDir = PathUtil::Make(CryPathString(engineRootDir.c_str()), CryPathString(CRYENGINE_ENGINE_FOLDER));
		m_env.pCryPak->SetAlias("%ENGINE%", engineDir.c_str(), true);

#	ifndef RELEASE
		if (m_bEditor)
		{
			const CryPathString editorDir = PathUtil::Make(CryPathString(engineRootDir.c_str()), CryPathString("Editor"));
			m_env.pCryPak->SetAlias("%EDITOR%", editorDir.c_str(), true);
		}
#	endif
	}
#endif

	// Now set up the log
	InitLog();

#if 0
	LogVersion();

	((CCryPak*)m_env.pCryPak)->SetLog(m_env.pLog);

	// Load value of sys_game_folder from system.cfg into the sys_game_folder console variable
	ILoadConfigurationEntrySink* pCVarsWhiteListConfigSink = GetCVarsWhiteListConfigSink();
#	if CRY_PLATFORM_ANDROID && !defined(ANDROID_OBB)
	string path = string(CryGetProjectStoragePath()) + "/system.cfg";
	LoadConfiguration(path.c_str(), pCVarsWhiteListConfigSink, eLoadConfigInit);
#	else
	LoadConfiguration("system.cfg", pCVarsWhiteListConfigSink, eLoadConfigInit);
#	endif
#endif

	// Initialize console before the project system
	// This ensures that "exec" and other early commands can be executed immediately on parsing
	if (m_env.pConsole != nullptr)
	{
		static_cast<CXConsole*>(m_env.pConsole)->PreProjectSystemInit();
	}

	if (!m_env.pProjectManager->ParseProjectFile())
	{
		return false;
	}

	bool bRes = m_env.pCryPak->Init("");
#if 0

	if (bRes)
	{
#	if !defined(_RELEASE)
		const ICmdLineArg* pakalias = m_pCmdLine->FindArg(eCLAT_Pre, "pakalias");
#	else
		const ICmdLineArg* pakalias = NULL;
#	endif // !defined(_RELEASE)
		if (pakalias && strlen(pakalias->GetValue()) > 0)
			m_env.pCryPak->ParseAliases(pakalias->GetValue());
	}

	// Create Engine folder mod mapping only for Engine assets
	pCryPak->AddMod("%ENGINEROOT%/" CRYENGINE_ENGINE_FOLDER);

#	if CRY_PLATFORM_ANDROID
	pCryPak->AddMod(CryGetProjectStoragePath());
#		if defined(ANDROID_OBB)
	pCryPak->SetAssetManager(androidGetAssetManager());
#		endif
#	elif CRY_PLATFORM_LINUX
	//apparently Linux needs the parent dir as a module for letting CryPak find the file system.cfg
	pCryPak->AddMod("./");
#	endif

	return (bRes);
#endif
	return true;
}

bool CSystem::LoadCrynetwork()
{
	//CRYNETWORK_API INetwork* CreateNetwork(ISystem * pSystem);
	typedef INetwork* (*PFNCREATENETWORK)(legacy::ISystem * pSystem);

	Log("Creating CryNetwork");
	return LoadSubsystem<PFNCREATENETWORK>("Legacy/CryNetwork.dll", "CreateNetwork", [&](PFNCREATENETWORK p)
										   {
											   m_pNetworkLegacy = p(m_pSystemLegacy);
											   if (m_pNetworkLegacy == nullptr)
												   return false;
											   //m_pNetworkLegacy->Init();
											   return true;
										   });
}

IGame* CSystem::CreateGame(IGame* game)
{
	string gameDLLName = "Game";
	if (ICVar* pCVarGameDir = gEnv->pConsole->GetCVar("sys_dll_game"))
	{
		gameDLLName = pCVarGameDir->GetString();
	}
	LoadSubsystem<PFNCREATEGAMEINSTANCE>(gameDLLName.c_str(), "CreateGameInstance", [&](PFNCREATEGAMEINSTANCE P)
										 {
											 m_pGame = P();
											 return true;
										 });
	return m_pGame;
}

typedef IPhysicalWorld* (*PFNCREATEPHYSICS)(ISystem* pSystem);
bool CSystem::InitPhysics()
{
	Log("Creating Physics");
	return LoadSubsystem<PFNCREATEPHYSICS>("Physics", "CreatePhysicalWorld", [&](PFNCREATEPHYSICS p)
										   {
											   m_env.pPhysicalWorld = p(this);
											   if (m_env.pPhysicalWorld == nullptr)
												   return false;
											   m_env.pPhysicalWorld->Init();
											   return true;
										   });
}
