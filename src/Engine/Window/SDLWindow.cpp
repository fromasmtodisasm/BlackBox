#include <BlackBox/Core/Platform/platform_impl.inl>
#define CRY_SUPPRESS_CRYENGINE_WINDOWS_FUNCTION_RENAMING
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/System/Platform/SDL/Window.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/IConsole.hpp>

#include <SDL.h>

#include <memory>
#include <iostream>
#include <cstdio>

#define WINDOW_MAX_PEEP   64


CSDLWindow::CSDLWindow(std::string, int width, int height)
{
}

CSDLWindow::~CSDLWindow()
{
}

bool CSDLWindow::init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen)
{
  //Initialize SDL
	bInFullScreen = fullscreen;
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
  }

  if (!Create(width, height, fullscreen))
  {
    return false;
  }
  if (SDL_GetDesktopDisplayMode(0, &m_DesktopMode) != 0) {
    gEnv->pLog->Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
    return 1;
	}
  //SetIcon(nullptr);

  return true;
}

void CSDLWindow::update()
{
  
  SDL_Event eventList[WINDOW_MAX_PEEP];

  int nEvents;
  unsigned type = 0;
  //EInputState newState;

  // Assuming that SDL_PumpEvents is called from CLinuxInput

  nEvents = SDL_PeepEvents(eventList, WINDOW_MAX_PEEP, SDL_GETEVENT, SDL_WINDOWEVENT, SDL_WINDOWEVENT);

  if (nEvents == -1)
  {
    GetISystem()->GetILog()->LogError("SDL_GETEVENT error: %s", SDL_GetError());
    return;
  }
  for (int i = 0; i < nEvents; ++i)
  {
	  handleEvent(eventList[i]);
  }

}

void CSDLWindow::clear()
{
}

bool CSDLWindow::closed()
{
  return false;
}

void CSDLWindow::swap()
{
  SDL_GL_SwapWindow(m_MainWindow);
}

void CSDLWindow::setTitle(const char* title)
{
  SDL_SetWindowTitle(m_MainWindow, title);
}

void CSDLWindow::show()
{
}

void CSDLWindow::handleEvent(SDL_Event& event)
{
#define SDL_Log(format, ...) gEnv->pLog->Log(format, __VA_ARGS__)
	auto& system = gEnv->pSystem;
  if (event.type == SDL_WINDOWEVENT) {
    switch (event.window.event) {
    case SDL_WINDOWEVENT_SHOWN:
      SDL_Log("Window %d shown", event.window.windowID);
      break;
    case SDL_WINDOWEVENT_HIDDEN:
      SDL_Log("Window %d hidden", event.window.windowID);
			gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE, 0, 0);
      break;
    case SDL_WINDOWEVENT_EXPOSED:
      SDL_Log("Window %d exposed", event.window.windowID);
			gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE, 1, 0);
      break;
    case SDL_WINDOWEVENT_MOVED:
      SDL_Log("Window %d moved to %d,%d",
        event.window.windowID, event.window.data1,
        event.window.data2);
			system->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_MOVE, event.window.data1, event.window.data2);
      break;
    case SDL_WINDOWEVENT_RESIZED:
      SDL_Log("Window %d resized to %dx%d",
        event.window.windowID, event.window.data1,
        event.window.data2);
			gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_RESIZE, event.window.data1, event.window.data2);
      break;
    case SDL_WINDOWEVENT_SIZE_CHANGED:
      SDL_Log("Window %d size changed to %dx%d",
        event.window.windowID, event.window.data1,
        event.window.data2);
      break;
    case SDL_WINDOWEVENT_MINIMIZED:
      SDL_Log("Window %d minimized", event.window.windowID);
			gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE, 0, 0);
      break;
    case SDL_WINDOWEVENT_MAXIMIZED:
      SDL_Log("Window %d maximized", event.window.windowID);
			gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE, 1, 0);
      break;
    case SDL_WINDOWEVENT_RESTORED:
      SDL_Log("Window %d restored", event.window.windowID);
      break;
    case SDL_WINDOWEVENT_ENTER:
      SDL_Log("Mouse entered window %d",
        event.window.windowID);
      break;
    case SDL_WINDOWEVENT_LEAVE:
      SDL_Log("Mouse left window %d", event.window.windowID);
      break;
    case SDL_WINDOWEVENT_FOCUS_GAINED:
      SDL_Log("Window %d gained keyboard focus",
        event.window.windowID);
			gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE, 1, 0);
      break;
    case SDL_WINDOWEVENT_FOCUS_LOST:
      SDL_Log("Window %d lost keyboard focus",
        event.window.windowID);
			gEnv->pSystem->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_GAMEWINDOW_ACTIVATE, 0, 0);
      break;
    case SDL_WINDOWEVENT_CLOSE:
      SDL_Log("Window %d closed", event.window.windowID);
      break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
    case SDL_WINDOWEVENT_TAKE_FOCUS:
      SDL_Log("Window %d is offered a focus", event.window.windowID);
      break;
    case SDL_WINDOWEVENT_HIT_TEST:
      SDL_Log("Window %d has a special hit test", event.window.windowID);
      break;
