#include "CSystem.hpp"
#pragma once

void CSystem::Init()
{
}

void CSystem::Start()
{
}

void CSystem::Release()
{
}

IShaderManager * CSystem::getShaderManager()
{
  return nullptr;
}

IRender * CSystem::getIRender()
{
  return nullptr;
}

SYSTEM_API ISystem * CreateISystem(void *)
{
  ISystem *system = new CSystem();
  return system;
}
