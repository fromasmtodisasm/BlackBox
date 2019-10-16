#include <BlackBox/Game/Game.hpp>
#include <BlackBox/Game/GameObject.hpp>
#include <BlackBox/Window.hpp>
#include <BlackBox/Triangle.hpp>
#include <BlackBox/Render/Texture.hpp>
#ifdef GUI
#include <BlackBox/GUI.hpp>
#endif // GUI
#include <BlackBox/Scene.hpp>
#include <BlackBox/Resources/SceneManager.hpp>
#include <BlackBox/Resources/MaterialManager.hpp>
#include <BlackBox/Render/FrameBufferObject.hpp>
#include <BlackBox/Render/FreeTypeFont.hpp>
#include <BlackBox/Render/IRender.hpp>
#include <BlackBox/Render/ShadowMapTechnique.hpp>
#include <BlackBox/Render/HdrTechnique.hpp>
#include <BlackBox/Render/TechniqueManager.hpp>
#include <BlackBox/Utils.hpp>
#include <BlackBox/Profiler/Profiler.h>
#include <BlackBox/Profiler/Utils.h>

#ifdef GUI
#include <imgui-SFML.h>
#include <imgui.h>
#endif

//#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ctime>
#include <cctype>

#include <sstream>

//using namespace std;

IGame *p_gIGame;


struct TextRenderInfo
{
  IFont* font;
  std::vector<std::string> text;
  glm::vec4 color;
  TextRenderInfo() : font(nullptr), color(glm::vec4(1.0)){}
  TextRenderInfo(IFont *f, glm::vec4 c)
    :
    font(f), color(c)
  {
  }
  void AddLine(std::string line)
  {
    text.push_back(line + '\n');
  }
  SDrawTextInfo getDTI()
  {
    SDrawTextInfo dti;
    dti.color[0] = color[0];
    dti.color[1] = color[1];
    dti.color[2] = color[2];
    dti.color[3] = color[3];
    dti.font = font;
    return dti;
  }
};

World *CGame::getWorld() const
{
    return m_World;
}


void CGame::PreRender()
{
	//glCheck(glViewport(0, 0, 1366, 768));
}

float CGame::getTime()
{
	return m_time;
}

CGame::CGame(std::string title) :
  camControl(nullptr),
  g_scene(nullptr),
#ifdef GUI
  gui(nullptr),
#endif // GUI
  listener(nullptr),
  m_Console(nullptr),
  m_Font(nullptr),
  m_Log(nullptr),
  m_ScreenShader(nullptr),
  m_Window(nullptr),
  m_inputHandler(nullptr),
  m_player(nullptr),
  m_pSystem(nullptr),
  m_scene(nullptr),
  m_sceneManager(nullptr),
  shaderManager(nullptr),
  m_World(new World()),m_Title(title)
{
#pragma warning(push)
#pragma warning(disable : 4244)
  srand(time(nullptr));
#pragma warning(push)
  m_deltaTime = 0.0f;
  m_lastTime = 0.0f;
  m_PlayList.setRootPath("res/music/");
  m_PlayList.addTrack("background.ogg");
  m_PlayList.addTrack("japan.ogg");
}

