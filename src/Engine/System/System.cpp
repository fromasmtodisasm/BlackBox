#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/System/System.hpp>

#include <BlackBox/Core/IGame.hpp>
#include <BlackBox/Network/INetwork.hpp>
#include <BlackBox/Profiler/Drawer2D.h>
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Renderer/Camera.hpp>
//#include <BlackBox/Renderer/FreeTypeFont.hpp>
#include <BlackBox/3DEngine/3DEngine.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Scene/Scene.hpp>
#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
#include <BlackBox/System/Console.hpp>

#ifndef LINUX
#	include <BlackBox/System/File/CryPak.hpp>
#endif
#include <BlackBox/System/HardwareMouse.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/IWindow.hpp>
#include <BlackBox/System/NullLog.hpp>
#include <BlackBox/System/SystemEventDispatcher.hpp>
#include <BlackBox/System/VersionControl.hpp>
#include <BlackBox/Core/Stream.hpp>
#include <BlackBox/World/IWorld.hpp>
//#include <BlackBox/Profiler/HP_Timer.h>
#include <BlackBox/System/CryLibrary.hpp>
#include <SDL2/SDL.h>

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <thread>

using namespace std;
namespace fs = std::filesystem;

#define DLL_MODULE_INIT_ISYSTEM "ModuleInitISystem"

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
		gEnv->pSystem->Log("Loading...");
		auto L = CryLoadLibrary(lib_name);
		if (L)
		{
      gEnv->pSystem->Log("Library found");
			auto P = GetProcedure<decltype(L), Proc>(L, proc_name);
			if (P)
			{
        gEnv->pSystem->Log("Entrypoint found");
				typedef void* (*PtrFunc_ModuleInitISystem)(ISystem * pSystem, const char* moduleName);
				PtrFunc_ModuleInitISystem pfnModuleInitISystem = (PtrFunc_ModuleInitISystem)CryGetProcAddress(L, DLL_MODULE_INIT_ISYSTEM);
				if (pfnModuleInitISystem)
				{
					pfnModuleInitISystem(gEnv->pSystem, lib_name);
				}
				return f(P);
			}
			return false;
		}
    else
    {
      gEnv->pSystem->Log("Library not found");
    }
		return false;
	}
} // namespace

CSystem::CSystem(SSystemInitParams& m_startupParams)
	:
#if defined(SYS_ENV_AS_STRUCT)
	  m_env(m_env),
#endif
	  m_startupParams(m_startupParams),
	  cvGameName(nullptr),
	  m_Render(nullptr),
	  m_pConsole(nullptr),
	  //m_pInput(nullptr),
	  //m_pFont(nullptr),
	  m_pGame(nullptr),
	  m_pLog(nullptr),
	  m_pWindow(nullptr),
	  m_pScriptSystem(nullptr),
	  m_ScriptObjectConsole(nullptr)
#if ENABLE_DEBUG_GUI
#endif
{
	m_pSystemEventDispatcher = new CSystemEventDispatcher(); // Must be first.
	if (m_pSystemEventDispatcher)
	{
		m_pSystemEventDispatcher->RegisterListener(this, "CSystem");
	}
	//////////////////////////////////////////////////////////////////////////
	// Initialize global environment interface pointers.
	m_env.pSystem = this;

#if !defined(SYS_ENV_AS_STRUCT)
	gEnv = &m_env;
#endif
}

