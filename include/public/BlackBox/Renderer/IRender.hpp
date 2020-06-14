#pragma once

#ifdef IRENDER_EXPORTS
#	define IRENDER_API DLL_EXPORT
#else
#	define IRENDER_API DLL_IMPORT
#endif

// Global typedefs.
//////////////////////////////////////////////////////////////////////
#ifndef uchar
typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned short ushort;
#endif

#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/Renderer/IShader.hpp>
#include <BlackBox/Renderer/Light.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <vector>

// Forward declarations.
//////////////////////////////////////////////////////////////////////
typedef void* WIN_HWND;
typedef void* WIN_HINSTANCE;
typedef void* WIN_HDC;
typedef void* WIN_HGLRC;

class CCamera;
class CMatInfo;
struct IRenderAuxGeom;
struct IFont;
struct IShader;
struct ISystem;
struct IWindow;
struct Material;

//////////////////////////////////////////////////////////////////////
typedef unsigned char bvec4[4];
typedef float vec4_t[4];
typedef unsigned char byte;
typedef float vec2_t[2];

// Interface to the graphics constant buffers
struct IGraphicsDeviceConstantBuffer
{
	// Set contents of the device buffer
	virtual void SetData(const uint8* data, size_t size) = 0;
};
typedef _smart_ptr<IGraphicsDeviceConstantBuffer> IGraphicsDeviceConstantBufferPtr;

/*
   template <typename T>
   class CTypedGraphicsDeviceConstantBuffer
   {
   T m_hostBuffer;
   IGraphicsDeviceConstantBuffer m_constantBuffer;

   public:
   CTypedConstantBuffer() : m_constantBuffer(nullptr) {}
   CTypedConstantBuffer(const CTypedConstantBuffer<T> &cb) : m_constantBuffer(nullptr) { m_hostBuffer = cb.m_hostBuffer; }

   bool IsDeviceBufferAllocated() { return m_constantBuffer != nullptr; }
   IGraphicsDeviceConstantBuffer GetDeviceConstantBuffer()
   {
    if (!m_constantBuffer)
    {
      CreateDeviceBuffer();
    }
    return m_constantBuffer;
   }

   void CreateDeviceBuffer()
   {
    int size = sizeof(T);
    m_constantBuffer = gcpRendD3D->m_DevBufMan.CreateConstantBuffer(size);
    CopyToDevice();
   }
   void CopyToDevice()
   {
    m_constantBuffer->SetData(&m_hostBuffer, sizeof(m_hostBuffer));
   }

   T* operator->() { return &m_hostBuffer; }
   T& operator=(const T& hostData)
   {
    return m_hostBuffer = hostData;
   }
   };
 */

struct Transform
{
	Vec3 position;
	Vec3 rotation;
	Vec3 scale;

	Transform() = default;
	Transform(Vec3 position, Vec3 rotation, Vec3 scale)
		: position(position), rotation(rotation), scale(scale)
	{
	}
};

// Render State flags
#define GS_BLSRC_MASK 0xf
#define GS_BLSRC_ZERO 0x1
#define GS_BLSRC_ONE 0x2
#define GS_BLSRC_DSTCOL 0x3
#define GS_BLSRC_ONEMINUSDSTCOL 0x4
#define GS_BLSRC_SRCALPHA 0x5
#define GS_BLSRC_ONEMINUSSRCALPHA 0x6
#define GS_BLSRC_DSTALPHA 0x7
#define GS_BLSRC_ONEMINUSDSTALPHA 0x8
#define GS_BLSRC_ALPHASATURATE 0x9

#define GS_BLDST_MASK 0xf0
#define GS_BLDST_ZERO 0x10
#define GS_BLDST_ONE 0x20
#define GS_BLDST_SRCCOL 0x30
#define GS_BLDST_ONEMINUSSRCCOL 0x40
#define GS_BLDST_SRCALPHA 0x50
#define GS_BLDST_ONEMINUSSRCALPHA 0x60
#define GS_BLDST_DSTALPHA 0x70
#define GS_BLDST_ONEMINUSDSTALPHA 0x80

#define GS_POLYLINE 0x00001000
#define GS_TEXPARAM_CLAMP 0x00002000
#define GS_TEXPARAM_UCLAMP 0x00004000
#define GS_TEXPARAM_VCLAMP 0x00008000
#define GS_COLMASKONLYALPHA 0x00010000
#define GS_NODEPTHTEST 0x00020000
#define GS_COLMASKONLYRGB 0x00040000
#define GS_DEPTHFUNC_EQUAL 0x00100000
#define GS_DEPTHFUNC_GREAT 0x00200000
#define GS_STENCIL 0x00400000

