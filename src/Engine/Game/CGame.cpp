#include <BlackBox/Game/CGame.hpp>
#include <BlackBox/Game/GameObject.hpp>
#include <BlackBox/CWindow.hpp>
#include <BlackBox/Triangle.hpp>
#include <BlackBox/Render/Texture.hpp>
#include <BlackBox/GUI.hpp>
#include <BlackBox/Scene.hpp>
#include <BlackBox/Resources/SceneManager.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/Utils.hpp>

#include <imgui-SFML.h>
#include <imgui.h>

//#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <ctime>
#include <cctype>

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


void CGame::PreRender()
{
	glCheck(glViewport(0, 0, 1366, 768));
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
	m_Console = m_pSystem->getIConsole();
  p_gIGame = reinterpret_cast<IGame*>(this);
	m_Window = m_pSystem->getIWindow();
	m_inputHandler = m_pSystem->getIInputHandler();
	m_Window->setFlags(CWindow::DRAW_GUI);
  

	if (!loadScene()) {
		m_Log->AddLog("[FAILED] Failed init objects\n");
		return false;
	}
	//m_Log->AddLog("[OK] Objects inited\n");
	m_Console->PrintLine("[OK] Objects inited\n");
	FrameBufferObject *sceneBuffer = new FrameBufferObject(m_Window->getWidth(), m_Window->getHeight());
	sceneBuffer->create();
	m_scene->setRenderTarget(sceneBuffer);
 
  gui = new GameGUI();
  gui->game = this;

  glm::vec3 pos = glm::vec3(0,0,0);//0, player_pos.y + 3, 0);
  // create an camera looking at the light
  m_camera1 = new CCamera(
    pos
  );
	camControl = new CameraController(m_camera1);

  m_inputHandler->AddEventListener(this);


  // Set scene before camera, camera setted to active scene in world
  m_World->setScene(m_scene);
  m_World->setCamera(m_camera1);
  m_active_camera = m_camera1;
  initPlayer();
  m_inputHandler->mouseLock(true);

  //m_World->setCamera(camera2);
	m_World->getActiveScene()->getObject("brick_normal_box_2")->m_Material->nextDiffuse();
	m_World->setPretRenderCallback(this);
	m_World->setPostRenderCallback(this);
	
	postProcessors.push_back(nullptr);
	postProcessors.push_back(new PostProcessor("negative"));
	postProcessors.push_back(new PostProcessor("grayscale"));
	postProcessors.push_back(new PostProcessor("kernel.outline"));
	postProcessors.push_back(new PostProcessor("kernel.blur"));
	m_World->getActiveScene()->setPostProcessor(postProcessors[0]);
	m_Font = new FreeTypeFont();
	m_Font->Init("arial.ttf", 16, 18);

	initCommands();
	m_Console->ExecuteFile("res/scripts/init.cfg");
	//m_Console->ExecuteString("clear");
  return true;
}

bool CGame::update() {
  while (!m_Window->closed() &&  m_running) {
    sf::Time deltaTime = deltaClock.restart();
    m_deltaTime = deltaTime.asSeconds();
		float fps =  1000.0f / deltaTime.asMilliseconds();
    input();
		execScripts();
    m_Window->update();
    m_World->update(m_deltaTime);
    setRenderState();

    render();
		m_World->getActiveScene()->present(m_Window->getWidth(), m_Window->getHeight());
		drawHud(fps);
    m_Window->swap();
  }
	return true;
}

void CGame::execScripts()
{
	/*
	for (auto& script : scripts)
	{
		//doFile(script);
	}
	*/
}

