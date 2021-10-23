#include <BlackBox/3DEngine/StatObject.hpp>
#include <BlackBox/Renderer/Camera.hpp>
//#include <BlackBox/Geometry/ObjLoader.hpp>
#include <BlackBox/Renderer/Pipeline.hpp>
#include <BlackBox/Renderer/Material.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>

#include <BlackBox\Core\Path.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include <fstream>
#include <iostream>
#include <cctype>
#include <cstdio>
#include <sstream>
#include <memory>

#define NOT_IMPLEMENTED                                         \
	CryFatalError("Method [%s] not implemented", __FUNCTION__); \
	//return {};

CStatObj::CStatObj(CIndexedMesh IndexedMesh)
	: m_IndexedMesh(IndexedMesh)
{

}

CIndexedMesh* CStatObj::GetTriData()
{
	NOT_IMPLEMENTED;
	return {};
}
CLeafBuffer* CStatObj::GetLeafBuffer()
{
	NOT_IMPLEMENTED;
	return {};
}
void CStatObj::SetLeafBuffer(CLeafBuffer* buf)
{
	NOT_IMPLEMENTED;
}
bool CStatObj::EnableLightamapSupport()
{
	NOT_IMPLEMENTED;
	return {};
}
phys_geometry* CStatObj::GetPhysGeom(int nType)
{
	NOT_IMPLEMENTED;
	return {};
}
const char* CStatObj::GetScriptMaterialName(int Id)
{
	NOT_IMPLEMENTED;
	return {};
}
Legacy::Vec3 CStatObj::GetBoxMin()
{
	NOT_IMPLEMENTED;
	return {};
}
Legacy::Vec3 CStatObj::GetBoxMax()
{
	NOT_IMPLEMENTED;
	return {};
}
void CStatObj::SetBBoxMin(const Legacy::Vec3& vBBoxMin)
{
	NOT_IMPLEMENTED;
}
void CStatObj::SetBBoxMax(const Legacy::Vec3& vBBoxMax)
{
	NOT_IMPLEMENTED;
}
float CStatObj::GetRadius()
{
	NOT_IMPLEMENTED;
	return {};
}
void CStatObj::SetShaderFloat(const char* Name, float Val)
{
	NOT_IMPLEMENTED;
}
void CStatObj::SetColor(const char* Name, float fR, float fG, float fB, float fA)
{
	NOT_IMPLEMENTED;
}
void CStatObj::Refresh(int nFlags)
{
	NOT_IMPLEMENTED;
}
void CStatObj::Render(const struct SRendParams& rParams, const Legacy::Vec3& t, int nLodLevel)
{
	if (!m_VertexBuffer)
	{
		m_VertexBuffer = gEnv->pRenderer->CreateBuffer(m_IndexedMesh.m_nVertCount, m_IndexedMesh.m_VertexFormat, "stat_obj", false);	
		gEnv->pRenderer->UpdateBuffer(m_VertexBuffer, m_IndexedMesh.m_VertexBuffer, m_IndexedMesh.m_nVertCount, false);

		m_IndexBuffer = SVertexStream();
		gEnv->pRenderer->CreateIndexBuffer(&m_IndexBuffer, m_IndexedMesh.m_Indices.data(), m_IndexedMesh.m_Indices.size());
	}


	int texId = rParams.texture;
	if(texId == -1)
	{
		texId = m_IndexedMesh.m_DiffuseMap;
	}
	gEnv->pAuxGeomRenderer->DrawMesh(m_VertexBuffer, *rParams.pMatrix, texId);
	//GlobalResources::Bo


}
IStatObj* CStatObj::GetLodObject(int nLodLevel)
{
	NOT_IMPLEMENTED;
	return {};
}
void CStatObj::RenderShadowVolumes(const struct SRendParams* pParams, int nLimitLod)
{
	NOT_IMPLEMENTED;
}
const CDLight* CStatObj::GetLightSources(int nId)
{
	NOT_IMPLEMENTED;
	return {};
}
const char* CStatObj::GetFolderName()
{
	NOT_IMPLEMENTED;
	return {};
}
const char* CStatObj::GetFileName()
{
	NOT_IMPLEMENTED;
	return {};
}
const char* CStatObj::GetGeoName()
{
	NOT_IMPLEMENTED;
	return {};
}
bool CStatObj::IsSameObject(const char* szFileName, const char* szGeomName)
{
	NOT_IMPLEMENTED;
	return {};
}
Legacy::Vec3 CStatObj::GetHelperPos(const char* szHelperName)
{
	NOT_IMPLEMENTED;
	return {};
}
const char* CStatObj::GetHelperById(int nId, Legacy::Vec3& vPos, Legacy::Matrix44* pMat, int* pnType)
{
	NOT_IMPLEMENTED;
	return {};
}
const Legacy::Matrix44* CStatObj::GetHelperMatrixByName(const char* szHelperName)
{
	NOT_IMPLEMENTED;
	return {};
}
void CStatObj::RegisterUser()
{
	NOT_IMPLEMENTED;
}
void CStatObj::UnregisterUser()
{
	NOT_IMPLEMENTED;
}
bool CStatObj::IsDefaultObject()
{
	NOT_IMPLEMENTED;
	return {};
}
bool CStatObj::MakeObjectPicture(unsigned char* pRGBAData, int nWidth)
{
	NOT_IMPLEMENTED;
	return {};
}
ItShadowVolume* CStatObj::GetShadowVolume()
{
	NOT_IMPLEMENTED;
	return {};
}
void CStatObj::SetShadowVolume(ItShadowVolume* pSvObj)
{
	NOT_IMPLEMENTED;
}
bool CStatObj::GetOcclusionVolume(list2<Legacy::Vec3>*& plstOcclVolVerts, list2<int>*& plstOcclVolInds)
{
	NOT_IMPLEMENTED;
	return {};
}
void CStatObj::FreeTriData()
{
	NOT_IMPLEMENTED;
}
void CStatObj::GetMemoryUsage(struct ICrySizer* pSizer) const
{
	pSizer->AddObject(this, sizeof(*this),1);
}
bool CStatObj::CheckValidVegetation()
{
	NOT_IMPLEMENTED;
	return {};
}
float& CStatObj::GetRadiusVert()
{
	return m_RadiusVert;
}
float& CStatObj::GetRadiusHors()
{
	return m_RadiusHors;
}

