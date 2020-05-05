#pragma once
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Input/IInput.hpp>

class DLL_EXPORT ImGuiManager : public IInputEventListener
{
public:
  ImGuiManager(ISystem* pSystem);
  ~ImGuiManager();
  // Inherited via IInputEventListener
  virtual bool OnInputEvent(const SInputEvent& event) override;
  virtual bool OnInputEventUI(const SUnicodeEvent& event) override;
  virtual int GetPriority() const { return 3; }

  bool Init();
  void NewFrame();
  void Render();
  void ShowDemoWindow();
  void ShowNodeEditor();
  void HideDemoWindow();
private:
  bool show_demo_window = true;
};

