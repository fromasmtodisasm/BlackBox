#pragma once
#include <BlackBox/Renderer/BaseRenderer.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>
#include <BlackBox/Renderer/BufferManager.hpp>
#include <BlackBox/Renderer/FrameBufferObject.hpp>
#include <BlackBox/Renderer/Sampler.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>

struct IWindow;
class World;

typedef int glContextAttribute;

enum AttributeType
{
  DEBUG,
  RELEASE,
  CORE
};

struct Hardware
{
  const GLubyte* unknown_desc = nullptr;
  Hardware()
    :
      vendor(unknown_desc),
      render(unknown_desc),
      version(unknown_desc),
      glsl_version(unknown_desc)
  {

  }
  const GLubyte* vendor = nullptr;
  const GLubyte* render = nullptr;
  const GLubyte* version = nullptr;
  const GLubyte* glsl_version = nullptr;
};

class GLRenderer :
  public CRenderer,
  public IConsoleVarSink,
  public IInputEventListener,
  public ISystemEventListener
{
public:
  GLRenderer(ISystem* engine);
  ~GLRenderer();

public:
  // Inherited via IConsoleVarSink
  virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) override;
	virtual void OnAfterVarChange(ICVar* pVar) override;
	virtual void OnVarUnregister(ICVar* pVar) override;
  // Inherited via IInputEventListener
  virtual bool OnInputEvent(const SInputEvent& event) override;
  virtual void SetRenderTarget(int nHandle) override;
  // Inherited via ISystemEventListener
  virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
  // Inherited via IRenderer
  virtual IWindow* Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window = nullptr) override;
  virtual bool ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen) override;
  virtual void Release() override;
  virtual void BeginFrame(void) override;
  virtual void Update(void) override;
  virtual void GetViewport(int* x, int* y, int* width, int* height) override;
  virtual void SetViewport(int x = 0, int y = 0, int width = 0, int height = 0) override;
  virtual void SetScissor(int x = 0, int y = 0, int width = 0, int height = 0) override;
  virtual void Draw3dBBox(const Vec3& mins, const Vec3& maxs) override;
  virtual void SetCamera(const CCamera& cam) override;
  virtual const CCamera& GetCamera() override;
  virtual bool ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) override;
  virtual void ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
  virtual void Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info) override;
  virtual int SetPolygonMode(int mode) override;
  virtual int GetWidth() override;
  virtual int GetHeight() override;
  virtual void ScreenShot(const char* filename = nullptr) override;
  virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) override;
  virtual void DrawImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a) override;
  virtual void PrintLine(const char* szText, SDrawTextInfo& info) override;
  virtual void SetState(State state, bool enable) override;
  virtual void DrawFullScreenImage(int texture_id) override;
  virtual void SetCullMode(CullMode mode = CullMode::BACK) override;
  virtual bool DeleteContext(WIN_HWND hWnd) override;
  virtual bool CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX = 1, int SSY = 1) override;
  virtual bool SetCurrentContext(WIN_HWND hWnd) override;
  virtual void MakeMainContextActive() override;
  virtual WIN_HWND GetCurrentContextHWND() override;
  virtual bool IsCurrentContextMainVP() override;
  virtual int GetCurrentContextViewportHeight() const override;
  virtual int GetCurrentContextViewportWidth() const override;
  virtual void SetClearColor(const Vec3& vColor) override;
  virtual void ClearDepthBuffer() override;
  virtual void ClearColorBuffer(const Vec3 vColor) override;

  virtual IShaderProgram* Sh_Load(const char* name, int flags) override;
  virtual IShaderProgram* Sh_Load(const char* vertex, const char* fragment) override;

  virtual void DrawFullscreenQuad() override;
  virtual int GetFrameID(bool bIncludeRecursiveCalls = true) override;
	virtual void Set2DMode(bool enable, int ortox, int ortoy) override;
	virtual CVertexBuffer* CreateBuffer(int vertexcount, int vertexformat, const char* szSource, bool bDynamic = false) override;
	virtual void ReleaseBuffer(CVertexBuffer* bufptr) override;
	virtual void DrawBuffer(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start = 0, int vert_stop = 0, CMatInfo* mi = NULL) override;
	virtual void UpdateBuffer(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs = 0, int Type = 0) override;
	virtual void CreateIndexBuffer(SVertexStream* dest, const void* src, int indexcount) override;
	virtual void UpdateIndexBuffer(SVertexStream* dest, const void* src, int indexcount, bool bUnLock = true) override;
	virtual void ReleaseIndexBuffer(SVertexStream* dest) override;
	virtual void ProjectToScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz) override;
	virtual int UnProject(float sx, float sy, float sz, float* px, float* py, float* pz, const float modelMatrix[16], const float projMatrix[16], const int viewport[4]) override;
	virtual int UnProjectFromScreen(float sx, float sy, float sz, float* px, float* py, float* pz) override;
	virtual void GetModelViewMatrix(float* mat) override;
	virtual void GetModelViewMatrix(double* mat) override;
	virtual void GetProjectionMatrix(double* mat) override;
	virtual void GetProjectionMatrix(float* mat) override;
	virtual Vec3 GetUnProject(const Vec3& WindowCoords, const CCamera& cam) override;

  virtual IRenderAuxGeom* GetIRenderAuxGeom() override;

  virtual void PushProfileMarker(char* label) override;
  virtual void PopProfileMarker(char* label) override;

  virtual ITexture* LoadTexture(const char* nameTex, uint flags, byte eTT) override;
  virtual IFont* GetIFont() override;
