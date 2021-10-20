#pragma once
//#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/Utils/smartptr.hpp>
#include <map>
#include <string>
#include <string_view>
#include <utility>

struct ITexture;
class CCamera;

struct IShader;
class CCObject;

using ShaderRef = _smart_ptr<IShader>;

struct IIncluder
{
	virtual ~IIncluder()									  = default;
	virtual std::string_view OnInclude(std::string_view file) = 0;
};

class BasicIncluder final : public IIncluder
{
	std::string_view OnInclude(std::string_view file) override
	{
		return "";
	}
};

// SShader::m_Flags
// Different useful flags
#define EF_RELOAD 1 // Shader needs tangent vectors array.
#define EF_FORCE_RELOAD 2
#define EF_RELOADED 4

enum EEfResTextures
{
	EFTT_DIFFUSE = 0,
	EFTT_NORMALS,
	EFTT_SPECULAR,
	EFTT_ENV,
	EFTT_DETAIL_OVERLAY,
	EFTT_SMOOTHNESS,
	EFTT_HEIGHT,
	EFTT_DECAL_OVERLAY,
	EFTT_SUBSURFACE,
	EFTT_CUSTOM,
	EFTT_CUSTOM_SECONDARY,
	EFTT_OPACITY,
	EFTT_TRANSLUCENCY,
	EFTT_EMITTANCE,

	EFTT_MAX,
	EFTT_UNKNOWN = EFTT_MAX
};


//////////////////////////////////////////////////////////////////////////
// Type of the texture
enum ETexType
{
  eTT_Base,
  eTT_Cubemap,
  eTT_AutoCubemap,
  eTT_Bumpmap,
  eTT_DSDTBump,
  eTT_Rectangle,
	eTT_3D
};

#define FFILT_NONE   0
#define FFILT_POINT  1
#define FFILT_LINEAR 2
#define FFILT_ANISOTROPIC 3

#define FTU_OPAQUE 1
#define FTU_CLAMP  2
#define FTU_NOMIPS 4
#define FTU_NOBUMP 8
#define FTU_PROJECTED       0x10
#define FTU_FILTERBILINEAR  0x20
#define FTU_FILTERTRILINEAR 0x40
#define FTU_FILTERNEAREST   0x80
#define FTU_FILTERLINEAR    0x100
#define FTU_BUMPPLANTS      0x1000
#define FTU_NOSCALE         0x2000



enum class ShaderLangId
{
	None,
	Glsl,
	Hlsl
};

struct IShader
{
  public:
	enum Type : int
	{
		E_VERTEX,
		E_GEOMETRY,
		E_FRAGMENT,
		E_NUM
	};
	virtual ~IShader() {}
	virtual int GetID()							= 0;
	virtual void AddRef()						= 0;
	virtual int Release()						= 0;
	virtual bool Reload()						= 0;
	virtual IShader::Type GetType()				= 0;
	virtual const char* GetName()				= 0;
	#if 0
	virtual eVertexFormat GetVertexFormat(void) = 0;
	#endif
	virtual void Bind()							= 0;
};

#if 0
//////////////////////////////////////////////////////////////////////
class CRendElement
{
public:
	#if 0
  EDataType m_Type;
  #endif
  uint m_Flags;

public:
  int m_nCountCustomData;
  void *m_CustomData;
  float m_fFogScale;
  #if 0
  int m_CustomTexBind[MAX_CUSTOM_TEX_BINDS_NUM];
  #endif
  CFColor m_Color;
  int m_SortId;

  static CRendElement m_RootGlobal;
  CRendElement *m_NextGlobal;
  CRendElement *m_PrevGlobal;

  class CVProgram *m_LastVP;      // Last Vertex program which updates Z buffer

protected:


  _inline void UnlinkGlobal()
  {
	  if (!m_NextGlobal || !m_PrevGlobal)
		  return;
	  m_NextGlobal->m_PrevGlobal = m_PrevGlobal;
	  m_PrevGlobal->m_NextGlobal = m_NextGlobal;
	  m_NextGlobal = m_PrevGlobal = NULL;
  }
  _inline void LinkGlobal(CRendElement* Before)
  {
	  if (m_NextGlobal || m_PrevGlobal)
		  return;
	  m_NextGlobal						 = Before->m_NextGlobal;
	  Before->m_NextGlobal->m_PrevGlobal = this;
	  Before->m_NextGlobal				 = this;
	  m_PrevGlobal						 = Before;
  }

public:
  CRendElement()
  {
#if 0
    m_Type = eDATA_Unknown;
#endif
	  m_NextGlobal = NULL;
	  m_PrevGlobal = NULL;
	  m_Flags	   = 0;
	  m_CustomData = NULL;
	  #if 0
	  for (int i = 0; i < MAX_CUSTOM_TEX_BINDS_NUM; i++)
		  m_CustomTexBind[i] = -1;
	  #endif
	  m_fFogScale = 0;
	  m_SortId	  = 0;
	  m_LastVP	  = NULL;
	  if (!m_RootGlobal.m_NextGlobal)
	  {
		  m_RootGlobal.m_NextGlobal = &m_RootGlobal;
		  m_RootGlobal.m_PrevGlobal = &m_RootGlobal;
	  }
	  if (this != &m_RootGlobal)
		  LinkGlobal(&m_RootGlobal);
  }

  virtual ~CRendElement()
  {
	  #if 0
	  if ((m_Flags & FCEF_ALLOC_CUST_FLOAT_DATA) && m_CustomData)
	  {
		  delete[]((float*)m_CustomData);
		  m_CustomData = 0;
	  }
	  #endif
	  UnlinkGlobal();
  }

  const char* mfTypeString();

  #if 0
  EDataType mfGetType() { return m_Type; }

  void mfSetType(EDataType t) { m_Type = t; }
#endif
  uint mfGetFlags(void) { return m_Flags; }
  void mfSetFlags(uint fl) { m_Flags = fl; }
  void mfUpdateFlags(uint fl) { m_Flags |= fl; }
  void mfClearFlags(uint fl) { m_Flags &= ~fl; }

  virtual void			   mfPrepare();
  virtual bool			   mfCullByClipPlane(CCObject* pObj);
  virtual CMatInfo*		   mfGetMatInfo();
  virtual list2<CMatInfo>* mfGetMatInfoList();
  virtual int			   mfGetMatId();
  virtual bool			   mfCull(CCObject* obj);
  #if 0
  virtual bool			   mfCull(CCObject* obj, SShader* ef);
  #endif
  virtual void			   mfReset();
  virtual CRendElement*	   mfCopyConstruct(void);
  virtual void			   mfCenter(Vec3& centr, CCObject* pObj);
  virtual void			   mfGetBBox(Vec3& vMins, Vec3& vMaxs)
  {
	  vMins.Set(0, 0, 0);
	  vMaxs.Set(0, 0, 0);
  }
  virtual void			mfGetPlane(Plane& pl);
  virtual float			mfDistanceToCameraSquared(const CCObject& thisObject);
  virtual void			mfEndFlush();
  virtual void			Release();
  virtual int			mfTransform(Matrix44& ViewMatr, Matrix44& ProjMatr, vec4_t* verts, vec4_t* vertsp, int Num);
  #if 0
  virtual bool			mfIsValidTime(SShader* ef, CCObject* obj, float curtime);
  virtual void			mfBuildGeometry(SShader* ef);
  virtual bool			mfCompile(SShader* ef, char* scr);
  virtual CRendElement* mfCreateWorldRE(SShader* ef, SInpData* ds);
  virtual bool			mfDraw(SShader* ef, SShaderPass* sfm);
  virtual void*			mfGetPointer(ESrcPointer ePT, int* Stride, int Type, ESrcPointer Dst, int Flags);
  virtual bool			mfPreDraw(SShaderPass* sl) { return true; }
  virtual float			mfMinDistanceToCamera(CCObject* pObj) { return -1; };
  #endif
  virtual bool			mfCheckUpdate(int nVertFormat, int Flags)
  {
	  int i = Flags;
	  return true;
  }
  virtual int Size() { return 0; }
};

//////////////////////////////////////////////////////////////////////
// Objects using in shader pipeline

// Size of CCObject currently is 256 bytes. If you add new members better to ask me before or just
// make sure CCObject is cache line aligned (64 bytes)
class CCObject
{
public:

  CCObject()
  {
	  #if 0
    m_ShaderParams = NULL;
	#endif
    m_bShaderParamCreatedInRenderer = false;
    m_VPMatrixId = -1;
    m_VPMatrixFrame = -1;
  }
  ~CCObject();

