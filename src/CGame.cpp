#include "CGame.hpp"
#include <iostream>

using namespace std;

CGame::CGame(char *title) : 
  m_Title(title)
{

}

bool CGame::init() {
  m_Window = CreateIWindow(); 
  if (m_Window != nullptr ) {
    if (!m_Window->init() || !m_Window->create())
      return false;
    m_ShaderProgram = new CShaderProgram("../res/vertex.glsl", "../res/fragment.glsl");
    if (!m_ShaderProgram) return false;
    return true;
  } 
  return false;
}

bool CGame::update() {
  while (!m_Window->closed()) {
    m_Window->clear();
    m_Window->update();
    /* Rendering code here */
    m_ShaderProgram->use();
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
