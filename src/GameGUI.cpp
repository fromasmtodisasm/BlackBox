#include <BlackBox/GUI.hpp>
#include <imgui.h>
#include <BlackBox/Light.hpp>

struct FileClose : MenuItem
{
  CGame **game;
  FileClose(CGame **game) : game(game)
  {
    this->name = "Close";
  }
  virtual void execute() override
  {
    if (game != nullptr && *game != nullptr)
      (*game)->Stop();
  }
};
struct FileOpen : MenuItem
{
  CGame **game;
  FileOpen(CGame **game) : game(game)
  {
    this->name = "Open";
  }
  virtual void execute() override
  {
    if (game != nullptr && *game != nullptr)
    {
      // Execute 
    }
  }
};

struct FileMenu : public Menu
{
  CGame **game;
  FileMenu(CGame **game) : game(game)
  {
    this->name = "File";
  }
  virtual void execute() override
  {
    for (auto& item : items)
    {
      if (ImGui::MenuItem(item->name))
      {
        item->execute();
      }
    }
  }
};

GameGUI::GameGUI() : mainMenu(MainMenu())
{
  FileMenu *fileMenu = new FileMenu(&game);
  fileMenu->items.push_back(new FileOpen(&game));
  fileMenu->items.push_back(new FileClose(&game));
  mainMenu.menus.push_back(fileMenu);
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
  mainMenu.execute();
  //window_flags |= ImGuiWindowFlags_NoMove;
  window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
  //window_flags |= ImGuiWindowFlags_NoCollapse;
  //window_flags |= ImGuiWindowFlags_MenuBar;
  // We specify a default position/size in case there's no data in the .ini file. Typically this isn't required! We only do it to make the Demo applications a little more welcoming.
  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(0, mainMenu.size.y));
  ImGui::SetNextWindowSizeConstraints(ImVec2(150, 0), ImVec2(350, io.DisplaySize.y - 20));
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
      ImGui::Separator();
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
        if (ImGui::TreeNode("Lights"))
        {
          for (const auto& light : game->m_scene->m_PointLights)
          {
            showLights(light.second, light.first.c_str());
          }
          for (const auto& light : game->m_scene->m_DirectionLight)
          {
            showLights(light.second, light.first.c_str());
          }
          for (const auto& light : game->m_scene->m_SpotLights)
          {
            showLights(light.second, light.first.c_str());
          }
          ImGui::TreePop();
        }
        ImGui::Separator();
        for (auto &obj : game->m_scene->m_Objects)
          objectInfo(obj.second, obj.first);
       ImGui::TreePop();
      }
    }
    ImGui::Separator();
    ImGui::Text("Input");
    ImGui::SliderFloat("Mouse sensivity", &game->m_camera1->MouseSensitivity, 0.0, 1.0);
    ImGui::SliderFloat("MovementSpeed", &game->m_camera1->MovementSpeed, 1.0, 1000.0);
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
    
  ImGui::Separator();
  if (ImGui::TreeNode("Camera"))
  {
    if (ImGui::SliderFloat("Pitch", &game->m_scene->m_Camera->Pitch, -89.0, 89.0))
      game->m_scene->m_Camera->updateCameraVectors();
    if (ImGui::SliderFloat("Yaw", &game->m_scene->m_Camera->Yaw, 0.0f, 360.0f))
      game->m_scene->m_Camera->updateCameraVectors();
    ImGui::Text("Position");
    ImGui::DragFloat3("##pos", &game->m_scene->m_Camera->Position[0], 0.1);
    ImGui::TreePop();
  }
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

void GameGUI::showLights(BaseLight* light, const char *name)
{

  if (ImGui::TreeNode(name)) {
    //ImGui::InputScalar("position", ImGuiDataType_Float, &game->m_World->gravity);
    if (light->BaseLight::type == BaseLight::DIRECTIONAL)
    {
      DirectionLight* _light = reinterpret_cast<DirectionLight*>(light);
      ImGui::Text("direction");
      ImGui::DragFloat("x", &_light->direction.x);
      ImGui::DragFloat("y", &_light->direction.y);
      ImGui::DragFloat("z", &_light->direction.z);
    }
    else if (light->BaseLight::type == BaseLight::POINT || light->BaseLight::type == BaseLight::SPOT)
    {
      PointLight* _light = reinterpret_cast<PointLight*>(light);
      ImGui::Text("position");
      ImGui::DragFloat("x", &_light->position.x);
      ImGui::DragFloat("y", &_light->position.y);
      ImGui::DragFloat("z", &_light->position.z);

      ImGui::DragFloat("constant", &_light->constant, 0.01f);
      ImGui::DragFloat("linear", &_light->linear, 0.01f);
      ImGui::DragFloat("quadratic", &_light->quadratic, 0.0001f, 0.0f, 1.0f, "%.4f");
      if (light->BaseLight::type == BaseLight::SPOT)
      {
        SpotLight* _light = reinterpret_cast<SpotLight*>(light);
        ImGui::DragFloat("cutOff", &_light->cutOff, 0.01f);
        ImGui::DragFloat("outerCutOff", &_light->outerCutOff, 0.001f);
      }
    }

    editing = false;
    if (ImGui::ColorEdit3("ambient", (float*) &light->ambient))
      editing = true;
    if (ImGui::ColorEdit3("diffuse", (float*) &light->diffuse)) 
      editing = true;
    if (ImGui::ColorEdit3("specular", (float*) &light->specular)) 
      editing = true;

    ImGui::Checkbox("enabled", &light->enabled);

    ImGui::TreePop();
  }
}

bool GameGUI::OnInputEvent(sf::Event& event)
{
  
  switch (event.type)
  {
  case sf::Event::KeyPressed:
    switch(event.key.code)
    {
    case sf::Keyboard::P:
      //isWireFrame = !isWireFrame;
      return true;
    }
  case sf::Event::MouseButtonPressed:
  {
    if (event.mouseButton.button == sf::Mouse::Button::Left)
    {
      if (!editing)
        game->gotoGame();
    }
  }
  default:
  {

  }
  }
  return false;

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
      ImGui::Text("Position");
      ImGui::DragFloat3("##pos", &obj->m_transform.position[0], 0.1, f32_lo_a, f32_hi_a, "%.3f");
      ImGui::Text("Rotation");
      ImGui::DragFloat3("##rot", &obj->m_transform.rotation[0], 0.1, f32_lo_a, f32_hi_a, "%.3f");
      ImGui::Text("Scale");
      ImGui::DragFloat3("##scale", &obj->m_transform.scale[0], 0.1, f32_lo_a, f32_hi_a, "%.3f");
      char materialName[64];
      ImGui::InputText("Material", (char*)obj->m_Material->name->data(), obj->m_Material->name->size(),ImGuiInputTextFlags_ReadOnly);
      ImGui::TreePop();
    }
    ImGui::Text("Type: %s", obj->type.c_str());
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

