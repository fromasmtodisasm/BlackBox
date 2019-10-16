#ifdef GUI


#include "..\include\BlackBox\GUI.hpp"

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