bool CStatObj::IsPhysicsExist()
{
	NOT_IMPLEMENTED;
	return {};
}
void CStatObj::PreloadResources(float fDist, float fTime, int dwFlags)
{
	NOT_IMPLEMENTED;
}

CStatObj* CStatObj::Load(const char* szFileName, const char* szGeomName)
{
	CIndexedMesh im;
	if (im.LoadCGF(szFileName, szGeomName))
	{
		return new CStatObj(im);
	}

	return nullptr;
}

bool CIndexedMesh::LoadCGF(const char* szFileName, const char* szGeomName)
{
	Assimp::Importer import;
	const aiScene*	 scene = import.ReadFile(szFileName, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		CryError("[ASSIMP] %s", import.GetErrorString());
		return {};
	}

	m_Name = szFileName;
	if (scene->HasMeshes())
	{
		for (size_t i = 0; i < scene->mNumMeshes; i++)
		{
			auto mesh		  = scene->mMeshes[i];

			bool bNeedCol	  = mesh->HasVertexColors(i);
			bool bNeedNormals = mesh->HasNormals();
			bool bHasTC		  = mesh->HasTextureCoords(i);

			m_VertexFormat = 9;
			auto RealFormat = VertFormatForComponents(bNeedCol, false, bNeedNormals, bHasTC);
			if (RealFormat != 9) 
			{
				CryError("[ASSIMP] VertexFormat not eq 9");
			}

			char* vb = (char*)(m_VertexBuffer = CreateVertexBuffer(m_VertexFormat, mesh->mNumVertices));
			

			auto stride = gVertexSize[m_VertexFormat];

			auto TCOffset = g_VertFormatUVOffsets[RealFormat];
			auto NormalsOffset = g_VertFormatNormalOffsets[RealFormat];
			auto vertexSize	   = gVertexSize[RealFormat];

			auto UVs = mesh->mTextureCoords[0];
			m_nVertCount = mesh->mNumVertices;
			for (size_t i = 0; i < m_nVertCount; i++)
			{
				memcpy(&vb[i * stride], &mesh->mVertices[i], sizeof(Legacy::Vec3));
				if (TCOffset != -1) {
					auto _uv = UVs[i];
					Legacy::Vec2 uv	 = Legacy::Vec2(_uv.x, _uv.y);
					memcpy(&vb[i * stride + g_VertFormatUVOffsets[m_VertexFormat]], &uv, sizeof(Legacy::Vec2));

				}
				if (NormalsOffset != -1) {
					auto _N = mesh->mNormals[i];
					Legacy::Vec3 N	 = Legacy::Vec3(_N.x, _N.y, _N.z);
					memcpy(&vb[i * stride + g_VertFormatNormalOffsets[m_VertexFormat]], &N, sizeof(Legacy::Vec3));

				}
			}
			for (int i = 0; i < mesh->mNumFaces; i++)
			{
				const aiFace& Face = mesh->mFaces[i];
				if (Face.mNumIndices == 3)
				{
					m_Indices.push_back(Face.mIndices[0]);
					m_Indices.push_back(Face.mIndices[1]);
					m_Indices.push_back(Face.mIndices[2]);
				}
			}
			std::vector<string> Textures;
			if (scene->HasMaterials())
			{
				auto mat = scene->mMaterials[mesh->mMaterialIndex];
				//m_lstMatTable.Add(mat);
				//aiTextureType textureTypes[aiTextureType::aiTextureType_UNKNOWN];
				//for (auto tt : textureTypes)
				{
					if (mat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) > 0)
					{
						aiString path;
						m_DiffuseMap = mat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path);
						//path.C_Str()
						m_DiffuseMap = gEnv->pRenderer->LoadTexture(path.C_Str());
						if (m_DiffuseMap == -1)
						{
							auto new_path = PathUtil::GetParentDirectory(m_Name);
							m_DiffuseMap = gEnv->pRenderer->LoadTexture((PathUtil::AddSlash(new_path) + path.C_Str()).c_str());
						}
					}
				}
			}
		}
	}
	return true;

}

CIndexedMesh::CIndexedMesh()
{

}
