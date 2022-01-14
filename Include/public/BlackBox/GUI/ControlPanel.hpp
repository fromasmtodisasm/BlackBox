#pragma once
#include <functional>
#include <set>

class ControlPanel
{
public:
	static void AddEntry(std::function<void()> f)
	{
		#ifdef USE_GUI
		ImGui::Begin("ControlPanel");	
		f();
		ImGui::End();
		#endif
	}
	
	std::set<uint> buttons;
	uint last_button = 0;
};