bool CGame::init(ISystem *pEngine)  {
  m_pSystem /*= gISystem */= pEngine;
	m_pRender = m_pSystem->GetIRender();
  m_pScriptSystem = m_pSystem->GetIScriptSystem();
  m_Log = m_pSystem->GetILog();
	m_Console = m_pSystem->GetIConsole();
  p_gIGame = reinterpret_cast<IGame*>(this);
	m_Window = m_pSystem->GetIWindow();
	m_inputHandler = m_pSystem->GetIInputHandler();
  m_inputHandler->AddEventListener(this);
	m_Window->setFlags(CWindow::DRAW_GUI);
  
	initCommands();
	initVariables();
	InitScripts();
  auto init_cfg = m_Console->GetCVar("game_config");
  if (init_cfg == nullptr)
  {
    //TODO: log: game config not specified
    return false;
  }
	m_Console->ExecuteFile((std::string("res/scripts/") + init_cfg->GetString()).c_str());
  auto is_complete = m_Console->GetCVar("g_init_complete");
  if (is_complete == nullptr)
  {
    //TODO: log: error load init.cfg
    return false;
  }

	if (!loadScene()) {
		m_Log->AddLog("[FAILED] Failed init objects\n");
		return false;
	}
  // Set scene before camera, camera setted to active scene in world
  m_World->setScene(m_scene);
#ifdef GUI
  gui = new GameGUI();
  gui->game = this;
#endif // GUI
  initPlayer();
  m_inputHandler->mouseLock(true);

  auto tech = new HdrTechnique();
  tech->Init(m_World->getActiveScene(), nullptr);
  m_World->getActiveScene()->setTechnique(tech);
	
	postProcessors.push_back(nullptr);
	postProcessors.push_back(new PostProcessor("negative"));
	postProcessors.push_back(new PostProcessor("grayscale"));
	postProcessors.push_back(new PostProcessor("kernel.outline"));
	postProcessors.push_back(new PostProcessor("kernel.blur"));
	m_World->getActiveScene()->setPostProcessor(postProcessors[0]);
	m_Font = new FreeTypeFont();
	m_Font->Init("arial.ttf", 16, 18);

  ITexture* consoleBackGround = new Texture();
#if 0 
	consoleBackGround->load("console/fc.jpg"); 
#endif
  m_Console->SetImage(consoleBackGround);

  return true;
}

bool CGame::update() {
  while (!m_Window->closed() &&  m_running) {
		m_pSystem->Update();
		m_pSystem->BeginFrame();
		{
			sf::Time deltaTime = deltaClock.restart();
			m_deltaTime = deltaTime.asSeconds();
			m_time += m_deltaTime;
			fps =  1000.0f / deltaTime.asMilliseconds();
			PROFILER_PUSH_CPU_MARKER("INPUT", Utils::COLOR_LIGHT_BLUE);
				input();
			PROFILER_POP_CPU_MARKER();
			execScripts();
			m_Window->update();
			m_World->update(m_deltaTime);
			setRenderState();

			{
				DEBUG_GROUP("ALL RENDERING");
					PROFILER_PUSH_CPU_MARKER("CPU RENDER", Utils::COLOR_YELLOW);
						render();
					PROFILER_POP_CPU_MARKER();
						m_World->getActiveScene()->present(m_Window->getWidth(), m_Window->getHeight());
					PROFILER_PUSH_CPU_MARKER("DrawHud", Utils::COLOR_CYAN);
						drawHud(fps);
					PROFILER_POP_CPU_MARKER();
			}
		}
		m_pSystem->EndFrame();
    m_Window->swap();
  }
	return true;
}

void CGame::execScripts()
{

}

void CGame::drawHud(float fps)
{
  if (r_displayinfo->GetIVal() != 0)
  {
    DisplayInfo(fps);
  }
  if (m_Console->IsOpened())
  {
    m_Console->Draw();
  }
}

