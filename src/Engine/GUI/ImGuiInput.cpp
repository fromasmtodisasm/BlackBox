#include <BlackBox/GUI/ImGuiManager.hpp>
#include <BlackBox/Input/IHardwareMouse.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/System/IWindow.hpp>
#include "imgui.h"
#include "ImGuiInput.hpp"
#include "ImGuiRenderer.hpp"

// FIXME: work around input system
#include <SDL2/SDL.h>
#include <SDL2/SDL_syswm.h>

static WIN_HWND     g_Window = NULL;
static char*        g_ClipboardTextData = NULL;
// Forward Declarations
static void ImGui_ImplSDL2_UpdateMonitors();
static void ImGui_ImplSDL2_InitPlatformInterface(WIN_HWND window);
static void ImGui_ImplSDL2_ShutdownPlatformInterface();

static const char* ImGui_ImplSDL2_GetClipboardText(void*)
{
	if (g_ClipboardTextData)
		SDL_free(g_ClipboardTextData);
	g_ClipboardTextData = SDL_GetClipboardText();
	return g_ClipboardTextData;
}

static void ImGui_ImplSDL2_SetClipboardText(void*, const char* text)
{
	SDL_SetClipboardText(text);
}
// FIXME-PLATFORM: SDL doesn't have an event to notify the application of display/monitor changes
static void ImGui_ImplSDL2_UpdateMonitors()
{
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Monitors.resize(0);
    int display_count = SDL_GetNumVideoDisplays();
    for (int n = 0; n < display_count; n++)
    {
        // Warning: the validity of monitor DPI information on Windows depends on the application DPI awareness settings, which generally needs to be set in the manifest or at runtime.
        ImGuiPlatformMonitor monitor;
        SDL_Rect r;
        SDL_GetDisplayBounds(n, &r);
        monitor.MainPos = monitor.WorkPos = ImVec2((float)r.x, (float)r.y);
        monitor.MainSize = monitor.WorkSize = ImVec2((float)r.w, (float)r.h);
#if SDL_HAS_USABLE_DISPLAY_BOUNDS
        SDL_GetDisplayUsableBounds(n, &r);
        monitor.WorkPos = ImVec2((float)r.x, (float)r.y);
        monitor.WorkSize = ImVec2((float)r.w, (float)r.h);
#endif
#if SDL_HAS_PER_MONITOR_DPI
        float dpi = 0.0f;
        if (!SDL_GetDisplayDPI(n, &dpi, NULL, NULL))
            monitor.DpiScale = dpi / 96.0f;
#endif
        platform_io.Monitors.push_back(monitor);
    }
}

