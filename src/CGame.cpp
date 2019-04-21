#include <BlackBox/CGame.hpp>
#include <BlackBox/GameObject.hpp>
#include <BlackBox/CWindow.hpp>
#include <BlackBox/Triangle.hpp>
#include <BlackBox/Texture.hpp>

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

    if ((m_inputHandler = new CInputHandler(m_Window)) == nullptr)
      return false;
		if (!init_opbject()) {
			cout << "Failed init objects" << endl;
			return false;
		}
		cout << "Objects inited" << endl;
  } 
  glm::vec3 player_pos = m_World->operator[]("MyPlayer")->m_transform.position;
  glm::vec3 pos = glm::vec3(0,10,10);//0, player_pos.y + 3, 0);
  // create an camera looking at the light
  m_camera1 = new CCamera(
    pos,
    glm::normalize(player_pos - pos),
    glm::vec3(0,1,0)
  );
	camControl = new CameraController(m_camera1);
  m_camera1->setView(0,0,m_Window->getWidth(),m_Window->getHeight());
  m_camera2 = new CCamera();
  m_player->attachCamera(m_camera1);
  m_inputHandler->AddEventListener(m_camera1);
  m_inputHandler->AddEventListener(m_camera2);
  m_inputHandler->AddEventListener(reinterpret_cast<CWindow*>(m_Window));
  m_inputHandler->AddEventListener(reinterpret_cast<CGame*>(this));
  m_World->setCamera(m_camera1);
  m_active_camera = m_camera1;
  //m_World->setCamera(camera2);
  return true;
}

bool CGame::update() {
  sf::Time deltaTime = deltaClock.restart();
  while (!m_Window->closed() &&  m_running) {
    m_deltaTime = deltaTime.asMicroseconds()*0.001;
    input();
    m_Window->update();
		if (!m_isPaused)
		{
			gotoGame();
			m_World->update(m_deltaTime);
			setRenderState();
		}
		else gotoMenu();

    render();
		if (m_isPaused)
			guiControls();
		m_Window->swap();
  }
	return true;
}

bool CGame::run() {
	cout << "Game started" << endl;
  deltaClock.restart();
  m_PlayList.setVolume(10.f);
  m_PlayList.play();
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

bool CGame::init_opbject() {
	//world.add("triangle", Primitive::create(Primitive::TRIANGLE, m_ShaderProgram));
  Texture *text = new Texture("container.jpg");
  Texture *plane_texture = new Texture("check.jpg");
  Texture *player_texture = new Texture("pengium.png");
  Object *obj;
  glm::vec3 light_pos(4,4,-4);
  Object *cube = Primitive::create(Primitive::CUBE, "vertex.glsl", "fragment.glsl");
  Object *BB = Primitive::create(Primitive::CUBE, "vertex.glsl", "fragment.glsl");
  Object *plane = Primitive::create(Primitive::PLANE, "vertex.glsl", "fragment.glsl");
  m_player = new CPlayer();
  m_World->add("MyPlayer", m_player);
  CShaderProgram *shader = new CShaderProgram("res/" "vertex.glsl", "res/""fragment.glsl");
  Object *light =  Primitive::create(Primitive::CUBE,"vertex.glsl", "basecolor.frag");
  light->move(light_pos);
  light->scale(glm::vec3(0.3f));
  //plane->moveTo(glm::vec3(0,0,0));
  plane->moveTo(glm::vec3(0,0,0));
  //plane->rotate(90, glm::vec3(1,0,0));
  plane->scale(glm::vec3(50,50,50));
  plane->setTexture(plane_texture);
  plane->move(glm::vec3(0,-3,0));
  //m_player->setTexture(text);
  m_player->setShaderProgram(shader);
  m_player->scale({10,10,10});
  m_player->setTexture(player_texture);

  BB->scale(glm::vec3(70,70,70));
  m_World->add("light", light);
  m_World->add("plane", plane);
  m_World->add("BB", BB);
  shader->create();
  for (int i = 0; i < 20; i++)
  {
    obj = Object::load("cube.obj");
    obj->setShaderProgram(shader);
    obj->setType(OBJType::TPRIMITIVE);
    obj->moveTo({
     (glm::vec3(0,0,0))
    /*
      static_cast<float>(rand() % 45 - 7),
      static_cast<float>(rand() % 45 - 7),
      static_cast<float>(rand() % 45 - 7)
      });
                    */
                });
    obj->scale(glm::vec3(i*20));

  /*
    obj->rotate(
      static_cast<float>(rand() % 360),
      {
      static_cast<float>(rand() % 65 - 7),
      static_cast<float>(rand() % 65 - 7),
      static_cast<float>(rand() % 65 - 7)
      });
      */
    obj->setTexture(plane_texture);
    m_World->add("cube" + std::to_string(i), obj);
  }
  GameObject *go = GameObject::create(Primitive::CUBE);
  go->setTexture(text);
  go->setShaderProgram(cube->getShaderProgram());
  m_inputHandler->AddEventListener(go);
  m_inputHandler->AddEventListener(m_player);
  //m_World->add("listener", reinterpret_cast<Object*>(go));
  /*
  world.add("triangle", new Triangle(m_ShaderProgram));
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
  m_Window->clear();
  /* Rendering code here */
  m_active_camera->update(m_deltaTime);
  /*
  m_camera1->setView(0,0,
         m_Window->getWidth(),m_Window->getHeight()
         );
         */
  m_World->setCamera(m_camera1);
  m_World->draw(m_deltaTime);
  /*
  m_camera2->setView(
         m_Window->getWidth()/2,0,
         m_Window->getWidth(),m_Window->getHeight()
         );
  m_World->setCamera(m_camera2);
  m_World->draw(m_deltaTime);
  */
  //m_Window->swap();
}


void CGame::guiControls()
{
	static bool show_player=1, show_camera=1, show_demo=0;
	
	ImGui::Begin("Control panel");
		ImGui::Checkbox("Show Plyer", &show_player);
		ImGui::Checkbox("Show Camera", &show_camera);
		ImGui::Checkbox("Show Demo", &show_demo);
	ImGui::End();
	if (show_player) {
		ImGui::Begin("Music Player");
		if (ImGui::Button("Pause"))
		{
			m_PlayList.pause();
		}
		if (ImGui::Button("Play"))
		{
			m_PlayList.play();
		}
		if (ImGui::Button("Next"))
		{
			m_PlayList.next();
		}
		if (ImGui::Button("Previos"))
		{
			m_PlayList.play();
		}
		if (ImGui::Button("Stop"))
		{
			m_PlayList.stop();
		}
		ImGui::End();
	}

	if (show_camera) {
		ImGui::Begin("Camera");
			if (ImGui::Button("Reset"))
			{
				m_active_camera->reset();	
			}
		ImGui::End();
	}
	if (show_demo)
	{
		ImGui::ShowDemoWindow();
	}
	if (ImGui::Button("Exit"))
	{
		m_running = false;	
	}
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
		reinterpret_cast<sf::RenderWindow*>(m_Window->getHandle())->setMouseCursorVisible(false);
	}
}

void CGame::gotoMenu()
{
	if (m_InGame)
	{
		m_InGame = false;
		reinterpret_cast<sf::RenderWindow*>(m_Window->getHandle())->setMouseCursorVisible(true);
	}
}