CSystem::~CSystem()
{
	CSystem::Log("Releasing system");

	SAFE_RELEASE(m_pGame);
	//SAFE_DELETE(m_pFont);
	SAFE_RELEASE(m_pWindow);
	SAFE_RELEASE(m_pConsole);
	SAFE_RELEASE(m_Render);

	
  SAFE_DELETE(m_ScriptObjectConsole);
  SAFE_DELETE(m_ScriptObjectScript);
  SAFE_DELETE(m_ScriptObjectRenderer);
	SAFE_RELEASE(m_pScriptSystem);

	SAFE_RELEASE(m_pLog);
}

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
		std::cout << "work dir = " << ca->GetValue() << endl;
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
	std::string prompt = "Initializing System";
	if (m_env.IsDedicated())
		prompt += " on dedicated server";
	Log(prompt.c_str());
	//====================================================
	Log("Initializing Console");
	if (!CreateConsole())
		return false;
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
	if (!InitRender())
		return false;
	auto splash = gEnv->pRenderer->LoadTexture("fcsplash.bmp", 0, 0);
	RenderBegin();
	gEnv->pRenderer->DrawFullScreenImage(splash->getId());
	RenderEnd();
	if (!Init3DEngine())
		return false;
	m_env.pInput->PostInit();

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
	if (!InitConsole())
		return false;
	//====================================================
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
        m_env.pInput->AddEventListener(static_cast<CConsole*>(m_pConsole));
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
	m_pConsole->PrintLine("[OK] IGame created\n");
	Tests();

	return true;
}

bool CSystem::CreateLog()
{
	m_pLog = new NullLog(m_startupParams.sLogFileName);
	if (m_pLog == nullptr)
		return false;
	m_env.pLog = m_pLog;
	return true;
}

void CSystem::Start()
{
	bool bRelaunch = m_env.IsDedicated();

	m_pGame->Run(bRelaunch);

	NOW  = SDL_GetPerformanceCounter();
	LAST = 0;

	m_DeltaTime = 0.0;

	while (bRelaunch)
	{
		m_pGame->Release();
		m_pGame = CreateGame(nullptr);
		if (!m_pGame->Init(this, m_env.IsDedicated(), m_startupParams.bEditor, "Normal"))
			break;
		m_pGame->Run(bRelaunch);
	}
}

void CSystem::Release()
{
	delete this;
}

IRenderer* CSystem::GetIRenderer()
{
	return m_Render;
}

ILog* CSystem::GetILog()
{
	return m_pLog;
}

IConsole* CSystem::GetIConsole()
{
	return m_pConsole;
}

IInput* CSystem::GetIInput()
{
	return m_env.pInput;
}

IGame* CSystem::GetIGame()
{
	return m_pGame;
}

IGame* CSystem::CreateGame(IGame* game)
{
    LoadSubsystem<PFNCREATEGAMEINSTANCE>("Game", "CreateIGame", [&](PFNCREATEGAMEINSTANCE P) {
		m_pGame = P();
		return true;
	});
	return m_pGame;
}

void CSystem::Quit()
{
	m_pGame->SendMessage("Quit");
	Release();

	exit(0);
}

IFont* CSystem::GetIFont()
{
	assert(0);
	return nullptr;
	//return new FreeTypeFont;
}

IWindow* CSystem::GetIWindow()
{
	return m_pWindow;
}

#if 0
IInputHandler* CSystem::GetIInputHandler()
{
  return m_InputHandler;
}
#endif

bool CSystem::ConfigLoad(const char* file)
{
	//m_pConsole->ExecuteFile(file);
	LoadConfiguration(file);

	return true;
}

bool CSystem::CreateConsole()
{
	m_env.pConsole = m_pConsole = new CConsole();
	if (m_pConsole == nullptr)
		return false;
	return true;
}

