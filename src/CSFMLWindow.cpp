#include <BlackBox/CSFMLWindow.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/System/Vector2.hpp>
#include <iostream>
using namespace std;

CSFMLWindow::CSFMLWindow(std::string title, int width, int height) :
  m_Width(width), m_Height(height), m_Title(title), m_bClose(false)
{
  
}

CSFMLWindow::~CSFMLWindow()
{
  ImGui::SFML::Shutdown();
  m_window->close();
}

bool CSFMLWindow::create()
{
  return true;
}

bool CSFMLWindow::init()
{
  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  settings.antialiasingLevel = 4;
  settings.majorVersion = 3;
  settings.minorVersion = 3;

  // Create the main window
  sf::VideoMode desktop = 	sf::VideoMode::getDesktopMode();
  //auto fullscreen = 	sf::VideoMode::getFullscreenModes();
  sf::VideoMode mode = desktop;
  m_window = new sf::RenderWindow(mode, sf::String(m_Title), sf::Style::Default, settings);//, sf::Style::Fullscreen);
  m_window->setVerticalSyncEnabled(true);
  m_window->setFramerateLimit(60);
	m_window->setMouseCursorGrabbed(true);

  ImGui::SFML::Init(*m_window);
  //m_window->setMouseCursorVisible(false);
  //m_window->
  // Make it the active window for OpenGL calls
  m_window->setActive();
  if (!OpenGLLoader())
    return false;
  glInit();

  return true;
}

void CSFMLWindow::update()
{
	if (m_flags == DRAW_GUI)
	{
		ImGui::SFML::Update(*m_window, deltaClock.restart());
	}
}

void CSFMLWindow::clear()
{
  float depth = 1.0f;
  glClearBufferfv(GL_COLOR, 0, &m_BackColor[0]);
  glClearBufferfv(GL_DEPTH, 0, &depth);
  //glClearBufferfv(GL_DEPTH, 0 );
}

bool CSFMLWindow::closed()
{
  return m_bClose;
}

void CSFMLWindow::swap()
{

  ImGui::SFML::Render(*m_window);
  m_window->display();
}

void CSFMLWindow::setTitle(const char *title)
{
  m_window->setTitle(title);
}

void CSFMLWindow::show()
{
}

void *CSFMLWindow::getHandle()
{
  return m_window;
}

bool CSFMLWindow::OnInputEvent(sf::Event &event)
{
  ImGui::SFML::ProcessEvent(event);
  // Close window: exit
  if (event.type == sf::Event::Closed)
    m_bClose = true;

  // Escape key: exit
  if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
  {
    m_window->close();
    m_bClose = true;
  }
  if (event.type == sf::Event::Resized)
  {
		m_Width = event.size.width;
		m_Height = event.size.height;
    glViewport(0, 0, m_Width = event.size.width, m_Height = event.size.height);
  }
  if (event.type == sf::Event::MouseMoved)
  {

  }
  return true;
}

int CSFMLWindow::getWidth()
{
  return m_Width;
}

int CSFMLWindow::getHeight()
{
  return m_Height;
}

void CSFMLWindow::setFlags(int flags)
{
	m_flags = flags;
}

void CSFMLWindow::glInit()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_SMOOTH);
  glEnable(GL_TEXTURE_2D);
  //glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
}
