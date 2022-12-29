#pragma once

#ifdef IRENDER_EXPORTS
	#define IRENDER_API DLL_EXPORT
#else
	#define IRENDER_API DLL_IMPORT
#endif

// Global typedefs.
//////////////////////////////////////////////////////////////////////
#ifndef uchar
typedef unsigned char  uchar;
typedef unsigned int   uint;
typedef unsigned short ushort;
#endif

#include <BlackBox/Math/MathHelper.hpp>
#include <BlackBox/Core/smartptr.hpp>

#include <string_view>
#include <vector>

#ifndef NOT_IMPLEMENTED_V
	#if defined(ASSERT_NOT_IMPLEMENTED) || !defined(_____)
		#define NOT_IMPLEMENTED_V      \
			assert(0 && __FUNCTION__); \
			return {};
	#else
		#define NOT_IMPLEMENTED_V                                                          \
			Env::Log()->LogError("[Renderer] Function [%s] not implemened", __FUNCTION__); \
			return {};
	#endif
#endif
#ifndef NOT_IMPLEMENTED
	#define NOT_IMPLEMENTED \
		assert(0 && __FUNCTION__);
#endif

// Forward declarations.
//////////////////////////////////////////////////////////////////////
typedef void* WIN_HWND;
typedef void* WIN_HINSTANCE;
typedef void* WIN_HDC;
typedef void* WIN_HGLRC;

class CCamera;
struct CMatInfo;
struct IMatInfo;
struct IRenderAuxGeom;
struct ITechniqueManager;
struct IFont;
struct IShader;
struct ISystem;
struct IWindow;
struct Material;
struct ITechniqueManager;
class CDLight;

//////////////////////////////////////////////////////////////////////
typedef unsigned char bvec4[4];
typedef float         vec4_t[4];
typedef unsigned char byte;
typedef float         vec2_t[2];

using ColorB = glm::u8vec3;
using ColorF = glm::f32vec4;

enum class RenderBackend
{
	GL,
	DX
};

// Uncomment one of the two following typedefs:
typedef uint32 vtx_idx;
// typedef uint16 vtx_idx;

// Interface to the graphics constant buffers
struct IGraphicsDeviceConstantBuffer
{
	virtual ~IGraphicsDeviceConstantBuffer()             = default;
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
	Legacy::Vec3 position;
	Legacy::Vec3 rotation;
	Legacy::Vec3 scale;

	Transform() = default;
	Transform(Legacy::Vec3 position, Legacy::Vec3 rotation, Legacy::Vec3 scale)
	    : position(position)
	    , rotation(rotation)
	    , scale(scale)
	{
	}
};

// DOC-IGNORE-BEGIN
#include "ColorDefs.h"
#include "TArray.h"
#include "..\Core\Platform\Platform.hpp"

//#include <IFont.h>
// DOC-IGNORE-END

//////////////////////////////////////////////////////////////////////
// Texture flags

#define FT_PROJECTED              0x1
#define FT_NOMIPS                 0x2
#define FT_HASALPHA               0x4
#define FT_NORESIZE               0x8
#define FT_HDR                    0x10
#define FT_UPDATE                 0x20
#define FT_ALLOCATED              0x40
#define FT_BUILD                  0x80

#define FT_NODOWNLOAD             0x100
#define FT_CONV_GREY              0x200
#define FT_LM                     0x400
#define FT_HASDSDT                0x800

#define FT_HASNORMALMAP           0x1000
#define FT_DYNAMIC                0x2000
#define FT_NOREMOVE               0x4000
#define FT_HASMIPS                0x8000
#define FT_PALETTED               0x10000
#define FT_NOTFOUND               0x20000
#define FT_FONT                   0x40000
#define FT_SKY                    0x80000
#define FT_SPEC_MASK              0x7f000
#define FT_CLAMP                  0x100000
#define FT_NOSTREAM               0x200000
#define FT_DXT1                   0x400000
#define FT_DXT3                   0x800000
#define FT_DXT5                   0x1000000
#define FT_DXT                    0x1c00000
#define FT_3DC                    0x2000000
#define FT_3DC_A                  0x4000000
#define FT_ALLOW3DC               0x8000000

#define FT_BUMP_SHIFT             27
#define FT_BUMP_MASK              0xf0000000
#define FT_BUMP_DETALPHA          0x10000000
#define FT_BUMP_DETRED            0x20000000
#define FT_BUMP_DETBLUE           0x40000000
#define FT_BUMP_DETINTENS         0x80000000

