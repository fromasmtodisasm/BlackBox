#include <BlackBox/GUI/ImGuiManager.hpp>

#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Input/IHardwareMouse.hpp>

#include <BlackBox/Renderer/OpenGL/Core.hpp>

//#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM <BlackBox/Renderer/OpenGL/Core.hpp>
#include "imgui_impl_opengl3.h"

namespace
{
  class ImGuiInput
  {
  public:

    void NewFrame()
    {
      ImGuiIO& io = ImGui::GetIO();
      IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer back-end. Missing call to renderer _NewFrame() function? e.g. ImGui_ImplOpenGL3_NewFrame().");

      // Setup display size (every frame to accommodate for window resizing)
      int w, h;
      int display_w, display_h;
      auto r = GetISystem()->GetIRender();
      // TODO: rewrite it
      //SDL_GetWindowSize(window, &w, &h);
      w = display_w = r->GetWidth();
      h = display_h = r->GetHeight();
      io.DisplaySize = ImVec2((float)w, (float)h);
      if (w > 0 && h > 0)
        io.DisplayFramebufferScale = ImVec2((float)display_w / w, (float)display_h / h);

      // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
      // TODO: rewrite it
      static auto frequency = 0;// SDL_GetPerformanceFrequency();
      auto current_time = 0;// SDL_GetPerformanceCounter();
      io.DeltaTime = m_Time > 0 ? (float)((double)(current_time - m_Time) / frequency) : (float)(1.0f / 60.0f);
      m_Time = current_time;

      UpdateMousePosAndButtons();
      //ImGui_ImplSDL2_UpdateMouseCursor();

      // Update game controllers (if enabled and available)
      UpdateGamepads();
    }
    bool Init()
    {

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
      /*
      m_MouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
      m_MouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
      m_MouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
      m_MouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
      m_MouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
      m_MouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
      m_MouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
      m_MouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
      m_MouseCursors[ImGuiMouseCursor_NotAllowed] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
      */

      // Check and store if we are on Wayland
      //g_MouseCanUseGlobalState = strncmp(SDL_GetCurrentVideoDriver(), "wayland", 7) != 0;
      //g_MouseCanUseGlobalState = false;// strncmp(SDL_GetCurrentVideoDriver(), "wayland", 7) != 0;

#if 0
#ifdef _WIN32
      SDL_SysWMinfo wmInfo;
      SDL_VERSION(&wmInfo.version);
      SDL_GetWindowWMInfo(window, &wmInfo);
      io.ImeWindowHandle = wmInfo.info.win.window;
#else
      (void)window;
#endif
#endif

      return true;
    }
    void UpdateGamepads()
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
    void UpdateMousePosAndButtons()
    {
      ImGuiIO& io = ImGui::GetIO();


      // Set OS mouse position if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
      if (io.WantSetMousePos)
        gEnv->pHardwareMouse->SetHardwareMousePosition((int)io.MousePos.x, (int)io.MousePos.y);
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
        gEnv->pHardwareMouse->GetHardwareMousePosition(&m_MousePose.x, &m_MousePose.y);
      io.MousePos = m_MousePose;
#endif
    }

    bool OnInputEvent(const SInputEvent& event, ImGuiManager &imguiManager)
    {
      ImGuiIO& io = ImGui::GetIO();

      bool mouseMoution = event.state == eIS_Changed;
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
        case eKI_Escape: GetISystem()->GetIInput()->RemoveEventListener(&imguiManager);
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
        return true;
      }
      return false;

    }
    
    bool OnInputEventUI(const SUnicodeEvent& event)
    {
      ImGuiIO& io = ImGui::GetIO();
      io.AddInputCharactersUTF8(reinterpret_cast<const char*>(&event.inputChar));
      return true;

    }
public:
    IInput* m_pInput = nullptr;
    bool m_MousePressed[3] = { false, false, false };
    uint64_t m_Time = 0;
    ImVec2 m_MousePose;
  };

  static ImGuiInput input;
  static ImGuiRender render;
}
void ShowExampleAppCustomNodeGraph(bool* opened);

ImGuiManager::ImGuiManager(ISystem* pSystem)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;}
}

ImGuiManager::~ImGuiManager()
{
  render.Shutdown();
}

bool ImGuiManager::OnInputEvent(const SInputEvent& event)
{
  return input.OnInputEvent(event, *this);
}

bool ImGuiManager::OnInputEventUI(const SUnicodeEvent& event)
{
  return input.OnInputEventUI(event);
}

bool ImGuiManager::Init()
{
  // Setup Dear ImGui style
  bool result = true;
  ImGui::StyleColorsLight();
  result &= input.Init();
  result &= render.Init(GetISystem()->GetIRender());
  return result;
}

void ImGuiManager::NewFrame()
{
  render.NewFrame();
  input.NewFrame();
  ImGui::NewFrame();
}

