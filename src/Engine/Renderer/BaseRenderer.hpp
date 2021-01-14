#pragma once
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Renderer/AuxRenderer.hpp>
#include <BlackBox/Renderer/BufferManager.hpp>
#include <BlackBox/Renderer/IRender.hpp>
//#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Renderer/Quad.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/System/IConsole.hpp>

#include "Shaders/FxParser.h"
//#include <BlackBox/Renderer/FrameBufferObject.hpp>

extern FxParser* g_FxParser;
class Texture;
struct IFont;

class RenderCVars
{
  public:
	void InitCVars();
	~RenderCVars();

	int r_Width		 = 0;
	int r_Height	 = 0;
	int r_Bpp		 = 0;
	int r_Zbpp		 = 0;
	int r_Sbpp		 = 0;
	int r_Fullscreen = 0;

	int r_MSAA		   = 1;
	int r_MSAA_samples = 2;

	int r_Vsync		   = true;
	int r_DisplayIndex = 0;

	static int CV_r_GetScreenShot;
};
struct CBaseShaderProgram;
class ShaderMan
{
  public:
	~ShaderMan()
	{
	}
	IShaderProgram* Sh_Load(const char* vertex, const char* fragment)
	{
		#if 0
		using ShaderInfo = IShaderProgram::ShaderInfo;
		auto* vs		 = CShader::Load(ShaderDesc(vertex, IShader::E_VERTEX));
		auto* fs		 = CShader::Load(ShaderDesc(fragment, IShader::E_FRAGMENT));
		auto* p			 = new CShaderProgram(ShaderInfo(vs, std::string(vertex)), ShaderInfo(fs, std::string(fragment)));
		p->Create((std::string(vertex) + std::string(fragment)).data());
		m_Shaders.emplace_back(p);
		return p;
		#else
		return nullptr;
		#endif
	}
	IShaderProgram* Sh_Load(const char* name, int flags, uint64 nMaskGen)
	{
		#if 0
		CBaseShaderProgram* p = nullptr;
		if (!(p = Sh_LoadBinary(name, flags, nMaskGen)))
		{
			if (p = Compile(name, flags, nMaskGen))
			{
				p->SaveBinaryShader(name, flags, nMaskGen);
			}
		}
		return p;
		#else
		return nullptr;
		#endif
	}
	CBaseShaderProgram* Sh_LoadBinary(const char* name, int flags, uint64 nMaskGen) const
	{
		#if 0
		return gEnv->pConsole->GetCVar("r_SkipShaderCache")->GetIVal() ? nullptr : CBaseShaderProgram::LoadBinaryShader(name, flags, nMaskGen);
		#else
		return nullptr;
		#endif
	}

	CBaseShaderProgram* Compile(std::string_view name, int flags, uint64 nMaskGen)
	{
		using ShaderInfo = IShaderProgram::ShaderInfo;

		PEffect pEffect = nullptr;
		std::stringstream path;
		path << "res/shaders/fx/" << name << ".fx";
		if (g_FxParser->Parse(path.str().data(), &pEffect))
		{
			CryLog("Dumping shaders of effect: %s", name.data());
			for (auto i = 0; i < pEffect->GetNumShaders(); i++)
			{
				auto& str = pEffect->GetShader(i).name;
				CryLog("[%s]", str.c_str());
			}
			#if 0
			const auto vs		 = CShader::LoadFromEffect(pEffect, IShader::E_VERTEX, true);
			const auto fs		 = CShader::LoadFromEffect(pEffect, IShader::E_FRAGMENT, true);
			auto* p				 = new CShaderProgram(vs, fs);
			p->Create(name.data());
			delete pEffect;
			m_Shaders.emplace_back(p);
			return p;
			#else
			return nullptr;
			#endif
		}
		return nullptr;
	}
	void ReloadAll()
	{
		#if 0
		for (auto& s : m_Shaders)
		{
			s->Reload();
		}
		#endif
	}

	//std::vector<_smart_ptr<CShaderProgram>> m_Shaders;
};

