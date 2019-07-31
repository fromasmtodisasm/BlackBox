#include <BlackBox/CEngine.hpp>
#include <BlackBox/CLog.hpp>
#include <BlackBox/CConsole.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/CConsole.hpp>
#include <BlackBox/Render/CRender.hpp>
#include <BlackBox/IConsole.hpp>

#include <cstdlib>
#pragma once

CEngine::CEngine()
	:
	r_window_width(nullptr),
	r_window_height(nullptr),
	r_bpp(nullptr),
	r_zbpp(nullptr),
	r_sbpp(nullptr)
{

}

CEngine::~CEngine()
{
}

bool CEngine::Init()
{
  m_pLog = new CLog();
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

	if (!ConfigLoad("res/scripts/engine.cfg"))
		return false;
	if (!(m_pWindow = m_Render->Init(
		0,0, 
		r_window_width->GetIVal(), r_window_height->GetIVal(), 
		r_bpp->GetIVal(), r_zbpp->GetIVal(), r_sbpp->GetIVal(), 
		false, m_pWindow))
		)
		return false;
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
	r_window_width->Set(std::atoi(r_window_width->GetString()));
	r_window_height->Set(std::atoi(r_window_height->GetString()));
	r_bpp->Set(std::atoi(r_bpp->GetString()));
	r_zbpp->Set(std::atoi(r_zbpp->GetString()));
	r_sbpp->Set(std::atoi(r_sbpp->GetString()));

	return true;
}

SYSTEM_API IEngine * CreateIEngine(void *)
{
  IEngine *system = new CEngine();
  return system;
}