  short m_Id;
  short m_Counter;
  short m_VisId;
  short m_VPMatrixId;
  uint m_ObjFlags;
  Matrix44 m_Matrix;
  bool m_bShaderParamCreatedInRenderer;
  #if 0
  TArray<SShaderParam> *m_ShaderParams;
  #endif
  // Lightmap textures ID's
	short m_nLMId;
	short m_nLMDirId;
  short m_nOcclId;
	short m_nHDRLMId;
  uint m_Reserved1;

  uint m_DynLMMask;
  float m_Trans2[3];
  float m_Angs2[3];
  // Different useful vars (ObjVal component in shaders)
  // [0] - used for blending trees sun-rabbits on distance (0-1)
  float m_TempVars[5]; 
  float m_fDistanceToCam;

  CRendElement *m_RE;
  IShader *m_EF;
  void *m_CustomData;
  uint m_RenderState;

  float m_SortId; // Custom sort value

  short m_InvMatrixId;
  short m_VPMatrixFrame;
  short m_NumCM; // Custom Cube/Texture id (indoor engine soft shadows)
  union
  {
    short m_nLod;
    short m_NumWFX;
    short m_TexId0;
  };
  union
  {
    short m_nTemplId;
    short m_NumWFY;
    short m_TexId1;
  };
  union
  {
    short m_FrameLight;
    bool m_bVisible;
  };
  short m_LightStyle;
  short m_nScissorX1, m_nScissorY1, m_nScissorX2, m_nScissorY2;
  
  union
  {
    float m_fRefract;
    float m_StartTime;
	};
  union
  {
    float m_fBump;
    float m_fLightFadeTime;
  };
  float m_fHeatFactor;  

  list2<struct ShadowMapLightSourceInstance> * m_pShadowCasters; // list of shadow casters 
  struct ShadowMapFrustum * m_pShadowFrustum; // will define projection of shadow from this object

  Vec3d m_AmbColor;
  CFColor m_Color;

  //IDeformableRenderMesh * m_pCharInstance;
  struct CLeafBuffer *m_pLMTCBufferO;

  union
  {
	  ITexPic	*m_pLightImage;
    CDLight *m_pLight;
    float m_fBending;
    float m_fLastUpdate;
    byte m_OcclLights[4];
  };

  _inline Vec3 GetTranslation() const
  {
    return m_Matrix.GetTranslationOLD();
  }
  _inline float GetScaleX() const
  {
    return cry_sqrtf(m_Matrix(0,0)*m_Matrix(0,0) + m_Matrix(0,1)*m_Matrix(0,1) + m_Matrix(0,2)*m_Matrix(0,2));
  }
  _inline float GetScaleZ() const
  {
    return cry_sqrtf(m_Matrix(2,0)*m_Matrix(2,0) + m_Matrix(2,1)*m_Matrix(2,1) + m_Matrix(2,2)*m_Matrix(2,2));
  }
  _inline bool IsMergable()
  {
    if (m_Color.a != 1.0f)
      return false;
    if (m_pShadowCasters)
      return false;
	#if 0
    if (m_pCharInstance)
      return false;
	#endif
    return true;
  }

  #if 0
  static TArray<SWaveForm2> m_Waves;
  static MatrixArray16 m_ObjMatrices;
  #endif
  void Init();

  void CloneObject(CCObject *srcObj)
  {
    int Id = m_Id;
    int VisId = m_VisId;
    memcpy(this, srcObj, sizeof(*srcObj));
    m_Id = Id;
    m_VisId = VisId;
  }

  Matrix44 &GetMatrix()
  {
    return m_Matrix;
  }

  Matrix44 &GetInvMatrix();
  Matrix44 &GetVPMatrix();

  void SetScissor();
  //void SetAlphaState(CPShader *pPS, int nCurState);