//////////////////////////////////////////////////////////////////////
#define FT2_NODXT                 1
#define FT2_RENDERTARGET          2
#define FT2_FORCECUBEMAP          4
#define FT2_WASLOADED             8
#define FT2_RELOAD                0x10
#define FT2_NEEDRESTORED          0x20
#define FT2_UCLAMP                0x40
#define FT2_VCLAMP                0x80
#define FT2_RECTANGLE             0x100
#define FT2_FORCEDXT              0x200

#define FT2_BUMPHIGHRES           0x400
#define FT2_BUMPLOWRES            0x800
#define FT2_PARTIALLYLOADED       0x1000
#define FT2_NEEDTORELOAD          0x2000
#define FT2_WASUNLOADED           0x4000
#define FT2_STREAMINGINPROGRESS   0x8000

#define FT2_FILTER_BILINEAR       0x10000
#define FT2_FILTER_TRILINEAR      0x20000
#define FT2_FILTER_ANISOTROPIC    0x40000
#define FT2_FILTER_NEAREST        0x80000
#define FT2_FILTER                (FT2_FILTER_BILINEAR | FT2_FILTER_TRILINEAR | FT2_FILTER_ANISOTROPIC | FT2_FILTER_NEAREST)
#define FT2_VERSIONWASCHECKED     0x100000
#define FT2_BUMPCOMPRESED         0x200000
#define FT2_BUMPINVERTED          0x400000
#define FT2_STREAMFROMDDS         0x8000000
#define FT2_DISCARDINCACHE        0x1000000
#define FT2_NOANISO               0x2000000
#define FT2_CUBEASSINGLETEXTURE   0x4000000
#define FT2_FORCEMIPS2X2          0x8000000
#define FT2_DIFFUSETEXTURE        0x10000000
#define FT2_WASFOUND              0x20000000
#define FT2_REPLICATETOALLSIDES   0x40000000
#define FT2_CHECKFORALLSEQUENCES  0x80000000

//////////////////////////////////////////////////////////////////////

// Render State flags
#define GS_BLSRC_MASK             0xf
#define GS_BLSRC_ZERO             0x1
#define GS_BLSRC_ONE              0x2
#define GS_BLSRC_DSTCOL           0x3
#define GS_BLSRC_ONEMINUSDSTCOL   0x4
#define GS_BLSRC_SRCALPHA         0x5
#define GS_BLSRC_ONEMINUSSRCALPHA 0x6
#define GS_BLSRC_DSTALPHA         0x7
#define GS_BLSRC_ONEMINUSDSTALPHA 0x8
#define GS_BLSRC_ALPHASATURATE    0x9

#define GS_BLDST_MASK             0xf0
#define GS_BLDST_ZERO             0x10
#define GS_BLDST_ONE              0x20
#define GS_BLDST_SRCCOL           0x30
#define GS_BLDST_ONEMINUSSRCCOL   0x40
#define GS_BLDST_SRCALPHA         0x50
#define GS_BLDST_ONEMINUSSRCALPHA 0x60
#define GS_BLDST_DSTALPHA         0x70
#define GS_BLDST_ONEMINUSDSTALPHA 0x80

#define GS_BUMP                   0xa0
#define GS_ENV                    0xb0

#define GS_DXT1                   0xc0
#define GS_DXT3                   0xd0
#define GS_DXT5                   0xe0

#define GS_BLEND_MASK             0xff

#define GS_DEPTHWRITE             0x00000100

#define GS_MODULATE               0x00000200
#define GS_NOCOLMASK              0x00000400
#define GS_ADDITIONALSTATE        0x00000800

#define GS_POLYLINE               0x00001000
#define GS_TEXPARAM_CLAMP         0x00002000
#define GS_TEXPARAM_UCLAMP        0x00004000
#define GS_TEXPARAM_VCLAMP        0x00008000
#define GS_COLMASKONLYALPHA       0x00010000
#define GS_NODEPTHTEST            0x00020000
#define GS_COLMASKONLYRGB         0x00040000
#define GS_DEPTHFUNC_EQUAL        0x00100000
#define GS_DEPTHFUNC_GREAT        0x00200000
#define GS_STENCIL                0x00400000

#define STR_DX11_RENDERER         "DX11"
#define STR_DX12_RENDERER         "DX12"
#define STR_VK_RENDERER           "VK"
#define STR_GL_RENDERER           "GL"

