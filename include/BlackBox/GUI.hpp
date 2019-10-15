#pragma once
#ifdef GUI
#include <BlackBox/Game/Game.hpp>
#include <BlackBox/InputHandler.hpp>

// Std inc
#include <vector>

struct BaseLight;
class Object;

struct IGuiElement
{
  virtual void execute() = 0;
};

struct GuiElement : public IGuiElement
{
  const char* name;
  ImVec2 pos;
  ImVec2 size;
};

struct MenuItem : public GuiElement
{
  const char* short_cut;
  bool selected = false;
  bool enabled = true;

  // Унаследовано через IGuiElement
  virtual void execute() override;
};

struct Menu : public GuiElement
{
  std::vector<MenuItem*> items;

  // Унаследовано через IGuiElement
  virtual void execute() override;
};

struct MainMenu : public GuiElement
{
  std::vector<Menu*> menus;

  // Унаследовано через IGuiElement
  virtual void execute() override;
};

struct MyWindow
{
  ImVec2 size;
  ImVec2 pos;

};

class GameGUI : public IInputEventListener
{
  friend class CGame;
  CGame *game;
  bool editing = false;
  //Menu
  float MainMenuHeight;
  MainMenu mainMenu;
  //ViewPort

  // Control panel
  enum Style : int
  {
    DARK,
    LIGHT
  }style = DARK;
  bool
      show_player=1,
      show_camera=1,
      show_demo=0,
      edit_player = 1,
      lighting = 0;
  bool open = true;
  MyWindow leftPanel;
  MyWindow assetsWindow;
  MyWindow viewport;
  Object* selectedObject = nullptr;
  ImGuiWindowFlags window_flags = 0;
public:
  GameGUI();
  ~GameGUI();

  void Draw();
  void objectInfo(Object *obj, std::string);
  void musiListController();
  void cameraController();
  void showLights(BaseLight* light, const char *name);
  void controlPanel();
  void assets();
  void setStyle();
  void showScene(Scene *scene);
  void drawFullScreenViewPort();

  // Унаследовано через IInputEventListener
  virtual bool OnInputEvent(sf::Event& event) override;
};

#endif
