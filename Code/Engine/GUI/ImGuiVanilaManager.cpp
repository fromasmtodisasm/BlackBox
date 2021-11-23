#ifdef IMGUI_VANILA_MANAGER
#include <SDL.h>
#include <BlackBox/Input/Events.h>

#include "ImGuiBackend/Vanila/imgui_impl_dx11.cpp"
#include "imgui_impl_sdl.cpp"
#include <BlackBox/System/ConsoleRegistration.h>

#include "Console.cpp"

static uint KeyboardEvent = 0;


struct CVars
{
	int gui_docking{1};

	void Init()
	{
		REGISTER_CVAR(gui_docking, gui_docking, 0, "");
	}
};

class ImGuiManager : public IImGuiManager, public ISystemEventListener, public CVars
{
  public:
	
	ImGuiManager(ISystem* pSystem)
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		// FIXME: rewrite platform window handling
		io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
		(void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
		// FIXME: rewrite platform window handling
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
		io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		KeyboardEvent = gEnv->pSystem->GetISystemEventDispatcher()->RegisterEvent(InputNewFrame);
		gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "ImGuiManager");

		CVars::Init();
        gEnv->pConsole->AddOutputPrintSink(&console);
	}


	~ImGuiManager()
	{
	
	}
	// Inherited via IInputEventListener
	virtual bool OnInputEvent(const SInputEvent& event) override
	{
		return false;	
	}
	virtual bool OnInputEventUI(const SUnicodeEvent& event) override
	{
		return false;	
	}

	bool Init() override
	{
		auto pDevice = (ID3D11Device*)gEnv->pRenderer->EF_Query(ERenderQueryTypes::EFQ_D3DDevice);
		ID3D11DeviceContext* pImmediateContext{};
		pDevice->GetImmediateContext(&pImmediateContext);
		auto result = ImGui_ImplDX11_Init(pDevice,pImmediateContext);
		if (result)
			result &= ImGui_ImplSDL2_InitForD3D((SDL_Window*)gEnv->pRenderer->GetCurrentContextHWND());
		return result;
	}
	void NewFrame() override
	{
		// Start the Dear ImGui frame
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplSDL2_NewFrame((SDL_Window*)gEnv->pRenderer->GetCurrentContextHWND());
		ImGui::NewFrame();
		bool p_open{};
		#if 1
		if (auto v = gEnv->pConsole->GetCVar("gui_docking"); v)
		{
			if (v->GetIVal())
				ShowExampleAppDockSpace(&show_demo_window);
		}
		bool open_console = true;
        console.Draw("Console", &open_console);
		#endif
	}
	void Render() override
	{
		auto io = ImGui::GetIO();
		// Rendering
        ImGui::Render();
		#if 0
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		#endif
		gEnv->pRenderer->SetViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
            SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
        }	
	}
	void ShowDemoWindow() override
	{
	
	}
	void ShowNodeEditor() override
	{
	
	}
	void HideDemoWindow() override
	{
	
	}

  private:
	bool show_demo_window = true;
    ImGuiConsole console;

	// Inherited via ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override
	{
		if (event == KeyboardEvent)
		{
			ImGui_ImplSDL2_ProcessEvent((SDL_Event*)lparam);
		}
	}
};

#endif