union UCol {
	uint dcolor;
	bvec4 bcolor;
	/*
	UCol& operator=(Vec4& v)
	{
		UCol c;
		c.bcolor[0] = static_cast<char>(v[0] * 255);
		c.bcolor[1] = static_cast<char>(v[1] * 255);
		c.bcolor[2] = static_cast<char>(v[2] * 255);
		c.bcolor[3] = static_cast<char>(v[3] * 255);
		return c;
	};
	*/
	UCol() = default;
	UCol(const Vec4& v)
	{
		bcolor[0] = static_cast<char>(v[3] * 255.f);
		bcolor[1] = static_cast<char>(v[1] * 255.f);
		bcolor[2] = static_cast<char>(v[2] * 255.f);
		bcolor[3] = static_cast<char>(v[0] * 255.f);
	}
	UCol(const Vec3& v)
		: UCol(Vec4(v, 1))
	{
	}
	UCol(const bvec4 v): 
		 bcolor{v[0], v[1], v[2], v[3]}
	{
	}
	UCol(uint8 v0, uint8 v1, uint8 v2, uint8 v3): 
		 bcolor{v0, v1, v2, v3}
	{
	}
			
};

enum class RenderPrimitive
{
	LINES,
	LINE_STRIP,
	LINE_LOOP,
	TRIANGLES,
	TRIANGLE_STRIP
};

struct ITexture
{
	virtual int AddRef()
	{
		return 0;
	}
	virtual int Release()
	{
		return 0;
	}
	virtual bool load(const char* name)			  = 0;
	virtual void bind()							  = 0;
	virtual void setUnit(uint unit)				  = 0;
	virtual uint getUnit() const				  = 0;
	virtual void setType(const char* TextureType) = 0;
	virtual const char* typeToStr()				  = 0;
	virtual int getWidth() const				  = 0;
	virtual int getHeight() const				  = 0;
	virtual int getId() const					  = 0;
};

//////////////////////////////////////////////////////////////////////////
//! This structure used in DrawText method of renderer.
//! It provide all necesarry information of how to render text on screen.
//! @see IRenderer::Draw2dText
struct SDrawTextInfo
{
	//! One of EDrawTextFlags flags.
	//! @see EDrawTextFlags
	int flags;
	//! Text color, (r,g,b,a) all members must be specified.
	float color[4];
	float xscale;
	float yscale;
	IFont* font;

	SDrawTextInfo()
		: xscale(1.0), yscale(1.0)
	{
		flags	= 0;
		color[0] = color[1] = color[2] = color[3] = 1;
		font									  = 0;
	}
};

//////////////////////////////////////////////////////////////////////

struct SDispFormat
{
	int m_Width;
	int m_Height;
	int m_BPP;
	SDispFormat()
	{
	}
};

//////////////////////////////////////////////////////////////////////////
// Stream ID's
enum EStreamID
{
	VSF_GENERAL = 0, // General vertex buffer
					 //#define VSF_TANGENTS 1  // Tangents buffer

	VSF_NUM // Number of vertex streams
};

// Stream Masks (Used during updating)
#define VSM_GENERAL (1 << VSM_GENERAL)
#define VSM_TANGENTS (1 << VSF_TANGENTS)

//////////////////////////////////////////////////////////////////////////
union UHWBuf {
	void* m_pPtr;
	uint m_nID;
};

//////////////////////////////////////////////////////////////////////////
struct SVertexStream
{
	void* m_VData;	// pointer to buffer data
	UHWBuf m_VertBuf; // HW buffer descriptor
	int m_nItems;
	bool m_bLocked; // Used in Direct3D only
	bool m_bDynamic;
	int m_nBufOffset;
	struct SVertPool* m_pPool;
	SVertexStream()
	{
		Reset();
		m_bDynamic   = false;
		m_nBufOffset = 0;
		m_pPool		 = NULL;
	}

	void Reset()
	{
		m_VData			 = NULL;
		m_VertBuf.m_pPtr = NULL;
        m_nItems		 = 0;
		m_bLocked		 = false;
	}
};