void ImGuiInput::NewFrame()
{
  ImGuiIO& io = ImGui::GetIO();
  IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

  // Setup display size (every frame to accommodate for window resizing)
  int w, h;
  int display_w, display_h;
  auto r = GetISystem()->GetIRenderer();
  // TODO: rewrite it
  //SDL_GetWindowSize(window, &w, &h);
  w = display_w = r->GetWidth();
  h = display_h = r->GetHeight();

  //int vp[4]; 
  //r->GetViewport(&vp[0], &vp[1], &vp[2], &vp[3]);
  //w				 = vp[2];
  //h				 = vp[3];
  io.DisplaySize = ImVec2((float)w, (float)h);
  if (w > 0 && h > 0)
    io.DisplayFramebufferScale = ImVec2((float)w / w, (float)h / h);

  // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
  // TODO: rewrite it
  static auto frequency = 0;// SDL_GetPerformanceFrequency();
  auto current_time = 0;// SDL_GetPerformanceCounter();
  io.DeltaTime = m_Time > 0 ? (float)((double)(current_time - m_Time) / frequency) : (float)(1.0f / 60.0f);
  m_Time = current_time;

  UpdateMousePosAndButtons();
  UpdateMouseCursor();

  // Update game controllers (if enabled and available)
  UpdateGamepads();
}
bool ImGuiInput::Init()
{

  auto window = g_Window = gEnv->pRenderer->GetCurrentContextHWND();
  // Setup back-end capabilities flags
  ImGuiIO& io = ImGui::GetIO();
  io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
  io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)
  io.BackendPlatformName = "ImGui_Impl_BlackBoxInput";

  // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
  io.KeyMap[ImGuiKey_Tab] = eKI_Tab;
  io.KeyMap[ImGuiKey_LeftArrow] = eKI_Left;
  io.KeyMap[ImGuiKey_RightArrow] = eKI_Right;
  io.KeyMap[ImGuiKey_UpArrow] = eKI_Up;
  io.KeyMap[ImGuiKey_DownArrow] = eKI_Down;
  io.KeyMap[ImGuiKey_PageUp] = eKI_PgUp;
  io.KeyMap[ImGuiKey_PageDown] = eKI_PgDn;
  io.KeyMap[ImGuiKey_Home] = eKI_Home;
  io.KeyMap[ImGuiKey_End] = eKI_End;
  io.KeyMap[ImGuiKey_Insert] = eKI_Insert;
  io.KeyMap[ImGuiKey_Delete] = eKI_Delete;
  io.KeyMap[ImGuiKey_Backspace] = eKI_Backspace;
  io.KeyMap[ImGuiKey_Space] = eKI_Space;
  io.KeyMap[ImGuiKey_Enter] = eKI_Enter;
  io.KeyMap[ImGuiKey_Escape] = eKI_Escape;
  io.KeyMap[ImGuiKey_KeyPadEnter] = eKI_NP_Enter;
  io.KeyMap[ImGuiKey_A] = eKI_A;
  io.KeyMap[ImGuiKey_C] = eKI_C;
  io.KeyMap[ImGuiKey_V] = eKI_V;
  io.KeyMap[ImGuiKey_X] = eKI_X;
  io.KeyMap[ImGuiKey_Y] = eKI_Y;
  io.KeyMap[ImGuiKey_Z] = eKI_Z;

  //io.SetClipboardTextFn = ImGui_ImplSDL2_SetClipboardText;
  //io.GetClipboardTextFn = ImGui_ImplSDL2_GetClipboardText;
  io.ClipboardUserData = NULL;

  // Load mouse cursors
  m_MouseCursors[ImGuiMouseCursor_Arrow] = gEnv->pInput->CreateCursor(ICursor::ARROW);
  m_MouseCursors[ImGuiMouseCursor_TextInput] = gEnv->pInput->CreateCursor(ICursor::IBEAM);
  m_MouseCursors[ImGuiMouseCursor_ResizeAll] = gEnv->pInput->CreateCursor(ICursor::SIZEALL);
  m_MouseCursors[ImGuiMouseCursor_ResizeNS] = gEnv->pInput->CreateCursor(ICursor::SIZENS);
  m_MouseCursors[ImGuiMouseCursor_ResizeEW] = gEnv->pInput->CreateCursor(ICursor::SIZEWE);
  m_MouseCursors[ImGuiMouseCursor_ResizeNESW] = gEnv->pInput->CreateCursor(ICursor::SIZENESW);
  m_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = gEnv->pInput->CreateCursor(ICursor::SIZENWSE);
  m_MouseCursors[ImGuiMouseCursor_Hand] = gEnv->pInput->CreateCursor(ICursor::HAND);
  m_MouseCursors[ImGuiMouseCursor_NotAllowed] = gEnv->pInput->CreateCursor(ICursor::NO);

  // Check and store if we are on Wayland
  //g_MouseCanUseGlobalState = strncmp(SDL_GetCurrentVideoDriver(), "wayland", 7) != 0;
  //g_MouseCanUseGlobalState = false;// strncmp(SDL_GetCurrentVideoDriver(), "wayland", 7) != 0;

	// Our mouse update function expect PlatformHandle to be filled for the main viewport
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    main_viewport->PlatformHandle = (void*)window;
#if defined(_WIN32)
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo((SDL_Window*)window, &info))
        main_viewport->PlatformHandleRaw = info.info.win.window;
#endif

    // Update monitors
    ImGui_ImplSDL2_UpdateMonitors();

    // We need SDL_CaptureMouse(), SDL_GetGlobalMouseState() from SDL 2.0.4+ to support multiple viewports.
    // We left the call to ImGui_ImplSDL2_InitPlatformInterface() outside of #ifdef to avoid unused-function warnings.
    if ((io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) && (io.BackendFlags & ImGuiBackendFlags_PlatformHasViewports))
        ImGui_ImplSDL2_InitPlatformInterface(window);

    gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "ImGuiInput");

    return true;
}
void ImGuiInput::UpdateGamepads()
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
#if 0
  m_pInput->GetDevice
    // Update gamepad inputs
