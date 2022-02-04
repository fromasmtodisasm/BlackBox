#pragma once

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/3DEngine//IStatObj.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/Core/smartptr.hpp>

#include <IStatObj.h>
#include <list2.h>

#include <memory>

struct Material;
struct aiMaterial;

class CIndexedMesh
{
public:
	// geometry data
	//CObjFace* m_pFaces;
	Legacy::Vec3d* m_pVerts;
	//TexCoord* m_pCoors;
	struct CBasis* m_pTangBasis;
	int* m_pVertMats;

	Legacy::Vec3d* m_pNorms;
	//UColor* m_pColor;
	//UColor* m_pColorSec;
	int m_nFaceCount;
	int m_nVertCount;
	int m_nCoorCount; // number of texture coordinates in m_pCoors array
	int m_nNormCount;

	// bbox
	Legacy::Vec3d m_vBoxMin, m_vBoxMax;

	// materials table
	#if 1
	list2<CMatInfo> m_lstMatTable;
	#else
	list2<aiMaterial> m_lstMatTable;
	#endif
	int m_DiffuseMap = -1;

	// list of geom names from cgf file
	//list2<char*> m_lstGeomNames;

	FILETIME  m_fileTime;

	int	  m_VertexFormat;
	void* m_VertexBuffer;
	std::vector<uint16_t>  m_Indices;

	// constructor
	//CIndexedMesh() {}
	CIndexedMesh();
	const char* m_Name;
	bool LoadCGF(const char* szFileName, const char* szGeomName);


};

//! Type of static sub object.
enum EStaticSubObjectType
{
	STATIC_SUB_OBJECT_MESH,         //!< This simple geometry part of the multi-sub object geometry.
	STATIC_SUB_OBJECT_HELPER_MESH,  //!< Special helper mesh, not rendered usually, used for broken pieces.
	STATIC_SUB_OBJECT_POINT,
	STATIC_SUB_OBJECT_DUMMY,
	STATIC_SUB_OBJECT_XREF,
	STATIC_SUB_OBJECT_CAMERA,
	STATIC_SUB_OBJECT_LIGHT,
};

class CStatObj : public IStatObj
{
  public:
	struct SSubObject
	{
		SSubObject() {  }

		EStaticSubObjectType nType;
		string				 name;
		string				 properties;
		int					 nParent;			  //!< Index of the parent sub object, if there`s hierarchy between them.
		Matrix34			 tm;				  //!< Transformation matrix.
		Matrix34			 localTM;			  //!< Local transformation matrix, relative to parent.
		IStatObj*			 pStatObj;			  //!< Static object for sub part of CGF.
		Vec3				 helperSize;		  //!< Size of the helper (if helper).

		void GetMemoryUsage(ICrySizer* pSizer) const
		{
			pSizer->AddObject(name);
			pSizer->AddObject(properties);
		}
	};

	CStatObj(CIndexedMesh IndexedMesh);

	void GetBBox(Legacy::Vec3& Mins, Legacy::Vec3& Maxs) override
	{
		Mins = m_IndexedMesh.m_vBoxMin;
		Maxs = m_IndexedMesh.m_vBoxMax;
	}
	bool SetShaderTemplate(int nTemplate, const char* TemplName, const char* ShaderName, bool bOnlyRegister = false, int* pnNewTemplateId = NULL) override
	{
		return false;
	}

	virtual void SetTexture(unsigned int texture)
	{
		m_Texture = texture;
	}

	virtual int GetTexture() const
	{
		return m_Texture;
	}

	CIndexedMesh*	 GetTriData() override;
	CLeafBuffer*	 GetLeafBuffer() override;
	void			 SetLeafBuffer(CLeafBuffer* buf) override;
	bool			 EnableLightamapSupport() override;
	phys_geometry*	 GetPhysGeom(int nType = 0) override;
	const char*		 GetScriptMaterialName(int Id = -1) override;
	Legacy::Vec3			 GetBoxMin() override;
	Legacy::Vec3			 GetBoxMax() override;
	void			 SetBBoxMin(const Legacy::Vec3& vBBoxMin) override;
	void			 SetBBoxMax(const Legacy::Vec3& vBBoxMax) override;
	float			 GetRadius() override;
	void			 SetShaderFloat(const char* Name, float Val) override;
	void			 SetColor(const char* Name, float fR, float fG, float fB, float fA) override;
	void			 Refresh(int nFlags) override;
	void			 Render(const struct SRendParams& rParams, const Legacy::Vec3& t, int nLodLevel = 0) override;
	IStatObj*		 GetLodObject(int nLodLevel) override;
	void			 RenderShadowVolumes(const struct SRendParams* pParams, int nLimitLod = -1) override;
	const CDLight*	 GetLightSources(int nId) override;
	const char*		 GetFolderName() override;
	const char*		 GetFileName() override;
	const char*		 GetGeoName() override;
	bool			 IsSameObject(const char* szFileName, const char* szGeomName) override;
	Legacy::Vec3			 GetHelperPos(const char* szHelperName) override;
	const char*		 GetHelperById(int nId, Legacy::Vec3& vPos, Legacy::Matrix44* pMat = NULL, int* pnType = NULL) override;
	const Legacy::Matrix44*	 GetHelperMatrixByName(const char* szHelperName) override;
	void			 RegisterUser() override;
	void			 UnregisterUser() override;
	bool			 IsDefaultObject() override;
	bool			 MakeObjectPicture(unsigned char* pRGBAData, int nWidth) override;
	ItShadowVolume*	 GetShadowVolume() override;
	void			 SetShadowVolume(ItShadowVolume* pSvObj) override;
	bool			 GetOcclusionVolume(list2<Legacy::Vec3>*& plstOcclVolVerts, list2<int>*& plstOcclVolInds) override;
	void			 FreeTriData() override;
	void			 GetMemoryUsage(struct ICrySizer* pSizer) const override;
	bool			 CheckValidVegetation() override;
	float&			 GetRadiusVert() override;
	float&			 GetRadiusHors() override;
	bool			 IsPhysicsExist() override;
	void			 PreloadResources(float fDist, float fTime, int dwFlags) override;
	struct IStatObj* GetIStatObj() { return this; }

	static CStatObj* Load(const char* szFileName, const char* szGeomName);


	CIndexedMesh m_IndexedMesh;
	CVertexBuffer* m_VertexBuffer{};
	SVertexStream  m_IndexBuffer{};

	int m_Texture = -1;
	float m_RadiusVert;
	float m_RadiusHors;
	glm::vec3 m_BoxMin{};
	glm::vec3 m_BoxMax{};
};
