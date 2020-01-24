#pragma once
#include <BlackBox/Input/IInput.hpp>
#include <imgui.h>

class ImGuiManager : public IInputEventListener
{
public:
  ImGuiManager(IInput* pInput);
  // Inherited via IInputEventListener
  virtual bool OnInputEvent(const SInputEvent& event) override;
  virtual bool OnInputEventUI(const SUnicodeEvent& event) override;
private:
  void ImGui_ImplSDL2_UpdateGamepads();

  void ImGui_ImplSDL2_UpdateMousePosAndButtons();
  void ImGui_ImplSDL2_NewFrame();
  

private:
  IInput* m_pInput = nullptr;
  bool m_MousePressed[3] = { false, false, false };
  uint64_t m_Time = 0;

};