#endif
    default:
      SDL_Log("Window %d got unknown event %d",
        event.window.windowID, event.window.event);
      break;
    }
  }
  #undef SDL_Log
}

void* CSDLWindow::getHandle()
{
  return m_MainWindow;
}

int CSDLWindow::getWidth()
{
  int w = 0;
  SDL_GetWindowSize(m_MainWindow, &w, nullptr);
  return w;
}

int CSDLWindow::getHeight()
{
  int h = 0;
  SDL_GetWindowSize(m_MainWindow, nullptr, &h);
  return h;
}

void CSDLWindow::setFlags(int flags)
{
}

bool CSDLWindow::Create(int width, int height, bool fullscreen)
{
  /* Request opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */

  SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  //SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

  /* Turn on double buffering with a 24bit Z buffer.
   * You may need to change this to 16 or 32 for your system */
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,8);

  int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE /*| SDL_WINDOW_HIDDEN*/;
  int posx = 0;
  int posy = 0;
  if (fullscreen)
  {
    //posx = 0;
    //posy = 0;
    //flags |= SDL_WINDOW_FULLSCREEN;
  }
//  SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT
  // Create window
  SelectDisplay(posx, posy, width, height);
  m_MainWindow = SDL_CreateWindow(m_Title.c_str(), posx, posy, width,height, flags);
  if (m_MainWindow == NULL)
  {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }
  SDL_GL_SetSwapInterval(-1);
  //SDL_HideWindow(m_MainWindow);

  // Now I need to create another window from hEternalHwnd for my swap chain that will have the same pixel format as mainWindow, so set the hint
  #if 0
  char addres[32];
  sprintf(addres, "%p", m_MainWindow);
  SDL_SetHint(SDL_HINT_VIDEO_WINDOW_SHARE_PIXEL_FORMAT, addres);

  m_SecondaryWindow = SDL_CreateWindowFrom(childWindow.m_hWnd);

  if (m_SecondaryWindow == NULL)
  {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }
  // Create an OpenGL context associated with the window.
  //glThreadContext = SDL_GL_CreateContext(m_Window);
  #endif
  glRenderContext = SDL_GL_CreateContext(m_MainWindow);
  #if 0
  if (SDL_GL_MakeCurrent(m_SecondaryWindow, glRenderContext) != 0)
  {
    printf("Can't create context current! SDL_Error: %s\n", SDL_GetError());

  }
  #endif
  //SDL_DestroyWindow(m_MainWindow);
  //std::swap(m_MainWindow, m_SecondaryWindow);
  if (fullscreen)
  {
    SDL_SetWindowFullscreen(m_MainWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_DisplayMode dm;
    if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
      SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
      return false;
    }
    SDL_SetWindowDisplayMode(m_MainWindow, &dm);
  }
  //SDL_AddEventWatch(EventWatcher, this);
  SDL_SetWindowInputFocus(m_MainWindow);
  return true;
}

void CSDLWindow::SelectDisplay(int &x, int &y, int w, int h)
{
  // enumerate displays
	int displays = SDL_GetNumVideoDisplays();
  auto display = gEnv->pConsole->GetCVar("r_DisplayIndex")->GetIVal();
	display   	 = std::min(displays, display);

	// get display bounds for all displays
	std::vector< SDL_Rect > displayBounds;
	for( int i = 0; i < displays; i++ ) {
			displayBounds.push_back( SDL_Rect() );
			SDL_GetDisplayBounds( i, &displayBounds.back() );
	}

	// window of dimensions 500 * 500 offset 100 pixels on secondary monitor
	x = (displayBounds[display].w - w) / 2 + displayBounds[display].x;
	y = (displayBounds[display].h - h) / 2 + displayBounds[display].y;
}
Rect& CSDLWindow::getViewPort()
{
  return viewPort;
}

bool CSDLWindow::create(Params params)
{
  return false;
}

void CSDLWindow::changeSize(int w, int h)
{
  SDL_SetWindowSize(m_MainWindow, w, h);
}

void CSDLWindow::setCursor(Cursor* cursor)
{
}

GLContext CSDLWindow::getContext()
{
  return static_cast<GLContext>(glThreadContext);
}