//////////////////////////////////////////////////////////////////////
// Texture object interface
struct ITexPic
{
	virtual void        AddRef()                                       = 0;
	virtual void        Release(int bForce = false)                    = 0;
	virtual const char* GetName()                                      = 0;
	virtual int         GetWidth()                                     = 0;
	virtual int         GetHeight()                                    = 0;
	virtual int         GetOriginalWidth()                             = 0;
	virtual int         GetOriginalHeight()                            = 0;
	virtual int         GetTextureID()                                 = 0;
	virtual int         GetFlags()                                     = 0;
	virtual int         GetFlags2()                                    = 0;
	virtual void        SetClamp(bool bEnable)                         = 0;
	virtual bool        IsTextureLoaded()                              = 0;
	virtual void        PrecacheAsynchronously(float fDist, int Flags) = 0;
	virtual void        Preload(int Flags)                             = 0;
	virtual byte*       GetData32()                                    = 0;
	virtual bool        SetFilter(int nFilter)                         = 0;
};

#define FORMAT_8_BIT  8
#define FORMAT_24_BIT 24
#define FORMAT_32_BIT 32

//////////////////////////////////////////////////////////////////////
// Import and Export interfaces passed to the renderer
struct SCryRenderInterface
{
	class CMalloc*   igcpMalloc;

	struct ILog*     ipLog;
	struct IConsole* ipConsole;
	struct ITimer*   ipTimer;
	struct ISystem*  ipSystem;
	int*             ipTest_int;
#if 0
	IPhysicalWorld* pIPhysicalWorld;
#endif
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
	virtual bool        load(const char* name)           = 0;
	virtual void        bind()                           = 0;
	virtual void        setUnit(uint unit)               = 0;
	virtual uint        getUnit() const                  = 0;
	virtual void        setType(const char* TextureType) = 0;
	virtual const char* typeToStr()                      = 0;
	virtual int         getWidth() const                 = 0;
	virtual int         getHeight() const                = 0;
	virtual int         getId() const                    = 0;
	virtual uint64      getBindlesId() const             = 0;
	virtual void        bind() const                     = 0;
};

struct ISammplerState
{
};

//////////////////////////////////////////////////////////////////////////
//! This structure used in DrawText method of renderer.
//! It provide all necesarry information of how to render text on screen.
//! @see IRenderer::Draw2dText
struct SDrawTextInfo
{
	//! One of EDrawTextFlags flags.
	//! @see EDrawTextFlags
	int    flags;
	//! Text color, (r,g,b,a) all members must be specified.
	float  color[4];
	float  xscale;
	float  yscale;
	IFont* font;

	SDrawTextInfo()
	    : xscale(1.0)
	    , yscale(1.0)
	{
		flags    = 0;
		color[0] = color[1] = color[2] = color[3] = 1;
		font                                      = 0;
	}
};

//////////////////////////////////////////////////////////////////////

struct SDispFormat
{
	int m_Width  = 0;
	int m_Height = 0;
	int m_BPP    = 0;
};

//! Stream IDs.
enum EStreamIDs
{
	VSF_GENERAL,         //!< General vertex buffer.
	VSF_TANGENTS,        //!< Tangents buffer.
	VSF_QTANGENTS,       //!< Tangents buffer.
	VSF_HWSKIN_INFO,     //!< HW skinning buffer.
	VSF_VERTEX_VELOCITY, //!< Velocity buffer.
#if ENABLE_NORMALSTREAM_SUPPORT
	VSF_NORMALS, //!< Normals, used for skinning.
#endif
	//   <- Insert new stream IDs here.
	VSF_NUM, //!< Number of vertex streams.

	VSF_MORPHBUDDY         = 8,  //!< Morphing (from m_pMorphBuddy).
	VSF_INSTANCED          = 9,  //!< Data is for instance stream.
	VSF_MORPHBUDDY_WEIGHTS = 15, //!< Morphing weights.
};

//! Stream Masks (Used during updating).
enum EStreamMasks : uint16_t
{
	VSM_GENERAL         = BIT(VSF_GENERAL),
	VSM_TANGENTS        = BIT(VSF_TANGENTS) | BIT(VSF_QTANGENTS), // either or
	VSM_QTANGENTS       = BIT(VSF_QTANGENTS),
	VSM_HWSKIN          = BIT(VSF_HWSKIN_INFO),
	VSM_VERTEX_VELOCITY = BIT(VSF_VERTEX_VELOCITY),
#if ENABLE_NORMALSTREAM_SUPPORT
	VSM_NORMALS = BIT(VSF_NORMALS),
#endif

