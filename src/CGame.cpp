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
  m_Title(title), m_World(new World())
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
    if ((inputHandler = new CInputHandler(m_Window)) == nullptr)
      return false;
		if (!init_opbject()) {
			cout << "Failed init objects" << endl;
			return false;
		}
		cout << "Objects inited" << endl;
  } 
  CCamera *camera = new CCamera();
  inputHandler->AddEventListener(camera);
  inputHandler->AddEventListener(reinterpret_cast<CWindow*>(m_Window));
  m_World->setCamera(camera);
  return true;
}

bool CGame::update() {
  while (!m_Window->closed()) {
    input();
    m_Window->clear();
    //m_Window->update();
    /* Rendering code here */
    m_World->draw();
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
  while ((cmd = inputHandler->handleInput()) != nullptr)
    ;//cmd->execute();
}

bool CGame::init_opbject() {
	//world.add("triangle", Primitive::create(Primitive::TRIANGLE, m_ShaderProgram));
  Object *obj;
  /*
  for (int i = 0; i < 10; i++)
  {
    char n[5];
    obj = Primitive::create(Primitive::CUBE);
    obj->move({
      rand() % 5 -1,
      rand()% 5 - 1,
      rand()% 5 - 1
      });
    m_World->add("cube" + char(i + '0'), obj);
  }
  */
	GameObject *go = GameObject::create(Primitive::CUBE);
  //Object *cube = Primitive::create(Primitive::CUBE, "vertex.glsl", "fragment.glsl");
  //go->setShaderProgram(cube->getShaderProgram());
	inputHandler->AddEventListener(go);
  m_World->add("listener", reinterpret_cast<Object*>(go));
  //m_World->add("cube", cube);
  //m_World->add("plane", Primitive::create(Primitive::PLANE, "vertex.glsl", "fragment.glsl"));
  /*
  world.add("triangle", new Triangle(m_ShaderProgram));
	*/
	return true;
}

IGame *CreateIGame(char *title) {
  CGame *game = new CGame(title);
  return (game);
}

CGame::EventListener::EventListener(CGame *game) : m_Game(game)
{

}

bool CGame::EventListener::OnInputEvent(sf::Event & event)
{
  return false;
}