  //virtual void AddWaves(SWaveForm2 **wf);
  //virtual void SetShaderFloat(const char *Name, float Val);
  //virtual void RemovePermanent();

#ifdef _RENDERER
#ifdef DEBUGALLOC
#undef new
#endif
	// Sergiy: it's enough to allocate 16 bytes more, even on 64-bit machine
	// - and we need to store only the offset, not the actual pointer
  void* operator new( size_t Size )
  {
    byte *ptr = (byte *)malloc(Size+16+4);
    memset(ptr, 0, Size+16+4);
    byte *bPtrRes = (byte *)((INT_PTR)(ptr+4+16) & ~0xf);
    ((byte**)bPtrRes)[-1] = ptr;

    return bPtrRes;
  }
  void* operator new[](size_t Size)
  {
    byte *ptr = (byte *)malloc(Size+16+2*sizeof(INT_PTR));
    memset(ptr, 0, Size+16+2*sizeof(INT_PTR));
    byte *bPtrRes = (byte *)((INT_PTR)(ptr+16+2*sizeof(INT_PTR)) & ~0xf);
    ((byte**)bPtrRes)[-2] = ptr;

    return bPtrRes-sizeof(INT_PTR);
  }
  void operator delete( void *Ptr )
  {
    byte *bActualPtr = ((byte **)Ptr)[-1];
		assert (bActualPtr <= (byte*)Ptr && (byte*)Ptr-bActualPtr < 20);
    free ((void *)bActualPtr);
  }

  void operator delete[]( void *Ptr )
  {
		byte *bActualPtr = ((byte **)Ptr)[-1];
		free ((void *)bActualPtr);
  }
#ifdef DEBUGALLOC
#define new DEBUG_CLIENTBLOCK
#endif
#endif // _RENDERER
};
#endif
//////////////////////////////////////////////////////////////////////
struct ShaderDesc
{
	ShaderDesc()
	{
	}
	ShaderDesc(std::string name)
		: name(std::move(name))
	{
	}

	ShaderDesc(std::string name, IShader::Type type);

	inline void AddMacro(const std::string& key, const std::string& value)
	{
		macro.insert(Macro::value_type(key, value));
	}

	using Macro = std::map<std::string, std::string>;
	std::string name;
	IShader::Type type;
	Macro macro;

	const std::string root = "res/shaders/";
};
struct ProgramDesc
{
	std::string name;
	ShaderDesc vs;
	ShaderDesc fs;
	ShaderDesc gs;
	ShaderDesc cs;
};


//////////////////////////////////////////////////////////////////////
// DLights
#define DLF_DETAIL					1
#define DLF_DIRECTIONAL			2
#define DLF_DYNAMIC					4							//dynamic lights
#define DLF_ACTIVE					8							//light is active / inactive
#define DLF_CASTSHADOW_MAPS 0x10					//light casting shadows
#define DLF_POINT						0x20
#define DLF_PROJECT					0x40
#define DLF_CASTSHADOW_VOLUME			0x80		//light casting shadows
#define DLF_POSITIONCHANGED 0x100
#define DLF_NOATTENUATION   0x200
#define DLF_UPDATED					0x400
#define DLF_INWORLDSPACE  	0x800
#define DLF_TEMP   					0x1000
#define DLF_STATIC_ADDED		0x2000				//this static light has been already added to the list
#define DLF_HASAMBIENT			0x4000
#define DLF_HEATSOURCE			0x8000
#define DLF_LIGHTSOURCE			0x10000
#define DLF_FAKE      			0x20000				//actually it's not LS, just render elements (Flares, beams, ...)
#define DLF_SUN      			  0x40000				//only sun may use this flag
#define DLF_COPY     			  0x80000
#define DLF_LOCAL    			  0x100000
#define DLF_LM      			  0x200000
#define DLF_THIS_AREA_ONLY  0x400000			// affects only current area/sector
#define DLF_AMBIENT_LIGHT		0x800000			// only used to add better ambient lighting to polybump characters
#define DLF_IGNORE_OWNER		0x1000000			// do not affect light owner object
#define DLF_IGNORE_TERRAIN	0x2000000			// do not affect heightmap
#define DLF_ONLY_FOR_HIGHSPEC	0x4000000		//!< This light is active as dynamic light only for high spec machines.
#define DLF_SPECULAR_ONLY_FOR_HIGHSPEC	0x8000000 //!< This light have specular component enabled only for high spec machines.
#define DLF_LMDOT3          0x10000000
#define DLF_FAKE_RADIOSITY  0x20000000
#define DLF_LMOCCL					0x40000000

#define DLF_LIGHTTYPE_MASK (DLF_DIRECTIONAL | DLF_POINT | DLF_PROJECT)

struct IEntity;
struct ShadowMapLightSourceInstance;
// Marco's NOTE: Andrey / Vlad please subdivide this class
// by putting the members into functions and use the names
// info only if in debug mode, other structures can be
// allocated only if needed in the constructor and destroyed in
// the destructor, and you can even define an operator -> to
// access the data into the various structures inside the class

