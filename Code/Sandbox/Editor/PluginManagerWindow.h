#pragma once
#include <imgui.h>

namespace PluginManagerWindow
{
	void Draw()
	{
		auto plugins = GetIEditorImpl()->GetPluginManager()->GetPluginList();	
		ImGui::Begin("Plugin Manager");
		for (auto& p : plugins)
		{
			ImGui::Text(p.pPlugin->GetPluginName());
			//p.pPlugin->GetPluginName();
		}
		ImGui::End();
	}
};