bool CSystem::InitConsole()
{
    if (!static_cast<CConsole*>(m_pConsole)->Init(this))
		return false;
	m_pConsole->ShowConsole(true);
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

void CSystem::ParseCMD()
{
	std::string cmd = m_startupParams.szSystemCmdLine;
	if (cmd.find("-nsightDebug") != std::string::npos)
	{
		m_pConsole->CreateVariable("nsightDebug", 1, VF_NULL, "Debuggin via Nsight Graphics");
	}
}

void CSystem::LoadScreen()
{
	if (m_env.IsDedicated())
	{
		return;
	}
	m_pConsole->Clear();
	m_pConsole->SetScrollMax(600);
	m_pConsole->ShowConsole(true);

	string sLoadingScreenTexture = string("loading.png");

	m_pConsole->SetLoadingImage(sLoadingScreenTexture.c_str());
	m_pConsole->ResetProgressBar(0x7fffffff);
	//GetILog()->UpdateLoadingScreen("");	// just to draw the console
}

bool CSystem::InitScripts()
{
	m_ScriptObjectConsole = new CScriptObjectConsole();
	CScriptObjectConsole::InitializeTemplate(m_pScriptSystem);

	m_ScriptObjectScript = new CScriptObjectScript();
	CScriptObjectScript::InitializeTemplate(m_pScriptSystem);

	m_ScriptObjectRenderer = new CScriptObjectRenderer();
	CScriptObjectRenderer::InitializeTemplate(m_pScriptSystem);

    m_ScriptObjectConsole->Init(GetIScriptSystem(), m_pConsole);
	m_ScriptObjectScript->Init(GetIScriptSystem());

	return m_pScriptSystem->ExecuteFile("scripts/engine.lua");
}

/////////////////////////////////////////////////////////////////////////////////
bool CSystem::InitFileSystem()
{
	//LOADING_TIME_PROFILE_SECTION;
#ifndef LINUX
	m_pCryPak = new CCryPak(m_pLog);
#endif

#if 0
	if (m_pUserCallback)
		m_pUserCallback->OnInitProgress("Initializing File System...");

	bool bLvlRes = false;               // true: all assets since executable start are recorded, false otherwise

#	if !defined(_RELEASE)
	const ICmdLineArg* pArg = m_pCmdLine->FindArg(eCLAT_Pre, "LvlRes");      // -LvlRes command line option

	if (pArg)
		bLvlRes = true;
#	endif // !defined(_RELEASE)

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
	return true;
}

void CSystem::SetWorkingDirectory(const std::string& path) const
{
	fs::current_path(fs::path(path));
}

void CSystem::LogCommandLine() const
{
	std::cout << "Log command line" << std::endl;
	for (int i = 0; i < m_pCmdLine->GetArgCount(); i++)
	{
		std::cout << "\t" << m_pCmdLine->GetArg(i)->GetValue() << std::endl;
	}
}

void CSystem::Tests()
{
#if 0
	auto plane_mesh = CreatePlane(4, 4);
	mesh = std::make_shared<std::vector<Mesh>>();
	mesh->push_back(*plane_mesh.get());
	obj = Object::getEmpty();
	obj->m_Mesh = mesh;
	obj->m_path = "";
#endif

	//SceneManager::instance()->currentScene()->addObject("subdiveded plane", obj);
	//Sleep(3000);
}

void CSystem::PollEvents()
{

}

void CSystem::CreateRendererVars(const SSystemInitParams& startupParams)
{
	REGISTER_CVAR2("r_InitialWindowSizeRatio", &m_rIntialWindowSizeRatio, 0.666f, VF_DUMPTODISK,
	                                          "Sets the size ratio of the initial application window in relation to the primary monitor resolution.\n"
	                                          "Usage: r_InitialWindowSizeRatio [1.0/0.666/..]");

	int iFullScreenDefault  = 1;
	int iDisplayInfoDefault = 1;
	int iWidthDefault       = 1280;
	int iHeightDefault      = 720;
#if BB_PLATFORM_WINDOWS && 0
	iFullScreenDefault = 0;
	const float initialWindowSizeRatio = m_rIntialWindowSizeRatio->GetFVal();
	iWidthDefault = static_cast<int>(GetSystemMetrics(SM_CXSCREEN) * initialWindowSizeRatio);
	iHeightDefault = static_cast<int>(GetSystemMetrics(SM_CYSCREEN) * initialWindowSizeRatio);
#elif BB_PLATFORM_LINUX || BB_PLATFORM_APPLE
	iFullScreenDefault = 0;
#endif

#if defined(RELEASE)
	iDisplayInfoDefault = 0;
#endif

	// load renderer settings from engine.ini
	REGISTER_CVAR2("r_Width", &m_rWidth, iWidthDefault, VF_DUMPTODISK,
		"Sets the display width, in pixels.\n"
		"Usage: r_Width [800/1024/..]"
		);
	REGISTER_CVAR2("r_Height", &m_rHeight, iHeightDefault, VF_DUMPTODISK,
		"Sets the display height, in pixels.\n"
		"Usage: r_Height [600/768/..]"
	);
	REGISTER_CVAR2("r_ColorBits", &m_rColorBits, 32, VF_DUMPTODISK | VF_REQUIRE_APP_RESTART,
		"Sets the color resolution, in bits per pixel. Default is 32.\n"
		"Usage: r_ColorBits [32/24/16/8]");
	REGISTER_CVAR2("r_DepthBits", &m_rDepthBits, 24, VF_DUMPTODISK | VF_REQUIRE_APP_RESTART,
		"Sets the depth precision, in bits per pixel. Default is 24.\n"
		"Usage: r_DepthBits [32/24/16]");
	REGISTER_CVAR2("r_StencilBits", &m_rStencilBits, 8, VF_DUMPTODISK,
		"Sets the stencil precision, in bits per pixel. Default is 8.\n");


	REGISTER_CVAR2("r_Fullscreen", &m_rFullscreen, iFullScreenDefault, VF_DUMPTODISK,
		"Toggles fullscreen mode. Default is 1 in normal game and 0 in DevMode.\n"
		"Usage: r_Fullscreen [0=window/1=fullscreen]");

	#if 0
	REGISTER_CVAR2("r_FullscreenNativeRes", &m_rFullscreenNativeRes, 0, VF_DUMPTODISK,
		"Toggles native resolution upscaling.\n"
		"If enabled, scene gets upscaled from specified resolution while UI is rendered in native resolution.");
	#endif

	REGISTER_CVAR2("r_DisplayInfo", &m_rDisplayInfo, 1, VF_RESTRICTEDMODE | VF_DUMPTODISK,
		"Toggles debugging information display.\n"
		"Usage: r_DisplayInfo [0=off/1=show/2=enhanced/3=minimal/4=fps bar/5=heartbeat]");
	REGISTER_CVAR2("r_Debug", &m_rDebug, 0, VF_RESTRICTEDMODE | VF_DUMPTODISK,
		"Toggles debugging of renderer.\n"
		"Usage: r_DisplayInfo [0=off/1=on]");
	REGISTER_CVAR2("r_Tonemap", &m_rTonemap, 1, VF_DUMPTODISK,
		"Using tonemap.\n"
		"Usage: r_Tonemap [0=off/1=on]");
}

void CSystem::CreateSystemVars()
{
}

void CSystem::EnableGui(bool enable)
{
#if ENABLE_DEBUG_GUI
	if (enable)
	{
		m_env.pInput->AddEventListener(m_GuiManager);
	}
	else
	{
		m_env.pInput->RemoveEventListener(m_GuiManager);
	}
#endif
}

void CSystem::SaveConfiguration()
{
}

float CSystem::GetDeltaTime()
{
	return static_cast<float>(m_DeltaTime);
}

const SFileVersion& CSystem::GetFileVersion()
{
	return m_FileVersion;
}

const SFileVersion& CSystem::GetProductVersion()
{
	return m_ProductVersion;
}

IEntitySystem* CSystem::GetIEntitySystem()
{
	return nullptr;
}

ICryPak* CSystem::GetIPak()
{
	return m_pCryPak;
}

INetwork* CSystem::GetINetwork()
{
	return m_pNetwork;
}

void CSystem::Render()
{
	PROFILER_PUSH_CPU_MARKER("CPU RENDER", Utils::COLOR_YELLOW);
	{
		m_Render->SetState(IRenderer::State::DEPTH_TEST, true);
		m_env.p3DEngine->SetCamera(GetViewCamera());
		m_env.p3DEngine->Draw();
	}
	PROFILER_POP_CPU_MARKER();
}

ITimer* CSystem::GetITimer()
{
	return &m_Time;
}

bool CSystem::IsDevMode()
{
	return true;
}

void CSystem::Error(const char* message)
{
}

void CSystem::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	switch (event)
	{
	case ESYSTEM_EVENT_CHANGE_FOCUS:
		break;
	case ESYSTEM_EVENT_MOVE:
		break;
	case ESYSTEM_EVENT_RESIZE:
		m_rWidth = wparam;
		m_rHeight = lparam;
		break;
	case ESYSTEM_EVENT_ACTIVATE:
		break;
	case ESYSTEM_EVENT_LEVEL_LOAD_START:
		break;
	case ESYSTEM_EVENT_LEVEL_GAMEPLAY_START:
		break;
	case ESYSTEM_EVENT_LEVEL_POST_UNLOAD:
		break;
	case ESYSTEM_EVENT_LANGUAGE_CHANGE:
		break;
	case ESYSTEM_EVENT_TOGGLE_FULLSCREEN:
		m_rFullscreen = wparam;
		break;
	case ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE:
		m_bIsActive = bool(wparam);
		break;
	default:
		break;
	}
}

