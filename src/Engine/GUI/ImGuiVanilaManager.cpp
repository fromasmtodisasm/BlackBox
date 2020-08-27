#ifdef IMGUI_VANILA_MANAGER
#include <SDL.h>
#include <BlackBox/Input/Events.h>

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "ImGuiBackend/Vanila/imgui_impl_opengl3.cpp"
#include "ImGuiBackend/Vanila/imgui_impl_sdl.cpp"

static uint KeyboardEvent = 0;

class ImGuiManager : public IImGuiManager, public ISystemEventListener
{
  public:
	
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
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// Enable Docking
		// FIXME: rewrite platform window handling
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;}

		KeyboardEvent = gEnv->pSystem->GetISystemEventDispatcher()->RegisterEvent(InputNewFrame);
		gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "ImGuiManager");
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
		// Initialize OpenGL loader
		#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
			bool err = gl3wInit() != 0;
		#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
			bool err = glewInit() != GLEW_OK;
		#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
			bool err = gladLoadGL() == 0;
		#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
			bool err = gladLoadGL((GLADloadfunc)SDL_GL_GetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
		#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
			bool err = false;
			glbinding::Binding::initialize();
		#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
			bool err = false;
			glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)SDL_GL_GetProcAddress(name); });
		#else
			bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
		#endif
		if (err)
		{
			fprintf(stderr, "Failed to initialize OpenGL loader!\n");
			return 1;
		}
	   // Setup Platform/Renderer bindings
		ImGui_ImplSDL2_InitForOpenGL((SDL_Window*)gEnv->pRenderer->GetCurrentContextHWND(), SDL_GL_GetCurrentContext());
		ImGui_ImplOpenGL3_Init("#version 130");
		return true;
	}
	void NewFrame() override
	{
		// Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame((SDL_Window*)gEnv->pRenderer->GetCurrentContextHWND());
        ImGui::NewFrame();
	}
	void Render() override
	{
		auto io = ImGui::GetIO();
		// Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