void ImGuiManager::Render()
{
  // Rendering
  auto& io = ImGui::GetIO();
  ImGui::Render();
  gl::ViewPort(0, 0, io.DisplaySize.x, io.DisplaySize.y);
  render.RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::ShowDemoWindow()
{
  // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
  if (show_demo_window)
    ImGui::ShowDemoWindow(&show_demo_window);
}

void ImGuiManager::ShowNodeEditor()
{
	bool t = true;
	ShowExampleAppCustomNodeGraph(&t);
}

void ImGuiManager::HideDemoWindow()
{
}

// Creating a node graph editor for ImGui
// Quick demo, not production code! This is more of a demo of how to use ImGui to create custom stuff.
// Better version by @daniel_collin here https://gist.github.com/emoon/b8ff4b4ce4f1b43e79f2
// See https://github.com/ocornut/imgui/issues/306
// v0.03: fixed grid offset issue, inverted sign of 'scrolling'
// Animated gif: https://cloud.githubusercontent.com/assets/8225057/9472357/c0263c04-4b4c-11e5-9fdf-2cd4f33f6582.gif

#include <math.h> // fmodf

// NB: You can use math functions/operators on ImVec2 if you #define IMGUI_DEFINE_MATH_OPERATORS and #include "imgui_internal.h"
// Here we only declare simple +/- operators so others don't leak into the demo code.
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y);
}
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs)
{
	return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y);
}

