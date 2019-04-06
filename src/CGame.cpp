#include "CGame.hpp"
#include "GameObject.hpp"
#include "CWindow.hpp"
#include <iostream>
#include <vector>
#include <glm/ext/matrix_transform.hpp>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Pointer to Global ISystem.
static ISystem* gISystem = 0;
ISystem* GetISystem()
{
  return gISystem;
}

CGame::CGame(char *title) : 
  m_Title(title)
{

}

bool CGame::init(ISystem *pSystem)  {
  m_pSystem = pSystem;
  m_Window = new CWindow(m_Title); 
  if (m_Window != nullptr ) {
    if (!m_Window->init() || !m_Window->create())
      return false;
  if (!init_opbject())
    return false;
  } 
  return true;
}

bool CGame::update() {
  while (!m_Window->closed()) {
    m_Window->clear();
    m_Window->update();
    /* Rendering code here */
    world.draw();
    m_Window->swap();
  }
	return true;
}

bool CGame::run() {
  update();
  return true;
}

void CGame::input()
{
  ICommand *cmd;
  //std::vector<ICommand*> qcmd;  
  //while ((cmd = inputHandler->handleInput()) != nullptr)
  //  ;//cmd->execute();
}

bool CGame::init_opbject()
{
    if (m_ShaderProgram == nullptr) return false;
    else {
      //world.add("triangle", Primitive::create(Primitive::TRIANGLE, m_ShaderProgram));
      world.add("cube", Primitive::create(Primitive::CUBE, m_ShaderProgram));
      /*
      world.add("triangle", new Triangle(m_ShaderProgram));
      world.add("triangle", new Triangle(m_ShaderProgram));
      */
      return true;
    }
}

IGame *CreateIGame(char *title) {
  CGame *game = new CGame(title);
  return (game);
}

bool CGame::EventListener::OnInputEvent(sf::Event & event)
{
  return false;
}