	VSM_MORPHBUDDY = BIT(VSF_MORPHBUDDY),
	VSM_INSTANCED  = BIT(VSF_INSTANCED),

	VSM_MASK       = MASK(VSF_NUM),
	VSM_NONE       = 0,
};

//////////////////////////////////////////////////////////////////////////
union UHWBuf
{
	void* m_pPtr;
	uint  m_nID;
};

//////////////////////////////////////////////////////////////////////////
struct SVertexStream
{
	void*             m_VData;   // pointer to buffer data
	UHWBuf            m_VertBuf; // HW buffer descriptor
	int               m_nItems;
	bool              m_bLocked; // Used in Direct3D only
	bool              m_bDynamic;
	int               m_nBufOffset;
	struct SVertPool* m_pPool;
	SVertexStream()
	{
		Reset();
		m_bDynamic   = false;
		m_nBufOffset = 0;
		m_pPool      = NULL;
	}

	void Reset()
	{
		m_VData          = NULL;
		m_VertBuf.m_pPtr = NULL;
		m_nItems         = 0;
		m_bLocked        = false;
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
		m_fence        = 0;
		m_bFenceSet    = 0;
		m_NumVerts     = 0;
		m_vertexformat = 0;
	}

	CVertexBuffer(void* pData, int nVertexFormat, int nVertCount = 0)
	{
		for (int i = 0; i < VSF_NUM; i++)
		{
			m_VS[i].m_VData          = NULL;
			m_VS[i].m_VertBuf.m_pPtr = NULL;
			m_VS[i].m_bLocked        = false;
		}
		m_VS[VSF_GENERAL].m_VData = pData;
		m_vertexformat            = nVertexFormat;
		m_fence                   = 0;
		m_bFenceSet               = 0;
		m_NumVerts                = nVertCount;
	}

	void*         GetStream(int nStream, int* nOffs);

	SVertexStream m_VS[VSF_NUM]; // 1 vertex streams and one index stream

	uint          m_bFenceSet : 1;
	uint          m_bDynamic : 1;
	int           m_vertexformat;
	unsigned int  m_fence;
	int           m_NumVerts;
	uint          m_Container;
	//## MM unused?	void *pPS2Buffer;

	int           Size(int Flags, int nVerts);
};

//////////////////////////////////////////////////////////////////////////
// DOC-IGNORE-BEGIN
#include <BlackBox/Renderer/IShader.hpp>
// DOC-IGNORE-END

struct IRenderCallback
{
	enum Type
	{
		eOnRender,
		eBeforeSwapBuffers
	};
	virtual void CallBack(Type type) = 0;
};

//! \cond INTERNAL
//! Describes rendering viewport dimensions
struct SRenderViewport
{
	int   x      = 0;
	int   y      = 0;
	int   width  = 0;
	int   height = 0;
	float zmin   = 0.f;
	float zmax   = 1.f;

	SRenderViewport() {}

	SRenderViewport(int newX, int newY, int newWidth, int newHeight)
	    : x(newX)
	    , y(newY)
	    , width(newWidth)
	    , height(newHeight)
	    , zmin(0.0f)
	    , zmax(1.0f)
	{
	}

	bool operator==(const SRenderViewport& v)
	{
		return x == v.x && y == v.y && width == v.width && height == v.height && zmin == v.zmin && zmax == v.zmax;
	}
	bool operator!=(const SRenderViewport& v)
	{
		return !(*this == v);
	}
};
//! \endcond

// The following may be (and sometimes is) used for testing. Please don't remove.
struct IRenderer;
class IRendererCallbackServer;

// Renderer callback client.
// Derive your class (a renderer client) from this one in order to register its instances
// with the renderer.
class IRendererCallbackClient
{
public:
	// within this function, you can render your stuff and it won't flicker in the viewport
	virtual void OnRenderer_BeforeEndFrame() {}
	// unregisters itsef from the server
	virtual void Renderer_SelfUnregister() {}
};

// renderer callback server.
// Derive your renderer (server) from this one in order for it to be able to serve
// renderer callback client requests
class IRendererCallbackServer
{
public:
	// registers the given client  - do nothing by default
	virtual void RegisterCallbackClient(IRendererCallbackClient* pClient) {}
	// unregisters the given client - do nothing by default
	virtual void UnregisterCallbackClient(IRendererCallbackClient* pClient) {}
};

