#include <BlackBox/SFMLWindow.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/ITexture.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>
#ifdef GUI
  #include <imgui-SFML.h>
  #include <imgui.h>
#endif
#include <SFML/System/Vector2.hpp>
#include <iostream>
using namespace std;

CSFMLWindow::CSFMLWindow(std::string title, int width, int height) :
  m_Width(width), m_Height(height), m_Title(title), m_bClose(false),
  viewPort(0,0, width, height),
  m_Window(new sf::RenderWindow())
{
  ;
}

CSFMLWindow::~CSFMLWindow()
{
#ifdef GUI
  ImGui::SFML::Shutdown();
#endif // GUI
  m_Window->close();
}

bool CSFMLWindow::init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen)
{
	Create(width, height, fullscreen, cbpp);
  //m_Window->setVerticalSyncEnabled(true);
  //m_Window->setFramerateLimit(60);
  m_Window->setMouseCursorGrabbed(true);

#ifdef GUI
  ImGui::SFML::Init(*m_Window);
#endif // GUI

  // Make it the active window for OpenGL calls
  m_Window->setActive();

  // Input handling specific
  Mouse.curr_pos = Mouse.curr_pos = sf::Mouse::getPosition(*m_Window);
  Mouse.x_wraped = Mouse.y_wraped = false;
  Mouse.locked = false;

	//auto ip = GetISystem()->GetIConsole()->GetCVar("image_path");
	//if (ip)
	//{
		//std::string icon(ip->GetString());
		Image img;
		if (img.load("res/images/icon.jpg", nullptr))
		{
			;// m_Window->setIcon(img.width, img.height, static_cast<sf::Uint8*>(img.data));
		}
	//}

  return true;
}

void CSFMLWindow::Create(int width, int height, bool fullscreen, unsigned int cbpp)
{
	// Create the main window
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	sf::Vector2i position((desktop.width - width) / 2, (desktop.height - height) / 2);
	decltype(desktop) mode;
	int style;

	m_bFullScreen = fullscreen;
	if (fullscreen)
	{
		mode = sf::VideoMode::getFullscreenModes()[0];
		m_Width = mode.width;
		m_Height = mode.height;
		position.x = 0;
		position.y = 0;
		style = sf::Style::Fullscreen;
	}
	else
	{
		mode = sf::VideoMode(m_Width = width, m_Height = height, cbpp);
		//style = sf::Style::Default;
		style = sf::Style::Titlebar | sf::Style::Close;
	}
#if 1
	m_contextSettings.antialiasingLevel = 8;
#endif
	//m_Window = new sf::RenderWindow(mode, sf::String(m_Title), sf::Style::Default, m_contextSettings);//, sf::Style::Fullscreen);
	m_Window->create(mode, sf::String(m_Title), style, m_contextSettings);
	m_Window->setPosition(position);
}

void CSFMLWindow::update()
{
	if (m_flags == DRAW_GUI)
	{
    //ImGui::SFML::Update(*m_Window, deltaClock.restart());
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

  //ImGui::SFML::Render(*m_Window);
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
  //ImGui::SFML::ProcessEvent(event);
  // Close window: exit
  if (event.type == sf::Event::Closed)
    m_bClose = true;

  if (event.type == sf::Event::Resized)
  {
		m_Width = event.size.width;
		m_Height = event.size.height;
    viewPort.width = m_Width - viewPort.left;
    viewPort.height = m_Height;
    glViewport(0, 0, m_Width = event.size.width, m_Height = event.size.height);
		return true;
  }
  return false;
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
}
