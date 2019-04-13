#include "CGame.hpp"
#include "GameObject.hpp"
#include "CWindow.hpp"
#include "Triangle.hpp"
#include "Texture.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <ctime>

#include <imgui-SFML.h>
#include <imgui.h>

using namespace std;

//////////////////////////////////////////////////////////////////////
// Pointer to Global ISystem.
static ISystem* gISystem = nullptr;
ISystem* GetISystem()
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
  glm::vec3 player_pos = m_World->operator[]("MyPlayer")->m_transform.position;
  glm::vec3 pos = glm::vec3(0,3,0);//0, player_pos.y + 3, 0);
  // create an camera looking at the light
  m_camera1 = new CCamera(
    pos,
    -player_pos,
    glm::vec3(0,1,0)
  );
  m_camera1->setView(0,0,m_Window->getWidth(),m_Window->getHeight());
  m_camera2 = new CCamera();
  m_player->attachCamera(m_camera1);
  inputHandler->AddEventListener(m_camera1);
  inputHandler->AddEventListener(m_camera2);
  inputHandler->AddEventListener(reinterpret_cast<CWindow*>(m_Window));
  inputHandler->AddEventListener(reinterpret_cast<CGame*>(this));
  m_World->setCamera(m_camera1);
  m_active_camera = m_camera1;
  //m_World->setCamera(camera2);
  return true;
}

bool CGame::update() {
  sf::Time deltaTime = deltaClock.restart();
  while (!m_Window->closed()) {
    m_deltaTime = deltaTime.asMicroseconds()*0.001;
    input();
    m_Window->update();
    //guiControls();
    m_World->update(m_deltaTime);
    setRenderState();
    render();
  }
	return true;
}

bool CGame::run() {
	cout << "Game started" << endl;
  deltaClock.restart();
  m_PlayList.setVolume(10.f);
  m_PlayList.play();
  m_isMusicPlaying = true;
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
  Texture *text = new Texture("container.jpg");
  Object *obj;
  glm::vec3 light_pos(4,4,-4);
  Object *cube = Primitive::create(Primitive::CUBE, "vertex.glsl", "fragment.glsl");
  Object *plane = Primitive::create(Primitive::PLANE, "vertex.glsl", "fragment.glsl");
  m_player = new CPlayer();
  m_World->add("MyPlayer", m_player);
  CShaderProgram *shader = new CShaderProgram("res/" "vertex.glsl", "res/""fragment.glsl");
  Object *light =  Primitive::create(Primitive::CUBE,"vertex.glsl", "basecolor.frag");
  light->move(light_pos);
  light->scale(glm::vec3(0.3f));
  plane->moveTo(glm::vec3(0,0,0));
  plane->moveTo(glm::vec3(0,-3,0));
  //plane->rotate(90, glm::vec3(1,0,0));
  plane->scale(glm::vec3(50,50,50));
  plane->setTexture(text);
  m_player->setTexture(text);

  m_World->add("light", light);
  m_World->add("plane", plane);
  shader->create();
  for (int i = 0; i < 1; i++)
  {
    obj = Object::load("monkey.obj");
    obj->setShaderProgram(shader);
    obj->setType(OBJType::TPRIMITIVE);
    obj->move({
      static_cast<float>(rand() % 15 - 7),
      static_cast<float>(rand() % 15 - 7),
      static_cast<float>(rand() % 15 - 7)
      });
    obj->rotate(
      static_cast<float>(rand() % 360),
      {
      static_cast<float>(rand() % 15 - 7),
      static_cast<float>(rand() % 15 - 7),
      static_cast<float>(rand() % 15 - 7)
      });
    m_World->add("cube" + std::to_string(i), obj);
  }
  GameObject *go = GameObject::create(Primitive::CUBE);
  go->setShaderProgram(cube->getShaderProgram());
  inputHandler->AddEventListener(go);
  inputHandler->AddEventListener(m_player);
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
  glEnable(GL_CULL_FACE);
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
  m_Window->swap();
}


void CGame::guiControls()
{
	static bool show_player=1, show_camera=1;

	ImGui::Begin("Control panel");
		ImGui::Checkbox("Show Plyer", &show_player);
		ImGui::Checkbox("Show Camera", &show_camera);
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
      }
    }
  }
  return false;
}