void CSystem::ShowMessage(const char* message, const char* caption, MessageType messageType)
{
#ifdef _WIN32
	::MessageBox(NULL, message, caption, messageType == 0 ? MB_OK : MB_OKCANCEL);
#endif
}

void CSystem::Log(const char* message)
{
	//std::cout << "-- " << message << std::endl;
	m_pLog->Log("%s", message);
}

IScriptSystem* CSystem::GetIScriptSystem()
{
	//2841004695
	return m_pScriptSystem;
}

bool CSystem::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
	if (!strcmp(pVar->GetName(), "r_cap_profile"))
	{
		switch (std::atoi(sNewValue))
		{
		case 0:
			PROFILER_UNFROZE_FRAME();
			return true;
		case 1:
			PROFILER_FROZE_FRAME();
			return true;
		default:
			return false;
		}
	}
	return false;
}

void CSystem::RenderBegin()
{
	PROFILER_SYNC_FRAME();
	PROFILER_PUSH_CPU_MARKER("Full frame", COLOR_GRAY);
	m_Render->SetState(IRenderer::State::DEPTH_TEST, true);
	m_Render->BeginFrame();
#if ENABLE_DEBUG_GUI
	if (m_GuiManager)
	{
		m_GuiManager->NewFrame();
		m_GuiManager->ShowDemoWindow();
	}
	//m_GuiManager.ShowNodeEditor();
#endif
}

