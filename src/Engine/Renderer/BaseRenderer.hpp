#pragma once
#include <BlackBox/Core/Platform/Windows.hpp>
//#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Renderer/AuxRenderer.hpp>
//#include "BufferManager.hpp"
#include <BlackBox/Renderer/IRender.hpp>
//#include <BlackBox/Renderer/Shader.hpp>
#include <BlackBox/Renderer/Camera.hpp>
#include <BlackBox/Renderer/Quad.hpp>
#include <BlackBox/System/IConsole.hpp>

#include "Shaders/FxParser.h"
#ifdef VK_RENDERER
#	include "Vulkan/Shader.hpp"
#elif DX_RENDERER
#	include "D3D/Shader.hpp"
#endif
#ifndef VK_RENDERER
#	include "TypedConstantBuffer.hpp"
#endif
//#include <BlackBox/Renderer/FrameBufferObject.hpp>

extern FxParser* g_FxParser;
class Texture;
struct IFont;

class CBufferManager;

class RenderDebugger
{
  public:
	RenderDebugger() = default;
	RenderDebugger(const char* file);
	~RenderDebugger();

	static inline void SetIgnore(bool v)
	{
		ignore = v;
	}
	static void checkError(const char* file, int line, const char* expr);

	static inline void PushGroup(uint id, int length, const char* message)
	{
		//glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, id, length, message);
	}
	static inline void PushGroup(uint id, std::string_view message)
	{
		PushGroup(id, message.size(), message.data());
	}

	static inline void EndGroup()
	{
		//glPopDebugGroup();
	}

  private:
#if 0
	static void APIENTRY callBack(GLenum source, GLenum type, GLuint id,
								  GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
#endif

	static inline const char* SOURCE_TO_STRING(int s)
	{
#if 0
		switch (s)
		{
			GET_SOURCE(DEBUG_SOURCE_API);
			GET_SOURCE(DEBUG_SOURCE_WINDOW_SYSTEM);
			GET_SOURCE(DEBUG_SOURCE_SHADER_COMPILER);
			GET_SOURCE(DEBUG_SOURCE_THIRD_PARTY);
			GET_SOURCE(DEBUG_SOURCE_APPLICATION);
			GET_SOURCE(DEBUG_SOURCE_OTHER);
		default:
#endif
		return "Unknown source";
#if 0
		}
#endif
	}

	static inline const char* TYPE_TO_STRING(int t)
	{
#if 0
		switch (t)
		{
			GET_TYPE(DEBUG_TYPE_ERROR);
			GET_TYPE(DEBUG_TYPE_DEPRECATED_BEHAVIOR);
			GET_TYPE(DEBUG_TYPE_UNDEFINED_BEHAVIOR);
			GET_TYPE(DEBUG_TYPE_PORTABILITY);
			GET_TYPE(DEBUG_TYPE_PERFORMANCE);
			GET_TYPE(DEBUG_TYPE_MARKER);
			GET_TYPE(DEBUG_TYPE_PUSH_GROUP);
			GET_TYPE(DEBUG_TYPE_POP_GROUP);
			GET_TYPE(DEBUG_TYPE_OTHER);
		default:
			return "Unknown type";
		}
#endif
		return "Unknown type";
	}

	static inline const char* SEVERITY_TO_STRING(int s)
	{
#if 0
		switch (s)
		{
			GET_SEVERITY(DEBUG_SEVERITY_HIGH);
			GET_SEVERITY(DEBUG_SEVERITY_MEDIUM);
			GET_SEVERITY(DEBUG_SEVERITY_LOW);
			GET_SEVERITY(DEBUG_SEVERITY_NOTIFICATION);
		default:
			return "Unknown severity";
		}
#endif
		return "Unknown severity";
	}

  private:
	//std::ofstream debug_file;
	static bool isError;
	static bool ignore;
};

//////////////////////////////////////////////////////////////////////////
//! CFrameProfilerSection is an auto class placed where code block need to be profiled.
//! Every time this object is constructed and destruted the time between constructor
//! and destructur is merged into the referenced CFrameProfiler instance.
//!
class CDebugSection
{
  public:
	inline CDebugSection(size_t length, const char* message)
	{
		RenderDebugger::PushGroup(0, static_cast<int>(length), message);
	}
	inline CDebugSection(std::string_view message)
	{
		RenderDebugger::PushGroup(0, static_cast<int>(message.size()), message.data());
	}
	inline ~CDebugSection()
	{
		RenderDebugger::EndGroup();
	}
};

#define DEBUG_GROUP(message) CDebugSection debugSection(sizeof(message), message)

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

	int r_Vsync			   = true;
	int r_DisplayIndex	   = 0;
	int r_GraphicsDeviceId = -1;

