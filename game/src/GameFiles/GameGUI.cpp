#ifdef GAME_GUI
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/GUI/GUI.hpp>
#include <BlackBox/Renderer/Light.hpp>
//#include <BlackBox/Renderer/FrameBufferObject.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Renderer/SceneManager.hpp>

#include <imgui.h>

struct FileClose : MenuItem
{
  CGame** game;
  FileClose(CGame** game) : game(game)
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
  CGame** game;
  FileOpen(CGame** game) : game(game)
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
  CGame** game;
  FileMenu(CGame** game) : game(game)
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
  FileMenu* fileMenu = new FileMenu(&game);
  fileMenu->items.push_back(new FileOpen(&game));
  fileMenu->items.push_back(new FileClose(&game));
  mainMenu.menus.push_back(fileMenu);
  assetsWindow.size.x = 0;
  assetsWindow.size.y = 300;
  setStyle();
}

GameGUI::~GameGUI()
{
}

void GameGUI::Draw()
{
  mainMenu.execute();
  ImGuiIO& io = ImGui::GetIO();
  ImVec2 viewportSize;
  if (game->isFullScreen)
  {
    drawFullScreenViewPort();
    return;
  }
  ImGui::SetNextWindowPos(ImVec2(0, mainMenu.size.y));
  ImGui::SetNextWindowSizeConstraints(ImVec2(150, io.DisplaySize.y - 20), ImVec2(500, io.DisplaySize.y - 20));

  ImGui::Begin("Navigator", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
  ImGui::SetWindowSize(ImVec2(ImGui::GetWindowSize().x, io.DisplaySize.y));
  leftPanel.size = ImGui::GetWindowSize();
  if (ImGui::BeginTabBar("TestBar"))
  {
    if (ImGui::BeginTabItem("Control Panel"))
    {
      controlPanel();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Styles"))
    {
      ImGui::RadioButton("Dark", (int*)&style, DARK);
      ImGui::RadioButton("Light", (int*)&style, LIGHT);
      setStyle();
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::End();
  //Rendering ViewPort
  ImGui::SetNextWindowPos(ImVec2(leftPanel.size.x, mainMenu.size.y));
  viewport.size = ImVec2(io.DisplaySize.x - leftPanel.size.x, io.DisplaySize.y - 20 - assetsWindow.size.y);
  //ImVec2 viewPortSize = ImVec2(ImGui::GetContentRegionAvail());
  ImGui::SetNextWindowSize(viewport.size);
  ImGui::Begin("View", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
  ImGui::SameLine();
  if (ImGui::BeginTabBar("Scene"))
  {
    if (ImGui::BeginTabItem("ViewPort"))
    {
      game->m_Window->getViewPort().left = ImGui::GetCursorPosX() + leftPanel.size.x;
      game->m_Window->getViewPort().top = ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y + mainMenu.size.y;
      //ImGui::Image((void*)game->m_World->activeScene->m_RenderedScene->texture, viewport.size = ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::End();
  assets();
  game->m_Window->getViewPort().width = viewport.size.x;
  game->m_Window->getViewPort().height = viewport.size.y;
  if (show_camera) {
  }
}

void GameGUI::musiListController()
{
  if (ImGui::TreeNode("Music List")) {
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
  if (ImGui::TreeNode("Camera")) {
    if (ImGui::Button("Reset"))
    {
      //m_active_camera->reset();
    }
    ImGui::TreePop();
  }
}

void GameGUI::showLights(BaseLight* light, const char* name)
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
    if (ImGui::ColorEdit3("ambient", (float*)&light->ambient))
      editing = true;
    if (ImGui::ColorEdit3("diffuse", (float*)&light->diffuse))
      editing = true;
    if (ImGui::ColorEdit3("specular", (float*)&light->specular))
      editing = true;

    ImGui::Checkbox("enabled", &light->enabled);

    ImGui::TreePop();
  }
}

void GameGUI::controlPanel()
{
  window_flags |= ImGuiWindowFlags_AlwaysAutoResize;
  ImVec2 size = ImGui::GetWindowSize();
  game->m_Window->getViewPort().left = size.x;
  game->m_Window->getViewPort().width = game->m_Window->getWidth() - size.x;
  ImGui::Checkbox("Show Plyer", &show_player);
  ImGui::Checkbox("Show Demo", &show_demo);
  ImGui::Separator();
  if (ImGui::TreeNode("Scene manager"))
  {
    static char path[260] = "";
    ImGui::InputText("Scene name", path, sizeof(path)) && path[0] != '\0';
    if (ImGui::Button("Load Scene"))
    {
      Scene* scene;
      if ((scene = SceneManager::instance()->getScene(path)) != nullptr)
      {
        /*
        if (game->initPlayer())
          game->gotoGame();
        */
        FrameBufferObject* sceneBuffer = new FrameBufferObject(FrameBufferObject::buffer_type::SCENE_BUFFER, game->m_Window->getWidth(), game->m_Window->getHeight());
        sceneBuffer->Create();
        //scene->setRenderTarget(sceneBuffer);
        //scene->setCamera(new CCamera());
        CPlayer* player = static_cast<CPlayer*>(scene->getObject("MyPlayer"));
        player->attachCamera(scene->getCurrentCamera());
        player->setGame(game);
        game->m_pLog->Log("[OK] Scene %s loaded\n", path);
      }
      else {
        game->m_pLog->Log("[FAILED] Scene %s not founded\n", path);
      }
    }
    ImGui::SameLine();
    if (ImGui::Button("Save scene"))
    {
      game->m_scene->save(path);
    }
    for (auto& scene : SceneManager::instance()->cache) {
      if (ImGui::TreeNode(scene.first.c_str()))
      {
        game->m_World->setScene(scene.second);
        //game->m_active_camera = scene.second->m_Camera;
        game->m_player = (CPlayer*)scene.second->getObject("MyPlayer");
        showScene(scene.second);
        ImGui::TreePop();
      }
    }

    ImGui::TreePop();
  }

  ImGui::Separator();
  ImGui::Text("Input");
  ImGui::SliderFloat("Mouse sensivity", &game->m_camera1->MouseSensitivity, 0.0, 1.0);
  //ImGui::SliderFloat("MovementSpeed", &game->m_camera1->MovementSpeed, 1.0, 1000.0);
  if (show_player)
    musiListController();
  if (show_demo)
  {
    ImGui::ShowDemoWindow();
  }

  // Exit
  if (ImGui::Button("Exit"))
  {
    game->Stop();
  }
  ImGuiIO& igio = ImGui::GetIO();

  ImGui::Separator();
  if (ImGui::TreeNode("Camera"))
  {
    /*
    if (ImGui::SliderFloat("Pitch", &game->m_scene->m_Camera->Pitch, -89.0, 89.0))
      game->m_scene->m_Camera->updateCameraVectors();
    if (ImGui::SliderFloat("Yaw", &game->m_scene->m_Camera->Yaw, 0.0f, 360.0f))
      game->m_scene->m_Camera->updateCameraVectors();
    ImGui::Text("Position");
    ImGui::DragFloat3("##pos", &game->m_scene->m_Camera->Position[0], 0.1);
    */
    ImGui::TreePop();
  }
}

void GameGUI::assets()
{
#define out_texture(m,t) \
if (m != nullptr && m->t != nullptr) \
{\
  ImGui::SameLine();\
  ImGui::Image((void*)m->t->id, ImVec2(100,100), ImVec2(0, 1), ImVec2(1, 0));\
}

  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(leftPanel.size.x, io.DisplaySize.y - assetsWindow.size.y));
  //ImVec2 viewPortSize = ImVec2(ImGui::GetContentRegionAvail());
  assetsWindow.size.x = viewport.size.x;
  ImGui::SetNextWindowSize(assetsWindow.size);
  ImGui::Begin("##View", (bool*)true, ImGuiWindowFlags_NoDecoration);
  if (ImGui::BeginTabBar("Assets"))
  {
    if (ImGui::BeginTabItem("Log"))
    {
      GetISystem()->GetILog()->Draw("MyLog", (bool*)true);
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Console"))
    {
      //GetISystem()->GetIConsole()->Draw("MyConsole", (bool*)true);
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Textures"))
    {
      if (selectedObject != nullptr)
      {
        ImGui::NewLine();
        // TODO: Здесь можем упасть, разобраться с этим и исправить
        out_texture(selectedObject->m_Material, diffuse[selectedObject->m_Material->current_diffuse]);
        out_texture(selectedObject->m_Material, specular);
        out_texture(selectedObject->m_Material, bump);
        out_texture(selectedObject->m_Material, normal);
        ImGui::Checkbox("Enable normal", &selectedObject->m_Material->enabledNormal);
      }
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }
  ImGui::End();
}

void GameGUI::setStyle()
{
  switch (style)
  {
  case LIGHT:  ImGui::StyleColorsLight(); break;
  case DARK: ImGui::StyleColorsDark(); break;
  }
}

void GameGUI::showScene(Scene* scene)
{
  ImGui::InputScalar("World gravity", ImGuiDataType_Float, &game->m_World->gravity);
  int lightCount = 0;
  lightCount += scene->m_PointLights.size();
  lightCount += scene->m_DirectionLight.size();
  lightCount += scene->m_SpotLights.size();
  if (lightCount > 0)
    if (ImGui::TreeNode("Lights"))
    {
      for (const auto& light : scene->m_PointLights)
      {
        showLights(light.second, light.first.c_str());
      }
      for (const auto& light : scene->m_DirectionLight)
      {
        showLights(light.second, light.first.c_str());
      }
      for (const auto& light : scene->m_SpotLights)
      {
        showLights(light.second, light.first.c_str());
      }
      ImGui::TreePop();
    }
  ImGui::Separator();
  selectedObject = scene->selected_object_it->second;
  for (auto& obj : scene->m_Objects)
    objectInfo(obj.second, obj.first);

  ImGui::Separator();
  ImGui::Text("Selected Object");
  objectInfo(scene->selected_object_it->second, scene->selected_object_it->first);
}

void GameGUI::drawFullScreenViewPort()
{
  ImGuiIO& io = ImGui::GetIO();
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(io.DisplaySize);
  game->m_Window->getViewPort().left = ImGui::GetCursorPosX();
  game->m_Window->getViewPort().top = ImGui::GetWindowPos().y + ImGui::GetWindowContentRegionMin().y;
  ImGui::Begin("FullScreen", (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoDecoration);
  //ImGui::Image((void*)game->m_World->activeScene->m_RenderedScene->texture, viewport.size = ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
  ImGui::End();
}

bool GameGUI::OnInputEvent(sf::Event& event)
{
  //ImGuiIO &io = ImGui::GetIO();
  switch (event.type)
  {
  case sf::Event::KeyPressed:
    switch (event.key.code)
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
      {
        game->gotoGame();
      }
      else
      {
      }
    }
  }
  default:
  {
  }
  }
  return false;
}

void GameGUI::objectInfo(Object* obj, std::string name)
{
  const ImS32   s32_zero = 0, s32_one = 1, s32_fifty = 50, s32_min = INT_MIN / 2, s32_max = INT_MAX / 2, s32_hi_a = INT_MAX / 2 - 100, s32_hi_b = INT_MAX / 2;
  const ImU32   u32_zero = 0, u32_one = 1, u32_fifty = 50, u32_min = 0, u32_max = UINT_MAX / 2, u32_hi_a = UINT_MAX / 2 - 100, u32_hi_b = UINT_MAX / 2;
  const ImS64   s64_zero = 0, s64_one = 1, s64_fifty = 50, s64_min = LLONG_MIN / 2, s64_max = LLONG_MAX / 2, s64_hi_a = LLONG_MAX / 2 - 100, s64_hi_b = LLONG_MAX / 2;
  const ImU64   u64_zero = 0, u64_one = 1, u64_fifty = 50, u64_min = 0, u64_max = ULLONG_MAX / 2, u64_hi_a = ULLONG_MAX / 2 - 100, u64_hi_b = ULLONG_MAX / 2;
  const float   f32_zero = 0.f, f32_one = 1.f, f32_lo_a = -10000000000.0f, f32_hi_a = +10000000000.0f;
  const double  f64_zero = 0., f64_one = 1., f64_lo_a = -1000000000000000.0, f64_hi_a = +1000000000000000.0;

  static bool inputs_step = true;

  if (ImGui::TreeNode(name.c_str())) {
    selectedObject = obj;
    if (ImGui::TreeNode("Physics")) {
      ImGui::InputScalar("Friction", ImGuiDataType_Float, &obj->friction, inputs_step ? &f32_one : NULL);

      if (ImGui::TreeNode("Veolcity")) {
        ImGui::InputScalar("Veolcity.X", ImGuiDataType_Float, &obj->velocity.x, inputs_step ? &f32_one : NULL);
        ImGui::InputScalar("Veolcity.Y", ImGuiDataType_Float, &obj->velocity.y, inputs_step ? &f32_one : NULL);
        ImGui::InputScalar("Veolcity.Z", ImGuiDataType_Float, &obj->velocity.z, inputs_step ? &f32_one : NULL);
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
      ImGui::InputText("Material", (char*)obj->m_Material->name->data(), obj->m_Material->name->size(), ImGuiInputTextFlags_ReadOnly);
      ImGui::TreePop();
    }
    ImGui::Text("Type: %s", obj->type.c_str());
    if (ImGui::Button("Clone"))
    {
      char buff[10];
      Object* object = new Object(*game->m_scene->getObject(name));
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
#endif