#include <BlackBox/GUI.hpp>
#include <imgui.h>

GameGUI::GameGUI()
{

}

GameGUI::~GameGUI()
{

}

void GameGUI::Draw()
{
  static bool
      show_player=1,
      show_camera=1,
      show_demo=0,
      edit_player = 1,
      lighting = 0;
  bool open = true;
  ImGuiWindowFlags window_flags = 0;
  //window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
  //window_flags |= ImGuiWindowFlags_NoCollapse;
  //window_flags |= ImGuiWindowFlags_MenuBar;
  // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
  //ImGui::SetNextWindowPos(ImVec2(0, 0));
  //ImGui::SetNextWindowSize(game->cp_size);
  auto font = ImGui::GetFont();
  font->Scale = 1.5;
  ImGui::PushFont(font);
  ImGui::Begin("Control panel", &open);//, &open, window_flags);
  ImVec2 size = ImGui::GetWindowSize();
    game->m_Window->viewPort.left = size.x;
    game->m_Window->viewPort.width = game->m_Window->m_Width - size.x;
    ImGui::Checkbox("Show Plyer", &show_player);
    ImGui::Checkbox("Show Camera", &show_camera);
    ImGui::Checkbox("Show Demo", &show_demo);
    ImGui::Checkbox("Edit player", &edit_player);
    if (ImGui::Checkbox("Light", &lighting))
    {
      game->m_scene->lighting = true;
    }
    if (edit_player)
    {
      if (ImGui::TreeNode("Object Inspector"))
      {
        char path[260] = "";
        if (ImGui::InputText("Object path", path, sizeof(path)) && path[0] != '\0')
        {
          game->m_scene->load(path);
        }
        if (ImGui::Button("Load Objects"))
        {
          game->m_scene->load(path);
        }
        if (ImGui::Button("Save Objects"))
        {
          game->m_scene->save();
        }
        ImGui::InputScalar("World gravity",   ImGuiDataType_Float,  &game->m_World->gravity);
        for (auto &obj : game->m_scene->m_Objs)
          objectInfo(obj.second, obj.first);
       ImGui::TreePop();
      }
    }
    ImGui::Text("Input");
    ImGui::InputScalar("Mouse sensivity",   ImGuiDataType_Float,  &game->m_camera1->MouseSensitivity);
    ImGui::InputScalar("MovementSpeed",   ImGuiDataType_Float,  &game->m_camera1->MovementSpeed);
    if (show_player)
      musiListController();
    if (show_demo)
    {
      ImGui::ShowDemoWindow();
    }

    // Exit
    if (ImGui::Button("Exit"))
    {
      game->m_running = false;
    }
    ImGuiIO &igio = ImGui::GetIO();
    
    ImGui::Text("delta.x = %d, delta.y = %d",game->m_player->delta.x, game->m_player->delta.x);//,igio.MousePos.x,igio.MousePos.y);
  ImGui::End();
  ImGui::PopFont();


  if (show_camera) {

  }
}

void GameGUI::musiListController()
{
  if (ImGui::TreeNode("Music List")){
    if (ImGui::Button("Pause"))
    {
      game->m_PlayList.pause();
    }
    if (ImGui::Button("Play"))
    {
      game->m_PlayList.play();
    }
    if (ImGui::Button("Next"))
    {
      game->m_PlayList.next();
    }
    if (ImGui::Button("Previos"))
    {
      game->m_PlayList.play();
    }
    if (ImGui::Button("Stop"))
    {
      game->m_PlayList.stop();
    }
    ImGui::TreePop();
  }
}

void GameGUI::cameraController()
{

  if (ImGui::TreeNode("Camera")){

    if (ImGui::Button("Reset"))
    {
      //m_active_camera->reset();
    }
    ImGui::TreePop();
  }
}

