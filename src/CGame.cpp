#include "CGame.hpp"

CGame::CGame(char *title) : m_Title(title) {

}

bool CGame::init() {
  m_Window = CreateIWindow(); 
  if (m_Window != nullptr) {
    m_Window->init();
    return m_Window->create(); 
  }
  return false;
}

bool CGame::update() {
  while (!m_Window->closed()) {
    m_Window->clear();
    m_Window->update();
    /* Rendering code here */

    m_Window->swap();
  }
	return true;
}

bool CGame::run() {
  update();
  return true;
}


IGame *CreateIGame(char *title) {
  CGame *game = new CGame(title);
  return (game);
}
