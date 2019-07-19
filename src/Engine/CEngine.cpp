#include <BlackBox/CEngine.hpp>
#include <BlackBox/CLog.hpp>
#include <BlackBox/CConsole.hpp>
#include <BlackBox/IGame.hpp>
#pragma once

bool CEngine::Init()
{
  m_pLog = new CLog();
  if (m_pLog == nullptr)
    return false;
  m_pConsole = new CConsole();
  if (m_pConsole == nullptr)
    return false;
  if (CreateGame(nullptr) == nullptr)
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

SYSTEM_API IEngine * CreateIEngine(void *)
{
  IEngine *system = new CEngine();
  return system;
}
