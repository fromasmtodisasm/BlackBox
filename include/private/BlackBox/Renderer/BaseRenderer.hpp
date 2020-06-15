#pragma once
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Renderer/Quad.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/Input/IInput.hpp>

class RenderCVars
{
public:
  void InitCVars();
  ~RenderCVars();

  int r_Width = 0;
  int r_Height = 0;
  int r_Bpp = 0;
  int r_Zbpp = 0;
  int r_Sbpp = 0;
  int r_Fullscreen = 0;

  int r_MSAA = 1;
  int r_MSAA_samples = 2;

  int r_Vsync = true;
  int r_DisplayIndex = 0;
};

class CRenderer :
  public RenderCVars,
  public IRenderer
{
  //! Init the renderer, params are self-explanatory
  virtual IWindow* Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window = nullptr) = 0;

  //! Changes resolution of the window/device (doen't require to reload the level
  virtual bool	ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen) = 0;

  //! Shut down the renderer
  virtual void	Release() = 0;

  //! Should be called at the beginning of every frame
  virtual void	BeginFrame(void) = 0;

  //! Should be called at the end of every frame
  virtual void	Update(void) = 0;

  virtual	void	GetViewport(int* x, int* y, int* width, int* height) = 0;
  virtual	void	SetViewport(int x = 0, int y = 0, int width = 0, int height = 0) = 0;
  virtual	void	SetScissor(int x = 0, int y = 0, int width = 0, int height = 0) = 0;

  //! Draw a bbox specified by mins/maxs (debug puprposes)
  virtual	void	Draw3dBBox(const Vec3& mins, const Vec3& maxs) = 0;

  //! Set the renderer camera
  virtual	void	SetCamera(const CCamera& cam) = 0;

  //! Get the renderer camera
  virtual	const CCamera& GetCamera() = 0;

  //! Change display size
  virtual bool	ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) = 0;

  //! Chenge viewport size
  virtual void  ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

  //! Write a message on the screen with additional flags.
  //! for flags @see
  virtual void	Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info) = 0;

  //! Draw a image using the current matrix
  virtual void DrawImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a) = 0;

  virtual void DrawFullScreenImage(int texture_id) = 0;

  //! Set the polygon mode (wireframe, solid)
  virtual int	SetPolygonMode(int mode) = 0;

  //! Get screen width
  virtual	int		GetWidth() = 0;

  //! Get screen height
  virtual	int		GetHeight() = 0;

  //! Get a screenshot and save to a file
  virtual void ScreenShot(const char* filename = nullptr) = 0;

  virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) = 0;

  virtual void PrintLine(const char* szText, SDrawTextInfo& info) = 0;

  virtual int	 EnumDisplayFormats(SDispFormat* formats);

  virtual void  SetState(State state, bool enable) = 0;
  virtual void  SetCullMode(CullMode mode = CullMode::BACK) = 0;

  /////////////////////////////////////////////////////////////////////////////////
// Render-context management
/////////////////////////////////////////////////////////////////////////////////
  virtual bool     DeleteContext(WIN_HWND hWnd) = 0;
  virtual bool     CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX = 1, int SSY = 1) = 0;
  virtual bool     SetCurrentContext(WIN_HWND hWnd) = 0;
  virtual void     MakeMainContextActive() = 0;
  virtual WIN_HWND GetCurrentContextHWND() = 0;
  virtual bool     IsCurrentContextMainVP() = 0;

  //! Gets height of the current viewport.
  virtual int GetCurrentContextViewportHeight() const = 0;

  //! Gets width of the current viewport.
  virtual int GetCurrentContextViewportWidth() const = 0;
  /////////////////////////////////////////////////////////////////////////////////

  // 3d engine set this color to fog color
  virtual void SetClearColor(const Vec3& vColor) = 0;
  virtual void ClearDepthBuffer() = 0;
  virtual void ClearColorBuffer(const Vec3 vColor) = 0;

  virtual void SetRenderTarget(int nHandle) = 0;

  //virtual IShaderProgram* Sh_Load(ShaderDesc const& desc) = 0;
  virtual int GetFrameID(bool bIncludeRecursiveCalls=true) = 0;

};