#define MAP_BUTTON(NAV_NO, BUTTON_NO)       { io.NavInputs[NAV_NO] = (m_pInput BUTTON_NO) != 0) ? 1.0f : 0.0f; }
#define MAP_ANALOG(NAV_NO, AXIS_NO, V0, V1) { float vn = (float)(SDL_GameControllerGetAxis(game_controller, AXIS_NO) - V0) / (float)(V1 - V0); if (vn > 1.0f) vn = 1.0f; if (vn > 0.0f && io.NavInputs[NAV_NO] < vn) io.NavInputs[NAV_NO] = vn; }
    const int thumb_dead_zone = 8000;           // SDL_gamecontroller.h suggests using this value.
  MAP_BUTTON(ImGuiNavInput_Activate, SDL_CONTROLLER_BUTTON_A);               // Cross / A
  MAP_BUTTON(ImGuiNavInput_Cancel, SDL_CONTROLLER_BUTTON_B);               // Circle / B
  MAP_BUTTON(ImGuiNavInput_Menu, SDL_CONTROLLER_BUTTON_X);               // Square / X
  MAP_BUTTON(ImGuiNavInput_Input, SDL_CONTROLLER_BUTTON_Y);               // Triangle / Y
  MAP_BUTTON(ImGuiNavInput_DpadLeft, SDL_CONTROLLER_BUTTON_DPAD_LEFT);       // D-Pad Left
  MAP_BUTTON(ImGuiNavInput_DpadRight, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);      // D-Pad Right
  MAP_BUTTON(ImGuiNavInput_DpadUp, SDL_CONTROLLER_BUTTON_DPAD_UP);         // D-Pad Up
  MAP_BUTTON(ImGuiNavInput_DpadDown, SDL_CONTROLLER_BUTTON_DPAD_DOWN);       // D-Pad Down
  MAP_BUTTON(ImGuiNavInput_FocusPrev, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
  MAP_BUTTON(ImGuiNavInput_FocusNext, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
  MAP_BUTTON(ImGuiNavInput_TweakSlow, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);    // L1 / LB
  MAP_BUTTON(ImGuiNavInput_TweakFast, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);   // R1 / RB
  MAP_ANALOG(ImGuiNavInput_LStickLeft, SDL_CONTROLLER_AXIS_LEFTX, -thumb_dead_zone, -32768);
  MAP_ANALOG(ImGuiNavInput_LStickRight, SDL_CONTROLLER_AXIS_LEFTX, +thumb_dead_zone, +32767);
  MAP_ANALOG(ImGuiNavInput_LStickUp, SDL_CONTROLLER_AXIS_LEFTY, -thumb_dead_zone, -32767);
  MAP_ANALOG(ImGuiNavInput_LStickDown, SDL_CONTROLLER_AXIS_LEFTY, +thumb_dead_zone, +32767);
#endif

  io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
#undef MAP_BUTTON
#undef MAP_ANALOG
}
void ImGuiInput::UpdateMousePosAndButtons()
{
  ImGuiIO& io = ImGui::GetIO();


  // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
  if (io.WantSetMousePos)
    gEnv->pHardwareMouse->SetHardwareMousePosition(io.MousePos.x, io.MousePos.y);
  else
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

  io.MouseDown[0] = m_MousePressed[0];
  io.MouseDown[1] = m_MousePressed[1];
  io.MouseDown[2] = m_MousePressed[2];

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
  //if (SDL_GetWindowFlags(g_Window) & SDL_WINDOW_INPUT_FOCUS)
  if (true)
    gEnv->pHardwareMouse->GetHardwareMousePosition(&m_MousePos.x, &m_MousePos.y);
  io.MousePos = m_MousePos;
#endif
}
bool ImGuiInput::OnInputEvent(const SInputEvent& event, IImGuiManager* imguiManager)
{
  ImGuiIO& io = ImGui::GetIO();

  //if (event.pSymbol != nullptr)
  if (event.deviceType == eIDT_Mouse)
  {
    switch (event.keyId)
    {
    case eKI_MouseWheelUp: io.MouseWheel += 1; //return true;
      break;
    case eKI_MouseWheelDown: io.MouseWheel -= 1; //return true;
      break;
    case eKI_Mouse1: m_MousePressed[0] = event.state == eIS_Pressed;
      break;
    case eKI_Mouse2: m_MousePressed[1] = event.state == eIS_Pressed;
      break;
    case eKI_Mouse3: m_MousePressed[2] = event.state == eIS_Pressed;
      break;
    case eKI_Escape: GetISystem()->GetIInput()->RemoveEventListener(imguiManager);
      break;
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
    //return true;
  }
  if (io.WantCaptureKeyboard == true)
		return true;
  if (io.WantCaptureMouse == true)
		return true;
  return false;
}
bool ImGuiInput::OnInputEventUI(const SUnicodeEvent& event)
{
  ImGuiIO& io = ImGui::GetIO();
  io.AddInputCharactersUTF8(reinterpret_cast<const char*>(&event.inputChar));
  if (io.WantCaptureKeyboard)
	  return true;
  else
	  return false;
}

void ImGuiInput::UpdateMouseCursor()
{
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
		return;

	ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
	if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
	{
		// Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
		gEnv->pInput->ShowCursor(false);
	}
	else
	{
		// Show OS mouse cursor
		(m_MouseCursors[imgui_cursor] ? m_MouseCursors[imgui_cursor] : m_MouseCursors[ImGuiMouseCursor_Arrow])->Ativate();
		//SDL_ShowCursor(SDL_TRUE);
	}
}
void ImGuiInput::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	if (event == ESYSTEM_EVENT_MOVE || event == ESYSTEM_EVENT_RESIZE)
	{
		if (ImGuiViewport* viewport = ImGui::FindViewportByPlatformHandle(CurrentHandledWindow()))
		{
			switch (event)
			{
			case ESYSTEM_EVENT_MOVE:
				viewport->PlatformRequestMove = true;
				break;
			case ESYSTEM_EVENT_RESIZE:
				viewport->PlatformRequestResize = true;
				break;
			default:
				break;
			}
		}
	
    }

}
// Helper structure we store in the void* RenderUserData field of each ImGuiViewport to easily retrieve our backend data.
struct ImGuiViewportDataSDL2
{
    SDL_Window*     Window;
    Uint32          WindowID;
    bool            WindowOwned;
    SDL_GLContext   GLContext;

    ImGuiViewportDataSDL2() { Window = NULL; WindowID = 0; WindowOwned = false; GLContext = NULL; }
    ~ImGuiViewportDataSDL2() { IM_ASSERT(Window == NULL && GLContext == NULL); }
};

static void ImGui_ImplSDL2_CreateWindow(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = IM_NEW(ImGuiViewportDataSDL2)();
    viewport->PlatformUserData = data;

    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGuiViewportDataSDL2* main_viewport_data = (ImGuiViewportDataSDL2*)main_viewport->PlatformUserData;

    // Share GL resources with main context
	bool use_opengl = true;
	//(main_viewport_data->GLContext != NULL);
    SDL_GLContext backup_context = NULL;
    if (use_opengl)
    {
        backup_context = SDL_GL_GetCurrentContext();
		gEnv->pRenderer->SetCurrentContext(main_viewport_data->Window);
    }

    Uint32 sdl_flags = 0;
    sdl_flags |= SDL_WINDOW_OPENGL;
    sdl_flags |= SDL_GetWindowFlags((SDL_Window*)g_Window) & SDL_WINDOW_ALLOW_HIGHDPI;
    sdl_flags |= SDL_WINDOW_HIDDEN;
    sdl_flags |= (viewport->Flags & ImGuiViewportFlags_NoDecoration) ? SDL_WINDOW_BORDERLESS : 0;
    sdl_flags |= (viewport->Flags & ImGuiViewportFlags_NoDecoration) ? 0 : SDL_WINDOW_RESIZABLE;
#if !defined(_WIN32)
    // See SDL hack in ImGui_ImplSDL2_ShowWindow().
    sdl_flags |= (viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon) ? SDL_WINDOW_SKIP_TASKBAR : 0;
#endif
#if SDL_HAS_ALWAYS_ON_TOP
    sdl_flags |= (viewport->Flags & ImGuiViewportFlags_TopMost) ? SDL_WINDOW_ALWAYS_ON_TOP : 0;
#endif
    data->Window = SDL_CreateWindow("No Title Yet", (int)viewport->Pos.x, (int)viewport->Pos.y, (int)viewport->Size.x, (int)viewport->Size.y, sdl_flags);
    data->WindowOwned = true;
    if (use_opengl)
    {
        data->GLContext = SDL_GL_CreateContext(data->Window);
        SDL_GL_SetSwapInterval(0);
    }
    if (use_opengl && backup_context)
        SDL_GL_MakeCurrent(data->Window, backup_context);

    viewport->PlatformHandle = (void*)data->Window;
#if defined(_WIN32)
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(data->Window, &info))
        viewport->PlatformHandleRaw = info.info.win.window;
#endif
}