void CGame::drawHud(float fps)
{
	int num_objects = m_World->getActiveScene()->numObjects();
	int line = m_Window->getHeight();
	int step = 18;

	std::string mode = m_Mode == MENU ? "MENU"
		: m_Mode == FPS ? "FPS"
		: m_Mode == FLY ? "FLY"
		: "EDIT";

	glViewport(0, 0, m_Window->getWidth(), m_Window->getHeight());
	// Info
	{
		m_Font->RenderText(
			"FPS: " + std::to_string(fps),
			0.f, line-=step, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		m_Font->RenderText(
			"NUM OBJECTS: " + std::to_string(num_objects),
			0.f, line-=step, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		m_Font->RenderText(
			"Current mode: " + mode,
			0.f, line-=step, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		m_Font->RenderText(
			"Width = " + std::to_string(m_Window->getWidth()) + "Height = " + std::to_string(m_Window->getHeight()),
			0.f, line-=step, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		m_Font->RenderText(
			"Active scene: " + m_World->getActiveScene()->name,
			0.f, line-=step, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		m_Font->RenderText(
			"Selected Object: " + m_World->getActiveScene()->selectedObject()->first,
			0.f, line-=step, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		m_Font->RenderText(
			"Camera addres: " + std::to_string((int)(int*)m_active_camera),
			0.f, line-=step, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		m_Font->RenderText(
			"Player Camera addres: " + std::to_string((int)(int*)m_player->getCamera()),
			0.f, line-=step, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		m_Font->RenderText(
			"Camera speed: " + std::to_string(m_active_camera->MovementSpeed),
			0.f, line-=step, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		m_Font->RenderText(
			"Player addres: " + std::to_string((int)(int*)m_player),
			0.f, line-=step, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		m_Font->RenderText(
			"Pos: " + 
			std::to_string(m_active_camera->Position.x) + ", " +
			std::to_string(m_active_camera->Position.y) + ", " +
			std::to_string(m_active_camera->Position.z) + "; " +
			"Yaw: " + 
			std::to_string(m_active_camera->Yaw) + "; " +
			"Pitch: " + 
			std::to_string(m_active_camera->Pitch) + "; ",
			0.f, line-=step, 1.0f, glm::vec3(1.5, 0.1f, 0.2f));
		if (in_console)
		{
			m_Console->Draw();
		}
	}
}

bool CGame::run() {
	m_Log->AddLog("[OK] Game started\n");
  deltaClock.restart();
  m_PlayList.setVolume(10.f);
  //m_PlayList.play();
  m_isMusicPlaying = true;
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
	if (culling)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
  /*
  */

}

void CGame::render()
{
  m_Window->clear();
  /* Rendering code here */
  //int w = m_Window->viewPort.width - m_Window->viewPort.left;
  //int h = m_Window->viewPort.height - m_Window->viewPort.top;
  int w = m_Window->getWidth();
  int h = m_Window->getHeight();
  m_World->getActiveScene()->m_Camera->Ratio = ((float)w)/ h;

  //m_World->setCamera(m_camera1);
  m_World->draw(m_deltaTime);
}

void CGame::setPlayer(CPlayer* player)
{
  m_player = player;
}

void CGame::setCamera(CCamera* camera)
{
	m_active_camera = camera;
	m_World->setCamera(camera);
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
  case CGame::EDIT:
    return EditInputEvent(event);
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
		in_console = false;
  }
}

void CGame::gotoFly()
{
	m_Mode = FLY;
}

void CGame::gotoEdit()
{
	m_Mode = EDIT;
}

void CGame::showMenu()
{

}

IWindow* CGame::getWindow()
{
	return m_Window;
}

bool CGame::initPlayer()
{
  if (m_player != nullptr)
  {
    delete m_player;
  }
  if ((m_player = reinterpret_cast<CPlayer*>(m_scene->getObject("MyPlayer"))) != nullptr)
  {
    m_player->attachCamera(m_active_camera);
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
		/*
    case sf::Keyboard::P:
      isWireFrame = !isWireFrame;
      return true;
		*/
    case sf::Keyboard::Backspace:
      return true;
    case sf::Keyboard::Space:
      m_active_camera->mode = CCamera::Mode::FLY;
      m_Mode = Mode::FLY;
      return true;
    case sf::Keyboard::Escape:
      gotoMenu();
      return true;
		case sf::Keyboard::P:
			m_active_camera->MovementSpeed += 5;
			return true;
		case sf::Keyboard::E:
			//m_inputHandler->mouseLock(false);
			m_Mode = EDIT;
			return true;
		case sf::Keyboard::M:
			m_active_camera->MovementSpeed -= 5;
			return true;
		case sf::Keyboard::B:
			culling = !culling;
			return true;
		case sf::Keyboard::F1:
			m_World->getActiveScene()->selectedObject()->second->m_Material->nextDiffuse();
			return true;
		case sf::Keyboard::F9:
			if (event.key.shift)
				++currPP;
			else
				--currPP;
			m_World->getActiveScene()->setPostProcessor(postProcessors[currPP % postProcessors.size()]);
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
      m_inputHandler->mouseLock(false);
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
      m_inputHandler->mouseLock(false);
      m_Mode = Mode::MENU;
      m_running = false;
      return true;
    case sf::Keyboard::Enter:
			gotoGame();
      return true;
    default:
      return gui->OnInputEvent(event);
    }
  default:
    return gui->OnInputEvent(event);
  }
  return false;

}

bool CGame::DefaultInputEvent(sf::Event& event)
{
	return false;
}

bool CGame::EditInputEvent(sf::Event& event)
{
	
	m_Console->ShowConsole(in_console);
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Tilde:
			if (event.key.control)
			{
				in_console = !in_console;
				return true;
			}
			return false;
		case sf::Keyboard::Escape:
			gotoMenu();
			return true;
		case sf::Keyboard::I:
			m_World->getActiveScene()->selectedObject()->second->move(Movement::FORWARD);
			return true;
		case sf::Keyboard::U:
			m_World->getActiveScene()->selectedObject()->second->move(Movement::BACKWARD);
			return true;
		case sf::Keyboard::J:
			m_World->getActiveScene()->selectedObject()->second->move(Movement::DOWN);
			return true;
		case sf::Keyboard::K:
			m_World->getActiveScene()->selectedObject()->second->move(Movement::UP);
			return true;
		case sf::Keyboard::V:
			m_World->getActiveScene()->selectedObject()->second->setVisibility(!m_World->getActiveScene()->selectedObject()->second->visible());
			return true;
		case sf::Keyboard::Tab:
			if (event.key.shift)
			{
				m_World->getActiveScene()->selectPrevObject();
			}
			else
			{
				m_World->getActiveScene()->selectNextObject();
			}
			return true;
		default:
			return m_player->OnInputEvent(event);
		}
	case sf::Event::MouseWheelScrolled:
	{
		float factor = event.mouseWheel.delta < 0 ? 0.9 : 1.1;
		m_World->getActiveScene()->selectedObject()->second->m_transform.scale *= factor;
		return true;
	}
	case sf::Event::TextEntered:
	{
		if (event.text.unicode == ':')
		{
			in_console = true;
		}
	}
	default:
		return m_player->OnInputEvent(event);
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