void CSystem::RenderEnd()
{
	PROFILER_POP_CPU_MARKER();
	{
		//DEBUG_GROUP("DRAW_PROFILE");
		PROFILER_DRAW();
	}
	if (m_Render)
	{
		m_Render->Update();
		m_pConsole->Draw();
	#if ENABLE_DEBUG_GUI
		if (m_GuiManager)
			m_GuiManager->Render();
	#endif
		//if (m_bIsActive)
			m_pWindow->swap();
	}
}

bool CSystem::OnInputEvent(const SInputEvent& event)
{
	bool result = false;
	//TODO: handle resized
	bool resized = false;
	switch (event.deviceType)
	{
	case eIDT_Mouse:
	{
		bool mouseMoved = event.keyId == eKI_MouseX || event.keyId == eKI_MouseY || event.keyId == eKI_MouseZ;
		if (event.state == eIS_Pressed)
		{
			if (event.keyId == eKI_Mouse1)
			{
				PROFILER_ON_LEFT_CLICK();
			}
			break;
		}
		if (mouseMoved)
		{
			//TODO: IMPLEMENT THIS
#if 0
      PROFILER_ON_MOUSE_POS(event.mouseMove.x, event.mouseMove.y);
#endif
			break;
		}
	}
	case eIDT_Keyboard:
	{
		if (event.state == eIS_Pressed)
		{
			if (event.modifiers == eMM_Alt && event.modifiers == eMM_Shift)
			{
				if (event.keyId == eKI_P)
				{
					if (PROFILER_ISFROZEN())
					{
						PROFILER_UNFROZE_FRAME();
					}
					else
					{
						PROFILER_FROZE_FRAME();
					}
				}
			}
			else if (event.modifiers == eMM_Alt)
			{
				if (event.keyId == eKI_Enter)
				{
					//TODO: IMPLEMENT THIS
#if 0
          static_cast<CWindow*>(m_pWindow)->ToogleFullScreen(1366, 768);
#endif
				}
			}
		}
	}
	default:
		break;
	}
	if (resized)
	{
		assert(0 && "Not implemented");
#if 0
    PROFILER_ON_RESIZE(event.size.width, event.size.height);
#endif
	}

	return result;
}

