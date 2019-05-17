#pragma once
#include <BlackBox/CGame.hpp>
#include <BlackBox/InputHandler.hpp>

struct BaseLight;

class GameGUI : public IInputEventListener
{
  friend class CGame;
  CGame *game;
  bool editing = false;
public:
  GameGUI();
  ~GameGUI();

  void Draw();
  void objectInfo(Object *obj, std::string);
  void musiListController();
  void cameraController();
  void showLights(BaseLight* light, const char *name);

  // Унаследовано через IInputEventListener
  virtual bool OnInputEvent(sf::Event& event) override;
};