struct IRenderer : public IRendererCallbackServer
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
	virtual IWindow*       Init(int x, int y, int width, int height, unsigned int cbpp, int zbpp, int sbits, bool fullscreen, IWindow* window = nullptr)                                                                                   = 0;

	//! Changes resolution of the window/device (doen't require to reload the level
	virtual bool           ChangeResolution(int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen)                                                                                                          = 0;

	//! Shut down the renderer
	virtual void           Release()                                                                                                                                                                                                       = 0;

	//! Should be called at the beginning of every frame
	virtual void           BeginFrame(void)                                                                                                                                                                                                = 0;

	//! Should be called at the end of every frame
	virtual void           Update(void)                                                                                                                                                                                                    = 0;

	//! This renderer will share resources (textures) with specified renderer.
	//! Specified renderer must be of same type as this renderer.
	virtual void           ShareResources(IRenderer* renderer)                                                                                                                                                                             = 0;

	virtual void           SetRenderCallback(IRenderCallback* pCallback)                                                                                                                                                                   = 0;

	virtual void           GetViewport(int* x, int* y, int* width, int* height)                                                                                                                                                            = 0;
	virtual void           SetViewport(int x = 0, int y = 0, int width = 0, int height = 0)                                                                                                                                                = 0;
	virtual void           SetScissor(int x = 0, int y = 0, int width = 0, int height = 0)                                                                                                                                                 = 0;

	//! Draw a bbox specified by mins/maxs (debug puprposes)
	virtual void           Draw3dBBox(const Legacy::Vec3& mins, const Legacy::Vec3& maxs)                                                                                                                                                  = 0;

	//! Set the renderer camera
	virtual void           SetCamera(const CCamera& cam)                                                                                                                                                                                   = 0;

	//! Get the renderer camera
	virtual const CCamera& GetCamera()                                                                                                                                                                                                     = 0;

	//! Change display size
	virtual bool           ChangeDisplay(unsigned int width, unsigned int height, unsigned int cbpp)                                                                                                                                       = 0;

	//! Chenge viewport size
	virtual void           ChangeViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)                                                                                                                         = 0;

	//! Write a message on the screen with additional flags.
	//! for flags @see
	virtual void           Draw2dText(float posX, float posY, const char* szText, const SDrawTextInfo& info)                                                                                                                               = 0;

	//! Draw a 2d image on the screen (Hud etc.)
	virtual void           Draw2dImage(float xpos, float ypos, float w, float h, int texture_id, float s0 = 0, float t0 = 0, float s1 = 1, float t1 = 1, float angle = 0, float r = 1, float g = 1, float b = 1, float a = 1, float z = 1) = 0;

	//! Draw a image using the current matrix
	virtual void           DrawImage(float xpos, float ypos, float w, float h, uint64 texture_id, float s0, float t0, float s1, float t1, float r, float g, float b, float a)                                                              = 0;

	virtual void           DrawFullScreenImage(int texture_id)                                                                                                                                                                             = 0;

	//! Set the polygon mode (wireframe, solid)
	virtual int            SetPolygonMode(int mode)                                                                                                                                                                                        = 0;

	//! Get screen width
	virtual int            GetWidth()                                                                                                                                                                                                      = 0;

	//! Get screen height
	virtual int            GetHeight()                                                                                                                                                                                                     = 0;

	//! Memory status information
	virtual void           GetMemoryUsage(ICrySizer* Sizer) const                                                                                                                                                                          = 0;

	//! Get a screenshot and save to a file
	virtual void           ScreenShot(const char* filename = NULL)                                                                                                                                                                         = 0;

	//! Project to screen
	virtual void           ProjectToScreen(float ptx, float pty, float ptz,
	                                       float* sx, float* sy, float* sz)                                                                                                                                                                = 0;

	//! Unproject to screen
	virtual int            UnProject(float sx, float sy, float sz,
	                                 float* px, float* py, float* pz,
	                                 const float modelMatrix[16],
	                                 const float projMatrix[16],
	                                 const int   viewport[4])                                                                                                                                                                                = 0;

	//! Unproject from screen
	virtual int            UnProjectFromScreen(float sx, float sy, float sz,
	                                           float* px, float* py, float* pz)                                                                                                                                                            = 0;

	//! for editor
	virtual void           GetModelViewMatrix(float* mat)                                                                                                                                                                                  = 0;

	//! for editor
	virtual void           GetModelViewMatrix(double* mat)                                                                                                                                                                                 = 0;

	//! for editor
	virtual void           GetProjectionMatrix(double* mat)                                                                                                                                                                                = 0;

	//! for editor
	virtual void           GetProjectionMatrix(float* mat)                                                                                                                                                                                 = 0;

	//! for editor
	virtual Legacy::Vec3   GetUnProject(const Legacy::Vec3& WindowCoords, const CCamera& cam)                                                                                                                                              = 0;

	virtual void           RenderToViewport(const CCamera& cam, float x, float y, float width, float height)                                                                                                                               = 0;

	virtual void           PrintLine(const char* szText, SDrawTextInfo& info)                                                                                                                                                              = 0;

	virtual int            EnumDisplayFormats(SDispFormat* formats)                                                                                                                                                                        = 0;

	virtual float          ScaleCoordX(float value)                                                                                                                                                                                        = 0;
	virtual float          ScaleCoordY(float value)                                                                                                                                                                                        = 0;

	virtual void           SetState(State state, bool enable)                                                                                                                                                                              = 0;
	inline void            SetState(int State){
        // CryLog(__FUNCTION__);
        // NOT_IMPLEMENTED;
    };
	virtual void            SetCullMode(CullMode mode = CullMode::BACK)                                = 0;

	virtual void            PushProfileMarker(char* label)                                             = 0;
	virtual void            PopProfileMarker(char* label)                                              = 0;

	//////////////////////////////////////////////////////////////////////
	//! Interface for auxiliary geometry (for debugging, editor purposes, etc.)
	virtual IRenderAuxGeom* GetIRenderAuxGeom(/*EViewportType viewport = eViewportType_Default*/) = 0;
	//////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////////////////////
	// Render-context management
	/////////////////////////////////////////////////////////////////////////////////
	virtual bool            DeleteContext(WIN_HWND hWnd)                                               = 0;
	virtual bool            CreateContext(WIN_HWND hWnd, bool bMainViewport, int SSX = 1, int SSY = 1) = 0;
	virtual bool            SetCurrentContext(WIN_HWND hWnd)                                           = 0;
	virtual void            MakeMainContextActive()                                                    = 0;
	virtual WIN_HWND        GetCurrentContextHWND()                                                    = 0;
	virtual bool            IsCurrentContextMainVP()                                                   = 0;

	//! Gets height of the current viewport.
	virtual int             GetCurrentContextViewportHeight() const                                    = 0;

	//! Gets width of the current viewport.
	virtual int             GetCurrentContextViewportWidth() const                                     = 0;
	/////////////////////////////////////////////////////////////////////////////////
	// for stats
	virtual int             GetPolyCount()                                                             = 0;
	virtual void            GetPolyCount(int& nPolygons, int& nShadowVolPolys)                         = 0;

	// 3d engine set this color to fog color
	virtual void            SetClearColor(const Legacy::Vec3& vColor)                                  = 0;
	virtual void            ClearDepthBuffer()                                                         = 0;
	virtual void            ClearColorBuffer(const Legacy::Vec3 vColor)                                = 0;

	virtual int             GetFrameID(bool bIncludeRecursiveCalls = true)                             = 0;
	//////////////////////////////////////////////////////////////////////
	/*!	Draw an image on the screen as a label text
			@param vPos:	3d position
			@param fSize: size of the image
			@nTextureId:	Texture Id dell'immagine
	*/
	virtual void            DrawLabelImage(const Legacy::Vec3& vPos, float fSize, int nTextureId)
	{
		NOT_IMPLEMENTED;
	}

	virtual void DrawLabel(Legacy::Vec3 pos, float font_size, const char* label_text, ...)
	{
		NOT_IMPLEMENTED;
	}
	virtual void DrawLabelEx(Legacy::Vec3 pos, float font_size, float* pfColor, bool bFixedSize, bool bCenter, const char* label_text, ...)
	{
		NOT_IMPLEMENTED;
	}
	virtual void Draw2dLabel(float x, float y, float font_size, float* pfColor, bool bCenter, const char* label_text, ...)
	{
		NOT_IMPLEMENTED;
	}

	virtual void                           SetRenderTarget(int nHandle)                                                                                                                                   = 0;
	virtual int                            CreateRenderTarget()                                                                                                                                           = 0;

	virtual void                           DrawFullscreenQuad()                                                                                                                                           = 0;
	virtual void                           Set2DMode(bool enable, int ortox, int ortoy)                                                                                                                   = 0;

	//! Create a vertex buffer
	virtual CVertexBuffer*                 CreateBuffer(int vertexcount, int vertexformat, const char* szSource, bool bDynamic = false)                                                                   = 0;

	//! Release a vertex buffer
	virtual void                           ReleaseBuffer(CVertexBuffer* bufptr)                                                                                                                           = 0;

	//! Draw a vertex buffer
	virtual void                           DrawBuffer(CVertexBuffer* src, SVertexStream* indicies, int numindices, int offsindex, int prmode, int vert_start = 0, int vert_stop = 0, CMatInfo* mi = NULL) = 0;

	//! Update a vertex buffer
	virtual void                           UpdateBuffer(CVertexBuffer* dest, const void* src, int vertexcount, bool bUnLock, int nOffs = 0, int Type = 0)                                                 = 0;

	virtual void                           CreateIndexBuffer(SVertexStream* dest, const void* src, int indexcount)                                                                                        = 0;
	//! Update indicies
	virtual void                           UpdateIndexBuffer(SVertexStream* dest, const void* src, int indexcount, bool bUnLock = true)                                                                   = 0;
	virtual void                           ReleaseIndexBuffer(SVertexStream* dest)                                                                                                                        = 0;

	virtual ITechniqueManager*             GetITechniqueManager()                                                                                                                                         = 0;
	virtual struct IFont*                  GetIFont()                                                                                                                                                     = 0;

	virtual IGraphicsDeviceConstantBuffer* CreateConstantBuffer(int size)                                                                                                                                 = 0;

	virtual float                          GetDepthValue(int x, int y)                                                                                                                                    = 0;

	virtual void                           Flush()                                                                                                                                                        = 0;

	////////////////////////////////////////////////////////////////////////////////
	virtual IShader*                       Sh_Load(const char* name, int flags = 0, uint64 nMaskGen = 0)                                                                                                  = 0;
	virtual void                           Sh_Reload()                                                                                                                                                    = 0;
	// Loading of the texture for name(nameTex)
	// virtual ITexture* LoadTexture(const char* nameTex, uint flags, byte eTT) = 0;
	virtual ITexPic*                       EF_GetTextureByID(int Id)                                                                                                                                      = 0;
	virtual ITexPic*                       EF_LoadTexture(const char* nameTex, uint flags, uint flags2, byte eTT, float fAmount1 = -1.0f, float fAmount2 = -1.0f, int Id = -1, int BindId = 0)            = 0;
	// Return different common shader parameters (used in ShaderBrowser) CryIndEd.exe
	virtual void*                          EF_Query(int Query, int Param = 0)                                                                                                                             = 0;

	virtual unsigned int                   LoadTexture(const char* filename, int* tex_type = NULL, unsigned int def_tid = 0, bool compresstodisk = true, bool bWarn = true)                               = 0;
