#include <BlackBox/Core/Platform/platform_impl.inl>

#include <BlackBox/GUI/ImGuiManager.hpp>

#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>

#include <BlackBox/Renderer/OpenGL/Core.hpp>

#include "imgui.h"
#define IMGUI_VANILA_MANAGER

#ifdef IMGUI_VANILA_MANAGER
#include "ImGuiVanilaManager.cpp"
#else
#include "ImGuiManager.cpp"
#endif // VANILA_MANAGER

DLL_EXPORT IImGuiManager * CreateGUI(ISystem * pSystem)
{
  return new ImGuiManager(pSystem);
}
