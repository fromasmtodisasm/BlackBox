#pragma once
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <imgui.h>

class ImGuiManager : public IInputEventListener
{
  public:
	ImGuiManager(ISystem* pSystem);
	~ImGuiManager();
	// Inherited via IInputEventListener
	virtual bool OnInputEvent(const SInputEvent& event) override;
	virtual bool OnInputEventUI(const SUnicodeEvent& event) override;
	virtual int GetPriority() const
	{
		return 3;
	}

	bool Init();
	void NewFrame();
	void Render();
	void AddDemoWindow();

  private:
	bool show_demo_window = true;
};
