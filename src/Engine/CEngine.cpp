#include <BlackBox/CEngine.hpp>
#include <BlackBox/CLog.hpp>
#include <BlackBox/CConsole.hpp>
#include <BlackBox/IGame.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>

#include <cstdlib>
#pragma once

bool CEngine::Init()
{
  m_pLog = new CLog();
  if (m_pLog == nullptr)
    return false;
	//=============
  m_pFont = new FreeTypeFont();
  if (m_pFont == nullptr && !m_pFont->Init("arial.ttf", 16,18));
    return false;
	//=============
  m_pConsole = new CConsole();
  if (m_pConsole == nullptr)
    return false;
  if (CreateGame(nullptr) == nullptr)
    return false;
	CWindow* window = new CWindow("BlackBox", 1366, 768);
	m_pWindow = window;
	m_InputHandler = window;
	if (window == nullptr)
		return false;
	
  if (!m_pGame->init(this)) {
    return false;
	}
  m_pLog->AddLog("[OK] IGame created\n");
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
  return nullptr;
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

SYSTEM_API IEngine * CreateIEngine(void *)
{
  IEngine *system = new CEngine();
  return system;
}
