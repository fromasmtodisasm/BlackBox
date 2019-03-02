#include "CGame.hpp"

bool CGame::init() {
  win = CreateIWindow(); 
  if (win != nullptr) {
    win->init();
    return win->create(); 
  }
  return false;
}

bool CGame::update() {
  while (win->update()) {

  }
  return true;
}

bool CGame::run() {
  update();
  return true;
}


IGame *CreateIGame() {
  CGame *game = new CGame();
  return (game);
}
