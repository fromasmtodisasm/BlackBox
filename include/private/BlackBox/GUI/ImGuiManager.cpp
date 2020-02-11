#include "ImGuiManager.hpp"

#include <BlackBox/Renderer/IRender.hpp>

ImGuiManager::ImGuiManager(IInput* pInput)
  :
  m_pInput(pInput)
{
}

bool ImGuiManager::OnInputEvent(const SInputEvent& event)
{
  ImGuiIO& io = ImGui::GetIO();
  if (event.deviceType == eIDT_Mouse)
  {
    switch (event.keyId)
    {
    case eKI_MouseWheelUp: io.MouseWheel += 1; return true;
    case eKI_MouseWheelDown: io.MouseWheel -= 1; return true;
    case eKI_Mouse1: m_MousePressed[0] = true; return true;
    case eKI_Mouse2: m_MousePressed[1] = true; return true;
    case eKI_Mouse3: m_MousePressed[2] = true; return true;
    default:
      break;
    }

  }
  if (event.deviceType == eIDT_Keyboard)
  {
    int key = event.keyId;
    IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));
    io.KeysDown[key] = event.state == eIS_Pressed;
    io.KeyShift = event.modifiers & eMM_Shift;
    io.KeyCtrl = event.modifiers & eMM_Ctrl;
    io.KeyAlt = event.modifiers & eMM_Alt;
    io.KeySuper = event.modifiers & eMM_Win;
    return true;
  }
  return false;
}

bool ImGuiManager::OnInputEventUI(const SUnicodeEvent& event)
{
#if 0
  case SDL_TEXTINPUT:
      {
          io.AddInputCharactersUTF8(event->text.text);
          return true;
      }
#endif
  return false;
}

void ImGuiManager::ImGui_ImplSDL2_UpdateGamepads()
{
  ImGuiIO& io = ImGui::GetIO();
  memset(io.NavInputs, 0, sizeof(io.NavInputs));
  if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0)
    return;

  // Get gamepad
#if 0
  SDL_GameController* game_controller = SDL_GameControllerOpen(0);
  if (!game_controller)
  {
    io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
    return;
  }
#endif
  m_pInput->GetDevice
  // Update gamepad inputs
