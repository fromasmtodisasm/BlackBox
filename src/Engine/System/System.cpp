#include <BlackBox/platform_impl.inl>
#include <BlackBox/System.hpp>
#include <BlackBox/NullLog.hpp>
#include <BlackBox/Console.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/Render/Render.hpp>
#include <BlackBox/IConsole.hpp>
#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
#include <BlackBox/SystemEventDispatcher.hpp>

#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Resources/ObjectManager.hpp>
#include <BlackBox/Resources/SceneManager.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/Resources/TextureManager.hpp>
#include <BlackBox/IWindow.hpp>
//
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Profiler/HP_Timer.h>
#include <BlackBox/Profiler/Drawer2D.h>

#include <BlackBox/INetwork.hpp>

#include <SDL2/SDL.h>

#include <cstdlib>

using namespace Utils;

//////////////////////////////////////////////////////////////////////
// Pointer to Global ISystem.
static ISystem* gISystem = nullptr;
ISystem* GetISystem()
{
  return gISystem;
}


CSystem::CSystem(SSystemInitParams& m_startupParams)
	:
	m_startupParams(m_startupParams),
	r_window_width(nullptr),
	r_window_height(nullptr),
	r_bpp(nullptr),
	r_zbpp(nullptr),
	r_sbpp(nullptr),
	r_fullscreen(nullptr),
  m_Render(nullptr),
  m_pConsole(nullptr),
  m_pFont(nullptr),
  m_pGame(nullptr),
  m_pLog(nullptr),
  m_pWindow(nullptr),
	m_pScriptSystem(nullptr),
	m_ScriptObjectConsole(nullptr)
{
	m_pSystemEventDispatcher = new CSystemEventDispatcher(); // Must be first.
	if (m_pSystemEventDispatcher)
	{
		m_pSystemEventDispatcher->RegisterListener(this, "CSystem");
	}

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
	/////////////////////////////////////////////
	m_pCmdLine = new CCmdLine(m_startupParams.szSystemCmdLine);
	initTimer();
  m_pLog = new NullLog(m_startupParams.sLogFileName);
  if (m_pLog == nullptr)
    return false;
	//====================================================
	m_pWindow= CreateIWindow(/*"BlackBox", 1366, 768*/);
	if (m_pWindow == nullptr)
		return false;
	//====================================================
  m_pConsole = new CConsole();
  if (m_pConsole == nullptr)
    return false;
	//====================================================
	m_pInput = CreateInput(this, m_pWindow->getHandle());
	//====================================================
	m_Render = CreateIRender(this);
	if (m_Render == nullptr)
		return false;
	//====================================================
	if (!ConfigLoad("res/scripts/engine.cfg"))
		return false;
	if (!MaterialManager::init(this))
	{
		return false;
	}
	//====================================================
	if (!(m_pWindow = m_Render->Init(
		0,0,
		r_window_width->GetIVal(), r_window_height->GetIVal(),
		r_bpp->GetIVal(), r_zbpp->GetIVal(), r_sbpp->GetIVal(),
		r_fullscreen->GetIVal(), m_pWindow))
		)
		return false;
	//====================================================
	m_pInput->Init();
	//====================================================
	// Initialize the 2D drawer
	if (!drawer2D.init(m_Render->GetWidth(), m_Render->GetHeight()))
	{
		fprintf(stderr, "*** FAILED initializing the Drawer2D\n");
		return EXIT_FAILURE;
	}
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
	//====================================================
	m_pConsole->AddConsoleVarSink(this);
	ParseCMD();
	//====================================================
	m_ScriptObjectConsole = new CScriptObjectConsole();
	CScriptObjectConsole::InitializeTemplate(m_pScriptSystem);

	m_ScriptObjectScript = new CScriptObjectScript();
	CScriptObjectScript::InitializeTemplate(m_pScriptSystem);

	m_ScriptObjectConsole->Init(GetIScriptSystem(), m_pConsole);
	m_ScriptObjectScript->Init(GetIScriptSystem());

	m_pScriptSystem->ExecuteFile("scripts/engine.lua");
	//====================================================
  m_pFont = new FreeTypeFont();
	if (m_pFont != nullptr)
	{
		auto font = "arial.ttf";
		auto var = GET_CVAR("s_font");
		if (var)
			font = var->GetString();
		if (m_pFont->Init(font, 16,18) == false)
			return false;
	}
	m_pInput->AddEventListener(this);
	m_pInput->AddEventListener(m_pConsole);
  if (CreateGame(nullptr) == nullptr)
    return false;
	//====================================================
#if 0
	m_pNetwork = CreateNetwork(this);
	if (m_pNetwork == nullptr)
		return false;
#endif
	//====================================================

  if (!m_pGame->Init(this)) {
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
		if (!m_pGame->Init(this))
			break;
		m_pGame->Run(bRelaunch);
	}

}

void CSystem::Release()
{
	delete this;
}

IShaderManager * CSystem::GetShaderManager()
{
  return nullptr;
}

IRender * CSystem::GetIRender()
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
  m_pGame = CreateIGame("MyGame");
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

void CSystem::ParseCMD()
{
	std::string cmd = m_startupParams.szSystemCmdLine;
	if (cmd.find("-nsightDebug") != std::string::npos)
	{
		m_pConsole->CreateVariable("nsightDebug", 1, VF_NULL, "Debuggin via Nsight Graphics");
	}
}

float CSystem::GetDeltaTime()
{
	return static_cast<float>(m_DeltaTime);
}

INetwork* CSystem::GetINetwork()
{
	return m_pNetwork;
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
	::MessageBox(NULL, message, caption, messageType == 0 ? MB_OK : MB_OKCANCEL);
}

void CSystem::Log(const char* message)
{
	std::cout << "-- "<< message << std::endl;
}

IScriptSystem* CSystem::GetIScriptSystem()
{
	//2841004695
	return m_pScriptSystem;
}

bool CSystem::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
{
	if (!strcmp(pVar->GetName(),"r_cap_profile"))
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

void CSystem::BeginFrame()
{
	PROFILER_SYNC_FRAME();
	PROFILER_PUSH_CPU_MARKER("Full frame", COLOR_GRAY);
	m_Render->SetState(IRender::State::DEPTH_TEST, true);
	m_Render->BeginFrame();
}

void CSystem::EndFrame()
{
	PROFILER_POP_CPU_MARKER();
	{
		DEBUG_GROUP("DRAW_PROFILE");
		PROFILER_DRAW();
	}

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

BLACKBOX_EXPORT ISystem * CreateSystemInterface(SSystemInitParams& initParams)
{
	//MessageBox(NULL, "TEST", "Message", MB_OK);
  ISystem *system = new CSystem(initParams);
  return system;
}