	static int CV_r_GetScreenShot;
};
class ShaderMan
{
  public:
	IShader* Sh_Load(const char* vertex, const char* fragment)
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
	IShader* Sh_Load(const char* name, int flags, uint64 nMaskGen)
	{
		CShader* p = nullptr;
		if (!(p = Sh_LoadBinary(name, flags, nMaskGen)))
		{
			if (p = Compile(name, flags, nMaskGen))
			{
				p->SaveBinaryShader(name, flags, nMaskGen);
			}
		}
		return p;
	}
	CShader* Sh_LoadBinary(const char* name, int flags, uint64 nMaskGen) const
	{
		return gEnv->pConsole->GetCVar("r_SkipShaderCache")->GetIVal() ? nullptr : CShader::LoadBinaryShader(name, flags, nMaskGen);
	}

	CShader* Compile(std::string_view name, int flags, uint64 nMaskGen)
	{
		PEffect			  pEffect = nullptr;
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
			auto vs = CShader::LoadFromEffect(pEffect, IShader::E_VERTEX);
			auto fs = CShader::LoadFromEffect(pEffect, IShader::E_FRAGMENT);
			if (!vs || !fs)
			{
				SAFE_DELETE(vs);
				SAFE_DELETE(fs);
				return nullptr;
			}
			auto p							  = _smart_ptr<CShader>(new CShader);
			p->m_Shaders[IShader::E_VERTEX]	  = vs;
			p->m_Shaders[IShader::E_FRAGMENT] = fs;
			p->AddRef();
			//m_Shaders.push_back(p);
			p->Bind();

			delete pEffect;
			return p;
		}
		return nullptr;
	}
	void ReloadAll()
	{
		for (auto& s : m_Shaders)
		{
			s->Reload();
		}
	}

	std::vector<_smart_ptr<CShader>> m_Shaders;
};

class CRenderer : public RenderCVars
	, public IRenderer
	, public IConsoleVarSink
	, public ISystemEventListener
{
  public:
	// Inherited via IRendererCallbackServer
	void RegisterCallbackClient(IRendererCallbackClient* pClient) override;
	void UnregisterCallbackClient(IRendererCallbackClient* pClient) override;

	// Inherited via IConsoleVarSink
	virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) final;
	virtual void OnAfterVarChange(ICVar* pVar) final;
	virtual void OnVarUnregister(ICVar* pVar) final;
	// Inherited via ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) final;
	CRenderer(ISystem* engine);
	virtual ~CRenderer();
	//! Init the renderer, params are self-explanatory
	virtual IWindow* Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window = nullptr) /* final*/;
	virtual bool	 InitOverride() = 0;

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
	virtual void Draw3dBBox(const Legacy::Vec3& mins, const Legacy::Vec3& maxs) = 0;

	//! Set the renderer camera
	virtual void SetCamera(const CCamera& cam) final;

	//! Get the renderer camera
	virtual const CCamera& GetCamera() final;

	IShader* Sh_Load(const char* name, int flags, uint64 nMaskGen = 0) final;

	void Set2DMode(bool enable, int ortox, int ortoy) final;

	//! Change display size
	virtual bool ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) = 0;

	//! Chenge viewport size
	virtual void ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

	//! Write a message on the screen with additional flags.
	//! for flags @see
	virtual void Draw2dText(float posX, float posY, const char* szText, const SDrawTextInfo& info) final;

	//! Draw a 2d image on the screen (Hud etc.)
	virtual void Draw2dImage(float xpos, float ypos, float w, float h, int texture_id, float s0 = 0, float t0 = 0, float s1 = 1, float t1 = 1, float angle = 0, float r = 1, float g = 1, float b = 1, float a = 1, float z = 1) = 0;

	//! Draw a image using the current matrix
	virtual void DrawImage(float xpos, float ypos, float w, float h, uint64 texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a) final;

	virtual void DrawFullScreenImage(int texture_id) = 0;

	//! Set the polygon mode (wireframe, solid)
	virtual int SetPolygonMode(int mode) = 0;

	//! Get screen width
	virtual int GetWidth() final;

	//! Get screen height
	virtual int GetHeight() final;

	//! Memory status information
	virtual void GetMemoryUsage(ICrySizer* Sizer) const =  0;

	//! Get a screenshot and save to a file
	virtual void ScreenShot(const char* filename = nullptr) = 0;

	virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) = 0;

	virtual void PrintLine(const char* szText, SDrawTextInfo& info) final;

	virtual int EnumDisplayFormats(SDispFormat* formats);

	virtual void SetState(State state, bool enable)			 = 0;
	//virtual void SetState(int State)						 = 0;
	virtual void SetCullMode(CullMode mode = CullMode::BACK) = 0;

	/////////////////////////////////////////////////////////////////////////////////
	// Render-context management
	/////////////////////////////////////////////////////////////////////////////////
	virtual bool	 DeleteContext(WIN_HWND hWnd)												= 0;
	virtual bool	 CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX = 1, int SSY = 1) = 0;
	virtual bool	 SetCurrentContext(WIN_HWND hWnd)											= 0;
	virtual void	 MakeMainContextActive()													= 0;
	virtual WIN_HWND GetCurrentContextHWND()													= 0;
	virtual bool	 IsCurrentContextMainVP()													= 0;

	//! Gets height of the current viewport.
	virtual int GetCurrentContextViewportHeight() const = 0;

	//! Gets width of the current viewport.
	virtual int GetCurrentContextViewportWidth() const = 0;
	/////////////////////////////////////////////////////////////////////////////////

	// 3d engine set this color to fog color
	void SetClearColor(const Legacy::Vec3& vColor)
	{
		m_ClearColor = Legacy::Vec4(vColor, 1.f);	
	}
	virtual void ClearDepthBuffer()					 = 0;
	virtual void ClearColorBuffer(const Legacy::Vec3 vColor) = 0;

	virtual void SetRenderTarget(int nHandle) = 0;

	virtual void				   ProjectToScreen(float ptx, float pty, float ptz, float* sx, float* sy, float* sz) final;
	virtual int					   UnProject(float sx, float sy, float sz, float* px, float* py, float* pz, const float modelMatrix[16], const float projMatrix[16], const int viewport[4]) final;
	virtual int					   UnProjectFromScreen(float sx, float sy, float sz, float* px, float* py, float* pz) final;
	virtual void				   GetModelViewMatrix(float* mat) final;
	virtual void				   GetModelViewMatrix(double* mat) final;
	virtual void				   GetProjectionMatrix(double* mat) final;
	virtual void				   GetProjectionMatrix(float* mat) final;
	virtual Legacy::Vec3				   GetUnProject(const Legacy::Vec3& WindowCoords, const CCamera& cam) final;
	virtual int					   GetFrameID(bool bIncludeRecursiveCalls = true) final;
	virtual int					   GetPolyCount() { return INT_MIN; }
	virtual void				   GetPolyCount(int& nPolygons, int& nShadowVolPolys)
	{
		nPolygons		= INT_MIN;
		nShadowVolPolys = INT_MIN;
	}
	IGraphicsDeviceConstantBuffer* CreateConstantBuffer(int size) final;

	void			CreateQuad();
	IFont*			GetIFont() final;
	IRenderAuxGeom* GetIRenderAuxGeom() final;

	// Inherited via IRenderer
	virtual void			   ShareResources(IRenderer* renderer) override;
	virtual void			   SetRenderCallback(IRenderCallback* pCallback) override;
	virtual void			   PushProfileMarker(char* label) override;
	virtual void			   PopProfileMarker(char* label) override;
	virtual int				   CreateRenderTarget() override;
	virtual void			   DrawFullscreenQuad() override;
	virtual ITechniqueManager* GetITechniqueManager() final;
	virtual float			   GetDepthValue(int x, int y) override;
	virtual void			   Flush() final;
	virtual void			   Sh_Reload() override;

	void ShutDown();

  protected:
  protected:
	struct alignas(16) SPerViewConstantBuffer
	{
		Legacy::Mat4 Projection;
		Legacy::Mat4 OrthoProjection;
		Legacy::Mat4 View;
		Legacy::Mat4 ViewProjection;
		Legacy::Vec3 Eye;
	};
	struct alignas(16) SScreenConstantBuffer
	{
		glm::mat4 Model;
		glm::mat4 UvProjection;
		glm::vec4 Color;
		float	  Alpha;
	};

