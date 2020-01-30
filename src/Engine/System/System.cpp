#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/System/System.hpp>

#include <BlackBox/Core/IGame.hpp>
#include <BlackBox/Network/INetwork.hpp>
#include <BlackBox/Profiler/Drawer2D.h>
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/FreeTypeFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Resources/ObjectManager.hpp>
#include <BlackBox/Resources/SceneManager.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/Resources/TextureManager.hpp>
#include <BlackBox/Scene/Scene.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectConsole.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectRenderer.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectScript.hpp>
#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
#include <BlackBox/System/Console.hpp>
#ifndef LINUX
#include <BlackBox/System/File/CryPak.hpp>
#endif
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/IWindow.hpp>
#include <BlackBox/System/NullLog.hpp>
#include <BlackBox/System/SystemEventDispatcher.hpp>
#include <BlackBox/System/VersionControl.hpp>
#include <BlackBox/World/World.hpp>
#include <BlackBox/System/HardwareMouse.hpp>
//#include <BlackBox/Profiler/HP_Timer.h>
#include <SDL2/SDL.h>

#include <cstdlib>
#include <thread>

using namespace Utils;
using namespace std;

//////////////////////////////////////////////////////////////////////
// Pointer to Global ISystem.
static ISystem* gISystem = nullptr;
ISystem* GetISystem()
{
  return gISystem;
}

CSystem::CSystem(SSystemInitParams& m_startupParams)
  :
#if defined(SYS_ENV_AS_STRUCT)
	m_env(gEnv),
#endif
  m_startupParams(m_startupParams),
  r_window_width(nullptr),
  r_window_height(nullptr),
  r_bpp(nullptr),
  r_zbpp(nullptr),
  r_sbpp(nullptr),
  r_fullscreen(nullptr),
  cvGameName(nullptr),
  m_Render(nullptr),
  m_pConsole(nullptr),
  m_pFont(nullptr),
  m_pGame(nullptr),
  m_pLog(nullptr),
  m_pWindow(nullptr),
  m_pWorld(nullptr),
  m_pScriptSystem(nullptr),
  m_ScriptObjectConsole(nullptr),
  m_GuiManager(this)
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
  SAFE_DELETE(r_window_width);
  SAFE_DELETE(r_window_height);
  SAFE_DELETE(r_bpp);
  SAFE_DELETE(r_zbpp);
  SAFE_DELETE(r_sbpp);
  SAFE_DELETE(r_fullscreen);

  SAFE_DELETE(m_pLog);
  SAFE_DELETE(m_pConsole);
  SAFE_DELETE(m_pGame);
  SAFE_DELETE(m_pFont);
  SAFE_DELETE(m_pWindow);
  SAFE_DELETE(m_Render);
}

