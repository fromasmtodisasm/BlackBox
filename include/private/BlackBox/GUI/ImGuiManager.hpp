#pragma once
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <imgui.h>

class ImGuiManager : public IInputEventListener
{
public:
  ImGuiManager(ISystem* pSystem);
  // Inherited via IInputEventListener
  virtual bool OnInputEvent(const SInputEvent& event) override;
  virtual bool OnInputEventUI(const SUnicodeEvent& event) override;
  bool Init();
  void NewFrame();
  void Render();
  void AddDemoWindow();
private:
  void UpdateGamepads();
  void UpdateMousePosAndButtons();
  void SDL_NewFrame();
  bool SDL_Init();

  bool OpenGL3_Init(const char* glsl_version = NULL);
  void OpenGL3_Shutdown();
  void OpenGL3_NewFrame();
  void OpenGL3_RenderDrawData(ImDrawData* draw_data);

    

private:
  IInput* m_pInput = nullptr;
  bool m_MousePressed[3] = { false, false, false };
  uint64_t m_Time = 0;
  bool show_demo_window = true;
  ISystem* m_pSystem;
  ImVec2 m_MousePose;

};
