#include <BlackBox/CEngine.hpp>
#include <BlackBox/NullLog.hpp>
#include <BlackBox/CConsole.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/CConsole.hpp>
#include <BlackBox/Render/CRender.hpp>
#include <BlackBox/IConsole.hpp>
//
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Profiler/HP_Timer.h>
#include <BlackBox/Profiler/Drawer2D.h>

#include <cstdlib>
#pragma once

using namespace Utils;

CEngine::CEngine()
	:
	r_window_width(nullptr),
	r_window_height(nullptr),
	r_bpp(nullptr),
	r_zbpp(nullptr),
	r_sbpp(nullptr),
  m_InputHandler(nullptr),
  m_Render(nullptr),
  m_pConsole(nullptr),
  m_pFont(nullptr),
  m_pGame(nullptr),
  m_pLog(nullptr),
  m_pWindow(nullptr)
{

}

CEngine::~CEngine()
{

}

bool CEngine::Init()
{
	initTimer();
  m_pLog = new NullLog();
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
	if (!(m_pWindow = m_Render->Init(
		0,0, 
		r_window_width->GetIVal(), r_window_height->GetIVal(), 
		r_bpp->GetIVal(), r_zbpp->GetIVal(), r_sbpp->GetIVal(), 
		false, m_pWindow))
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
	m_pLog->AddLog("[OK] Window susbsystem inited\n");
	//=============
	if (!m_pConsole->Init())
		return false;
	//=============
  m_pFont = new FreeTypeFont();
	if (m_pFont != nullptr)
	{
		if (m_pFont->Init("arial.ttf", 16,18) == false)
			return false;
	}
	m_InputHandler->AddEventListener(m_pConsole);
  if (CreateGame(nullptr) == nullptr)
    return false;
	
  if (!m_pGame->init(this)) {
    return false;
	}
  m_pConsole->PrintLine("[OK] IGame created\n");
  return true;
}

void CEngine::Start()
{
  m_pGame->run();  
}

void CEngine::Release()
{
  SAFE_RELEASE(m_pLog);
  /*
  SAFE_RELEASE(m_pConsole);
  SAFE_RELEASE(m_pGame);
  SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pWindow);
	SAFE_RELEASE(m_InputHandler);
  */
	SAFE_RELEASE(m_Render);
}

IShaderManager * CEngine::getShaderManager()
{
  return nullptr;
}

IRender * CEngine::getIRender()
{
  return m_Render;
}

ILog* CEngine::getILog()
{
  return m_pLog;
}

IConsole* CEngine::getIConsole()
{
  return m_pConsole;
}

IGame* CEngine::getIGame()
{
  return m_pGame;
}

IGame* CEngine::CreateGame(IGame* game)
{
  m_pGame = CreateIGame("MyGame");
  return m_pGame;
}

IFont* CEngine::getIFont()
{
	return m_pFont;
}

IWindow* CEngine::getIWindow()
{
	return m_pWindow;
}

IInputHandler* CEngine::getIInputHandler()
{
	return m_InputHandler;
}

bool CEngine::ConfigLoad(const char* file)
{
	m_pConsole->ExecuteFile(file);

	r_window_width = m_pConsole->GetCVar("r_window_width");
	r_window_height = m_pConsole->GetCVar("r_window_height");
	r_bpp = m_pConsole->GetCVar("r_bpp");
	r_zbpp = m_pConsole->GetCVar("r_zbpp");
	r_sbpp = m_pConsole->GetCVar("r_sbpp");

	if (
		r_window_width == nullptr ||
		r_window_height == nullptr ||
		r_bpp == nullptr ||
		r_zbpp == nullptr ||
		r_sbpp == nullptr
		)
		return false;

	//r_window_height->Set(std::atoi(r_window_height->GetString()));
	//r_window_width->Set(std::atoi(r_window_width->GetString()));
	//r_window_height->Set(std::atoi(r_window_height->GetString()));
	//r_bpp->Set(std::atoi(r_bpp->GetString()));
	//r_zbpp->Set(std::atoi(r_zbpp->GetString()));
	//r_sbpp->Set(std::atoi(r_sbpp->GetString()));

	return true;
}

void CEngine::BeginFrame()
{
	PROFILER_PUSH_CPU_MARKER("Full frame", COLOR_GRAY);
}

void CEngine::EndFrame()
{
	PROFILER_POP_CPU_MARKER();
	PROFILER_DRAW();
}

bool CEngine::OnInputEvent(sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonPressed:
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			PROFILER_ON_LEFT_CLICK();
			return true;
		}
		return false;
	}
	case sf::Event::MouseMoved:
	{
		PROFILER_ON_MOUSE_POS(event.mouseMove.x, event.mouseMove.y);
		return true;
	}
	case sf::Event::Resized:
	{
		PROFILER_ON_RESIZE(event.size.width, event.size.height);
		return true;
	}

	default:
		break;
	}
	return false;
}

void CEngine::Update()
{
	PROFILER_SYNC_FRAME();
}

SYSTEM_API IEngine * CreateIEngine(void *)
{
  IEngine *system = new CEngine();
  return system;
}