static void ImGui_ImplSDL2_DestroyWindow(ImGuiViewport* viewport)
{
    if (ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData)
    {
        if (data->GLContext && data->WindowOwned)
            SDL_GL_DeleteContext(data->GLContext);
        if (data->Window && data->WindowOwned)
            SDL_DestroyWindow(data->Window);
        data->GLContext = NULL;
        data->Window = NULL;
        IM_DELETE(data);
    }
    viewport->PlatformUserData = viewport->PlatformHandle = NULL;
}

static void ImGui_ImplSDL2_ShowWindow(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
#if defined(_WIN32)
    HWND hwnd = (HWND)viewport->PlatformHandleRaw;

    // SDL hack: Hide icon from task bar
    // Note: SDL 2.0.6+ has a SDL_WINDOW_SKIP_TASKBAR flag which is supported under Windows but the way it create the window breaks our seamless transition.
    if (viewport->Flags & ImGuiViewportFlags_NoTaskBarIcon)
    {
        LONG ex_style = ::GetWindowLong(hwnd, GWL_EXSTYLE);
        ex_style &= ~WS_EX_APPWINDOW;
        ex_style |= WS_EX_TOOLWINDOW;
        ::SetWindowLong(hwnd, GWL_EXSTYLE, ex_style);
    }

    // SDL hack: SDL always activate/focus windows :/
    if (viewport->Flags & ImGuiViewportFlags_NoFocusOnAppearing)
    {
        ::ShowWindow(hwnd, SW_SHOWNA);
        return;
    }
#endif

    SDL_ShowWindow(data->Window);
}

