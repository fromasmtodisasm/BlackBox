#pragma once
#include <BlackBox/System/IWindow.hpp>
#include <BlackBox/System/IInputHandler.hpp>
//#include <BlackBox/Core/MathHelper.hpp>
#include <SDL_video.h>
#include <SDL_events.h>

#include <map>
#include <list>
#include <string>

class CSDLWindow final:
  public IWindow
{
public:
  friend class GameGUI;
  Rect viewPort;
  enum FLAGS
  {
    DRAW_GUI
  };
  CSDLWindow(std::string = DEFAULT_TITLE, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
  ~CSDLWindow();

  // IWindow interface
  virtual bool init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen) override;
  virtual void update() override;
  virtual void clear() override;
  virtual bool closed() override;
  virtual void swap() override;
  virtual void setTitle(const char* title) override;
  virtual void show() override;
  virtual int getWidth() override;
  virtual int getHeight() override;
  virtual void setFlags(int flags) override;
  virtual Rect& getViewPort() override;
  virtual bool create(Params params) override;
  virtual void changeSize(int w, int h) override;
  virtual void setCursor(Cursor* cursor) override;
  virtual GLContext getContext() override;
public:
  virtual void* getHandle() override;
private:
  bool Create(int width, int height, bool fullscreen);
  void SelectDisplay(int &x, int &y, int w, int h);
  void handleEvent(SDL_Event& event);
  void SetIcon(char* path);
  static int EventWatcher(void* self, SDL_Event* event);

private:
  static constexpr int DEFAULT_WIDTH = 1024;
  static constexpr int DEFAULT_HEIGHT = 768;
  static constexpr const char* DEFAULT_TITLE = "SDL Window";

  SDL_Window* m_MainWindow;
  SDL_Window* m_SecondaryWindow;
  SDL_GLContext glRenderContext, glThreadContext;

  bool m_bClose;
  int m_Width;
  int m_Height;
  bool bInFullScreen = false;
  //TODO: SDL CONTEXT SETTINGS
  //sf::ContextSettings m_contextSettings;
  //===============================================================================
  std::string m_Title;
  //Vec4 m_BackColor = { 1.0f, 1.0f, 1.0f, 1.0f };
  int m_flags = 0;
  SDL_DisplayMode m_DesktopMode;
  // For input handling
  //std::list<IInputEventListener*> listeners;

  // IWindow interface
public:
  virtual void Release() override;
  virtual DisplayMode GetDesktopMode() override;
  virtual void EnterFullscreen(bool mode) override;

};
