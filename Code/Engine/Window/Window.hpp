#pragma once
#include <BlackBox/System/IInputHandler.hpp>
#include <BlackBox/System/IWindow.hpp>
//#include <BlackBox/Math/MathHelper.hpp>
#include <SDL_events.h>
#include <SDL_video.h>

#include <list>
#include <map>
#include <string>

class CSDLWindow final : public IWindow
{
  public:
	friend class GameGUI;
	Rect viewPort;
	enum FLAGS
	{
		DRAW_GUI
	};
	CSDLWindow(NativeWindow* hwnd, std::string = DEFAULT_TITLE, int width = DEFAULT_WIDTH, int height = DEFAULT_HEIGHT);
	~CSDLWindow();

	//symotion-prefix) IWindow interface
	virtual bool  init(SInitParams* pInitParams) override;
	virtual void  update() override;
	virtual void  clear() override;
	virtual bool  closed() override;
	virtual void  swap() override;
	virtual void  setTitle(const char* title) override;
	virtual void  show() override;
	virtual int	  getWidth() override;
	virtual int	  getHeight() override;
	virtual void  setFlags(int flags) override;
	virtual Rect& getViewPort() override;
	virtual bool  create(Params params) override;
	virtual void  changeSize(int w, int h) override;
	//virtual void setCursor(Cursor* cursor) override;
	virtual GLContext getContext() override;
	virtual void	  close() override;

  public:
	virtual void* getHandle() override;
	virtual void* getNativeHandle() override;

  private:
	bool	   Create(int width, int height, bool fullscreen, RenderBackend backend);
	bool	   CreateDx(int width, int height, bool fullscreen);
	bool	   InitForGl();
	void	   SelectDisplay(int& x, int& y, int w, int h);
	void	   handleEvent(SDL_Event& event);
	void	   SetIcon(char* path);
	static int EventWatcher(void* self, SDL_Event* event);

  private:
	static constexpr int		 DEFAULT_WIDTH	= 1024;
	static constexpr int		 DEFAULT_HEIGHT = 768;
	static constexpr const char* DEFAULT_TITLE	= "SDL Window";

	SDL_Window*	  m_MainWindow;
	SDL_Window*	  m_SecondaryWindow;
	RenderBackend m_RendererBackend = RenderBackend::GL;
	NativeWindow* m_hWnd{};

	bool m_bClose;
	int	 m_Width;
	int	 m_Height;
	bool bInFullScreen = false;
	//TODO: SDL CONTEXT SETTINGS
	//sf::ContextSettings m_contextSettings;
	//===============================================================================
	std::string m_Title;
	//Legacy::Vec4 m_BackColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	int				m_flags = 0;
	SDL_DisplayMode m_DesktopMode;
	// For input handling
	//std::list<IInputEventListener*> listeners;

	// IWindow interface
  public:
	virtual void		Release() override;
	virtual DisplayMode GetDesktopMode() override;
	virtual void		EnterFullscreen(bool mode) override;
};
