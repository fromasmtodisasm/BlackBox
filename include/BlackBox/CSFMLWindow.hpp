#pragma once
#include <BlackBox/IWindow.hpp>
#include <BlackBox/InputHandler.hpp>
#include <BlackBox/Opengl.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <glm/glm.hpp>
#include <imgui-SFML.h>

class CSFMLWindow :
  public IWindow,
  public IInputEventListener
{
  static constexpr int DEFAULT_WIDTH = 1024;
  static constexpr int DEFAULT_HEIGHT = 768;
  static constexpr char *DEFAULT_TITLE = "SFML Window";

  sf::RenderWindow* m_window;
  bool m_bClose;
  int m_Width;
  int m_Height;
  std::string m_Title;
  glm::vec4 m_BackColor = { 0.5f, 0.5f, 0.5f, 1.0f };
  sf::Clock deltaClock;
	int m_flags = 0;
public:
	enum FLAGS
	{
		DRAW_GUI
	};
  CSFMLWindow(std::string = DEFAULT_TITLE, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
  ~CSFMLWindow();

  virtual bool create() override;
  virtual bool init() override;
  virtual void update() override;
  virtual void clear() override;
  virtual bool closed() override;
  virtual void swap() override;
  virtual void setTitle(const char *) override;
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
	virtual void setFlags(int flags) override;
private:
  void glInit();
};