static ImVec2 ImGui_ImplSDL2_GetWindowPos(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    int x = 0, y = 0;
    SDL_GetWindowPosition(data->Window, &x, &y);
    return ImVec2((float)x, (float)y);
}

static void ImGui_ImplSDL2_SetWindowPos(ImGuiViewport* viewport, ImVec2 pos)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    SDL_SetWindowPosition(data->Window, (int)pos.x, (int)pos.y);
}

static ImVec2 ImGui_ImplSDL2_GetWindowSize(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    int w = 0, h = 0;
    SDL_GetWindowSize(data->Window, &w, &h);
    return ImVec2((float)w, (float)h);
}

static void ImGui_ImplSDL2_SetWindowSize(ImGuiViewport* viewport, ImVec2 size)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    SDL_SetWindowSize(data->Window, (int)size.x, (int)size.y);
}

static void ImGui_ImplSDL2_SetWindowTitle(ImGuiViewport* viewport, const char* title)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    SDL_SetWindowTitle(data->Window, title);
}

#if SDL_HAS_WINDOW_ALPHA
static void ImGui_ImplSDL2_SetWindowAlpha(ImGuiViewport* viewport, float alpha)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    SDL_SetWindowOpacity(data->Window, alpha);
}
#endif

static void ImGui_ImplSDL2_SetWindowFocus(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    SDL_RaiseWindow(data->Window);
}

static bool ImGui_ImplSDL2_GetWindowFocus(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    return (SDL_GetWindowFlags(data->Window) & SDL_WINDOW_INPUT_FOCUS) != 0;
}