#if 0
	virtual bool		 DXTCompress(byte* raw_data, int nWidth, int nHeight, ETEX_Format eTF, bool bUseHW, bool bGenMips, int nSrcBytesPerPix, MIPDXTcallback callback = 0)
	{
		NOT_IMPLEMENTED_V;
	}
	virtual bool		 DXTDecompress(byte* srcData, byte* dstData, int nWidth, int nHeight, ETEX_Format eSrcTF, bool bUseHW, int nDstBytesPerPix)	
	{
		NOT_IMPLEMENTED_V;
	}
#endif
	virtual void RemoveTexture(unsigned int TextureId)          = 0;
	virtual void RemoveTexture(ITexPic* pTexPic)                = 0;

	//! Set the current binded texture
	virtual void SetTexture(int tnum, ETexType Type = eTT_Base) = 0;
};

extern "C"
{
	IRENDER_API IRenderer* CreateIRender(ISystem* pSystem /*, void* hinst, void* hwnd, bool usedinput*/);
	typedef IRenderer* (*PFNCREATERENDERERINTERFACE)(ISystem* pSystem);
}

//////////////////////////////////////////////////////////////////////////
// Query types for CryInd editor (used in EF_Query() function)
enum ERenderQueryTypes
{
	EFQ_NUMEFS              = 0,
	EFQ_LOADEDEFS           = 1,
	EFQ_NUMTEXTURES         = 2,
	EFQ_LOADEDTEXTURES      = 3,
	EFQ_NUMEFFILES0         = 6,
	EFQ_NUMEFFILES1         = 7,
	EFQ_EFFILENAMES0        = 12,
	EFQ_EFFILENAMES1        = 13,
	EFQ_VProgramms          = 16,
	EFQ_PShaders            = 17,
	EFQ_LightSource         = 18,
	EFQ_RecurseLevel        = 19,
	EFQ_Pointer2FrameID     = 20,
	EFQ_RegisteredTemplates = 21,
	EFQ_NumRenderItems      = 22,
	EFQ_DeviceLost          = 23,
	EFQ_CubeColor           = 24,
	EFQ_D3DDevice           = 25,
	EFQ_glReadPixels        = 26,

