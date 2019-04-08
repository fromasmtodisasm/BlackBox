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
  // Make it the active window for OpenGL calls
  m_window->setActive();
  if (!gladLoadGL())
    return false;
  glEnable(GL_DEPTH);
  glEnable(GL_SMOOTH);

  return true;
}

void CSFMLWindow::update()
{

}

void CSFMLWindow::clear()
{
  glClearBufferfv(GL_COLOR, 0, m_BackColor);
  //glClear(GL_COLOR_BUFFER_BIT);
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
  if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Up))
    ;//key_up->execute();
  if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Down))
    ;//key_down->execute();
  if ((event.type == sf::Event::JoystickButtonPressed))
    ;//key_down->execute();
  // Resize event: adjust the viewport
  if (event.type == sf::Event::Resized)
  {
    glViewport(0, 0, event.size.width, event.size.height);
  }
  return true;
}