bool CSystem::Update(int updateFlags /* = 0*/, int nPauseMode /* = 0*/)
{
	//PROFILER_SYNC_FRAME();
	// Update input
	LAST = NOW;
	NOW  = SDL_GetPerformanceCounter();

	//m_pNetwork->Update();
	if (nPauseMode)
	{
#if ENABLE_DEBUG_GUI
		m_env.pInput->AddEventListener(m_GuiManager);
#endif
	}

	m_pSystemEventDispatcher->Update();

	m_DeltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;
	{
		PROFILER_PUSH_CPU_MARKER("INPUT", Utils::COLOR_LIGHT_BLUE);
		//FIXME: CHECK IT
		if (m_env.pInput)
			m_env.pInput->Update(true);
		PROFILER_POP_CPU_MARKER();
	}
	if (m_pWindow)
		m_pWindow->update();
	if (m_pConsole)
		m_pConsole->Update();
	if (m_pNetwork)
		m_pNetwork->UpdateNetwork();
	if (m_pWindow && m_pWindow->closed())
	{
		m_pGame->SendMessage("Quit");
	}
	if (!nPauseMode)
	{
		m_env.p3DEngine->Update();
	}

	return true;
}

bool CSystem::WriteCompressedFile(char* filename, void* data, unsigned int bitlen)
{
	FILE* fp = fopen(filename, "wb");
	bool result = false;
	if (fp != nullptr)
	{
		if (BITS2BYTES(bitlen) == fwrite(data, 1, BITS2BYTES(bitlen), fp))
		{
			result = true;
		}
		fclose(fp);
	}
	return result;
}

unsigned int CSystem::ReadCompressedFile(char* filename, void* data, unsigned int maxbitlen)
{
	FILE* fp = fopen(filename, "rb");
	int result = 0;
	if (fp != nullptr)
	{
		result = fread(data, 1, BITS2BYTES(maxbitlen), fp);
		fclose(fp);
	}
	return result;
}

unsigned int CSystem::GetCompressedFileSize(char* filename)
{
	FILE* fp = fopen(filename, "rb");
	int size = 0;
	if (fp != nullptr)
	{
		fseek(fp, 0L, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0L, SEEK_SET);
		fclose(fp);
	}
	return BYTES2BITS(size);
}

void CSystem::OnAfterVarChange(ICVar* pVar)
{
}

void CSystem::OnVarUnregister(ICVar* pVar)
{
}


ISYSTEM_API ISystem* CreateSystemInterface(SSystemInitParams& initParams)
{
	std::unique_ptr<CSystem> pSystem = std::make_unique<CSystem>(initParams);
	ModuleInitISystem(pSystem.get(), "System");
#if CRY_PLATFORM_DURANGO
#if !defined(_LIB)
    m_env = pSystem->GetGlobalEnvironment();
#endif
    m_env.pWindow = startupParams.hWnd;
#endif
  if (!pSystem->Init())
  {
    return nullptr;
  }

  return pSystem.release();
}
