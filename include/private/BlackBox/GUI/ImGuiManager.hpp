#pragma once
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <imgui.h>

class ImGuiManager : public IInputEventListener
{
public:
  ImGuiManager(ISystem* pSystem);
  ~ImGuiManager();
  // Inherited via IInputEventListener
  virtual bool OnInputEvent(const SInputEvent& event) override;
  virtual bool OnInputEventUI(const SUnicodeEvent& event) override;

  bool Init();
  void NewFrame();
  void Render();
  void AddDemoWindow();
private:
  bool show_demo_window = true;
};