// Really dumb data structure provided for the example.
// Note that we storing links are INDICES (not ID) to make example code shorter, obviously a bad idea for any general purpose code.
static void ShowExampleAppCustomNodeGraph(bool* opened)
{
	ImGui::SetNextWindowSize(ImVec2(700, 600));
	if (!ImGui::Begin("Example: Custom Node Graph", opened))
	{
		ImGui::End();
		return;
	}

	// Dummy
	struct Node
	{
		int ID;
		char Name[32];
		ImVec2 Pos, Size;
		float Value;
		ImVec4 Color;
		int InputsCount, OutputsCount;

		Node(int id, const char* name, const ImVec2& pos, float value, const ImVec4& color, int inputs_count, int outputs_count)
		{
			ID = id;
			strncpy(Name, name, 31);
			Name[31]	 = 0;
			Pos			 = pos;
			Value		 = value;
			Color		 = color;
			InputsCount	 = inputs_count;
			OutputsCount = outputs_count;
		}

		ImVec2 GetInputSlotPos(int slot_no) const
		{
			return ImVec2(Pos.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)InputsCount + 1));
		}
		ImVec2 GetOutputSlotPos(int slot_no) const
		{
			return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float)slot_no + 1) / ((float)OutputsCount + 1));
		}
	};
	struct NodeLink
	{
		int InputIdx, InputSlot, OutputIdx, OutputSlot;

		NodeLink(int input_idx, int input_slot, int output_idx, int output_slot)
		{
			InputIdx   = input_idx;
			InputSlot  = input_slot;
			OutputIdx  = output_idx;
			OutputSlot = output_slot;
		}
	};

	static ImVector<Node> nodes;
	static ImVector<NodeLink> links;
	static bool inited		 = false;
	static ImVec2 scrolling	 = ImVec2(0.0f, 0.0f);
	static bool show_grid	 = true;
	static int node_selected = -1;
	if (!inited)
	{
		nodes.push_back(Node(0, "MainTex", ImVec2(40, 50), 0.5f, ImColor(255, 100, 100), 1, 1));
		nodes.push_back(Node(1, "BumpMap", ImVec2(40, 150), 0.42f, ImColor(200, 100, 200), 1, 1));
		nodes.push_back(Node(2, "Combine", ImVec2(270, 80), 1.0f, ImColor(0, 200, 100), 2, 2));
		links.push_back(NodeLink(0, 0, 2, 0));
		links.push_back(NodeLink(1, 0, 2, 1));
		inited = true;
	}

	// Draw a list of nodes on the left side
	bool open_context_menu	  = false;
	int node_hovered_in_list  = -1;
	int node_hovered_in_scene = -1;
	ImGui::BeginChild("node_list", ImVec2(100, 0));
	ImGui::Text("Nodes");
	ImGui::Separator();
	for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
	{
		Node* node = &nodes[node_idx];
		ImGui::PushID(node->ID);
		if (ImGui::Selectable(node->Name, node->ID == node_selected))
			node_selected = node->ID;
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_list = node->ID;
			open_context_menu |= ImGui::IsMouseClicked(1);
		}
		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginGroup();

	const float NODE_SLOT_RADIUS = 4.0f;
	const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

	// Create our child canvas
	ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
	ImGui::SameLine(ImGui::GetWindowWidth() - 100);
	ImGui::Checkbox("Show grid", &show_grid);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(60, 60, 70, 200));
	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);

	ImVec2 offset		  = ImGui::GetCursorScreenPos() + scrolling;
	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	// Display grid
	if (show_grid)
	{
		ImU32 GRID_COLOR = IM_COL32(200, 200, 200, 40);
		float GRID_SZ	 = 64.0f;
		ImVec2 win_pos	 = ImGui::GetCursorScreenPos();
		ImVec2 canvas_sz = ImGui::GetWindowSize();
		for (float x = fmodf(scrolling.x, GRID_SZ); x < canvas_sz.x; x += GRID_SZ)
			draw_list->AddLine(ImVec2(x, 0.0f) + win_pos, ImVec2(x, canvas_sz.y) + win_pos, GRID_COLOR);
		for (float y = fmodf(scrolling.y, GRID_SZ); y < canvas_sz.y; y += GRID_SZ)
			draw_list->AddLine(ImVec2(0.0f, y) + win_pos, ImVec2(canvas_sz.x, y) + win_pos, GRID_COLOR);
	}

	// Display links
	draw_list->ChannelsSplit(2);
	draw_list->ChannelsSetCurrent(0); // Background
	for (int link_idx = 0; link_idx < links.Size; link_idx++)
	{
		NodeLink* link = &links[link_idx];
		Node* node_inp = &nodes[link->InputIdx];
		Node* node_out = &nodes[link->OutputIdx];
		ImVec2 p1	   = offset + node_inp->GetOutputSlotPos(link->InputSlot);
		ImVec2 p2	   = offset + node_out->GetInputSlotPos(link->OutputSlot);
		draw_list->AddBezierCurve(p1, p1 + ImVec2(+50, 0), p2 + ImVec2(-50, 0), p2, IM_COL32(200, 200, 100, 255), 3.0f);
	}

	// Display nodes
	for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
	{
		Node* node = &nodes[node_idx];
		ImGui::PushID(node->ID);
		ImVec2 node_rect_min = offset + node->Pos;

		// Display node contents first
		draw_list->ChannelsSetCurrent(1); // Foreground
		bool old_any_active = ImGui::IsAnyItemActive();
		ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
		ImGui::BeginGroup(); // Lock horizontal position
		ImGui::Text("%s", node->Name);
		ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
		ImGui::ColorEdit3("##color", &node->Color.x);
		ImGui::EndGroup();

		// Save the size of what we have emitted and whether any of the widgets are being used
		bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
		node->Size				 = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
		ImVec2 node_rect_max	 = node_rect_min + node->Size;

		// Display node box
		draw_list->ChannelsSetCurrent(0); // Background
		ImGui::SetCursorScreenPos(node_rect_min);
		ImGui::InvisibleButton("node", node->Size);
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_scene = node->ID;
			open_context_menu |= ImGui::IsMouseClicked(1);
		}
		bool node_moving_active = ImGui::IsItemActive();
		if (node_widgets_active || node_moving_active)
			node_selected = node->ID;
		if (node_moving_active && ImGui::IsMouseDragging(0))
			node->Pos = node->Pos + ImGui::GetIO().MouseDelta;

		ImU32 node_bg_color = (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_selected == node->ID)) ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);
		draw_list->AddRectFilled(node_rect_min, node_rect_max, node_bg_color, 4.0f);
		draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
		for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++)
			draw_list->AddCircleFilled(offset + node->GetInputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
		for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++)
			draw_list->AddCircleFilled(offset + node->GetOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));

		ImGui::PopID();
	}
	draw_list->ChannelsMerge();

	// Open context menu
	if (!ImGui::IsAnyItemHovered() && ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))
	{
		node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
		open_context_menu											 = true;
	}
	if (open_context_menu)
	{
		ImGui::OpenPopup("context_menu");
		if (node_hovered_in_list != -1)
			node_selected = node_hovered_in_list;
		if (node_hovered_in_scene != -1)
			node_selected = node_hovered_in_scene;
	}

	// Draw context menu
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("context_menu"))
	{
		Node* node		 = node_selected != -1 ? &nodes[node_selected] : NULL;
		ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
		if (node)
		{
			ImGui::Text("Node '%s'", node->Name);
			ImGui::Separator();
			if (ImGui::MenuItem("Rename..", NULL, false, false))
			{
			}
			if (ImGui::MenuItem("Delete", NULL, false, false))
			{
			}
			if (ImGui::MenuItem("Copy", NULL, false, false))
			{
			}
		}
		else
		{
			if (ImGui::MenuItem("Add"))
			{
				nodes.push_back(Node(nodes.Size, "New node", scene_pos, 0.5f, ImColor(100, 100, 200), 2, 2));
			}
			if (ImGui::MenuItem("Paste", NULL, false, false))
			{
			}
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();

	// Scrolling
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
		scrolling = scrolling + ImGui::GetIO().MouseDelta;

	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::EndGroup();

	ImGui::End();
}
