#include <BlackBox/CEngine.hpp>
#include <BlackBox/NullLog.hpp>
#include <BlackBox/CConsole.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/CConsole.hpp>
#include <BlackBox/Render/CRender.hpp>
#include <BlackBox/IConsole.hpp>
#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
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
	m_pConsole->AddConsoleVarSink(this);
	//=============
	m_ScriptSystem = new CScriptSystem();
	if (!static_cast<CScriptSystem*>(m_ScriptSystem)->Init(this))
	{
		return false;
	}
	m_ScriptSystem->ExecuteBuffer("print(\"Lua!!!\")", 0);
	m_ScriptSystem->ExecuteBuffer("a = 7 + 3", 0);
	int a_cpp;
	if (m_ScriptSystem->GetGlobalValue("a", a_cpp))
		m_pConsole->PrintLine("a = %d", a_cpp);
	else
		m_pConsole->PrintLine("Cant get global value");
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
	return true;
}

IScriptSystem* CEngine::getIIScriptSystem()
{
	return m_ScriptSystem;
}

bool CEngine::OnBeforeVarChange(ICVar* pVar, const char* sNewValue)
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

void CEngine::BeginFrame()
{
	PROFILER_SYNC_FRAME();
	PROFILER_PUSH_CPU_MARKER("Full frame", COLOR_GRAY);
}

void CEngine::EndFrame()
{
	PROFILER_POP_CPU_MARKER();
	PROFILER_DRAW();
}

bool CEngine::OnInputEvent(sf::Event& event)
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
	}

	default:
		break;
	}
	return result;
}

void CEngine::Update()
{
	//PROFILER_SYNC_FRAME();
}

BLACKBOX_EXPORT ISystem * CreateSystemInterface(SSystemInitParams& initParams)
{
	//MessageBox(NULL, "TEST", "Message", MB_OK);
  ISystem *system = new CEngine();
  return system;
}