//////////////////////////////////////////////////////////////////////
// General VertexBuffer created by CreateVertexBuffer() function
class CVertexBuffer
{
  public:
	CVertexBuffer()
	{
		for (int i = 0; i < VSF_NUM; i++)
		{
			m_VS[i].Reset();
		}
		m_fence		   = 0;
		m_bFenceSet	= 0;
		m_NumVerts	 = 0;
		m_vertexformat = 0;
	}

	CVertexBuffer(void* pData, int nVertexFormat, int nVertCount = 0)
	{
		for (int i = 0; i < VSF_NUM; i++)
		{
			m_VS[i].m_VData			 = NULL;
			m_VS[i].m_VertBuf.m_pPtr = NULL;
			m_VS[i].m_bLocked		 = false;
		}
		m_VS[VSF_GENERAL].m_VData = pData;
		m_vertexformat			  = nVertexFormat;
		m_fence					  = 0;
		m_bFenceSet				  = 0;
		m_NumVerts				  = nVertCount;
	}
	void* GetStream(int nStream, int* nOffs);

	SVertexStream m_VS[VSF_NUM]; // 1 vertex streams and one index stream

	uint m_bFenceSet : 1;
	uint m_bDynamic : 1;
	int m_vertexformat;
	unsigned int m_fence;
	int m_NumVerts;
	uint m_Container;
	//## MM unused?	void *pPS2Buffer;

	int Size(int Flags, int nVerts);
};
//////////////////////////////////////////////////////////////////////
struct SVertBufComps
{
	bool m_bHasTC;
	bool m_bHasColors;
	bool m_bHasSecColors;
	bool m_bHasNormals;
};

