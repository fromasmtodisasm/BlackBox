#pragma once
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Input/IInput.hpp>

#if ENABLE_DEBUG_GUI
struct IImGuiManager : public IInputEventListener
{
  ~IImGuiManager() {}
  // Inherited via IInputEventListener
  virtual bool OnInputEvent(const SInputEvent& event) = 0;
  virtual bool OnInputEventUI(const SUnicodeEvent& event) = 0;

  virtual bool Init() = 0;
  virtual void NewFrame() = 0;
  virtual void Render() = 0;
  virtual void ShowDemoWindow() = 0;
  virtual void ShowNodeEditor() = 0;
  virtual void HideDemoWindow() = 0;
};

extern "C" {
  IRENDER_API IImGuiManager* CreateGUI(ISystem* pSystem/*, void* hinst, void* hwnd, bool usedinput*/);
  typedef IImGuiManager* (*PFNCREATEGUI)(ISystem* pSystem);
}

#endif
