#include <BlackBox/System.hpp>
#include <BlackBox/NullLog.hpp>
#include <BlackBox/Console.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/Render/Render.hpp>
#include <BlackBox/IConsole.hpp>
#include <BlackBox/ScriptSystem/ScriptSystem.hpp>

#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Resources/ObjectManager.hpp>
#include <BlackBox/Resources/SceneManager.hpp>
#include <BlackBox/Resources/ShaderManager.hpp>
#include <BlackBox/Resources/TextureManager.hpp>
//
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Profiler/HP_Timer.h>
#include <BlackBox/Profiler/Drawer2D.h>

#include <cstdlib>
#pragma once

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
  m_InputHandler(nullptr),
  m_Render(nullptr),
  m_pConsole(nullptr),
  m_pFont(nullptr),
  m_pGame(nullptr),
  m_pLog(nullptr),
  m_pWindow(nullptr),
	m_pScriptSystem(nullptr),
	m_ScriptObjectConsole(nullptr)
{

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
	//=============
	CWindow* window = new CWindow("BlackBox", 1366, 768);
	m_pWindow = window;
	m_InputHandler = window;
	if (window == nullptr)
		return false;
	//=============
  m_pConsole = new CConsole();
  if (m_pConsole == nullptr)
    return false;
	//=============
	m_Render = CreateIRender(this);
	if (m_Render == nullptr)
		return false;
	//=============
	if (!ConfigLoad("res/scripts/engine.cfg"))
		return false;
	if (!MaterialManager::init(this))
	{
		return false;
	}
	//=============
	if (!(m_pWindow = m_Render->Init(
		0,0, 
		r_window_width->GetIVal(), r_window_height->GetIVal(), 
		r_bpp->GetIVal(), r_zbpp->GetIVal(), r_sbpp->GetIVal(), 
		r_fullscreen->GetIVal(), m_pWindow))
		)
		return false;
	//=============
	// Initialize the 2D drawer
	if (!drawer2D.init(m_Render->GetWidth(), m_Render->GetHeight()))
	{
		fprintf(stderr, "*** FAILED initializing the Drawer2D\n");
		return EXIT_FAILURE;
	}
	//=============
	PROFILER_INIT(m_Render->GetWidth(), m_Render->GetHeight(), window->getCursorPos().x, window->getCursorPos().y);
	//=============
	m_pLog->Log("[OK] Window susbsystem inited\n");
	//=============
	m_pScriptSystem = new CScriptSystem();
	if (!static_cast<CScriptSystem*>(m_pScriptSystem)->Init(this))
	{
		return false;
	}
	if (!m_pConsole->Init(this))
		return false;
	//=============
	m_pConsole->AddConsoleVarSink(this);
	ParseCMD();

	m_ScriptObjectConsole = new CScriptObjectConsole();
	CScriptObjectConsole::InitializeTemplate(m_pScriptSystem);

	m_ScriptObjectScript = new CScriptObjectScript();
	CScriptObjectScript::InitializeTemplate(m_pScriptSystem);

	m_ScriptObjectConsole->Init(GetIScriptSystem(), m_pConsole);
	m_ScriptObjectScript->Init(GetIScriptSystem());

	m_pScriptSystem->ExecuteFile("scripts/engine.lua");
	//=============
  m_pFont = new FreeTypeFont();
	if (m_pFont != nullptr)
	{
		if (m_pFont->Init("arial.ttf", 16,18) == false)
			return false;
	}
	m_InputHandler->AddEventListener(this);
	m_InputHandler->AddEventListener(m_pConsole);
  if (CreateGame(nullptr) == nullptr)
    return false;
	
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
}

IFont* CSystem::GetIFont()
{
	return m_pFont;
}

IWindow* CSystem::GetIWindow()
{
	return m_pWindow;
}

IInputHandler* CSystem::GetIInputHandler()
{
	return m_InputHandler;
}

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

bool CSystem::IsDevMode()
{
	return true;
}

void CSystem::Error(const char* message)
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

bool CSystem::OnInputEvent(sf::Event& event)
{
	bool result = false;
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			PROFILER_ON_LEFT_CLICK();
		}
		break;
	}
	case sf::Event::MouseMoved:
	{
		PROFILER_ON_MOUSE_POS(event.mouseMove.x, event.mouseMove.y);
		break;
	}
	case sf::Event::Resized:
	{
		PROFILER_ON_RESIZE(event.size.width, event.size.height);
		break;
	}

	case sf::Event::KeyPressed:
	{
		if (event.key.alt && event.key.shift)
		{
			if (event.key.code == sf::Keyboard::P)
			{
				if (profiler.isFrozen())
				{
					PROFILER_UNFROZE_FRAME();
				}
				else
				{
					PROFILER_FROZE_FRAME();
				}
			}
		}
		else if (event.key.alt)
		{
			if (event.key.code == sf::Keyboard::Enter)
			{
				static_cast<CWindow*>(m_pWindow)->ToogleFullScreen(1366, 768);
			}
		}
	}

	default:
		break;
	}
	return result;
}

void CSystem::Update()
{
	//PROFILER_SYNC_FRAME();
	m_pConsole->Update();
	if (m_pWindow->closed())
	{
		m_pGame->SendMessage("Quit");
	}
}

BLACKBOX_EXPORT ISystem * CreateSystemInterface(SSystemInitParams& initParams)
{
	//MessageBox(NULL, "TEST", "Message", MB_OK);
  ISystem *system = new CSystem(initParams);
  return system;
}