class CRenderer : public RenderCVars
	, public IRenderer
	, public IConsoleVarSink
	, public ISystemEventListener
{
  public:
	// Inherited via IConsoleVarSink
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) final;
	virtual void OnAfterVarChange(ICVar* pVar) final;
	virtual void OnVarUnregister(ICVar* pVar) final;
	// Inherited via ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) final;
	CRenderer(ISystem* engine);
	~CRenderer();
	//! Init the renderer, params are self-explanatory
	virtual IWindow* Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window = nullptr) = 0;
	virtual bool InitOverride()																													   = 0;

	//! Changes resolution of the window/device (doen't require to reload the level
	virtual bool ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen) = 0;

	//! Shut down the renderer
	virtual void Release() final;

	//! Should be called at the beginning of every frame
	virtual void BeginFrame(void) = 0;

	//! Should be called at the end of every frame
	virtual void Update(void) = 0;

	virtual void GetViewport(int* x, int* y, int* width, int* height)			  = 0;
	virtual void SetViewport(int x = 0, int y = 0, int width = 0, int height = 0) = 0;
	virtual void SetScissor(int x = 0, int y = 0, int width = 0, int height = 0)  = 0;

	//! Create a vertex buffer
	virtual CVertexBuffer* CreateBuffer(int vertexcount, int vertexformat, const char* szSource, bool bDynamic = false) final;

	//! Release a vertex buffer
	virtual void ReleaseBuffer(CVertexBuffer* bufptr) final;

	//! Draw a vertex buffer
	virtual void DrawBuffer(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start = 0, int vert_stop = 0, CMatInfo* mi = NULL) final;

	//! Update a vertex buffer
	virtual void UpdateBuffer(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs = 0, int Type = 0) final;

	virtual void CreateIndexBuffer(SVertexStream* dest, const void* src, int indexcount) final;
	//! Update indicies
	virtual void UpdateIndexBuffer(SVertexStream* dest, const void* src, int indexcount, bool bUnLock = true) final;
	virtual void ReleaseIndexBuffer(SVertexStream* dest) final;

	//! Draw a bbox specified by mins/maxs (debug puprposes)
	virtual void Draw3dBBox(const Vec3& mins, const Vec3& maxs) = 0;

	//! Set the renderer camera
	virtual void SetCamera(const CCamera& cam) final;

	//! Get the renderer camera
	virtual const CCamera& GetCamera() final;

	IShaderProgram* Sh_Load(const char* name, int flags, uint64 nMaskGen = 0) final;

	void Set2DMode(bool enable, int ortox, int ortoy) final;

	//! Change display size
	virtual bool ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) = 0;

	//! Chenge viewport size
	virtual void ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

	//! Write a message on the screen with additional flags.
	//! for flags @see
	virtual void Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info) final;

	//! Draw a image using the current matrix
	virtual void DrawImage(float xpos, float ypos, float w, float h, uint64 texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a) = 0;

	virtual void DrawFullScreenImage(int texture_id) = 0;

	//! Set the polygon mode (wireframe, solid)
	virtual int SetPolygonMode(int mode) = 0;

	//! Get screen width
	virtual int GetWidth() final;

	//! Get screen height
	virtual int GetHeight() final;

	//! Get a screenshot and save to a file
	virtual void ScreenShot(const char* filename = nullptr) = 0;

	virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) = 0;

	virtual void PrintLine(const char* szText, SDrawTextInfo& info) final;

	ITexture* LoadTexture(const char* nameTex, uint flags, byte eTT) final;

	virtual int EnumDisplayFormats(SDispFormat* formats);

	virtual void SetState(State state, bool enable)			 = 0;
	virtual void SetCullMode(CullMode mode = CullMode::BACK) = 0;

	/////////////////////////////////////////////////////////////////////////////////
	// Render-context management
	/////////////////////////////////////////////////////////////////////////////////
	virtual bool DeleteContext(WIN_HWND hWnd)												= 0;
	virtual bool CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX = 1, int SSY = 1) = 0;
	virtual bool SetCurrentContext(WIN_HWND hWnd)											= 0;
	virtual void MakeMainContextActive()													= 0;
	virtual WIN_HWND GetCurrentContextHWND()												= 0;
	virtual bool IsCurrentContextMainVP()													= 0;

	//! Gets height of the current viewport.
	virtual int GetCurrentContextViewportHeight() const = 0;

	//! Gets width of the current viewport.
	virtual int GetCurrentContextViewportWidth() const = 0;
	/////////////////////////////////////////////////////////////////////////////////

	// 3d engine set this color to fog color
	virtual void SetClearColor(const Vec3& vColor)	 = 0;
	virtual void ClearDepthBuffer()					 = 0;
	virtual void ClearColorBuffer(const Vec3 vColor) = 0;

	virtual void SetRenderTarget(int nHandle) = 0;

	virtual void ProjectToScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz) final;
	virtual int UnProject(float sx, float sy, float sz, float* px, float* py, float* pz, const float modelMatrix[16], const float projMatrix[16], const int viewport[4]) final;
	virtual int UnProjectFromScreen(float sx, float sy, float sz, float* px, float* py, float* pz) final;
	virtual void GetModelViewMatrix(float* mat) final;
	virtual void GetModelViewMatrix(double* mat) final;
	virtual void GetProjectionMatrix(double* mat) final;
	virtual void GetProjectionMatrix(float* mat) final;
	virtual Vec3 GetUnProject(const Vec3& WindowCoords, const CCamera& cam) final;
	virtual int GetFrameID(bool bIncludeRecursiveCalls = true) final;
	IGraphicsDeviceConstantBuffer * CreateConstantBuffer(int size) final;

	void CreateQuad();
	IFont* GetIFont() final;
	IRenderAuxGeom* GetIRenderAuxGeom() final;

