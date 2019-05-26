#include <BlackBox/CGame.hpp>
#include <BlackBox/GameObject.hpp>
#include <BlackBox/CWindow.hpp>
#include <BlackBox/Triangle.hpp>
#include <BlackBox/Texture.hpp>
#include <BlackBox/GUI.hpp>
#include <BlackBox/Scene.hpp>
#include <BlackBox/SceneManager.hpp>
#include <BlackBox/MaterialManager.hpp>
#include <BlackBox/FrameBufferObject.hpp>



#include <imgui-SFML.h>
#include <imgui.h>

//#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <ctime>

#include <sstream>

//using namespace std;

IGame *p_gIGame;

//////////////////////////////////////////////////////////////////////
// Pointer to Global ISystem.
static IEngine* gISystem = nullptr;
IEngine* GetIEngine()
{
  return gISystem;
}

World *CGame::getWorld() const
{
    return m_World;
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
  m_pSystem = gISystem = pSystem;
  m_Log = m_pSystem->getILog();
  p_gIGame = reinterpret_cast<IGame*>(this);
  m_Window = new CWindow(m_Title, 1600, 900); 
	m_Window->setFlags(CWindow::DRAW_GUI);
  if (m_Window != nullptr ) {
    if (!m_Window->init() || !m_Window->create())
      return false;
		m_Log->AddLog("[OK] Window susbsystem inited\n");

    if ((m_inputHandler = m_Window) == nullptr)
      return false;
    if (!loadScene()) {
			m_Log->AddLog("[FAILED] Failed init objects\n");
			return false;
		}
		m_Log->AddLog("[OK] Objects inited\n");
    FrameBufferObject *sceneBuffer = new FrameBufferObject(m_Window->getWidth(), m_Window->getHeight());
    sceneBuffer->create();
    m_scene->setRenderTarget(sceneBuffer);
  } 
  gui = new GameGUI();
  gui->game = this;

  //glm::vec3 player_pos = m_World->operator[]("MyPlayer")->m_transform.position;
  glm::vec3 pos = glm::vec3(0,17,10);//0, player_pos.y + 3, 0);
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
  m_Window->mouseLock(true);

  //m_World->setCamera(camera2);
  return true;
}

bool CGame::update() {
  while (!m_Window->closed() &&  m_running) {
    sf::Time deltaTime = deltaClock.restart();
    m_deltaTime = deltaTime.asSeconds();
    input();
    m_Window->update();
    m_World->update(m_deltaTime);
    setRenderState();

    //cp_size = ImVec2(300, m_Window->getHeight());

    /*
    glViewport(
          0,
          0,
          m_Window->viewPort.width,
          m_Window->viewPort.height);
          */
    render();
    gui->Draw();

    m_Window->swap();
  }
	return true;
}

bool CGame::run() {
	m_Log->AddLog("[OK] Game started\n");
  deltaClock.restart();
  m_PlayList.setVolume(10.f);
  //m_PlayList.play();
  m_isMusicPlaying = true;
	gotoMenu();
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
  if (!ShaderManager::init() && (shaderManager = ShaderManager::instance()) == nullptr)
    return false;
  if (!MaterialManager::init("default.xml"))
    return false;
  if (!SceneManager::init())
    return false;

  m_scene = defaultScene;
  return m_scene != nullptr;
}

void CGame::setRenderState()
{
  if (isWireFrame)
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  else
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  /*
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  */

}

void CGame::render()
{
  m_Window->clear();
  /* Rendering code here */
  int w = m_Window->viewPort.width - m_Window->viewPort.left;
  int h = m_Window->viewPort.height - m_Window->viewPort.top;
  m_camera1->Ratio = ((float)w)/ h;

  //m_World->setCamera(m_camera1);
  m_World->draw(m_deltaTime);
}

void CGame::setPlayer(CPlayer* player)
{
  m_player = player;
}

extern "C" IGame *CreateIGame(const char *title) {
  CGame *game = new CGame(title);
  return (game);
}

bool CGame::OnInputEvent(sf::Event &event)
{
  switch (m_Mode)
  {
  case CGame::FPS:
    if (m_player != nullptr) return FpsInputEvent(event);
    else return false;
  case CGame::MENU:
    return MenuInputEvent(event);
  case CGame::FLY:
    return FlyInputEvent(event);
  default:
    break;
  }
  return false;
}

IInputHandler *CGame::getInputHandler()
{
  return m_inputHandler;
}

void CGame::gotoGame()
{
	if (!m_InGame && m_player != nullptr)
	{
    m_active_camera->mode = CCamera::Mode::FPS;
    m_Mode = FPS;
    m_inputHandler->mouseLock(true);
  }
}

void CGame::showMenu()
{

}

bool CGame::initPlayer()
{
  if (m_player != nullptr)
  {
    delete m_player;
  }
  if ((m_player = reinterpret_cast<CPlayer*>(m_scene->getObject("MyPlayer"))) != nullptr)
  {
    m_player->attachCamera(m_camera1);
    m_player->setGame(this);
    return true;
  }
  return false;
}

bool CGame::FpsInputEvent(sf::Event& event)
{
  switch (event.type)
  {
  case sf::Event::KeyPressed:
    switch(event.key.code)
    {
    case sf::Keyboard::P:
      isWireFrame = !isWireFrame;
      return true;
    case sf::Keyboard::Backspace:
      return true;
    case sf::Keyboard::Space:
      m_active_camera->mode = CCamera::Mode::FLY;
      m_Mode = Mode::FLY;
      return true;
    case sf::Keyboard::Escape:
      gotoMenu();
      return true;
    case sf::Keyboard::Enter:
      if (event.key.alt == true)
        gotoFullscreen();
      return true;
    }
  default:
    m_player->OnInputEvent(event);
  }
  return false;
}

bool CGame::FlyInputEvent(sf::Event& event)
{
 switch (event.type)
  {
  case sf::Event::KeyPressed:
    switch(event.key.code)
    {
    case sf::Keyboard::P:
      isWireFrame = !isWireFrame;
      return true;
    case sf::Keyboard::Backspace:
      return true;
    case sf::Keyboard::Space:
      gotoGame();
      return true;
    case sf::Keyboard::Escape:
      m_Window->mouseLock(false);
      m_Mode = Mode::MENU;
      return true;
    }
  default:
    m_player->OnInputEvent(event);
  }
  return false;
}

bool CGame::MenuInputEvent(sf::Event& event)
{
  switch (event.type)
  {
  case sf::Event::KeyPressed:
    switch (event.key.code)
    {
    case sf::Keyboard::Escape:
      m_Window->mouseLock(false);
      m_Mode = Mode::MENU;
      m_running = false;
      return true;
    default:
      return gui->OnInputEvent(event);
    }
  default:
    return gui->OnInputEvent(event);
  }
  return false;

}

void CGame::Stop()
{
  m_running = false;
}

float CGame::getDeltaTime()
{
  return m_deltaTime;
}

void CGame::PostRender()
{
  
}

void CGame::gotoMenu()
{
  m_Mode = MENU;
  m_inputHandler->mouseLock(false);
}

void CGame::gotoFullscreen()
{
  isFullScreen = !isFullScreen;

}