bool CSystem::Init()
{
  gISystem = this;
  gEnv->SetIsDedicated(m_startupParams.bDedicatedServer);
  /////////////////////////////////////////////
  m_pCmdLine = new CCmdLine(m_startupParams.szSystemCmdLine);
#ifdef ENABLE_PROFILER
  initTimer();
#endif
  m_pLog = new NullLog(m_startupParams.sLogFileName);
  if (m_pLog == nullptr)
    return false;
  Log("Initializing System");
  //====================================================
  Log("Initializing Console");
  if (!InitConsole())
    return false;
  //====================================================
  Log("Initializing System");
  if (!ConfigLoad("res/scripts/engine.cfg"))
    return false;
  //====================================================
  if (!OpenRenderLibrary("OpenGL"))
  {
    return false;
  }
  //====================================================
  m_pInput = CreateInput(this, m_pWindow->getHandle());
  //====================================================
  if (!MaterialManager::init(this))
  {
    return false;
  }
  //====================================================
  // In release mode it failed!!!
  // TODO: Fix it
  if (!(m_pWindow = m_Render->Init(
    0, 0,
    r_window_width->GetIVal(), r_window_height->GetIVal(),
    r_bpp->GetIVal(), r_zbpp->GetIVal(), r_sbpp->GetIVal(),
    r_fullscreen->GetIVal(), m_pWindow))
    )
    return false;
  //====================================================
#ifdef NEED_VC 
  m_pWindow->setTitle(cvGameName == nullptr ? DEFAULT_APP_NAME : 
    (std::string(cvGameName->GetString()) + " -- branch[" + GitBranch + "]; hash[" + Hash + "]; " + GitIsDirty + "; Message: [" + Message + "]").c_str()
  );
#endif
  //====================================================
  m_pInput->Init();
  //====================================================
  // Initialize the 2D drawer
#ifdef ENABLE_PROFILER
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
  m_pLog->Log("[OK] Window susbsystem inited\n");
  //====================================================
  m_pScriptSystem = new CScriptSystem();
  if (!static_cast<CScriptSystem*>(m_pScriptSystem)->Init(this))
  {
    return false;
  }
  if (!m_pConsole->Init(this))
    return false;
  m_pConsole->ShowConsole(true);
  //====================================================
  if (!InitResourceManagers())
    return false;
  //====================================================
  m_pConsole->AddConsoleVarSink(this);
  ParseCMD();
  LoadScreen();
  //====================================================
  InitScripts();
  //====================================================
  m_pFont = new FreeTypeFont();
  if (m_pFont != nullptr)
  {
    auto font = "arial.ttf";
    auto var = GET_CVAR("s_font");
    if (var)
      font = var->GetString();
    if (m_pFont->Init(font, 16, 18) == false)
      return false;
  }
  m_GuiManager.Init();

  //====================================================
  m_pInput->AddEventListener(this);
  m_pInput->AddEventListener(m_pConsole);
  m_pInput->AddEventListener(&m_GuiManager);
  if (CreateGame(nullptr) == nullptr)
    return false;
  //====================================================

		//////////////////////////////////////////////////////////////////////////
		// Hardware mouse
		//////////////////////////////////////////////////////////////////////////
		// - Dedicated server is in console mode by default (Hardware Mouse is always shown when console is)
		// - Mouse is always visible by default in Editor (we never start directly in Game Mode)
		// - Mouse has to be enabled manually by the Game (this is typically done in the main menu)
#ifdef DEDICATED_SERVER
		m_env.pHardwareMouse = NULL;
#else
		m_env.pHardwareMouse = new CHardwareMouse(true);
#endif

  //====================================================
  m_pNetwork = CreateNetwork(this);
  if (m_pNetwork == nullptr)
    return false;
  //====================================================
  m_pWorld = new World();
  if (!m_pGame->Init(this, m_startupParams.bDedicatedServer, m_startupParams.bEditor, "Normal")) {
    return false;
  }
  m_pConsole->PrintLine("[OK] IGame created\n");

  return true;
}

void CSystem::Start()
{
  bool bRelaunch = false;

  m_pGame->Run(bRelaunch);

  NOW = SDL_GetPerformanceCounter();
  LAST = 0;

  m_DeltaTime = 0.0;

  while (bRelaunch)
  {
    m_pGame->Release();
    m_pGame = CreateGame(nullptr);
    if (!m_pGame->Init(this, m_startupParams.bDedicatedServer, m_startupParams.bEditor, "Normal"))
      break;
    m_pGame->Run(bRelaunch);
  }
}

void CSystem::Release()
{
  delete this;
}

IShaderManager* CSystem::GetShaderManager()
{
  return nullptr;
}

IRenderer* CSystem::GetIRender()
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
  return m_pInput;
}

IGame* CSystem::GetIGame()
{
  return m_pGame;
}

IGame* CSystem::CreateGame(IGame* game)
{
  m_pGame = CreateIGame("");
  return m_pGame;
}

void CSystem::Quit()
{
  m_pGame->SendMessage("Quit");
  m_pGame->Release();

  exit(0);
}

