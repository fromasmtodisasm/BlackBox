#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/Platform/SDL/Window.hpp>

#include <BlackBox/GUI/GUI.hpp>
#include <SDL.h>
#include <memory>

CSDLWindow::CSDLWindow(std::string, int width, int height)
{
}

CSDLWindow::~CSDLWindow()
{
}

bool CSDLWindow::init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen)
{
	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	if (!Create(width, height, fullscreen))
	{
		return false;
	}

#ifdef GUI
	ImGui_ImplOpenGL3_Init();
	ImGui_ImplSDL2_InitForOpenGL(m_Window, glRenderContext);
#endif // GUI

	Mouse.x_wraped = Mouse.y_wraped = false;
	Mouse.locked					= false;

	return true;
}

void CSDLWindow::update()
{
	SDL_Event event;

	/*
  while (SDL_PollEvent(&event))
  {
      handleEvent(&event);
  }
  */
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
	SDL_GL_SwapWindow(m_Window);
}

void CSDLWindow::setTitle(const char* title)
{
	SDL_SetWindowTitle(m_Window, title);
}

void CSDLWindow::show()
{
}

void CSDLWindow::handleEvent(SDL_Event* event)
{
	return;
	if (event->type == SDL_WINDOWEVENT)
	{
		switch (event->window.event)
		{
		case SDL_WINDOWEVENT_SHOWN:
			SDL_Log("Window %d shown", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_HIDDEN:
			SDL_Log("Window %d hidden", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_EXPOSED:
			SDL_Log("Window %d exposed", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_MOVED:
			SDL_Log("Window %d moved to %d,%d",
					event->window.windowID, event->window.data1,
					event->window.data2);
			break;
		case SDL_WINDOWEVENT_RESIZED:
			SDL_Log("Window %d resized to %dx%d",
					event->window.windowID, event->window.data1,
					event->window.data2);
			break;
		case SDL_WINDOWEVENT_SIZE_CHANGED:
			SDL_Log("Window %d size changed to %dx%d",
					event->window.windowID, event->window.data1,
					event->window.data2);
			break;
		case SDL_WINDOWEVENT_MINIMIZED:
			SDL_Log("Window %d minimized", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_MAXIMIZED:
			SDL_Log("Window %d maximized", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_RESTORED:
			SDL_Log("Window %d restored", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_ENTER:
			SDL_Log("Mouse entered window %d",
					event->window.windowID);
			break;
		case SDL_WINDOWEVENT_LEAVE:
			SDL_Log("Mouse left window %d", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_FOCUS_GAINED:
			SDL_Log("Window %d gained keyboard focus",
					event->window.windowID);
			break;
		case SDL_WINDOWEVENT_FOCUS_LOST:
			SDL_Log("Window %d lost keyboard focus",
					event->window.windowID);
			break;
		case SDL_WINDOWEVENT_CLOSE:
			SDL_Log("Window %d closed", event->window.windowID);
			break;
#if SDL_VERSION_ATLEAST(2, 0, 5)
		case SDL_WINDOWEVENT_TAKE_FOCUS:
			SDL_Log("Window %d is offered a focus", event->window.windowID);
			break;
		case SDL_WINDOWEVENT_HIT_TEST:
			SDL_Log("Window %d has a special hit test", event->window.windowID);
			break;
#endif
		default:
			SDL_Log("Window %d got unknown event %d",
					event->window.windowID, event->window.event);
			break;
		}
	}
}

void* CSDLWindow::getHandle()
{
	return m_Window;
}

int CSDLWindow::getWidth()
{
	int w = 0;
	SDL_GetWindowSize(m_Window, &w, nullptr);
	return w;
}

int CSDLWindow::getHeight()
{
	int h = 0;
	SDL_GetWindowSize(m_Window, nullptr, &h);
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

#ifdef LINUX
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
#endif

	/* Turn on double buffering with a 24bit Z buffer.
   * You may need to change this to 16 or 32 for your system */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	int posx  = SDL_WINDOWPOS_UNDEFINED;
	int posy  = SDL_WINDOWPOS_UNDEFINED;
	if (fullscreen)
	{
		posx = 0;
		posy = 0;
	}
	// Create window
	m_Window = SDL_CreateWindow(m_Title.c_str(), posx, posy, width, height, flags);
	if (m_Window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	// Create an OpenGL context associated with the window.
	//glThreadContext = SDL_GL_CreateContext(m_Window);
	glRenderContext = SDL_GL_CreateContext(m_Window);
	if (!SDL_GL_MakeCurrent(m_Window, glRenderContext))
	{
		printf("Can't create context current! SDL_Error: %s\n", SDL_GetError());
	}
	return true;
}

Vec2 CSDLWindow::nextMousePos(Vec2& position)
{
	return Vec2();
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
	SDL_SetWindowSize(m_Window, w, h);
}

void CSDLWindow::setCursor(Cursor* cursor)
{
}

GLContext CSDLWindow::getContext()
{
	return static_cast<GLContext>(glThreadContext);
}

extern "C"
{
	IWindow* CreateIWindow()
	{
		return new CSDLWindow();
	}
}