struct IRenderCallback
{
	enum Type
	{
		eOnRender,
		eBeforeSwapBuffers
	};
	virtual void CallBack(Type type) = 0;
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
		PROGRAM_POINT_SIZE,
		MULTISAMPLE
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
	virtual bool ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen) = 0;

	//! Shut down the renderer
	virtual void Release() = 0;

	//! Should be called at the beginning of every frame
	virtual void BeginFrame(void) = 0;

	//! Should be called at the end of every frame
	virtual void Update(void) = 0;

	//! This renderer will share resources (textures) with specified renderer.
	//! Specified renderer must be of same type as this renderer.
	virtual void  ShareResources( IRenderer *renderer )=0;

	virtual void SetRenderCallback(IRenderCallback* pCallback) = 0;

	virtual void GetViewport(int* x, int* y, int* width, int* height)			  = 0;
	virtual void SetViewport(int x = 0, int y = 0, int width = 0, int height = 0) = 0;
	virtual void SetScissor(int x = 0, int y = 0, int width = 0, int height = 0)  = 0;

	//! Draw a bbox specified by mins/maxs (debug puprposes)
	virtual void Draw3dBBox(const Vec3& mins, const Vec3& maxs) = 0;

	//! Set the renderer camera
	virtual void SetCamera(const CCamera& cam) = 0;

	//! Get the renderer camera
	virtual const CCamera& GetCamera() = 0;

	//! Change display size
	virtual bool ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp) = 0;

	//! Chenge viewport size
	virtual void ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

	//! Write a message on the screen with additional flags.
	//! for flags @see
	virtual void Draw2dText(float posX, float posY, const char* szText, SDrawTextInfo& info) = 0;

	//! Draw a image using the current matrix
	virtual void DrawImage(float xpos, float ypos, float w, float h, int texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a) = 0;

	virtual void DrawFullScreenImage(int texture_id) = 0;

	//! Set the polygon mode (wireframe, solid)
	virtual int SetPolygonMode(int mode) = 0;

	//! Get screen width
	virtual int GetWidth() = 0;

	//! Get screen height
	virtual int GetHeight() = 0;

	//! Get a screenshot and save to a file
	virtual void ScreenShot(const char* filename = NULL) = 0;

	//! Project to screen
	virtual void ProjectToScreen(float ptx, float pty, float ptz,
								 float* sx, float* sy, float* sz) = 0;

	//! Unproject to screen
	virtual int UnProject(float sx, float sy, float sz,
						  float* px, float* py, float* pz,
						  const float modelMatrix[16],
						  const float projMatrix[16],
						  const int viewport[4]) = 0;

	//! Unproject from screen
	virtual int UnProjectFromScreen(float sx, float sy, float sz,
									float* px, float* py, float* pz) = 0;

	//! for editor
	virtual void GetModelViewMatrix(float* mat) = 0;

	//! for editor
	virtual void GetModelViewMatrix(double* mat) = 0;

	//! for editor
	virtual void GetProjectionMatrix(double* mat) = 0;

	//! for editor
	virtual void GetProjectionMatrix(float* mat) = 0;

	//! for editor
	virtual Vec3 GetUnProject(const Vec3& WindowCoords, const CCamera& cam) = 0;

	virtual void RenderToViewport(const CCamera& cam, float x, float y, float width, float height) = 0;

	virtual void PrintLine(const char* szText, SDrawTextInfo& info) = 0;

	virtual int EnumDisplayFormats(SDispFormat* formats) = 0;

	virtual void SetState(State state, bool enable)			 = 0;
	virtual void SetCullMode(CullMode mode = CullMode::BACK) = 0;

	virtual void PushProfileMarker(char* label) = 0;
	virtual void PopProfileMarker(char* label)  = 0;

	//////////////////////////////////////////////////////////////////////
	//! Interface for auxiliary geometry (for debugging, editor purposes, etc.)
	virtual IRenderAuxGeom* GetIRenderAuxGeom(/*EViewportType viewport = eViewportType_Default*/) = 0;
	//////////////////////////////////////////////////////////////////////

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
	virtual void SetClearColor(const Vec3& vColor)   = 0;
	virtual void ClearDepthBuffer()					 = 0;
	virtual void ClearColorBuffer(const Vec3 vColor) = 0;

	virtual int GetFrameID(bool bIncludeRecursiveCalls = true) = 0;

	virtual void SetRenderTarget(int nHandle) = 0;
  virtual int CreateRenderTarget() = 0;

	virtual void DrawFullscreenQuad()						  = 0;
	virtual void Set2DMode(bool enable, int ortox, int ortoy) = 0;

	//! Create a vertex buffer
	virtual CVertexBuffer* CreateBuffer(int vertexcount, int vertexformat, const char* szSource, bool bDynamic = false) = 0;

	//! Release a vertex buffer
	virtual void ReleaseBuffer(CVertexBuffer* bufptr) = 0;

	//! Draw a vertex buffer
	virtual void DrawBuffer(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start = 0, int vert_stop = 0, CMatInfo* mi = NULL) = 0;

	//! Update a vertex buffer
	virtual void UpdateBuffer(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs = 0, int Type = 0) = 0;

	virtual void CreateIndexBuffer(SVertexStream* dest, const void* src, int indexcount) = 0;
	//! Update indicies
	virtual void UpdateIndexBuffer(SVertexStream* dest, const void* src, int indexcount, bool bUnLock = true) = 0;
	virtual void ReleaseIndexBuffer(SVertexStream* dest)													  = 0;

	virtual struct ITechniqueManager* GetITechniqueManager() = 0;
	virtual struct IFont* GetIFont()						 = 0;

	virtual IGraphicsDeviceConstantBuffer* CreateConstantBuffer(int size) = 0;

	virtual float GetDepthValue(int x, int y) = 0;

	virtual void Flush() = 0;

	////////////////////////////////////////////////////////////////////////////////
	virtual IShaderProgram* Sh_Load(const char* name, int flags)			  = 0;
	virtual IShaderProgram* Sh_Load(const char* vertex, const char* fragment) = 0;
	virtual void Sh_Reload() = 0;
	// Loading of the texture for name(nameTex)
	virtual ITexture* LoadTexture(const char* nameTex, uint flags, byte eTT) = 0;
};

extern "C"
{
	IRENDER_API IRenderer* CreateIRender(ISystem* pSystem /*, void* hinst, void* hwnd, bool usedinput*/);
	typedef IRenderer* (*PFNCREATERENDERERINTERFACE)(ISystem* pSystem);
}

struct SRenderParams
{
	IShaderProgram* Shader;
	std::vector<UniformValue> uniforms;
	Material* Material;
	CCamera* Camera;
	DirectionLight* DirectionLight;
	Mat4 Transform;
};

struct IDrawable
{
	virtual void draw(SRenderParams& renderParams) = 0;
};

////////////////////////////////////////////////////////////////////////////////
class ScopedState
{
  public:
	ScopedState(IRenderer* pRender, IRenderer::State state, bool enable)
		: pRender(pRender),
		  state(state),
		  enable(enable)
	{
		pRender->SetState(state, enable);
	}
	~ScopedState()
	{
		pRender->SetState(state, !enable);
	}
	IRenderer* pRender;
	IRenderer::State state;
	bool enable;
};

#define RSS(r, s, e) ScopedState _ss##s(r, IRenderer::State::s, e)
