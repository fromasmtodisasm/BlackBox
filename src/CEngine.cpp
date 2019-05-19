#include <BlackBox/CEngine.hpp>
#include <BlackBox/CLog.hpp>
#pragma once

void CEngine::Init()
{
  m_pLog = new CLog();
}

void CEngine::Start()
{
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

SYSTEM_API IEngine * CreateIEngine(void *)
{
  IEngine *system = new CEngine();
  return system;
}
