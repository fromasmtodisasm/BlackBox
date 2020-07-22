#pragma once
#include <functional>
#include <set>

class ControlPanel
{
public:
	static void AddEntry(std::function<void()> f)
	{
		ImGui::Begin("ControlPanel");	
		f();
		ImGui::End();
	}
	
	std::set<uint> buttons;
	uint last_button = 0;
};