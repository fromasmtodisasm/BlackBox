#pragma once
#include "IWindow.hpp"
#include "InputHandler.hpp"
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <Opengl.hpp>
#include <glm/glm.hpp>

class CSFMLWindow :
  public IWindow,
  public IInputEventListener
{
  static constexpr int DEFAULT_WIDTH = 1024;
  static constexpr int DEFAULT_HEIGHT = 768;
  static constexpr char *DEFAULT_TITLE = "SFML Window";

  sf::Window* m_window;
  bool m_bClose;
  int m_Width;
  int m_Height;
  char *m_Title;
  glm::vec4 m_BackColor = { 0.5f, 0.5f, 0.5f, 1.0f };
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

  // IInputEventListener interface
public:
  virtual bool OnInputEvent(sf::Event &event) override;

  // Inherited via IWindow
  virtual int getWidth() override;
  virtual int getHeight() override;
};

