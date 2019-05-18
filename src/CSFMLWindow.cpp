#include <BlackBox/CSFMLWindow.hpp>
#include <imgui-SFML.h>
#include <imgui.h>
#include <SFML/System/Vector2.hpp>
#include <iostream>
using namespace std;

CSFMLWindow::CSFMLWindow(std::string title, int width, int height) :
  m_Width(width), m_Height(height), m_Title(title), m_bClose(false),
  viewPort(300,0, width - 300, height)
{
  ;
}

CSFMLWindow::~CSFMLWindow()
{
  ImGui::SFML::Shutdown();
  m_Window->close();
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
  m_Window = new sf::RenderWindow(sf::VideoMode(m_Width, m_Height), sf::String(m_Title), sf::Style::Default, settings);//, sf::Style::Fullscreen);
  m_Window->setVerticalSyncEnabled(true);
  m_Window->setFramerateLimit(60);
  m_Window->setMouseCursorGrabbed(true);

  ImGui::SFML::Init(*m_Window);
  // Make it the active window for OpenGL calls
  m_Window->setActive();
  if (!OpenGLLoader())
    return false;
  glInit();

  // Input handling specific
  Mouse.curr_pos = Mouse.curr_pos = sf::Mouse::getPosition(*m_Window);
  Mouse.x_wraped = Mouse.y_wraped = false;
  Mouse.locked = false;

  return true;
}

void CSFMLWindow::update()
{
	if (m_flags == DRAW_GUI)
	{
    ImGui::SFML::Update(*m_Window, deltaClock.restart());
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

  ImGui::SFML::Render(*m_Window);
  m_Window->display();
}

void CSFMLWindow::setTitle(const char *title)
{
  m_Window->setTitle(title);
}

void CSFMLWindow::show()
{
}

void *CSFMLWindow::getHandle()
{
  return m_Window;
}

bool CSFMLWindow::OnInputEvent(sf::Event &event)
{
  ImGui::SFML::ProcessEvent(event);
  // Close window: exit
  if (event.type == sf::Event::Closed)
    m_bClose = true;

  /*
  // Escape key: exit
  if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
  {
    m_Window->close();
    m_bClose = true;
  }
  */
  if (event.type == sf::Event::Resized)
  {
		m_Width = event.size.width;
		m_Height = event.size.height;
    viewPort.width = m_Width - viewPort.left;
    viewPort.height = m_Height;
    //glViewport(0, 0, m_Width = event.size.width, m_Height = event.size.height);
  }
  if (event.type == sf::Event::MouseMoved)
  {

  }
  return true;
}

int CSFMLWindow::getWidth()
{
  return m_Window->getSize().x;
}

int CSFMLWindow::getHeight()
{
  //return m_Height;
  return m_Window->getSize().y;
}

void CSFMLWindow::setFlags(int flags)
{
	m_flags = flags;
}

sf::Vector2i CSFMLWindow::nextMousePos(sf::Vector2i &position)
{
  sf::Vector2i nextPos = position;
  if (Mouse.locked)
  {
    if (position.x < viewPort.left + Mouse.limit)
    {
      nextPos.x = viewPort.left + viewPort.width - Mouse.limit - 3;
      Mouse.curr_pos = Mouse.prev_pos = nextPos;
      Mouse.x_wraped = true;
      sf::Mouse::setPosition(sf::Vector2i(nextPos.x, nextPos.y), *m_Window);
    }
    else if (position.x > viewPort.left + viewPort.width - Mouse.limit)
    {
      nextPos.x = getDeltaMouse().x + viewPort.left + 3;
      Mouse.curr_pos = Mouse.prev_pos = nextPos;
      Mouse.x_wraped = true;
      sf::Mouse::setPosition(sf::Vector2i(nextPos.x, nextPos.y), *m_Window);
    }
    if (position.y < viewPort.top + Mouse.limit)
    {
      nextPos.y = viewPort.top + viewPort.height - Mouse.limit - 3;
      Mouse.curr_pos = Mouse.prev_pos = nextPos;
      Mouse.y_wraped = true;
      sf::Mouse::setPosition(sf::Vector2i(nextPos.x, nextPos.y), *m_Window);
    }
    else if (position.y > viewPort.top + viewPort.height - Mouse.limit)
    {
      nextPos.y = getDeltaMouse().y + viewPort.top + 3;
      Mouse.curr_pos = Mouse.prev_pos = nextPos;
      Mouse.y_wraped = true;
      sf::Mouse::setPosition(sf::Vector2i(nextPos.x, nextPos.y), *m_Window);
    }
  }
  return nextPos;
}

void CSFMLWindow::setMouseWrap(bool wrap)
{
}

void CSFMLWindow::glInit()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_SMOOTH);
  glEnable(GL_TEXTURE_2D);
  /*
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  */
}