void GameGUI::objectInfo(Object *obj, std::string name)
{
    const ImS32   s32_zero = 0,   s32_one = 1,   s32_fifty = 50, s32_min = INT_MIN/2,   s32_max = INT_MAX/2,    s32_hi_a = INT_MAX/2 - 100,    s32_hi_b = INT_MAX/2;
    const ImU32   u32_zero = 0,   u32_one = 1,   u32_fifty = 50, u32_min = 0,           u32_max = UINT_MAX/2,   u32_hi_a = UINT_MAX/2 - 100,   u32_hi_b = UINT_MAX/2;
    const ImS64   s64_zero = 0,   s64_one = 1,   s64_fifty = 50, s64_min = LLONG_MIN/2, s64_max = LLONG_MAX/2,  s64_hi_a = LLONG_MAX/2 - 100,  s64_hi_b = LLONG_MAX/2;
    const ImU64   u64_zero = 0,   u64_one = 1,   u64_fifty = 50, u64_min = 0,           u64_max = ULLONG_MAX/2, u64_hi_a = ULLONG_MAX/2 - 100, u64_hi_b = ULLONG_MAX/2;
    const float   f32_zero = 0.f, f32_one = 1.f, f32_lo_a = -10000000000.0f, f32_hi_a = +10000000000.0f;
    const double  f64_zero = 0.,  f64_one = 1.,  f64_lo_a = -1000000000000000.0, f64_hi_a = +1000000000000000.0;

    // State

    static bool inputs_step = true;
  if (ImGui::TreeNode(name.c_str())) {
    if (ImGui::TreeNode("Physics")) {

      ImGui::InputScalar("Friction",   ImGuiDataType_Float,  &obj->friction, inputs_step ? &f32_one : NULL);

      if (ImGui::TreeNode("Veolcity")) {
        ImGui::InputScalar("Veolcity.X",   ImGuiDataType_Float,  &obj->velocity.x, inputs_step ? &f32_one : NULL);
        ImGui::InputScalar("Veolcity.Y",   ImGuiDataType_Float,  &obj->velocity.y, inputs_step ? &f32_one : NULL);
        ImGui::InputScalar("Veolcity.Z",   ImGuiDataType_Float,  &obj->velocity.z, inputs_step ? &f32_one : NULL);
        ImGui::TreePop();
      }

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Transform"))
    {
      if (ImGui::TreeNode("Position"))
      {
        ImGui::InputScalar("X",   ImGuiDataType_Float,  &obj->m_transform.position.x, inputs_step ? &f32_one : NULL);
        ImGui::InputScalar("Y",   ImGuiDataType_Float,  &obj->m_transform.position.y, inputs_step ? &f32_one : NULL);
        ImGui::InputScalar("Z",   ImGuiDataType_Float,  &obj->m_transform.position.z, inputs_step ? &f32_one : NULL);
        ImGui::TreePop();
      }
      if (ImGui::TreeNode("Rotation"))
      {
        ImGui::InputScalar("X",   ImGuiDataType_Float,  &obj->m_transform.rotation.x, inputs_step ? &f32_one : nullptr);
        ImGui::InputScalar("Y",   ImGuiDataType_Float,  &obj->m_transform.rotation.y, inputs_step ? &f32_one : nullptr);
        ImGui::InputScalar("Z",   ImGuiDataType_Float,  &obj->m_transform.rotation.z, inputs_step ? &f32_one : nullptr);
        ImGui::TreePop();
        //obj->m_transform.rotation = glm::radians(obj->m_transform.rotation);
      }
      if (ImGui::TreeNode("Scale"))
      {
        ImGui::InputScalar("X",   ImGuiDataType_Float,  &obj->m_transform.scale.x, inputs_step ? &f32_one : NULL);
        ImGui::InputScalar("Y",   ImGuiDataType_Float,  &obj->m_transform.scale.y, inputs_step ? &f32_one : NULL);
        ImGui::InputScalar("Z",   ImGuiDataType_Float,  &obj->m_transform.scale.z, inputs_step ? &f32_one : NULL);
        ImGui::TreePop();
      }
      char materialName[64];
      ImGui::InputText("Material", (char*)obj->m_Material->name->data(), obj->m_Material->name->size(),ImGuiInputTextFlags_ReadOnly);
      ImGui::TreePop();
    }

    if (ImGui::Button("Clone"))
    {
      char buff[10];
      Object *object = new Object(*game->m_scene->getObject(name));
      sprintf(buff, "%s_%0d", name.c_str(), object->refs);
      game->m_scene->addObject(buff, object);
    }
    if (ImGui::Button("Reset"))
    {
      obj->m_transform.position = glm::vec3(0.0);
      obj->velocity = glm::vec3(0.0);
    }

    ImGui::TreePop();
  }
}

