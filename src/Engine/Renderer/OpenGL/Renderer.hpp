#pragma once
#include "../BaseRenderer.hpp"
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/OpenGL/Core.hpp>
#include <BlackBox/Renderer/BufferManager.hpp>
#include <BlackBox/Renderer/FrameBufferObject.hpp>
#include <BlackBox/Renderer/Sampler.hpp>
#include <BlackBox/Renderer/VertexFormats.hpp>

struct IWindow;
class World;

enum class Samplers : int
{
	Default = 0,
	Nearest_Nearest,
	Linear_Nearest,
	Nearest_Linear,
	Linear_Linear,

	Font_Sampler,
	Count
};

struct D3D11_SAMPLER_DESC
{
	GLenum Filter		  = GL_NONE;
	GLenum AddressU		  = GL_CLAMP_TO_EDGE;
	GLenum AddressV		  = GL_CLAMP_TO_EDGE;
	GLenum AddressW		  = GL_CLAMP_TO_EDGE;
	FLOAT MipLODBias	  = 0.f;
	UINT MaxAnisotropy	  = 2;
	GLenum ComparisonFunc = GL_NONE;
	FLOAT BorderColor[4]  = {0, 0, 0, 0};
	FLOAT MinLOD		  = 0.f;
	FLOAT MaxLOD		  = 0.f;

	static D3D11_SAMPLER_DESC Create_Default()
	{
		return D3D11_SAMPLER_DESC();
	}

	static D3D11_SAMPLER_DESC Create_Font()
	{
		return Create_Default();
	}

};


extern GLuint g_Samplers[Samplers::Count];

typedef int glContextAttribute;

enum class AttributeType : int
{
	Debug,
	Release,
	Core
};

struct Hardware
{
	const GLubyte* unknown_desc = nullptr;
	Hardware()
		: vendor(unknown_desc),
		  render(unknown_desc),
		  version(unknown_desc),
		  glsl_version(unknown_desc)
	{
	}
	const GLubyte* vendor		= nullptr;
	const GLubyte* render		= nullptr;
	const GLubyte* version		= nullptr;
	const GLubyte* glsl_version = nullptr;
};

class GLRenderer : public CRenderer
{
  public:
	GLRenderer(ISystem* engine);
	~GLRenderer();

  public:
	// Inherited via IRenderer
	virtual void SetRenderTarget(int nHandle) override;

	virtual IWindow* Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window = nullptr) override;
	virtual bool ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen) override;
	virtual void BeginFrame(void) override;
	virtual void Update(void) override;
	virtual void GetViewport(int* x, int* y, int* width, int* height) override;
	virtual void SetViewport(int x = 0, int y = 0, int width = 0, int height = 0) override;
	virtual void SetScissor(int x = 0, int y = 0, int width = 0, int height = 0) override;
	virtual void Draw3dBBox(const Vec3& mins, const Vec3& maxs) override;
	virtual bool ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) override;
	virtual void ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
	virtual int SetPolygonMode(int mode) override;
	virtual void ScreenShot(const char* filename = nullptr) override;
	virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) override;
	virtual void DrawImage(float xpos, float ypos, float w, float h, uint64 texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a) override;
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

	virtual void DrawFullscreenQuad() override;
	virtual void Set2DMode(bool enable, int ortox, int ortoy) override;

	virtual IRenderAuxGeom* GetIRenderAuxGeom() override;

	virtual void PushProfileMarker(char* label) override;
	virtual void PopProfileMarker(char* label) override;

	virtual IFont* GetIFont() override;
	virtual ITechniqueManager* GetITechniqueManager() override;
	virtual float GetDepthValue(int x, int y) override;
	virtual void Sh_Reload() override;
	virtual int CreateRenderTarget() override;
	virtual void SetRenderCallback(IRenderCallback* pCallback) override;
	virtual void Flush() override;
	virtual void ShareResources(IRenderer* renderer) override;

  private:
	void GlInit();
	void FillSates();
	void PrintHardware();
	void AquireVB();
	bool VBF_InPool(int format);
	bool InitResourceManagers();

  private:
	const GLuint majorVersion = 4;
	const GLuint minorVersion = 3;
	const int antialiassing	  = 8;
	std::shared_ptr<OpenglDebuger> glDebug;
	// Shaders
	BaseShaderProgramRef m_ScreenShader;
#if defined(_DEBUG) || defined(GL_DEBUG) || !defined(NDEBUG)
	bool isDebug = true;
#else
	bool isDebug					 = false;
#endif
#if defined(_DEBUG) || defined(GL_DEBUG)
	glContextAttribute glContextType = (int)AttributeType::Debug;
#else
	glContextAttribute glContextType = (int)AttributeType::Core;
#endif
	// DEBUG

	std::map<State, GLenum> stateMap;

	HWND m_HWND;
	void* context;

	Vec4 m_clearColor = {
		0.2, 0.2, 0.2, 1.0
		//0.0, 0.0, 0.0
	};

	Hardware m_Hardware;

	//std::vector<SVertexPoolEntry> m_VertexBufferPool;
	std::vector<CShaderProgram*> m_Shaders;
	FrameBufferObject* m_MainMSAAFrameBuffer;
	FrameBufferObject* m_MainReslovedFrameBuffer;
	IRenderCallback* m_pRenderCallback = nullptr;
	std::vector<Texture*> m_RenderTargets;

};

extern class ShaderMan* gShMan;
