#include "CSFMLWindow.hpp"

CSFMLWindow::CSFMLWindow(char * title, int width, int height) :
  m_Width(width), m_Height(height), m_Title(title), m_bClose(false)
{
  
}

CSFMLWindow::~CSFMLWindow()
{
  m_window->close();
}

bool CSFMLWindow::create()
{
  return true;
}

bool CSFMLWindow::init()
{
  // Request a 24-bits depth buffer when creating the window
  sf::ContextSettings contextSettings;
  contextSettings.depthBits = 24;

  // Create the main window
  m_window = new sf::Window(sf::VideoMode(m_Width, m_Height), m_Title, sf::Style::Default, contextSettings);
  
  m_window->setMouseCursorVisible(false);
  //m_window->
  // Make it the active window for OpenGL calls
  m_window->setActive();
  if (!OpenGLLoader())
    return false;
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_SMOOTH);

  return true;
}

void CSFMLWindow::update()
{

}

void CSFMLWindow::clear()
{
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool CSFMLWindow::closed()
{
  return m_bClose;
}

void CSFMLWindow::swap()
{
  m_window->display();
}

void CSFMLWindow::setTitle(char *title)
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
    glViewport(0, 0, m_Width = event.size.width, m_Height = event.size.height);
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
