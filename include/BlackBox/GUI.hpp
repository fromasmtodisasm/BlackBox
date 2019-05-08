#pragma once
#include <BlackBox/CGame.hpp>

class GameGUI
{
  friend class CGame;
  CGame *game;
public:
  GameGUI();
  ~GameGUI();

  void Update();
  void objectInfo(Object *obj, std::string);
  void musiListController();
  void cameraController();
};
