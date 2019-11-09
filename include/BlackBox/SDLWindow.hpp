#pragma once
#include <BlackBox/IWindow.hpp>
#include <BlackBox/IInputHandler.hpp>
#include <BlackBox/MathHelper.hpp>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_events.h>

#include <glm/glm.hpp>

#include <map>
#include <list>
#include <string>



class CSDLWindow :
  public IWindow
{
  friend class GameGUI;
  static constexpr int DEFAULT_WIDTH = 1024;
  static constexpr int DEFAULT_HEIGHT = 768;
  static constexpr char *DEFAULT_TITLE = "SDL Window";

  SDL_Window* m_Window;
  bool m_bClose;
  int m_Width;
  int m_Height;
	//TODO: SDL CONTEXT SETTINGS
	//sf::ContextSettings m_contextSettings;
	//===============================================================================
  std::string m_Title;
  glm::vec4 m_BackColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	int m_flags = 0;
  // For input handling
  //std::list<IInputEventListener*> listeners;
  struct
  {
    Vec2 prev_pos;
    Vec2 curr_pos;
    bool x_wraped = false;
    bool y_wraped = false;
    bool locked = false;
    Vec2 lockedPos;
    int limit = 4;
  }Mouse;

public:
  Rect viewPort;
	enum FLAGS
	{
		DRAW_GUI
	};
  CSDLWindow(std::string = DEFAULT_TITLE, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
  ~CSDLWindow();

  virtual bool init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen) override;
  virtual void update() override;
  virtual void clear() override;
  virtual bool closed() override;
  virtual void swap() override;
  virtual void setTitle(const char *) override;
  virtual void show() override;

	void handleEvent(SDL_Event* event);

  // IWindow interface
public:
  virtual void *getHandle() override;
  // IInputEventListener interface
public:

  // Inherited via IWindow
  virtual int getWidth() override;
  virtual int getHeight() override;
	virtual void setFlags(int flags) override;
private:
	bool Create(int width, int height, bool fullscreen);
  Vec2 nextMousePos(Vec2 &position);
  void setMouseWrap(bool wrap);
  void glInit();


	// Унаследовано через IWindow
	virtual Rect &getViewPort() override;

	// Inherited via IWindow
	virtual bool create(Params params) override;

	// Inherited via IWindow
	virtual void changeSize(int w, int h) override;
	virtual void setCursor(Cursor* cursor) override;
};

