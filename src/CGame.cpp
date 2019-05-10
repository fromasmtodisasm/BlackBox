#include <BlackBox/CGame.hpp>
#include <BlackBox/GameObject.hpp>
#include <BlackBox/CWindow.hpp>
#include <BlackBox/Triangle.hpp>
#include <BlackBox/Texture.hpp>
#include <BlackBox/GUI.hpp>
#include <BlackBox/Scene.hpp>
#include <BlackBox/SceneManager.hpp>
#include <BlackBox/MaterialManager.hpp>



#include <imgui-SFML.h>
#include <imgui.h>

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <ctime>

#include <sstream>

using namespace std;

IGame *p_gIGame;

//////////////////////////////////////////////////////////////////////
// Pointer to Global ISystem.
static IEngine* gISystem = nullptr;
IEngine* GetIEngine()
{
  return gISystem;
}

CGame::CGame(std::string title) :
  m_World(new World()),m_Title(title)
{
  srand(time(nullptr));
  m_deltaTime = 0.0f;
  m_lastTime = 0.0f;
  m_PlayList.setRootPath("res/music/");
  m_PlayList.addTrack("background.ogg");
  m_PlayList.addTrack("japan.ogg");
}

bool CGame::init(IEngine *pSystem)  {
  m_pSystem = pSystem;
  p_gIGame = reinterpret_cast<IGame*>(this);
  m_Window = new CWindow(m_Title); 
	m_Window->setFlags(CWindow::DRAW_GUI);
  if (m_Window != nullptr ) {
    if (!m_Window->init() || !m_Window->create())
      return false;
		cout << "Window susbsystem inited" << endl;

    if ((m_inputHandler = m_Window) == nullptr)
      return false;
    if (!loadScene()) {
			cout << "Failed init objects" << endl;
			return false;
		}
		cout << "Objects inited" << endl;
  } 
  gui = new GameGUI();
  gui->game = this;

  //glm::vec3 player_pos = m_World->operator[]("MyPlayer")->m_transform.position;
  glm::vec3 pos = glm::vec3(0,10,10);//0, player_pos.y + 3, 0);
  // create an camera looking at the light
  m_camera1 = new CCamera(
    pos
  );
	camControl = new CameraController(m_camera1);

  m_inputHandler->AddEventListener(reinterpret_cast<CGame*>(this));


  // Set scene before camera, camera setted to active scene in world
  m_World->setScene(m_scene);
  m_World->setCamera(m_camera1);
  m_active_camera = m_camera1;
  initPlayer();

  //m_World->setCamera(camera2);
  return true;
}

bool CGame::update() {
  while (!m_Window->closed() &&  m_running) {
    sf::Time deltaTime = deltaClock.restart();
    m_deltaTime = deltaTime.asSeconds();
    input();
    m_Window->update();
    gotoGame();
    m_World->update(m_deltaTime);
    setRenderState();

    //cp_size = ImVec2(300, m_Window->getHeight());

    glViewport(
          m_Window->viewPort.left,
          0,
          m_Window->viewPort.width,
          m_Window->viewPort.height);
    render();
    gui->Draw();

    m_Window->swap();
  }
	return true;
}

bool CGame::run() {
	cout << "Game started" << endl;
  deltaClock.restart();
  m_PlayList.setVolume(10.f);
  //m_PlayList.play();
  m_isMusicPlaying = true;
	gotoGame();
  update();
  return true;
}

void CGame::input()
{
  ICommand *cmd;
  //std::vector<ICommand*> qcmd;  
  while ((cmd = m_inputHandler->handleInput()) != nullptr)
    ;//cmd->execute();
}

