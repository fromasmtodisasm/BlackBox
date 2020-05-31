#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/System/ILog.hpp>

#include "MainWindow.hpp"
#include "imgui.h"


MainWindow::MainWindow()
{
}

void MainWindow::Draw()
{
}

static void MixedItems()
{
	ImGui::Columns(2, "mixed");
	//ImGui::Separator();

	
	auto w = ImGui::GetContentRegionAvailWidth();
		ImGui::Image(reinterpret_cast<ImTextureID>(0), ImVec2(w,500), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::IsItemClicked())
		{
			gEnv->pLog->Log("View clicked");
		}
	ImGui::NextColumn();

	
	#if 0
	ImGui::Text("Sailor");
	ImGui::Button("Corniflower");
	static float bar = 1.0f;
	ImGui::InputFloat("blue", &bar, 0.05f, 0, "%.3f");
	ImGui::NextColumn();
	#endif

	//if (ImGui::CollapsingHeader("Category A")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
	//if (ImGui::CollapsingHeader("Category B")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
	//if (ImGui::CollapsingHeader("Category C")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
	ImGui::Columns(1);
	ImGui::Separator();
	//ImGui::TreePop();

}
static void Groups()
{
    //if (ImGui::TreeNode("Groups"))
    {
        //HelpMarker("BeginGroup() basically locks the horizontal position for new line. EndGroup() bundles the whole group so that you can use \"item\" functions such as IsItemHovered()/IsItemActive() or SameLine() etc. on the whole group.");
        ImGui::BeginGroup();
        {
            ImGui::BeginGroup();
							ImGui::Text("Renderer");
							ImGui::Text("Screen Resolution");
							ImGui::Text("Renderer");
            ImGui::EndGroup();
            ImGui::SameLine();
            ImGui::BeginGroup();
			static float t;
				ImGui::InputFloat("rend", &t, 1, 1, 1);
							ImGui::Text("Renderer");
							ImGui::Text("Renderer");
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("First group hovered");
            ImGui::EndGroup();
            ImGui::EndGroup();

						ImGui::BeginChild("lsdkfj");
            ImGui::Columns(3, NULL, false);
            static bool selected[16] = {};
            for (int i = 0; i < 16; i++)
            {
                char label[32]; sprintf(label, "Item %d", i);
                if (ImGui::Selectable(label, &selected[i])) {}
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
						ImGui::EndChild();
							return;
        }
        // Capture the group size and create widgets using the same size
        ImVec2 size = ImGui::GetItemRectSize();
        const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
        ImGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

        ImGui::Button("ACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f, size.y));
        ImGui::SameLine();
        ImGui::Button("REACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f, size.y));
        ImGui::EndGroup();
        ImGui::SameLine();

        ImGui::Button("LEVERAGE\nBUZZWORD", size);
        ImGui::SameLine();

        if (ImGui::ListBoxHeader("List", size))
        {
            ImGui::Selectable("Selected", true);
            ImGui::Selectable("Not Selected", false);
            ImGui::ListBoxFooter();
        }

        //ImGui::TreePop();
    }


}

bool MainWindow::Update()
{
	ImGui::ShowDemoWindow();
	ImGui::Begin("View",(bool*)true);
	{
		ImGui::Columns(2);
		LeftCollumn();
		ImGui::NextColumn();
		RightColumn();
	}
	ImGui::Columns(1);
	ImGui::Separator();
	//MixedItems();
	if (ImGui::Button("Quit"))
	{
		is_done = true;
	}

	ImGui::End();
	return true;
}

void MainWindow::LeftCollumn()
{
	//auto w = ImGui::GetColumnWidth();
	auto w = ImGui::GetContentRegionAvailWidth();
		ImGui::Image(reinterpret_cast<ImTextureID>(0), ImVec2(w,500), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::IsItemClicked())
		{
			gEnv->pLog->Log("View clicked");
		}

}

void MainWindow::RightColumn()
{
	if (ImGui::BeginTabBar("TabBar"))
	{
		if (ImGui::BeginTabItem("GameOptions"))
			ImGui::EndTabItem();
		if (ImGui::BeginTabItem("Video Options"))
		{
			//Groups();
			ImGui::BeginChild("lsdjf");
        ImGui::Columns(3, "mixed");
        ImGui::Separator();

        ImGui::Text("Hello");
        ImGui::Button("Banana");
        ImGui::NextColumn();

        ImGui::Text("ImGui");
        ImGui::Button("Apple");
        static float foo = 1.0f;
        ImGui::InputFloat("red", &foo, 0.05f, 0, "%.3f");
        ImGui::Text("An extra line here.");
        ImGui::NextColumn();

        ImGui::Text("Sailor");
        ImGui::Button("Corniflower");
        static float bar = 1.0f;
        ImGui::InputFloat("blue", &bar, 0.05f, 0, "%.3f");
        ImGui::NextColumn();

        if (ImGui::CollapsingHeader("Category A")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
        if (ImGui::CollapsingHeader("Category B")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
        if (ImGui::CollapsingHeader("Category C")) { ImGui::Text("Blah blah blah"); } ImGui::NextColumn();
        ImGui::Columns(1);
        ImGui::Separator();
		ImGui::EndChild();
			ImGui::EndTabItem();
		
		}
		if (ImGui::BeginTabItem("Video options (advanced)"))
			ImGui::EndTabItem();
		ImGui::EndTabBar();
	}
}

extern "C" int AppMain(int c, char** v)
{
	MainWindow mainWindow;
	while (!mainWindow.is_done)
	{
		gEnv->pSystem->Update(0, false);
		gEnv->pSystem->RenderBegin();
		//gEnv->pSystem->Render();
		mainWindow.Update();

		gEnv->pSystem->RenderEnd();
	}
	return 0;
}