#define MAP_BUTTON(NAV_NO, BUTTON_NO)       { io.NavInputs[NAV_NO] = (m_pInput BUTTON_NO) != 0) ? 1.0f : 0.0f; }
#define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float vn = (float)(SDL_GameControllerGetAxis(game_controller, AXIS_NO) - V0) / (float)(V1 - V0); if (vn > 1.0f) vn = 1.0f; if (vn > 0.0f && io.NavInputs[NAV_NO] < vn) io.NavInputs[NAV_NO] = vn; }
  const int thumb_dead_zone = 8000;           // SDL_gamecontroller.h suggests using this value.
  MAP_BUTTON(ImGuiNavInput_Activate,      SDL_CONTROLLER_BUTTON_A);               // Cross / A
  MAP_BUTTON(ImGuiNavInput_Cancel,        SDL_CONTROLLER_BUTTON_B);               // Circle / B
  MAP_BUTTON(ImGuiNavInput_Menu,          SDL_CONTROLLER_BUTTON_X);               // Square / X
  MAP_BUTTON(ImGuiNavInput_Input,         SDL_CONTROLLER_BUTTON_Y);               // Triangle / Y
  MAP_BUTTON(ImGuiNavInput_DpadLeft,      SDL_CONTROLLER_BUTTON_DPAD_LEFT);       // D-Pad Left
  MAP_BUTTON(ImGuiNavInput_DpadRight,     SDL_CONTROLLER_BUTTON_DPAD_RIGHT);      // D-Pad Right
  MAP_BUTTON(ImGuiNavInput_DpadUp,        SDL_CONTROLLER_BUTTON_DPAD_UP);         // D-Pad Up
  MAP_BUTTON(ImGuiNavInput_DpadDown,      SDL_CONTROLLER_BUTTON_DPAD_DOWN);       // D-Pad Down
  MAP_BUTTON(ImGuiNavInput_FocusPrev,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
  MAP_BUTTON(ImGuiNavInput_FocusNext,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
  MAP_BUTTON(ImGuiNavInput_TweakSlow,     SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
  MAP_BUTTON(ImGuiNavInput_TweakFast,     SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
  MAP_ANALOG(ImGuiNavInput_LStickLeft,    SDL_CONTROLLER_AXIS_LEFTX, -thumb_dead_zone, -32768);
  MAP_ANALOG(ImGuiNavInput_LStickRight,   SDL_CONTROLLER_AXIS_LEFTX, +thumb_dead_zone, +32767);
  MAP_ANALOG(ImGuiNavInput_LStickUp,      SDL_CONTROLLER_AXIS_LEFTY, -thumb_dead_zone, -32767);
  MAP_ANALOG(ImGuiNavInput_LStickDown,    SDL_CONTROLLER_AXIS_LEFTY, +thumb_dead_zone, +32767);

  io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
#undef MAP_BUTTON
#undef MAP_ANALOG
}

void ImGuiManager::ImGui_ImplSDL2_UpdateMousePosAndButtons()
{
  ImGuiIO& io = ImGui::GetIO();

#if 0
  // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
  if (io.WantSetMousePos)
    SDL_WarpMouseInWindow(g_Window, (int)io.MousePos.x, (int)io.MousePos.y);
  else
#endif
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

  int mx, my;
  auto mouse_buttons = SDL_GetMouseState(&mx, &my);
  io.MouseDown[0] = g_MousePressed[0] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;  // If a mouse press event came, always pass it as "mouse held this frame", so we don't miss click-release events that are shorter than 1 frame.
  io.MouseDown[1] = g_MousePressed[1] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
  io.MouseDown[2] = g_MousePressed[2] || (mouse_buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
  g_MousePressed[0] = g_MousePressed[1] = g_MousePressed[2] = false;

#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE && !defined(__EMSCRIPTEN__) && !defined(__ANDROID__) && !(defined(__APPLE__) && TARGET_OS_IOS)
  SDL_Window* focused_window = SDL_GetKeyboardFocus();
  if (g_Window == focused_window)
  {
    // SDL_GetMouseState() gives mouse position seemingly based on the last window entered/focused(?)
    // The creation of a new windows at runtime and SDL_CaptureMouse both seems to severely mess up with that, so we retrieve that position globally.
    int wx, wy;
    SDL_GetWindowPosition(focused_window, &wx, &wy);
    SDL_GetGlobalMouseState(&mx, &my);
    mx -= wx;
    my -= wy;
    io.MousePos = ImVec2((float)mx, (float)my);
  }

  // SDL_CaptureMouse() let the OS know e.g. that our imgui drag outside the SDL window boundaries shouldn't e.g. trigger the OS window resize cursor.
  // The function is only supported from SDL 2.0.4 (released Jan 2016)
  bool any_mouse_button_down = ImGui::IsAnyMouseDown();
  SDL_CaptureMouse(any_mouse_button_down ? SDL_TRUE : SDL_FALSE);
#else
  if (SDL_GetWindowFlags(g_Window) & SDL_WINDOW_INPUT_FOCUS)
    io.MousePos = ImVec2((float)mx, (float)my);
#endif
}

void ImGuiManager::ImGui_ImplSDL2_NewFrame()
{
  ImGuiIO& io = ImGui::GetIO();
  IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

  // Setup display size (every frame to accommodate for window resizing)
  int w, h;
  int display_w, display_h;
  SDL_GetWindowSize(window, &w, &h);
  SDL_GL_GetDrawableSize(window, &display_w, &display_h);
  io.DisplaySize = ImVec2((float)w, (float)h);
  if (w > 0 && h > 0)
    io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

  // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
  static Uint64 frequency = SDL_GetPerformanceFrequency();
  Uint64 current_time = SDL_GetPerformanceCounter();
  io.DeltaTime = m_Time > 0 ? (float)((double)(current_time - m_Time) / frequency) : (float)(1.0f / 60.0f);
  m_Time = current_time;

  ImGui_ImplSDL2_UpdateMousePosAndButtons();
  ImGui_ImplSDL2_UpdateMouseCursor();

  // Update game controllers (if enabled and available)
  ImGui_ImplSDL2_UpdateGamepads();
}
