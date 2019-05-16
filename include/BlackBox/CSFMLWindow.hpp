#pragma once
#include <BlackBox/IWindow.hpp>
#include <BlackBox/InputHandler.hpp>
#include <BlackBox/Opengl.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <glm/glm.hpp>
#include <imgui-SFML.h>

#include <map>
#include <list>

class CSFMLWindow :
  public IWindow,
  public IInputHandler
{
  friend class GameGUI;
  static constexpr int DEFAULT_WIDTH = 1024;
  static constexpr int DEFAULT_HEIGHT = 768;
  static constexpr char *DEFAULT_TITLE = "SFML Window";

  sf::RenderWindow* m_Window;
  bool m_bClose;
  int m_Width;
  int m_Height;
  std::string m_Title;
  glm::vec4 m_BackColor = { 0.0f, 0.0f, 0.0f, 1.0f };
  sf::Clock deltaClock;
	int m_flags = 0;
  // For input handling
  std::list<IInputEventListener*> listeners;
  struct
  {
    sf::Vector2i prev_pos;
    sf::Vector2i curr_pos;
    bool x_wraped;
    bool y_wraped;
    bool locked;
    sf::Vector2i lockedPos;
    int limit = 4;
  }Mouse;

public:
  sf::Rect<int> viewPort;
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
  bool OnInputEvent(sf::Event &event);

  // Inherited via IWindow
  virtual int getWidth() override;
  virtual int getHeight() override;
	virtual void setFlags(int flags) override;
private:
  sf::Vector2i nextMousePos(sf::Vector2i &position);
  void setMouseWrap(bool wrap);
  void glInit();

  // IInputHandler interface
public:
  virtual ICommand *handleInput() override;
  virtual void AddEventListener(IInputEventListener *pListener) override;
  virtual sf::Vector2i getDeltaMouse() override;
  virtual void mouseLock(bool lock) override;
};

