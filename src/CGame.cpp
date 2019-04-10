#include "CGame.hpp"
#include "GameObject.hpp"
#include "CWindow.hpp"
#include <iostream>
#include <vector>
#include <string>
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
  m_deltaTime = 0.0f;
  m_lastTime = 0.0f;
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
  HackCamera *camera = new HackCamera();
  camera->setView(m_Window->getWidth(), m_Window->getHeight());
  m_listener = new GameListener();
  inputHandler->AddEventListener(camera);
  inputHandler->AddEventListener(reinterpret_cast<CWindow*>(m_Window));
  inputHandler->AddEventListener(reinterpret_cast<GameListener*>(m_listener));
  m_World->setCamera(camera);
  return true;
}

bool CGame::update() {
  sf::Time deltaTime = deltaClock.restart();
  while (!m_Window->closed()) {
    input();
    m_Window->clear();
    m_World->update(deltaTime.asMicroseconds());
    /* Rendering code here */
    m_World->draw(deltaTime.asMicroseconds());
    m_Window->swap();
  }
	return true;
}

bool CGame::run() {
	cout << "Game started" << endl;
  deltaClock.restart();
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
  glm::vec3 light_pos(4,4,-4);
  Object *cube = Primitive::create(Primitive::CUBE, "vertex.glsl", "fragment.glsl");
  CShaderProgram *shader = new CShaderProgram("res/" "vertex.glsl", "res/""fragment.glsl");
  Object *light =  Primitive::create(Primitive::CUBE,"vertex.glsl", "basecolor.frag");
  light->move(light_pos);
  light->scale(glm::vec3(0.3f));

  m_World->add("light", light);
  shader->create();
  for (int i = 0; i < 0; i++)
  {
    //obj = Primitive::create(Primitive::CUBE, "vertex.glsl", "fragment.glsl");
    obj = Object::load("monkey.obj");
    obj->setShaderProgram(shader);
    obj->setType(OBJType::TPRIMITIVE);
    obj->move({
      rand() % 15 - 7,
      rand()% 15 - 7,
      rand()% 15 - 7
      });
    obj->rotate(
      rand() % 360,
      {
      rand() % 15 - 7,
      rand()% 15 - 7,
      rand()% 15 - 7
      });
    m_World->add("cube" + std::to_string(i), obj);
  }
	//GameObject *go = GameObject::create(Primitive::CUBE);
  //Object *cube = Primitive::create(Primitive::CUBE, "vertex.glsl", "fragment.glsl");
  //go->setShaderProgram(cube->getShaderProgram());
	//inputHandler->AddEventListener(go);
  //m_World->add("listener", reinterpret_cast<Object*>(go));
  m_World->add("cube", cube);
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
