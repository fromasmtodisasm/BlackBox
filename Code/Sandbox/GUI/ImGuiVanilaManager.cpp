#include "ImGuiVanilaManager.h"

#include <SDL.h>
#include "ImGuiBackend/Vanila/imgui_impl_dx11.cpp"
#include "imgui_impl_sdl.cpp"

static uint KeyboardEvent = 0;


ImGuiManager::ImGuiManager(ISystem* pSystem)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// FIXME: rewrite platform window handling
	io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	// FIXME: rewrite platform window handling
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	KeyboardEvent = gEnv->pSystem->GetISystemEventDispatcher()->RegisterEvent(InputNewFrame);
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "ImGuiManager");

	CVars::Init();
}

void ImGuiManager::Release() { 
	delete this; 
}

void ImGuiManager::Render()
{
	auto io = ImGui::GetIO();
	// Rendering
	ImGui::Render();
#if 0
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
#endif
	int x, y, z, w;
	gEnv->pRenderer->GetViewport(&x, &y, &z, &w);
	gEnv->pRenderer->SetViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	gEnv->pRenderer->SetViewport(x, y, z, w);

	// Update and Render additional Platform Windows
	// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
	//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ImGuiManager::NewFrame()
{
	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplSDL2_NewFrame((SDL_Window*)gEnv->pRenderer->GetCurrentContextHWND());
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	bool p_open{};
	bool open_console = true;
	#if 0
	console.Draw("Console", &open_console);
	#endif
}

bool ImGuiManager::Init()
{
	auto				 pDevice = (ID3D11Device*)gEnv->pRenderer->EF_Query(ERenderQueryTypes::EFQ_D3DDevice);
	ID3D11DeviceContext* pImmediateContext{};
	pDevice->GetImmediateContext(&pImmediateContext);
	auto result = ImGui_ImplDX11_Init(pDevice, pImmediateContext);
	if (result)
		result &= ImGui_ImplSDL2_InitForD3D((SDL_Window*)gEnv->pRenderer->GetCurrentContextHWND());
	return result;
}

bool ImGuiManager::OnInputEventUI(const SUnicodeEvent& event)
{
	return false;
}

// Inherited via IInputEventListener

bool ImGuiManager::OnInputEvent(const SInputEvent& event)
{
	return false;
}

ImGuiManager::~ImGuiManager()
{
	ImGui_ImplDX11_InvalidateDeviceObjects();
    ImGui_ImplDX11_Shutdown();
}

void ImGuiManager::ShowDemoWindow()
{
}

void ImGuiManager::ShowNodeEditor()
{
}

void ImGuiManager::HideDemoWindow()
{
}

// Inherited via ISystemEventListener

void ImGuiManager::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
	if (event == KeyboardEvent)
	{
		ImGui_ImplSDL2_ProcessEvent((SDL_Event*)lparam);
	}
}