void CSDLWindow::SetIcon(char* path)
{
  SDL_Surface *surface;     // Declare an SDL_Surface to be filled in with pixel data from an image file
  Uint16 pixels[16*16] = {  // ...or with raw pixel data:
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0aab, 0x0789, 0x0bcc, 0x0eee, 0x09aa, 0x099a, 0x0ddd,
    0x0fff, 0x0eee, 0x0899, 0x0fff, 0x0fff, 0x1fff, 0x0dde, 0x0dee,
    0x0fff, 0xabbc, 0xf779, 0x8cdd, 0x3fff, 0x9bbc, 0xaaab, 0x6fff,
    0x0fff, 0x3fff, 0xbaab, 0x0fff, 0x0fff, 0x6689, 0x6fff, 0x0dee,
    0xe678, 0xf134, 0x8abb, 0xf235, 0xf678, 0xf013, 0xf568, 0xf001,
    0xd889, 0x7abc, 0xf001, 0x0fff, 0x0fff, 0x0bcc, 0x9124, 0x5fff,
    0xf124, 0xf356, 0x3eee, 0x0fff, 0x7bbc, 0xf124, 0x0789, 0x2fff,
    0xf002, 0xd789, 0xf024, 0x0fff, 0x0fff, 0x0002, 0x0134, 0xd79a,
    0x1fff, 0xf023, 0xf000, 0xf124, 0xc99a, 0xf024, 0x0567, 0x0fff,
    0xf002, 0xe678, 0xf013, 0x0fff, 0x0ddd, 0x0fff, 0x0fff, 0xb689,
    0x8abb, 0x0fff, 0x0fff, 0xf001, 0xf235, 0xf013, 0x0fff, 0xd789,
    0xf002, 0x9899, 0xf001, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0xe789,
    0xf023, 0xf000, 0xf001, 0xe456, 0x8bcc, 0xf013, 0xf002, 0xf012,
    0x1767, 0x5aaa, 0xf013, 0xf001, 0xf000, 0x0fff, 0x7fff, 0xf124,
    0x0fff, 0x089a, 0x0578, 0x0fff, 0x089a, 0x0013, 0x0245, 0x0eff,
    0x0223, 0x0dde, 0x0135, 0x0789, 0x0ddd, 0xbbbc, 0xf346, 0x0467,
    0x0fff, 0x4eee, 0x3ddd, 0x0edd, 0x0dee, 0x0fff, 0x0fff, 0x0dee,
    0x0def, 0x08ab, 0x0fff, 0x7fff, 0xfabc, 0xf356, 0x0457, 0x0467,
    0x0fff, 0x0bcd, 0x4bde, 0x9bcc, 0x8dee, 0x8eff, 0x8fff, 0x9fff,
    0xadee, 0xeccd, 0xf689, 0xc357, 0x2356, 0x0356, 0x0467, 0x0467,
    0x0fff, 0x0ccd, 0x0bdd, 0x0cdd, 0x0aaa, 0x2234, 0x4135, 0x4346,
    0x5356, 0x2246, 0x0346, 0x0356, 0x0467, 0x0356, 0x0467, 0x0467,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff,
    0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff, 0x0fff
  };
  surface = SDL_CreateRGBSurfaceFrom(pixels,16,16,16,16*2,0x0f00,0x00f0,0x000f,0xf000);

  // The icon is attached to the window pointer
  SDL_SetWindowIcon(m_MainWindow, surface);

  // ...and the surface containing the icon pixel data is no longer required.
  SDL_FreeSurface(surface);

}
#include <iostream>
int CSDLWindow::EventWatcher(void *self, SDL_Event *event)
{
  using namespace  std;
  CSDLWindow *& w = reinterpret_cast<CSDLWindow*&>(self);
  cout << "Event: " << event->type << endl;
  return 1;
}

void CSDLWindow::Release()
{
    delete this;
}

DisplayMode CSDLWindow::GetDesktopMode()
{
	return &m_DesktopMode;
}
void CSDLWindow::EnterFullscreen(bool mode)
{
	if (mode)
	{
#ifdef _DEBUG
		static constexpr int flag = SDL_WINDOW_FULLSCREEN_DESKTOP;
	#else
		static constexpr int flag = SDL_WINDOW_FULLSCREEN;
#endif // DEBUG
		SDL_SetWindowFullscreen(m_MainWindow, flag); 	
  }
	else
	{
		SDL_SetWindowFullscreen(m_MainWindow, 0); 	
	}
}
extern "C" {
  IWINDOW_API IWindow* CreateIWindow()
  {
    return new CSDLWindow();
  }
}
