#if 0
#include <BlackBox/GUI/GUI.hpp>

void MenuItem::execute()
{
}

void Menu::execute()
{
  for (auto& item : items)
  {
    if (ImGui::MenuItem(item->name))
    {
      item->execute();
    }
  }
}

void MainMenu::execute()
{
  if (ImGui::BeginMainMenuBar())
  {
    //
    size = ImGui::GetWindowSize();
    for (auto& menu : menus)
    {
      if (ImGui::BeginMenu(menu->name))
      {
        menu->execute();
        ImGui::EndMenu();
      }
    }
    ImGui::EndMainMenuBar();
  }
}

#endif // GUI

#include <BlackBox/GUI/GUI.hpp>
//IMGUI_IMPL_API bool     ImGui_ImplOpenGL3_Init(const char* glsl_version = NULL);
//IMGUI_IMPL_API void     ImGui_ImplOpenGL3_NewFrame();
//IMGUI_IMPL_API void     ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data);