#ifndef VK_RENDERER
	CTypedConstantBuffer<SPerViewConstantBuffer> perViewBuffer;
	CTypedConstantBuffer<SScreenConstantBuffer>	 screenBuffer;
#endif

	void InitConsoleCommands() const;

	//private:
	IWindow* m_Window  = nullptr;
	ISystem* m_pSystem = nullptr;

	bool		 is_fullscreen = false;
	Legacy::Vec4		 m_viewPort;
	unsigned int cbpp  = 0;
	int			 zbpp  = 0;
	int			 sbits = 0;

	bool bInFullScreen = false;
	//============
	CCamera m_Camera;
	//============

	IRenderAuxGeom* m_RenderAuxGeom = nullptr;
	CBufferManager* m_BufferManager = nullptr;

	CVertexBuffer*		m_VertexBuffer = nullptr;
	_smart_ptr<CShader> m_ScreenShader;

#if 0
	FrameBufferObject* m_MainMSAAFrameBuffer;
	FrameBufferObject* m_MainReslovedFrameBuffer;
#endif

	std::vector<Texture*> m_RenderTargets;
	void*				  context;

	bool transit_to_FS = false;
	bool bIsActive	   = true;

	int m_FrameID = 0;

	// Windows context
	char	 m_WinTitle[80];
	WIN_HWND m_hWnd;		// The main app window
	WIN_HWND m_hWndDesktop; // The desktop window
	WIN_HWND m_hWndActive;	// The active window
#if BB_PLATFORM_WINDOWS
	HICON	m_hIconBig;	  // Icon currently being used on the taskbar
	HICON	m_hIconSmall; // Icon currently being used on the window
	HCURSOR m_hCursor;	  // Cursor currently being used on the window
	string	m_iconPath;	  // Path to the icon currently loaded
#endif

	std::vector<IFont*> m_Fonts;
	RenderBackend		m_Backend;
	Legacy::Vec4				m_ClearColor{};

	std::vector<IRendererCallbackClient*> m_RenderCallbackClients;
};
