#include <BlackBox/System/System.hpp>
#include <BlackBox/System/CryLibrary.hpp>
#include <BlackBox/Core/Utils.hpp>
#include <WindowsConsole.h>
#include <BlackBox/System/Console.hpp>
#include <BlackBox/System/IWindow.hpp>
#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/System/NullLog.hpp>
#ifndef LINUX
#	include <BlackBox/System/File/CryPak.hpp>
#endif

//////////////////////////////////////////////////////////////////////////
#define DEFAULT_LOG_FILENAME    "Log.txt"

class CNULLConsole : public IOutputPrintSink,
	                   public ISystemUserCallback,
	                   public ITextModeConsole
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
	virtual void OnSystemConnect(ISystem* pSystem) {};

	/** If working in Editor environment notify user that engine want to Save current document.
	   This happens if critical error have occured and engine gives a user way to save data and not lose it
	   due to crash.
	 */
	virtual bool OnSaveDocument() { return false; }

	/** Notify user that system wants to switch out of current process.
	   (For ex. Called when pressing ESC in game mode to go to Menu).
	 */
	virtual void OnProcessSwitch() {};

	// Notify user, usually editor about initialization progress in system.
	virtual void OnInitProgress(const char* sProgressMsg) {};

	// Initialization callback.  This is called early in CSystem::Init(), before
	// any of the other callback methods is called.
	virtual void OnInit(ISystem*);

	// Shutdown callback.
	virtual void OnShutdown() {};

	// Notify user of an update iteration.  Called in the update loop.
	virtual void OnUpdate();

	// to collect the memory information in the user program/application
	virtual void GetMemoryUsage(ICrySizer* pSizer) {};

	///////////////////////////////////////////////////////////////////////////////////////
	// ITextModeConsole
	///////////////////////////////////////////////////////////////////////////////////////
	virtual Vec2_tpl<int> 	      BeginDraw() { return Vec2_tpl<int>(0, 0); };
	virtual void          PutText(int x, int y, const char* msg);
	virtual void          EndDraw()   {};

	void                  SetRequireDedicatedServer(bool)
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
CNULLConsole::CNULLConsole(bool isDaemonMode) : m_isDaemon(isDaemonMode)
{
}

void CNULLConsole::Print(const char* inszText)
{
	if (m_isDaemon)
		return;

	#if CRY_PLATFORM_WINDOWS
	DWORD written;
	char buf[1024];
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
	int numPlayers = 0;
	float srvRate = 0;

	if (ITimer* pTimer = gEnv->pSystem->GetITimer())
		srvRate = pTimer->GetFrameRate();

	//m_syslogStats.Update(srvRate, numPlayers, netNub);
}

void CNULLConsole::PutText(int x, int y, const char* msg)
{
}

namespace
{
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

		if (true)
		{
			CryLog("%s", msg.c_str());
		}