//////////////////////////////////////////////////////////////////////
class CDLight
{
public:

	//! constructor
	CDLight( void )
  {
    memset(this, 0, sizeof(CDLight));
    m_fLightFrustumAngle = 45.0f;
    m_fRadius = 4.0f;
    m_fDirectFactor = 1.0f;
    //m_Flags = DLF_LIGHTSOURCE;
	#if 0
    m_Orientation.m_vForward = Legacy::Vec3(1,0,0);
    m_Orientation.m_vUp    = Legacy::Vec3(0,1,0);
    m_Orientation.m_vRight = Legacy::Vec3(0,0,1);
	#endif
    m_NumCM = -1;
		m_nEntityLightId = -1;
  }

	//! destructor
  ~CDLight()
  {
		SAFE_RELEASE( m_pShader );
		SAFE_RELEASE( m_pLightImage );
  }

	//!
  bool Parse( void )
  {
    if (!m_Name[0])
      return false;

    if (strncmp(m_Name, "ls_", 3)==0 || strstr(m_Name, "_ls"))
			m_Flags |= DLF_LIGHTSOURCE;

    if (strncmp(m_Name, "hs_", 3)==0 || strstr(m_Name, "_hs"))
			m_Flags |= DLF_HEATSOURCE;

    return true;
  }
  void MakeBaseParams()
  {
    m_BaseOrigin = m_Origin;
    m_BaseColor = m_Color;
    m_BaseSpecColor = m_SpecColor;
    m_fBaseRadius = m_fRadius;
    m_fBaseLightFrustumAngle = m_fLightFrustumAngle;
    m_BaseProjAngles = m_ProjAngles;
  }

	//! assignment operator
  CDLight& operator=( const CDLight& dl )
  {
    memcpy(this, &dl, sizeof(CDLight));
		if (m_pShader)
			m_pShader->AddRef();
		if (m_pLightImage)
			m_pLightImage->AddRef();
    m_Flags |= DLF_COPY;
    return *this;
  }

  int															m_Id;
  Legacy::Vec3														m_Origin;					 //world space position
  Legacy::Vec3														m_BaseOrigin;					 //world space position
  CFColor													m_Color;									//!< clampled diffuse light color
  CFColor													m_BaseColor;									//!< clampled diffuse light color
  CFColor 												m_SpecColor;
  CFColor 												m_BaseSpecColor;
  Legacy::Vec3														m_vObjectSpacePos;		 //Object space position
  float														m_fRadius;
  float														m_fBaseRadius;
  float   												m_fDirectFactor;
  float														m_fStartRadius;
  float														m_fEndRadius;
  float														m_fLastTime;
  int     												m_NumCM;

  // Scissor parameters (2d extent)
  short   					m_sX;
  short   					m_sY;
  short   					m_sWidth;
  short   					m_sHeight;
	// Far/near planes
	float							m_fNear;
	float							m_fFar;

  struct IEntityRender *					m_pOwner;
  
	//for static spot light sources casting volumetric shadows	
	int m_nReserved; // compensates for the vtbl
  //COrthoNormalBasis								m_Orientation;
	int															m_CustomTextureId;
  Legacy::Matrix44								m_TextureMatrix;

	//CCObject *											m_pObject[4][4];								//!< Object for light coronas and light flares

	//the light image
	ITexPic*												m_pLightImage;
  float														m_fLightFrustumAngle;
  float														m_fBaseLightFrustumAngle;
  float                           m_fAnimSpeed;

  IShader*												m_pShader;
  Legacy::Vec3														m_ProjAngles;
  Legacy::Vec3														m_BaseProjAngles;

  uint														m_Flags;									//!< flags from above (prefix DLF_)

  char														m_Name[64];
  int                             m_nLightStyle;
  float                           m_fCoronaScale;

  float														m_fStartTime;
  float														m_fLifeTime;							//!< lsource will be removed after this number of seconds

  char														m_sDebugName[8];					//!< name of light creator (for debuging, pointer can't be used since entity may be deleted)

  ShadowMapLightSourceInstance *	m_pShadowMapLightSource;	//!<

  //CLeafBuffer *										m_arrLightLeafBuffers[8]; //!< array of leafbuffers used for heightmap lighting pass

	int															m_nEntityLightId;					//!<
	int															m_nFrameID;								//!<

	//ICryCharInstance * m_pCharInstance; // pointer to character this source is attached to
};

