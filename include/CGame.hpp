#pragma once
#include "IGame.hpp"
#include "IWindow.hpp"

class CGame : public IGame {
private:
  IWindow *win;
public:
  bool init();
  bool update();
  bool run();
};