	EFQ_Orients             = 33,
	EFQ_NumOrients          = 34,
	EFQ_SkyShader           = 35,
	EFQ_SunFlares           = 36,
	EFQ_CurSunFlare         = 37,
	EFQ_Materials           = 38,
	EFQ_LightMaterials      = 39,
};

//////////////////////////////////////////////////////////////////////

#define STRIPTYPE_NONE           0
#define STRIPTYPE_ONLYLISTS      1
#define STRIPTYPE_SINGLESTRIP    2
#define STRIPTYPE_MULTIPLESTRIPS 3
#define STRIPTYPE_DEFAULT        4

/////////////////////////////////////////////////////////////////////

// DOC-IGNORE-BEGIN
#include "VertexFormats.hpp"
//#include "LeafBuffer.h"
// DOC-IGNORE-END

//////////////////////////////////////////////////////////////////////////
// this structure used to pass render parameters to Render() functions of IStatObj and ICharInstance
struct SRendParams
{
	SRendParams()
	{
		memset(this, 0, sizeof(SRendParams));
		nShaderTemplate = -2;
		fScale          = 1.f;
#if 0
	  vColor(1.f, 1.f, 1.f);
#endif
		fAlpha      = 1.f;
		fSQDistance = -1.f;
	}

	SRendParams(const SRendParams& rThat)
	{
		memcpy(this, &rThat, sizeof(SRendParams));
	}

