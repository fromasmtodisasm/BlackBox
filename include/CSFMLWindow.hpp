#pragma once
#include "IWindow.hpp"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <Opengl.hpp>

class CSFMLWindow :
  public IWindow
{
  static constexpr int DEFAULT_WIDTH = 1024;
  static constexpr int DEFAULT_HEIGHT = 768;
  static constexpr char *DEFAULT_TITLE = "SFML Window";

  sf::Window* m_window;
  bool m_bClose;
  int m_Width;
  int m_Height;
  char *m_Title;
  GLfloat m_BackColor[4] = { 0.5, 0.3, 0.8, 1.0 };
public:
  CSFMLWindow(char *title = DEFAULT_TITLE, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
  ~CSFMLWindow();

  virtual bool create() override;
  virtual bool init() override;
  virtual void update() override;
  virtual void clear() override;
  virtual bool closed() override;
  virtual void swap() override;
  virtual void setTitle(char *) override;
  virtual void show() override;

  // IWindow interface
public:
  virtual void *getHandle() override;
};

