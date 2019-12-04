#pragma once
#include <BlackBox/IWindow.hpp>
#include <BlackBox/IInputHandler.hpp>
#include <BlackBox/MathHelper.hpp>
#include <SDL_video.h>
#include <SDL_events.h>

#include <map>
#include <list>
#include <string>

class CSDLWindow :
  public IWindow
{
  SDL_GLContext glRenderContext, glThreadContext;

  friend class GameGUI;
  static constexpr int DEFAULT_WIDTH = 1024;
  static constexpr int DEFAULT_HEIGHT = 768;
  static constexpr char* DEFAULT_TITLE = "SDL Window";

  SDL_Window* m_Window;
  bool m_bClose;
  int m_Width;
  int m_Height;
  //TODO: SDL CONTEXT SETTINGS
  //sf::ContextSettings m_contextSettings;
  //===============================================================================
  std::string m_Title;
  Vec4 m_BackColor = { 1.0f, 1.0f, 1.0f, 1.0f };
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
  virtual void setTitle(const char*) override;
  virtual void show() override;

  void handleEvent(SDL_Event* event);

  // IWindow interface
public:
  virtual void* getHandle() override;
  // IInputEventListener interface
public:

  // Inherited via IWindow
  virtual int getWidth() override;
  virtual int getHeight() override;
  virtual void setFlags(int flags) override;
private:
  bool Create(int width, int height, bool fullscreen);
  Vec2 nextMousePos(Vec2& position);
  void setMouseWrap(bool wrap);
  void glInit();

  // Унаследовано через IWindow
  virtual Rect& getViewPort() override;

  // Inherited via IWindow
  virtual bool create(Params params) override;

  // Inherited via IWindow
  virtual void changeSize(int w, int h) override;
  virtual void setCursor(Cursor* cursor) override;

  // Inherited via IWindow
  virtual GLContext getContext() override;
};

/*
SDL_GLContext glRenderContext, glThreadContext;
SDL_Window* mainWindow;
SDL_atomic_t stuffIsReady;

int loadGlInBackground(void* ptr)
{
  SDL_GL_MakeCurrent(mainWindow, glThreadContext);

  //Do some fancy loading gl stuff in the background... 

  //It is very important in shared contexts to make sure the driver is done with all Objects before signaling other threads that they can use them! 
  GLsync fenceId = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
  GLenum result;
  while (true)
  {
    glClientWaitSync(fenceId, GL_SYNC_FLUSH_COMMANDS_BIT, GLuint64(5000000000)); //5 Second timeout 
    if (result != GL_TIMEOUT_EXPIRED) break; //we ignore timeouts and wait until all OpenGL commands are processed! 
  }

  SDL_AtomicIncRef(&stuffIsReady);

  //im not 100% sure what happens to a context that still is active when a thread gets closed. 
  //But you can't call SDL_GL_DeleteContext on it! 
  //So I just unbind it from this thread and call SDL_GL_DeleteContext in the main thread at closing. 
  SDL_GL_MakeCurrent(mainWindow, NULL);
  return 0;
}

int main(int argc, char* argv[])
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    cout << "Unable to initialize SDL: " << SDL_GetError() << endl;
    return -1;
  }
  else
  {
    cout << "sdl init ok" << endl;
  }
  mainWindow = SDL_CreateWindow("SDL2 window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

  if (mainWindow == NULL) {
    std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    return -1;
  }

  SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
  glThreadContext = SDL_GL_CreateContext(mainWindow);
  glRenderContext = SDL_GL_CreateContext(mainWindow);

  SDL_Thread* thread = SDL_CreateThread(loadGlInBackground, "testThread", (void*)NULL);
  while (true)
  {
    //Event handling and all the fancy rendering... 
    if (SDL_AtomicGet(&stuffIsReady) == 1)
    {
      //now the opengl stuff loaded by the thread can be used 
    }
  }

  int threadReturnValue;
  SDL_WaitThread(thread, &threadReturnValue);
  SDL_GL_DeleteContext(glThreadContext);
  SDL_GL_DeleteContext(glRenderContext);
  SDL_DestroyWindow(mainWindow);
  SDL_Quit();
  return 0;

}
*/