	int               texture = -1;
	//! position of render elements
	Legacy::Vec3      vPos;
	//! scale of render elements
	float             fScale;
	//! angles of the object
	Legacy::Vec3      vAngles;
	//! object transformations
	Legacy::Matrix44* pMatrix;
	//! custom offset for sorting by distance
	float             fCustomSortOffset;
	//! shader template to use
	int               nShaderTemplate;
	//! light mask to specifiy which light to use on the object
	unsigned int      nDLightMask;
	//! strongest light affecting the object
	unsigned int      nStrongestDLightMask;
	//! fog volume id
	int               nFogVolumeID;
	//! amount of bending animations for vegetations
	float             fBending;
	//! state shader
	IShader*          pStateShader;
	//! list of shadow map casters
#if 0
  list2<ShadowMapLightSourceInstance> * pShadowMapCasters;
#endif
	//! object color
	Legacy::Vec3                 vColor{1.f, 1.f, 1.f};
	//! object alpha
	float                        fAlpha;
	//! force a sort value for render elements
	int                          nSortValue;
	//! Ambient color for the object
	Legacy::Vec3                 vAmbientColor;
	//! distance from camera
	float                        fDistance;
	//! CCObject flags
	int                          dwFObjFlags;
	//! light source for shadow volume calculations
	CDLight*                     pShadowVolumeLightSource;
	//! reference to entity, allows to improve handling of shadow volumes of IStatObj instances
	struct IEntityRender*        pCaller;
	//! Heat Amount for heat vision
	float                        fHeatAmount;
	//! define size of shadow volume
	float                        fShadowVolumeExtent;
	//! lightmap informaion
	struct RenderLMData*         pLightMapInfo;
	struct CLeafBuffer*          pLMTCBuffer; // Object instance specific tex LM texture coords;
	byte                         arrOcclusionLightIds[4];
	//! Override material.
	IMatInfo*                    pMaterial;
	//! Scissor settings for this object
	//  int nScissorX1, nScissorY1, nScissorX2, nScissorY2;
	//! custom shader params
	TArray<struct SShaderParam>* pShaderParams;
	//! squared distance to the center of object
	float                        fSQDistance;
	//! CCObject custom data
	void*                        pCCObjCustomData;
};

////////////////////////////////////////////////////////////////////////////////
class ScopedState
{
public:
	ScopedState(IRenderer* pRender, IRenderer::State state, bool enable)
	    : pRender(pRender)
	    , state(state)
	    , enable(enable)
	{
		pRender->SetState(state, enable);
	}
	~ScopedState()
	{
		pRender->SetState(state, !enable);
	}
	IRenderer*       pRender;
	IRenderer::State state;
	bool             enable;
};

#define RSS(r, s, e) ScopedState _ss##s(r, IRenderer::State::s, e)
