#pragma once
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Input/Events.h>
#include <BlackBox/System/ConsoleRegistration.h>

#include "ImGuiManager.hpp"
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
	
	ImGuiManager(ISystem* pSystem);

	virtual void Release() override;

	~ImGuiManager();
	// Inherited via IInputEventListener
	virtual bool OnInputEvent(const SInputEvent& event) override;
	virtual bool OnInputEventUI(const SUnicodeEvent& event) override;

	bool Init() override;
	void NewFrame() override;
	void Render() override;
	void ShowDemoWindow() override;
	void ShowNodeEditor() override;
	void HideDemoWindow() override;

  private:
	bool show_demo_window = true;
	#if 0
    gui::Console console;
	#endif

	// Inherited via ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
};