		auto L = CryLoadLibrary(lib_name);
		if (L)
		{
			gEnv->pSystem->Log("Library found");
			auto P = GetProcedure<decltype(L), Proc>(L, proc_name);
			if (P)
			{
				gEnv->pLog->Log("Entrypoint [%s] found", proc_name);
				typedef void* (*PtrFunc_ModuleInitISystem)(ISystem * pSystem, const char* moduleName);
				PtrFunc_ModuleInitISystem pfnModuleInitISystem = (PtrFunc_ModuleInitISystem)CryGetProcAddress(L, DLL_MODULE_INIT_ISYSTEM);
				if (pfnModuleInitISystem)
				{
					pfnModuleInitISystem(gEnv->pSystem, lib_name);
				}
				return f(P);
			}
			else
			{
				gEnv->pLog->Log("Entrypoint %s not found", proc_name);
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
} // namespace

void CSystem::PreprocessCommandLine()
{
	if (const auto ca = m_pCmdLine->FindArg(eCLAT_Pre, "myproto"); ca)
	{
		std::string output;
		const auto input = ca->GetValue();
		output.resize(strlen(input) + 1);
		urldecode2(output.data(), input);
		printf("Decoded string: %s\n", output.data());
		/////////////////////////////////////////////////////////////////
		delete m_pCmdLine;
		/////////////////////////////////////////////////////////////////
		output			 = output.substr(strlen("myproto://"));
		std::string left = std::string(m_startupParams.szSystemCmdLine);
		auto p			 = left.find("-myproto");
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
	LogCommandLine();
	PreprocessCommandLine();
	ProcessCommandLine();
#ifdef ENABLE_PROFILER
	initTimer();
#endif
	if (!CreateLog())
		return false;
	GetIRemoteConsole()->RegisterConsoleVariables();
	GetIRemoteConsole()->Update();
	//====================================================
#if BB_PLATFORM_DESKTOP
	#if !defined(_RELEASE)
	bool isDaemonMode = (m_pCmdLine->FindArg(eCLAT_Pre, "daemon") != 0);
	#else
	bool isDaemonMode = false;
	#endif // !defined(_RELEASE)

	#if defined(USE_DEDICATED_SERVER_CONSOLE)

		#if !defined(_RELEASE)
	bool isSimpleConsole = (m_pCmdLine->FindArg(eCLAT_Pre, "simple_console") != 0);

	if (!(isDaemonMode || isSimpleConsole))
		#endif // !defined(_RELEASE)
	{
		string headerName;
		#if defined(USE_UNIXCONSOLE)
		CUNIXConsole* pConsole = pUnixConsole = new CUNIXConsole();
		headerName = "Unix ";
		#elif defined(USE_IOSCONSOLE)
		CIOSConsole* pConsole = new CIOSConsole();
		headerName = "iOS ";
		#elif defined(USE_WINDOWSCONSOLE)
		CWindowsConsole* pConsole = new CWindowsConsole();
		#elif defined(USE_ANDROIDCONSOLE)
		CAndroidConsole* pConsole = new CAndroidConsole();
		headerName = "Android "
		#else
		CNULLConsole * pConsole = new CNULLConsole(false);
		#endif
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
#if !defined(RELEASE) || defined(ENABLE_DEVELOPER_CONSOLE_IN_RELEASE)
			else if (m_pCmdLine->FindArg(eCLAT_Pre, "console"))
			{
				m_pUserCallback = pConsole;
				pConsole->SetRequireDedicatedServer(false);
				headerName.append("Client - Version ");
				headerName.append(getProductVersion());
				pConsole->SetHeader(headerName.c_str());
			}
#endif
		}
	}
		#if !defined(_RELEASE)
	else
		#endif
	#endif

	#if !(defined(USE_DEDICATED_SERVER_CONSOLE) && defined(_RELEASE))
	{
		CNULLConsole* pConsole = new CNULLConsole(isDaemonMode);
		m_pTextModeConsole = pConsole;

		if (m_pUserCallback == NULL && m_env.IsDedicated())
			m_pUserCallback = pConsole;
	}
	#endif
	{
		//////////////////////////////////////////////////////////////////////////
		// File system, must be very early
		//////////////////////////////////////////////////////////////////////////
		InitFileSystem(/*startupParams*/);

		//////////////////////////////////////////////////////////////////////////
		//InlineInitializationProcessing("CSystem::Init InitFileSystem");

		//here we should be good to ask Crypak to do something

		//#define GEN_PAK_CDR_CRC
#	ifdef GEN_PAK_CDR_CRC

		const char* filename = m_pCmdLine->GetArg(1)->GetName();
		gEnv->pCryPak->OpenPack(filename);

		int crc = gEnv->pCryPak->ComputeCachedPakCDR_CRC(filename, false);

		exit(crc);
#endif
	}

#endif // CRY_PLATFORM_DESKTOP
	if (m_pUserCallback)
		m_pUserCallback->OnInit(this);
	//====================================================
	std::string prompt = "Initializing System";
	if (m_env.IsDedicated())
		prompt += " on dedicated server";
	Log(prompt.c_str());
	//====================================================
	Log("Initializing Console");
	//====================================================
	Log("Loading config");
	if (!ConfigLoad("system.cfg"))
		return false;
	CreateRendererVars(m_startupParams);
	//====================================================
	if (!OpenRenderLibrary("OpenGL"))
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
		auto splash = gEnv->pRenderer->LoadTexture("fcsplash.bmp", 0, 0);
		for (int i = 0; i < 3; i++)
		{
			RenderBegin();
			//gEnv->pRenderer->DrawFullScreenImage(splash->getId());
			gEnv->pRenderer->DrawImage(
				gEnv->pRenderer->GetWidth() / 2.f - splash->getWidth() / 2,
				gEnv->pRenderer->GetHeight() / 2.f - splash->getHeight() / 2,
				(float)splash->getWidth(),
				(float)splash->getHeight(),
				splash->getId(),
				0, 0, 1, 1, 1, 1, 1, 1);
			RenderEnd();
		}
	}
	if (m_env.pConsole != nullptr)
	{
		static_cast<CXConsole*>(m_env.pConsole)->PostRendererInit();
	}
	if (!Init3DEngine())
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
	if (!drawer2D.init(m_Render->GetWidth(), m_Render->GetHeight()))
	{
		fprintf(stderr, "*** FAILED initializing the Drawer2D\n");
		return EXIT_FAILURE;
	}
#endif
	//====================================================
	//TODO: IMPLEMENT THIS
#if 0
  PROFILER_INIT(m_Render->GetWidth(), m_Render->GetHeight(), window->getCursorPos().x, window->getCursorPos().y);
#endif
	//====================================================
	//m_pLog->Log("[OK] Window susbsystem inited\n");
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
	m_pConsole->AddConsoleVarSink(this);
	ParseCMD();
	LoadScreen();
	//====================================================
	InitScripts();
	//====================================================
	//====================================================
	if (!m_env.IsDedicated())
	{
		m_env.pInput->AddEventListener(this);
		m_env.pInput->AddEventListener(static_cast<CXConsole*>(m_pConsole));
#if ENABLE_DEBUG_GUI
		if (!m_env.IsDedicated())
		{
			if (!InitGUI())
				return false;
		}
		m_env.pInput->AddEventListener(m_GuiManager);
#endif
	}


	if (CreateGame(nullptr) == nullptr)
		return false;
	//====================================================

	//====================================================
	if (!InitNetwork())
		return false;
	//====================================================
	Log("Initialize Game");
	if (!m_pGame->Init(this, m_env.IsDedicated(), m_startupParams.bEditor, "Normal"))
	{
		return false;
	}
	ExecuteCommandLine();
	Tests();

	return true;
}

bool CSystem::CreateLog()
{
	if (m_startupParams.pLog == nullptr)
	{
		m_pLog = new CLog(this);
		string sLogFileName = m_startupParams.sLogFileName != nullptr ? m_startupParams.sLogFileName : DEFAULT_LOG_FILENAME;

		const ICmdLineArg* logfile = m_pCmdLine->FindArg(eCLAT_Pre, "logfile");
		if (logfile && strlen(logfile->GetValue()) > 0)
		{
			sLogFileName = logfile->GetValue();
		}

		m_pLog->SetFileName(m_startupParams.sLogFileName);
		m_env.pLog = m_pLog;
	}
	else
	{
		m_env.pLog = m_pLog = m_startupParams.pLog;
	}
	return true;
}

bool CSystem::InitConsole()
{
	if (!static_cast<CXConsole*>(m_pConsole)->Init(this))
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
	return LoadSubsystem<PTRCREATEINPUTFUNC>("Input", "CreateInput", [&](PTRCREATEINPUTFUNC p) {
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
	return LoadSubsystem<CREATESCRIPTSYSTEM_FNCPTR>("ScriptSystem", "CreateScriptSystem", [&](CREATESCRIPTSYSTEM_FNCPTR p) {
		m_pScriptSystem = p(this, true);
		return m_pScriptSystem != nullptr;
	});
}

bool CSystem::InitEntitySystem()
{
	Log("Creating EntitySystem");
	return LoadSubsystem<PFNCREATEENTITYSYSTEM>("EntitySystem", "CreateEntitySystem", [&](PFNCREATEENTITYSYSTEM p) {
		m_pEntitySystem = p(this);
		return m_pEntitySystem != nullptr;
	});
}

bool CSystem::InitNetwork()
{
	Log("Creating Network");
	return LoadSubsystem<PFNCREATENETWORK>("Network", "CreateNetwork", [&](PFNCREATENETWORK p) {
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
	if (LoadSubsystem<PFNCREATEGUI>("GUI", "CreateGUI", [&](PFNCREATEGUI p) {
			m_env.pLog->Log("Creating GUI");
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
	return LoadSubsystem<PFNCREATE3DENGINE>("3DEngine", "Create3DEngine", [&](PFNCREATE3DENGINE p) {
		m_env.p3DEngine = p(this, "0.0.0");
		if (m_env.p3DEngine == nullptr)
			return false;
		return m_env.p3DEngine->Init();
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
	if (!LoadSubsystem<PFNCREATEWINDOW>("Window", "CreateIWindow", [&](PFNCREATEWINDOW p) {
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

	return LoadSubsystem<PFNCREATERENDERERINTERFACE>("Renderer", "CreateIRender", [&](PFNCREATERENDERERINTERFACE p) {
		Log("Load Render Library");
		m_env.pRenderer = m_Render = p(this);
		if (m_Render == nullptr)
			return false;
		else
			return true;
	});

#if 0
	CryFatalError("Unknown renderer type: %s", t_rend);
	return false;
#endif
}

/////////////////////////////////////////////////////////////////////////////////
bool CSystem::InitFileSystem()
{
	//LOADING_TIME_PROFILE_SECTION;
#ifndef LINUX
	m_pCryPak = new CCryPak(m_pLog);
#endif

	if (m_pUserCallback)
		m_pUserCallback->OnInitProgress("Initializing File System...");

	bool bLvlRes = false;               // true: all assets since executable start are recorded, false otherwise

#	if !defined(_RELEASE)
	const ICmdLineArg* pArg = m_pCmdLine->FindArg(eCLAT_Pre, "LvlRes");      // -LvlRes command line option

	if (pArg)
		bLvlRes = true;
#	endif // !defined(_RELEASE)

#if 0
	CCryPak* pCryPak;
	pCryPak = new CCryPak(m_env.pLog, &g_cvars.pakVars, bLvlRes, pGameStartup);
	pCryPak->SetGameFolderWritable(m_bGameFolderWritable);
	m_env.pCryPak = pCryPak;

	// Check if root folder is overridden by command-line
	const ICmdLineArg* root = m_pCmdLine->FindArg(eCLAT_Pre, "root");
	if (root)
	{
		string temp = PathUtil::ToUnixPath(PathUtil::AddSlash(root->GetValue()));
		if (pCryPak->MakeDir(temp.c_str()))
			m_root = temp;
	}

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

	// Now set up the log
	InitLog();

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

	if (!m_pProjectManager->ParseProjectFile())
	{
		return false;
	}

	bool bRes = m_env.pCryPak->Init("");

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
	// Initialize console before the project system
	// This ensures that "exec" and other early commands can be executed immediately on parsing
	if (m_env.pConsole != nullptr)
	{
		static_cast<CXConsole*>(m_env.pConsole)->PreProjectSystemInit();
	}
	return true;
}

IGame* CSystem::CreateGame(IGame* game)
{
	LoadSubsystem<PFNCREATEGAMEINSTANCE>("Game", "CreateIGame", [&](PFNCREATEGAMEINSTANCE P) {
		m_pGame = P();
		return true;
	});
	return m_pGame;
}