bool CGame::loadScene() {
  if ((m_scene = new Scene("default")) == nullptr)
    return false;
  ;
  if (!ShaderManager::init() && (shaderManager = ShaderManager::instance()) == nullptr)
    return false;
  if (!MaterialManager::init("default.xml"))
    return false;

  m_scene->load("default.xml");

  //m_player = m_scene->getObject("MyPlayer");
  //m_inputHandler->AddEventListener(m_player);
  /*
  Texture *text = new Texture("container.jpg");
  Texture *plane_texture = new Texture("check.jpg");
  Texture *player_texture = new Texture("pengium.png");

  Object *obj;
  glm::vec3 light_pos(4,4,-4);
  //Creating objects
  Object *cube = Primitive::create(Primitive::CUBE, shaderManager->getProgram("vertex.glsl", "fragment.glsl"));
  Object *BB = Primitive::create(Primitive::CUBE, shaderManager->getProgram("vertex.glsl", "fragment.glsl"));
  Object *plane = Primitive::create(Primitive::PLANE, shaderManager->getProgram("vertex.glsl", "fragment.glsl"));

  m_player = new CPlayer();
  m_scene->addObject("MyPlayer", m_player);

  CShaderProgram *shader = shaderManager->getProgram("vertex.glsl", "fragment.glsl");
  Object *light =  Primitive::create(Primitive::CUBE,shaderManager->getProgram("vertex.glsl", "basecolor.frag"));
  light->move(light_pos);
  light->scale(glm::vec3(0.3f));
  plane->moveTo(glm::vec3(0,0,0));
  plane->scale(glm::vec3(50,50,50));
  plane->setTexture(plane_texture);
  plane->move(glm::vec3(0,-3,0));

  BB->setTexture(text);
  //m_player->setTexture(text);
  m_player->setShaderProgram(shader);
  m_player->scale({10,10,10});
  m_player->setTexture(player_texture);

  BB->scale(glm::vec3(70,70,70));
  m_scene->addObject("light", light);
  m_scene->addObject("plane", plane);
  m_scene->addObject("BB", BB);
  shader->create();
  for (int i = 0; i < 1; i++)
  {
    obj = ObjectManager::instance()->getObject("cube.obj");
    obj->setShaderProgram(shader);
    obj->setType(OBJType::TPRIMITIVE);
    obj->moveTo(glm::vec3(0,0,0));
    obj->scale(glm::vec3(i*20));

    obj->setTexture(plane_texture);
    m_scene->addObject("cube" + std::to_string(i), obj);
  }
  GameObject *go = GameObject::create(Primitive::CUBE);
  go->setTexture(text);
  go->setShaderProgram(cube->getShaderProgram());
  m_inputHandler->AddEventListener(go);
  m_inputHandler->AddEventListener(m_player);
  */
  return true;
}

void CGame::setRenderState()
{
  if (isWireFrame)
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  else
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glEnable(GL_DEPTH_TEST);
  //glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);

}

void CGame::render()
{
  /*
  ImGui::Text("Player.x = %f;Player.y = %f;Player.z = %f;Player.velocity(%f,%f,%f)",
              m_player->m_transform.position.x,
              m_player->m_transform.position.y,
              m_player->m_transform.position.z,
              m_player->velocity.x,
              m_player->velocity.y,
              m_player->velocity.z
              );
  */
  m_Window->clear();
  /* Rendering code here */
  int w = m_Window->viewPort.width;
  int h = m_Window->viewPort.height;
  //glViewport(300,0, w-300, h);
  m_camera1->Ratio = ((float)w - 300)/ h;

  m_World->setCamera(m_camera1);
  m_World->draw(m_deltaTime);
}

extern "C" IGame *CreateIGame(const char *title) {
  CGame *game = new CGame(title);
  return (game);
}

bool CGame::OnInputEvent(sf::Event &event)
{
  switch (event.type)
    {
    case sf::Event::KeyPressed:
    if (event.key.control) {
      switch (event.key.code) {
      case sf::Keyboard::Right:
        m_PlayList.next();
      }
      switch (event.key.code) {
      case sf::Keyboard::Left:
        m_PlayList.prev();
      }
      switch (event.key.code) {
      case sf::Keyboard::Up:
        m_PlayList.setVolume(m_PlayList.getVolume() + 2.f);
      }
      switch (event.key.code) {
      case sf::Keyboard::Down:
        m_PlayList.setVolume(m_PlayList.getVolume() - 2.f);
      }
    }
    else {
      switch(event.key.code)
      {
      case sf::Keyboard::P:
        isWireFrame = !isWireFrame;
        return true;
      case sf::Keyboard::Num9:
        m_active_camera = m_camera1;
        return true;
      case sf::Keyboard::Num0:
        m_active_camera = m_camera2;
        return true;
      case sf::Keyboard::Space:
        if (!m_isMusicPlaying){
          m_PlayList.play();
        }
        else {
          m_PlayList.pause();
        }
        m_isMusicPlaying = !m_isMusicPlaying;
        return true;
      case sf::Keyboard::Backspace:
				m_isPaused = !m_isPaused;
        return true;
      }
    }
  }
  return false;
}

IInputHandler *CGame::getInputHandler()
{
  return m_inputHandler;
}

void CGame::gotoGame()
{
	if (!m_InGame)
	{
		m_InGame = true;
    //m_inputHandler->mouseLock(true);
    //reinterpret_cast<sf::RenderWindow*>(m_Window->getHandle())->setMouseCursorVisible(false);
  }
}

void CGame::showMenu()
{

}

bool CGame::initPlayer()
{
  m_player = new CPlayer();
  m_player->attachCamera(m_camera1);
  m_player->setGame(this);
  m_inputHandler->AddEventListener(m_player);
}



float CGame::getDeltaTime()
{
  return m_deltaTime;
}

void CGame::gotoMenu()
{
	if (m_InGame)
	{
		m_InGame = false;
    //m_inputHandler->mouseLock(false);
    //reinterpret_cast<sf::RenderWindow*>(m_Window->getHandle())->setMouseCursorVisible(true);
	}
}