private:
  void glInit();
  void fillSates();
  void InitConsoleCommands();
  void printHardware();
  void AquireVB();
  bool VBF_InPool(int format);
  bool InitResourceManagers();
  void ShareWith(GLRenderer* renderer);

	void CreateQuad();

private:
  IWindow* m_Window = nullptr;
  ISystem* m_pSystem = nullptr;

  bool is_fullscreen = false;
  Vec4 m_viewPort;
  unsigned int cbpp = 0;
  int zbpp = 0;
  int sbits = 0;

  bool bInFullScreen = false;

  //============
  const GLuint majorVersion = 4;
  const GLuint minorVersion = 3;
  const int antialiassing = 8;
	std::shared_ptr<OpenglDebuger> glDebug;
  //============
  CCamera m_Camera;
  // Shaders
  BaseShaderProgramRef m_ScreenShader;
  BaseShaderProgramRef m_AuxGeomShader;
#if defined(_DEBUG) || defined(GL_DEBUG) || !defined(NDEBUG)
  bool isDebug = true;
#else
  bool isDebug = false;
#endif
#if defined(_DEBUG) || defined(GL_DEBUG)
  glContextAttribute glContextType = AttributeType::DEBUG;
#else
  glContextAttribute glContextType = AttributeType::CORE;
#endif
  // DEBUG

  std::map<State, GLenum> stateMap;

  HWND m_HWND;
  void* context;

  Vec3 m_clearColor = {0.2, 0.2, 0.2};
  float m_clearDepth;

  int m_CurrentTarget;

  Hardware m_Hardware;

	//std::vector<SVertexPoolEntry> m_VertexBufferPool;
	int m_FrameID = 0;

  CRenderAuxGeom* m_RenderAuxGeom;
  CBufferManager* m_BufferManager;

	CVertexBuffer* m_VertexBuffer = nullptr;

  std::vector<CShaderProgram*> m_Shaders;
	FrameBufferObject* m_MainMSAAFrameBuffer;
	FrameBufferObject* m_MainReslovedFrameBuffer;
  IRenderCallback* m_pRenerCallback = nullptr;
	std::vector<Texture*> m_RenderTargets;

  bool transit_to_FS = false;
	bool bIsActive	   = true;


  // Inherited via CRenderer
  virtual IGraphicsDeviceConstantBuffer* CreateConstantBuffer(int size) override;


  // Inherited via CRenderer
  virtual ITechniqueManager* GetITechniqueManager() override;


// Унаследовано через CRenderer
  virtual float GetDepthValue(int x, int y) override;

  // Inherited via CRenderer
  virtual void Sh_Reload() override;

  // Inherited via CRenderer
  virtual int CreateRenderTarget() override;

  // Inherited via CRenderer
  virtual void SetRenderCallback(IRenderCallback* pCallback) override;

  // Inherited via CRenderer
  virtual void Flush() override;

  // Inherited via CRenderer
  virtual void ShareResources(IRenderer* renderer) override;
};

extern class ShaderMan* gShMan;