static bool ImGui_ImplSDL2_GetWindowMinimized(ImGuiViewport* viewport)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    return (SDL_GetWindowFlags(data->Window) & SDL_WINDOW_MINIMIZED) != 0;
}

static void ImGui_ImplSDL2_RenderWindow(ImGuiViewport* viewport, void*)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    if (data->GLContext)
        SDL_GL_MakeCurrent(data->Window, data->GLContext);
}

static void ImGui_ImplSDL2_SwapBuffers(ImGuiViewport* viewport, void*)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    if (data->GLContext)
    {
        SDL_GL_MakeCurrent(data->Window, data->GLContext);
        SDL_GL_SwapWindow(data->Window);
    }
}

// Vulkan support (the Vulkan renderer needs to call a platform-side support function to create the surface)
// SDL is graceful enough to _not_ need <vulkan/vulkan.h> so we can safely include this.
#if SDL_HAS_VULKAN
#include <SDL_vulkan.h>
static int ImGui_ImplSDL2_CreateVkSurface(ImGuiViewport* viewport, ImU64 vk_instance, const void* vk_allocator, ImU64* out_vk_surface)
{
    ImGuiViewportDataSDL2* data = (ImGuiViewportDataSDL2*)viewport->PlatformUserData;
    (void)vk_allocator;
    SDL_bool ret = SDL_Vulkan_CreateSurface(data->Window, (VkInstance)vk_instance, (VkSurfaceKHR*)out_vk_surface);
    return ret ? 0 : 1; // ret ? VK_SUCCESS : VK_NOT_READY
}
#endif // SDL_HAS_VULKAN

void ImGui_ImplSDL2_InitPlatformInterface(WIN_HWND window)
{
    // Register platform interface (will be coupled with a renderer interface)
    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Platform_CreateWindow = ImGui_ImplSDL2_CreateWindow;
    platform_io.Platform_DestroyWindow = ImGui_ImplSDL2_DestroyWindow;
    platform_io.Platform_ShowWindow = ImGui_ImplSDL2_ShowWindow;
    platform_io.Platform_SetWindowPos = ImGui_ImplSDL2_SetWindowPos;
    platform_io.Platform_GetWindowPos = ImGui_ImplSDL2_GetWindowPos;
    platform_io.Platform_SetWindowSize = ImGui_ImplSDL2_SetWindowSize;
    platform_io.Platform_GetWindowSize = ImGui_ImplSDL2_GetWindowSize;
    platform_io.Platform_SetWindowFocus = ImGui_ImplSDL2_SetWindowFocus;
    platform_io.Platform_GetWindowFocus = ImGui_ImplSDL2_GetWindowFocus;
    platform_io.Platform_GetWindowMinimized = ImGui_ImplSDL2_GetWindowMinimized;
    platform_io.Platform_SetWindowTitle = ImGui_ImplSDL2_SetWindowTitle;
    platform_io.Platform_RenderWindow = ImGui_ImplSDL2_RenderWindow;
    platform_io.Platform_SwapBuffers = ImGui_ImplSDL2_SwapBuffers;
#if SDL_HAS_WINDOW_ALPHA
    platform_io.Platform_SetWindowAlpha = ImGui_ImplSDL2_SetWindowAlpha;
#endif
#if SDL_HAS_VULKAN
    platform_io.Platform_CreateVkSurface = ImGui_ImplSDL2_CreateVkSurface;
#endif

    // SDL2 by default doesn't pass mouse clicks to the application when the click focused a window. This is getting in the way of our interactions and we disable that behavior.
#if SDL_HAS_MOUSE_FOCUS_CLICKTHROUGH
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
#endif

    // Register main window handle (which is owned by the main application, not by us)
    // This is mostly for simplicity and consistency, so that our code (e.g. mouse handling etc.) can use same logic for main and secondary viewports.
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGuiViewportDataSDL2* data = IM_NEW(ImGuiViewportDataSDL2)();
    data->Window = (SDL_Window*)window;
    data->WindowID = SDL_GetWindowID((SDL_Window*)window);
    data->WindowOwned = false;
    //data->GLContext = sdl_gl_context;
    main_viewport->PlatformUserData = data;
    main_viewport->PlatformHandle = data->Window;
}