IFont* CSystem::GetIFont()
{
  return new FreeTypeFont;
}

IWindow* CSystem::GetIWindow()
{
  return m_pWindow;
}

IWorld* CSystem::GetIWorld()
{
  return m_pWorld;
}

#if 0
IInputHandler* CSystem::GetIInputHandler()
{
  return m_InputHandler;
}
#endif

bool CSystem::ConfigLoad(const char* file)
{
  m_pConsole->ExecuteFile(file);

  r_window_width = m_pConsole->GetCVar("r_Width");
  r_window_height = m_pConsole->GetCVar("r_Height");
  r_bpp = m_pConsole->GetCVar("r_bpp");
  r_zbpp = m_pConsole->GetCVar("r_zbpp");
  r_sbpp = m_pConsole->GetCVar("r_sbpp");
  r_fullscreen = m_pConsole->GetCVar("r_fullscreen");
  cvGameName = m_pConsole->GetCVar("cvGameName");

  if (
    r_window_width == nullptr ||
    r_window_height == nullptr ||
    r_bpp == nullptr ||
    r_zbpp == nullptr ||
    r_sbpp == nullptr ||
    r_fullscreen == nullptr
    )
    return false;
  return true;
}

bool CSystem::InitConsole()
{
  m_env.pConsole = m_pConsole = new CConsole();
  if (m_pConsole == nullptr)
    return false;
  return true;
}

bool CSystem::OpenRenderLibrary(std::string_view render)
{
  Log("Open Render Library");
	if (gEnv->IsDedicated())
		return true;
  //====================================================
  m_pWindow = CreateIWindow(/*"BlackBox", 1366, 768*/);
  if (m_pWindow == nullptr)
    return false;
  //====================================================

  m_env.pRenderer = m_Render = CreateIRender(this);
  if (m_Render == nullptr)
    return false;
  else
    return true;

#if 0
	CryFatalError("Unknown renderer type: %s", t_rend);
	return false;
#endif

}

bool CSystem::InitResourceManagers()
{
  m_pConsole->PrintLine("Begin loading resources");
  if (!ShaderManager::init())
    return false;
  if (!MaterialManager::init("default.xml"))
    return false;
  m_pConsole->PrintLine("End loading resources");
  return true;
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
  m_ScriptObjectRenderer->Init(m_pScriptSystem, nullptr);

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

#if !defined(_RELEASE)
	const ICmdLineArg* pArg = m_pCmdLine->FindArg(eCLAT_Pre, "LvlRes");      // -LvlRes command line option

	if (pArg)
		bLvlRes = true;
#endif // !defined(_RELEASE)

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

#ifndef RELEASE
		if (m_bEditor)
		{
			const CryPathString editorDir = PathUtil::Make(CryPathString(engineRootDir.c_str()), CryPathString("Editor"));
			m_env.pCryPak->SetAlias("%EDITOR%", editorDir.c_str(), true);
		}
#endif
	}

	// Now set up the log
	InitLog();

	LogVersion();

	((CCryPak*)m_env.pCryPak)->SetLog(m_env.pLog);

	// Load value of sys_game_folder from system.cfg into the sys_game_folder console variable
	ILoadConfigurationEntrySink* pCVarsWhiteListConfigSink = GetCVarsWhiteListConfigSink();
#if CRY_PLATFORM_ANDROID && !defined(ANDROID_OBB)
	string path = string(CryGetProjectStoragePath()) + "/system.cfg";
	LoadConfiguration(path.c_str(), pCVarsWhiteListConfigSink, eLoadConfigInit);
#else
	LoadConfiguration("system.cfg", pCVarsWhiteListConfigSink, eLoadConfigInit);
