#pragma once
#include "IGame.hpp"
#include "IWindow.hpp"

class CGame : public IGame {
private:
  IWindow *m_Window;
  char *m_Title;
public:
  CGame(char *);
  bool init();
  bool update();
  bool run();
};
