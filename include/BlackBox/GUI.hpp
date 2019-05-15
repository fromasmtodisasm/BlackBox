#pragma once
#include <BlackBox/CGame.hpp>

struct BaseLight;

class GameGUI
{
  friend class CGame;
  CGame *game;
public:
  GameGUI();
  ~GameGUI();

  void Draw();
  void objectInfo(Object *obj, std::string);
  void musiListController();
  void cameraController();
  void showLights(BaseLight* light, const char *name);
};
