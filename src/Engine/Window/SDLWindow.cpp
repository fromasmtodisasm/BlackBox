#include <BlackBox/SDLWindow.hpp>
#include <BlackBox/Render/BaseTexture.hpp>

#include <SDL2/SDL.h>

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
  //m_Window->setVerticalSyncEnabled(true);
  //m_Window->setFramerateLimit(60);
  //m_Window->setMouseCursorGrabbed(true);

#ifdef GUI
  ImGui::SFML::Init(*m_Window);
#endif // GUI

  // Make it the active window for OpenGL calls
  //m_Window->setActive();

  // Input handling specific
  //Mouse.curr_pos = Mouse.curr_pos = sf::Mouse::getPosition(*m_Window);
  Mouse.x_wraped = Mouse.y_wraped = false;
  Mouse.locked = false;

	//auto ip = GetISystem()->GetIConsole()->GetCVar("image_path");
	//if (ip)
	//{
		//std::string icon(ip->GetString());
		Image img;
		if (img.load("res/images/icon.jpg", nullptr))
		{
			;// m_Window->setIcon(img.width, img.height, static_cast<sf::Uint8*>(img.data));
		}
	//}

  return true;

}

void CSDLWindow::update()
{
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
}

void CSDLWindow::setTitle(const char*)
{
}

void CSDLWindow::show()
{
}

void* CSDLWindow::getHandle()
{
	return nullptr;
}

int CSDLWindow::getWidth()
{
	return 0;
}

int CSDLWindow::getHeight()
{
	return 0;
}

void CSDLWindow::setFlags(int flags)
{
}

bool CSDLWindow::Create(int width, int height, bool fullscreen)
{
	// Create window
	m_Window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, DEFAULT_WIDTH, DEFAULT_HEIGHT, SDL_WINDOW_SHOWN);
	if (m_Window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

Vec2 CSDLWindow::nextMousePos(Vec2& position)
{
	return Vec2();
}

void CSDLWindow::setMouseWrap(bool wrap)
{
}

void CSDLWindow::glInit()
{
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
}

void CSDLWindow::setCursor(Cursor* cursor)
{
}

extern "C" {
	inline IWindow* CreateIWindow()
	{
		return new CSDLWindow();
	}
}