#endif

	if (!m_pProjectManager->ParseProjectFile())
	{
		return false;
	}

	bool bRes = m_env.pCryPak->Init("");

	if (bRes)
	{
#if !defined(_RELEASE)
		const ICmdLineArg* pakalias = m_pCmdLine->FindArg(eCLAT_Pre, "pakalias");
#else
		const ICmdLineArg* pakalias = NULL;
#endif // !defined(_RELEASE)
		if (pakalias && strlen(pakalias->GetValue()) > 0)
			m_env.pCryPak->ParseAliases(pakalias->GetValue());
	}

	// Create Engine folder mod mapping only for Engine assets
	pCryPak->AddMod("%ENGINEROOT%/" CRYENGINE_ENGINE_FOLDER);

#if CRY_PLATFORM_ANDROID
	pCryPak->AddMod(CryGetProjectStoragePath());
	#if defined(ANDROID_OBB)
	pCryPak->SetAssetManager(androidGetAssetManager());
	#endif
#elif CRY_PLATFORM_LINUX
	//apparently Linux needs the parent dir as a module for letting CryPak find the file system.cfg
	pCryPak->AddMod("./");
#endif

	return (bRes);
#endif
  return true;
}

float CSystem::GetDeltaTime()
{
  return static_cast<float>(m_DeltaTime);
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
    if (nullptr == m_pWorld->GetActiveScene())
      return;
    m_Render->SetState(IRenderer::State::DEPTH_TEST, true);
    float w;
    float h;
    if (GET_CVAR("r_aspect")->GetIVal())
    {
      w = m_Render->GetHeight();
      h = m_Render->GetWidth();
    }
    else
    {
      w = GET_CVAR("r_cam_w")->GetIVal();
      h = GET_CVAR("r_cam_h")->GetIVal();
    }

    auto r = ((float)w) / h;
    m_pWorld->GetActiveScene()->getCurrentCamera()->Ratio = r > 1 ? r : (float)h / w;

    //m_World->setCamera(m_camera1);
    m_pWorld->Draw(m_DeltaTime);

    if (m_pWorld->GetActiveScene()) m_pWorld->GetActiveScene()->present(m_Render->GetWidth(), m_Render->GetHeight());

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
}

void CSystem::ShowMessage(const char* message, const char* caption, MessageType messageType)
{
#ifdef _WIN32
  ::MessageBox(NULL, message, caption, messageType == 0 ? MB_OK : MB_OKCANCEL);
#endif
}

void CSystem::Log(const char* message)
{
  std::cout << "-- " << message << std::endl;
  m_pLog->Log("-- %s\n", message);
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
  m_GuiManager.NewFrame();
  m_GuiManager.AddDemoWindow();
}

void CSystem::RenderEnd()
{
  PROFILER_POP_CPU_MARKER();
  {
    DEBUG_GROUP("DRAW_PROFILE");
    PROFILER_DRAW();
  }

  m_GuiManager.Render();

  m_pWindow->swap();
}

bool CSystem::OnInputEvent(const SInputEvent& event)
{
  bool result = false;
  bool mouseMoved = false;
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

bool CSystem::Update(int updateFlags/* = 0*/, int nPauseMode/* = 0*/)
{
  //PROFILER_SYNC_FRAME();
  // Update input
  LAST = NOW;
  NOW = SDL_GetPerformanceCounter();

  //m_pNetwork->Update();

  m_DeltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001;
  {
    PROFILER_PUSH_CPU_MARKER("INPUT", Utils::COLOR_LIGHT_BLUE);
    //FIXME: CHECK IT
    m_pInput->Update(true);
    PROFILER_POP_CPU_MARKER();
  }
  m_pSystemEventDispatcher->Update();
  m_pWindow->update();
  m_pConsole->Update();
  m_Render->Update();
  if (m_pWindow->closed())
  {
    m_pGame->SendMessage("Quit");
  }
  return true;
}

ISystem* CreateSystemInterface(SSystemInitParams& initParams)
{
  //MessageBox(NULL, "TEST", "Message", MB_OK);
  ISystem* system = new CSystem(initParams);
  return system;
}