void CGame::DisplayInfo(float fps)
{
  auto num_objects = m_World->getActiveScene()->numObjects();
  auto line = m_Window->getHeight();
  auto step = 18;

  std::string mode = m_Mode == MENU ? "MENU"
    : m_Mode == FPS ? "FPS"
    : m_Mode == FLY ? "FLY"
    : "EDIT";

  glViewport(0, 0, m_Window->getWidth(), m_Window->getHeight());
  // Info

  TextRenderInfo info(m_Font, glm::vec4(0.5, 1.0f, 0.6f, 1.0));
  SDrawTextInfo dti = info.getDTI();

  //
  auto render = m_pSystem->GetIRender();

  //===========

  m_Font->SetXPos(0);
  m_Font->SetYPos(18);
  auto& text = info.text;
  auto& color = info.color;
  auto camera = m_World->getActiveScene()->getCurrentCamera();

  auto objPos = m_World->getActiveScene()->selectedObject()->second->m_transform.position;
  info.AddLine("FPS: " + std::to_string(fps));
  info.AddLine("NUM OBJECTS: " + std::to_string(num_objects));
  info.AddLine("Current mode: " + mode);
  info.AddLine("Width = " + std::to_string(m_Window->getWidth()) + "Height = " + std::to_string(m_Window->getHeight()));
  info.AddLine("Active scene: " + m_World->getActiveScene()->name);
  info.AddLine("Selected Object: " + m_World->getActiveScene()->selectedObject()->first);
  info.AddLine("    visible: " + std::to_string(m_World->getActiveScene()->selectedObject()->second->visible()));
  info.AddLine("    Pos: " +
    std::to_string(objPos.x) + ", " +
    std::to_string(objPos.y) + ", " +
    std::to_string(objPos.z) + "; ");
  info.AddLine("Camera speed: " + std::to_string(camera->MovementSpeed->GetFVal()));
  auto camPos = camera->getPosition();
  auto camRot = camera->getRotation();
  auto pos = "Pos: " +
    std::to_string(camPos.x) + ", " +
    std::to_string(camPos.y) + ", " +
    std::to_string(camPos.z) + "; " +
    "Yaw: " +
    std::to_string(camRot.y) + "; " +
    "Pitch: " +
    std::to_string(camRot.x) + "; "
    ;

  for (auto& text : info.text)
  {
    render->PrintLine(text.c_str(), dti);
  }

  render->PrintLine("To hide depth buffer press <;>\n", dti);
  render->PrintLine((std::string("Camera width = ") + std::to_string(GetISystem()->GetIConsole()->GetCVar("r_cam_w")->GetFVal()) + "\n").c_str(), dti);
  render->PrintLine((std::string("Camera height = ") + std::to_string(GetISystem()->GetIConsole()->GetCVar("r_cam_h")->GetFVal()) + "\n").c_str(), dti);

  info.color = glm::vec4(1.0f, 0.f, 0.f, 1.0f);
  render->PrintLine(pos.c_str(), info.getDTI());
}

bool CGame::run() {
	m_Log->AddLog("[OK] Game started\n");
  m_time = deltaClock.restart().asSeconds();
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
  if (!SceneManager::init(m_Console->GetCVar("g_scene")->GetString()))
    return false;

  TechniqueManager::init();

  m_scene = defaultScene;
  return m_scene != nullptr;
}

