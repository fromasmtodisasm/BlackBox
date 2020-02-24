#pragma once

#ifdef WIN32
#ifdef IRENDER_EXPORTS
#define IRENDER_API __declspec(dllexport)
#else
#ifdef DLL
#define IRENDER_API __declspec(dllimport)
#else
#define IRENDER_API
#endif
#endif
#else
#define IRENDER_API
#endif

// Global typedefs.
//////////////////////////////////////////////////////////////////////
#ifndef uchar
typedef unsigned char		uchar;
typedef unsigned int		uint;
typedef unsigned short	ushort;
#endif

#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IShader.hpp>

// Forward declarations.
//////////////////////////////////////////////////////////////////////
typedef void* WIN_HWND;
typedef void* WIN_HINSTANCE;
typedef void* WIN_HDC;
typedef void* WIN_HGLRC;

struct ISystem;
struct IWindow;
struct IFont;
class CCamera;
struct IShader;

class Image
{
public:
  int width;
  int height;
  int bpp;
  uint8_t* data;

  Image() : width(0), height(0), bpp(0), data(nullptr) {}
  ~Image();
  bool load(const char* name, bool* hasAlpha);
  void free();
};

struct ITexture
{
  virtual int             AddRef() { return 0; }
  virtual int             Release() { return 0; }
  virtual bool load(const char* name) = 0;
  virtual void bind() = 0;
  virtual void setUnit(uint unit) = 0;
  virtual void setType(const char* TextureType) = 0;
  virtual const char* typeToStr() = 0;
  virtual int getWidth() = 0;
  virtual int getHeight() = 0;
  virtual int getId() = 0;
};

//////////////////////////////////////////////////////////////////////////
//! This structure used in DrawText method of renderer.
//! It provide all necesarry information of how to render text on screen.
//! @see IRenderer::Draw2dText
struct SDrawTextInfo
{
  //! One of EDrawTextFlags flags.
  //! @see EDrawTextFlags
  int			flags;
  //! Text color, (r,g,b,a) all members must be specified.
  float		color[4];
  float xscale;
  float yscale;
  IFont* font;

  SDrawTextInfo() : xscale(1.0), yscale(1.0)
  {
    flags = 0;
    color[0] = color[1] = color[2] = color[3] = 1;
    font = 0;
  }
};

//////////////////////////////////////////////////////////////////////

struct SDispFormat
{
  int m_Width;
  int m_Height;
  int m_BPP;
  SDispFormat() {}
};

struct IRenderer
{
  enum class State
  {
    DEPTH_TEST,
    CULL_FACE,
    BLEND,
    DEBUG_OUTPUT,
    DEBUG_OUTPUT_SYNCHRONOUS,
    FRAMEBUFFER_SRGB,
    POLYGON_OFFSET_FILL,
    SCISSOR_TEST,
    STENCIL_TEST,
    PROGRAM_POINT_SIZE 
  };

  enum class CullMode : unsigned int
  {
    /*DISABLE,
    NONE,*/
    FRONT,
    BACK,
    FRONT_AND_BACK
  };
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

  virtual int	 EnumDisplayFormats(SDispFormat* formats) = 0;

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

  ////////////////////////////////////////////////////////////////////////////////
  virtual IShader* Sh_Load(ShaderDesc const& desc) = 0;
};

extern "C" {
  IRENDER_API IRenderer* CreateIRender(ISystem* pSystem/*, void* hinst, void* hwnd, bool usedinput*/);
}
