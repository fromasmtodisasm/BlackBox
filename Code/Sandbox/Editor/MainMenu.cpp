#include "MainMenu.hpp"
#include <imgui.h>

void MainMenu::Draw()
{
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
			
        ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}