protected:

	struct alignas(16) SPerViewConstantBuffer
	{
		Mat4 Projection;
		Mat4 OrthoProjection;
		Mat4 View;
		Mat4 ViewProjection;
		Vec3 Eye;
	};
	struct alignas(16) SScreenConstantBuffer
	{
		glm::mat4 Model;
		glm::mat4 UvProjection;
		glm::vec4 Color;
		float Alpha;
	};

	#if 0
	using PerViewBuffer	   = gl::ConstantBuffer<SPerViewConstantBuffer>;
	using PerViewBufferPtr = std::shared_ptr<PerViewBuffer>;
	PerViewBufferPtr perViewBuffer;

	using ScreenBuffer	  = gl::ConstantBuffer<SScreenConstantBuffer>;
	using ScreenBufferPtr = std::shared_ptr<ScreenBuffer>;
	ScreenBufferPtr screenBuffer;
	#endif

	void InitConsoleCommands() const;

	//private:
	IWindow* m_Window  = nullptr;
	ISystem* m_pSystem = nullptr;

	bool is_fullscreen = false;
	Vec4 m_viewPort;
	unsigned int cbpp = 0;
	int zbpp		  = 0;
	int sbits		  = 0;

	bool bInFullScreen = false;
	//============
	CCamera m_Camera;
	//============

	IRenderAuxGeom* m_RenderAuxGeom;
	CBufferManager* m_BufferManager;

	CVertexBuffer* m_VertexBuffer = nullptr;
	BaseShaderProgramRef m_ScreenShader;

	#if 0
	FrameBufferObject* m_MainMSAAFrameBuffer;
	FrameBufferObject* m_MainReslovedFrameBuffer;
	#endif

	std::vector<Texture*> m_RenderTargets;
	void* context;

	bool transit_to_FS = false;
	bool bIsActive	   = true;

	int m_FrameID = 0;

	// Windows context
	char     m_WinTitle[80];
	WIN_HWND m_hWnd;                  // The main app window
	WIN_HWND m_hWndDesktop;           // The desktop window
	WIN_HWND m_hWndActive;            // The active window
#if BB_PLATFORM_WINDOWS
	HICON    m_hIconBig;              // Icon currently being used on the taskbar
	HICON    m_hIconSmall;            // Icon currently being used on the window
	HCURSOR  m_hCursor;               // Cursor currently being used on the window
	string   m_iconPath;              // Path to the icon currently loaded
#endif

	std::vector<IFont*> m_Fonts;
	RenderBackend m_Backend;
};
