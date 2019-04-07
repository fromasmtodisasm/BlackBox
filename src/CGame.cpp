#include "CGame.hpp"
#include "GameObject.hpp"
#include "CWindow.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <ctime>

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
  srand(time(0));
}

bool CGame::init(ISystem *pSystem)  {
  m_pSystem = pSystem;
  m_Window = new CWindow(m_Title); 
  if (m_Window != nullptr ) {
    if (!m_Window->init() || !m_Window->create())
      return false;
		cout << "Window susbsystem inited" << endl;
		if (!init_opbject()) {
			cout << "Failed init objects" << endl;
			return false;
		}
		cout << "Objects inited" << endl;
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
	cout << "Game started" << endl;
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

bool CGame::init_opbject() {
	//world.add("triangle", Primitive::create(Primitive::TRIANGLE, m_ShaderProgram));
  Object *obj;
  for (int i = 0; i < 10; i++)
  {
    char n[5];
    obj = Primitive::create(Primitive::CUBE);
    obj->move({
      rand() % 5 -1,
      rand()% 5 - 1,
      rand()% 5 - 1
      });
    world.add("cube" + char(i + '0'), obj);
  }
	/*
	world.add("triangle", new Triangle(m_ShaderProgram));
	world.add("triangle", new Triangle(m_ShaderProgram));
	*/
	return true;
}

IGame *CreateIGame(char *title) {
  CGame *game = new CGame(title);
  return (game);
}

bool CGame::EventListener::OnInputEvent(sf::Event & event)
{
  return false;
}