void CGame::setRenderState()
{
  if (isWireFrame)
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
  else
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	
	m_pRender->SetState(IRender::State::DEPTH_TEST, true);
	m_pRender->SetState(IRender::State::BLEND, false);
	if (culling)
	{
		m_pRender->SetState(IRender::State::CULL_FACE, true);
		m_pRender->SetCullMode(IRender::CullMode::BACK);
	}
	else
	{
		m_pRender->SetState(IRender::State::CULL_FACE, false);
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
  m_World->getActiveScene()->getCurrentCamera()->Ratio = ((float)w)/ h;

  //m_World->setCamera(m_camera1);
  m_World->draw(m_deltaTime);
}

void CGame::setPlayer(CPlayer* player)
{
  m_player = player;
}

void CGame::setCamera(CCamera* camera)
{
	//m_active_camera = camera;
	//m_World->setCamera(camera);
}

extern "C" IGame *CreateIGame(const char *title) {
  CGame *game = new CGame(title);
  return (game);
}

bool CGame::OnInputEvent(sf::Event &event)
{
	{
		bool retflag;
		bool retval = ShouldHandleEvent(event, retflag);
		if (retflag) return retval;
	}

  auto lpx = m_Console->GetCVar("lpx");
  auto lpy = m_Console->GetCVar("lpy");
  auto lpz = m_Console->GetCVar("lpz");
  auto useBoxFilter = m_Console->GetCVar("bf");
  bool result = OnInputEventProxy(event);
  switch (event.type)
  {
  case sf::Event::KeyPressed:
  {
    if (event.key.code == sf::Keyboard::SemiColon)
      openShadowMap = !openShadowMap;
    switch (event.key.code)
    {
    case sf::Keyboard::Up:
    {
      lpy->Set(lpy->GetFVal() + 0.5f);
      break;
    }
    case sf::Keyboard::Down:
    {
      lpy->Set(lpy->GetFVal() - 0.5f);
      break;
    }
    case sf::Keyboard::Left:
    {
      lpx->Set(lpx->GetFVal() - 0.5f);
      break;
    }
    case sf::Keyboard::Right:
    {
      lpx->Set(lpx->GetFVal() + 0.5f);
      break;
    }
    case sf::Keyboard::Insert:
    {
			bool ubf = useBoxFilter->GetIVal();
			useBoxFilter->Set(!ubf);
      break;
    }
    default:
      break;
    }
  }
  default:
    break;
  }
  return result;
}

IInputHandler *CGame::getInputHandler()
{
  return m_inputHandler;
}

void CGame::gotoGame()
{
	if (m_player != nullptr)
	{
    m_World->getActiveScene()->getCurrentCamera()->mode = CCamera::Mode::FPS;
    m_Mode = FPS;
    m_inputHandler->mouseLock(true);
		m_Console->ShowConsole(false);
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
    m_player->attachCamera(m_World->getActiveScene()->getCurrentCamera());
    m_player->setGame(this);
    return true;
  }
  return false;
}

bool CGame::FpsInputEvent(sf::Event& event)
{
  /*
	if (m_Console->IsOpened())
		return false;
  */
  auto camera = m_World->getActiveScene()->getCurrentCamera();
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
      camera->mode = CCamera::Mode::FLY;
      m_Mode = Mode::FLY;
      return true;
    case sf::Keyboard::Escape:
      gotoMenu();
      return true;
		case sf::Keyboard::P:
			camera->MovementSpeed->Set(camera->MovementSpeed->GetFVal() + 5.0f);
			return true;
		case sf::Keyboard::E:
			//m_inputHandler->mouseLock(false);
			m_Mode = EDIT;
			return true;
		case sf::Keyboard::M:
			camera->MovementSpeed->Set(camera->MovementSpeed->GetFVal() + 5.0f);
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
  /*
	if (m_Console->IsOpened())
		return false;
  */
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
  /*
	if (m_Console->IsOpened())
		return false;
  */
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
#ifdef GUI
      return gui->OnInputEvent(event);
#else
      return false;
#endif // GUI

    }
  default:
#ifdef GUI
      return gui->OnInputEvent(event);
#else
      return false;
#endif // GUI
  }
  return false;

}

bool CGame::DefaultInputEvent(sf::Event& event)
{
	return false;
}

bool CGame::EditInputEvent(sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
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
		float factor = event.mouseWheel.delta < 0 ? 0.9f : 1.1f;
		m_World->getActiveScene()->selectedObject()->second->m_transform.scale *= factor;
		return true;
	}
	case sf::Event::TextEntered:
	{
		if (event.text.unicode == ':')
		{
			m_Console->ShowConsole(true);
		}
	}
	default:
		return m_player->OnInputEvent(event);
	}
  return false;
}

bool CGame::OnInputEventProxy(sf::Event& event)
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

bool CGame::ShouldHandleEvent(sf::Event& event, bool& retflag)
{
	retflag = true;
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		switch (event.key.code)
		{
		case sf::Keyboard::Tilde:
			if (event.key.control)
			{
				if (m_Console->IsOpened())
					m_Console->ShowConsole(false);
				else
					m_Console->ShowConsole(true);
				return true;
			}
			return false;
		}
	}

	if (m_Console->IsOpened())
		return false;

	retflag = false;
	return {};
}

void CGame::Stop()
{
  m_running = false;
}

float CGame::getDeltaTime()
{
  return m_deltaTime;
}

float CGame::getFPS()
{
	return fps;
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
