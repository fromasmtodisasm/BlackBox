#pragma once
class ImGuiManager;
class ImGuiInput
{
public:
  void NewFrame();
  bool Init();
  void UpdateGamepads();
  void UpdateMousePosAndButtons();
  bool OnInputEvent(const SInputEvent& event, struct IImGuiManager* imguiManager);
  bool OnInputEventUI(const SUnicodeEvent& event);
public:
  IInput* m_pInput = nullptr;
  bool m_MousePressed[3] = { false, false, false };
  uint64_t m_Time = 0;
  ImVec2 m_MousePos;
};

