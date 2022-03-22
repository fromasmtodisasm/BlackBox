#pragma once

#include "stdafx.h"
#include "Crc32.h"
#include "CryHeaders.h"
#include "ChunkFileReader.h"
#include "CgfUtils.h"

//#include "StringUtils.h"
#include <BlackBox/Core/StringUtils.h>

#include <assimp/BaseImporter.h>
#include <assimp/Importer.hpp>
#include <assimp/IOSystem.hpp>
#include <assimp/scene.h>

namespace
{
	////////////////////////////////////////////
	template<typename T>
	using DynArray = std::vector<T>;

	using CNodeCGF = aiNode;
	using CMesh    = aiMesh;
#if 0
	using CMaterialCGF    = aiMaterial;
#else
	class CMaterialCGF : public _reference_target_t
	{
	};
#endif

#if 1
	using CNodeCGFPtr     = CNodeCGF*;
	using CMaterialCGFPtr = CMaterialCGF*;
#else
	using CNodeCGFPtr     = std::shared_ptr<CNodeCGF>;
	using CMaterialCGFPtr = std::shared_ptr<CMaterialCGF>;
#endif
} // namespace

class CContentCGF
{
public:
public:
	CContentCGF(const char* filename)
	{
		strcpy(m_filename, filename);
	}
	//////////////////////////////////////////////////////////////////////////
	//! Access to CGF nodes.
	void AddNode(CNodeCGF* pNode)
	{
		m_nodes.push_back(pNode);
	}

	int GetNodeCount() const
	{
		return m_nodes.size();
	}

	CNodeCGF* GetNode(int i)
	{
		return m_nodes[i];
	}

	const CNodeCGF* GetNode(int i) const
	{
		return m_nodes[i];
	}

	void ClearNodes()
	{
		m_nodes.clear();
	}

	void RemoveNode(CNodeCGF* pNode)
	{
		assert(0);
		assert(pNode);
#if 0
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			if (m_nodes[i] == pNode)
			{
				pNode->mParent = 0;
				m_nodes.erase(i);
				break;
			}
		}
#endif
	}
	//////////////////////////////////////////////////////////////////////////

	// Access to CGF materials.
	void AddMaterial(CMaterialCGF* pNode)
	{
		m_materials.push_back(pNode);
	}

	int GetMaterialCount() const
	{
		return m_materials.size();
	}

	CMaterialCGF* GetMaterial(int i)
	{
		return m_materials[i];
	}

	const CMaterialCGF* GetMaterial(int i) const
	{
		return m_materials[i];
	}

	void ClearMaterials()
	{
		m_materials.clear();
	}

	CMaterialCGF* GetCommonMaterial() const
	{
		return m_pCommonMaterial;
	}

	void SetCommonMaterial(CMaterialCGF* pMtl)
	{
		m_pCommonMaterial = pMtl;
	}

	DynArray<int>& GetUsedMaterialIDs()
	{
		return m_usedMaterialIds;
	}

	const DynArray<int>& GetUsedMaterialIDs() const
	{
		return m_usedMaterialIds;
	}

	//////////////////////////////////////////////////////////////////////////

#if 0
	bool ValidateMeshes(const char** const ppErrorDescription) const
	{
		for (int i = 0; i < m_nodes.size(); ++i)
		{
			const CNodeCGF* const pNode = m_nodes[i];
			if (pNode && pNode->pMesh && (!pNode->pMesh->Validate(ppErrorDescription)))
			{
				return false;
			}
		}
		return true;
	}

	//! Set chunk file that this CGF owns.
	void SetChunkFile(IChunkFile* pChunkFile)
	{
		m_pOwnChunkFile = pChunkFile;
	}
#endif

public:
	////////////////////////////////////////////
	char                      m_filename[260];
	DynArray<CNodeCGFPtr>     m_nodes;
	DynArray<CMaterialCGFPtr> m_materials;
	DynArray<int>             m_usedMaterialIds;
	CMaterialCGFPtr           m_pCommonMaterial{};

#if 0
	CPhysicalizeInfoCGF                m_physicsInfo;
	CExportInfoCGF                     m_exportInfo;
	SFoliageInfoCGF                    m_foliageInfo;
	SVClothInfoCGF                     m_vclothInfo;

	IChunkFile* m_pOwnChunkFile;
#endif
};
// helper to get order for Vec3d
struct CVec3dOrder /*: public std::binary_function< Vec3d, Vec3d, bool>*/
{
	bool operator()(const Vec3d& a, const Vec3d& b) const
	{
		// first sort by x
		if (a.x < b.x) return (true);
		if (a.x > b.x) return (false);

		// then by y
		if (a.y < b.y) return (true);
		if (a.y > b.y) return (false);

		// then by z
		if (a.z < b.z) return (true);
		if (a.z > b.z) return (false);

		return (false);
	}
};

class CLoaderCGF
{
public:
	using TMemoryBlob   = IMemoryBlob;
	using MemoryBlobPtr = _smart_ptr<TMemoryBlob>;
	using FileReader    = CChunkFileReader<TMemoryBlob>;
	using FileReaderPtr = _smart_ptr<FileReader>;

	typedef std::map<Vec3d, unsigned, CVec3dOrder>           VertexWelderMap;
	typedef std::set<Vec3d, CVec3dOrder>                     VertexWelderSet;
	typedef std::map<Vec3d, std::set<unsigned>, CVec3dOrder> VertexWelderMapSet;

	typedef std::set<std::string>                            StringArray;

public:
	// chunk counters

	struct CChunkSizeProps
	{
		CChunkSizeProps();
		CChunkSizeProps(unsigned nSize);
		void     inc(unsigned nSize);

		unsigned size() const;
		unsigned count() const;
		double   mean() const;

	protected:
		unsigned m_nSize, m_nCount;
	};

	struct CMeshData
	{
		CMeshData(unsigned nChunkId, FileReaderPtr ChunkFile);

		unsigned         nVerts;
		const CryVertex* pVerts;
	};

	enum BoneIdentificationEnum
	{
		kBoneID,
		kBoneCtrlID
	};

public:
	CLoaderCGF(MemoryBlobPtr fileMapping, aiScene* scene, CContentCGF* pContentCGF);

public:
	void Load(const char* filename);

	//std::string getChunkName (unsigned nChunkType);
public:
	void printOptionHelp(const char* szOption, const char* szDescription);
	void addNodeName(const std::string& strName);
	void print(const StringArray& arrStrings);
	void addChunkToCount(unsigned nChunkType, unsigned nChunkSize);
	void printChunkCounts();

public:
	void        InitChunkTypeNameMap();
	std::string getChunkName(unsigned nChunkType);
	const char* getFileType(unsigned nFileType);

	void        printChunkUnknown(const void* pChunk, int nSize);
	void        print(const CryIRGB& color);
	void        print(const char* szName, const TextureMap3& tex);
	void        print(const char* szName, const TextureMap2& tex);
	void        print(const char* szName, const TextureMap& tex);
	void        initVertIndices(std::vector<int>& arrVertIndices, int nVerts);

public:
	static float unpackComponent(const CryVertex& rVector, int nComponent);
	static float unpackComponent(const SMeshMorphTargetVertex& rVector, int nComponent);

public:
	void        sortVertIndices(const CryVertex* pVerts, int nVerts, std::vector<int>& arrVertIndices);
	void        sortVertIndices(const SMeshMorphTargetVertex* pVerts, int nVerts, std::vector<int>& arrVertIndices);

	const char* getCtrlName(unsigned nCtrl);

	const char* getBoneName(const std::vector<const char*>& arrNames, int nBone, BoneIdentificationEnum nIdentity = kBoneID);
	void        getBoneNames(const BONENAMELIST_CHUNK_DESC_0744* pChunk, unsigned nSize, std::vector<const char*>& arrNames);
	const char* getBoneName0744(const BONENAMELIST_CHUNK_DESC_0744* pChunk, unsigned nSize, int nBone, BoneIdentificationEnum nIdentity = kBoneID);
	void        getBoneNames(const BONENAMELIST_CHUNK_DESC_0745* pNameChunk, unsigned nChunkSize, std::vector<const char*>& arrGeomBoneNameTable);
	const char* getBoneName0745(const BONENAMELIST_CHUNK_DESC_0745* pNameChunk, unsigned nChunkSize, int nBone, BoneIdentificationEnum nIdentity = kBoneID);
	const char* getBoneName(int nBone, BoneIdentificationEnum nIdentity = kBoneID);

	// searches the given object's node (the node with the object id set to this id)
	// and returns its name
	const char* getObjectName(int nChunkId);
	// searches for the given material node and returns its name
	const char* getMaterialName(int nMtlId);
	void        printMtlSet(const char* szFormat, const std::set<unsigned>& setMtls, const char* szPostfix = "");

	// checks whether the given chunk is really the given chunk; returns true if it's valid
	template<typename Desc>
	bool        checkChunk(const Desc* pData, int nSize);

	std::string getHelperType(HelperTypes nType);

	template<typename T>
	void printSet(const char* szFormat, const std::set<T>& setMtls, const char* szPostfix = "");

public:
	void LoadChunkController(const CONTROLLER_CHUNK_DESC_0826* pChunk, int nSize);
	void LoadChunkController(const CONTROLLER_CHUNK_DESC_0827* pChunk, int nSize);
	void LoadChunkTiming(const TIMING_CHUNK_DESC* pChunk, int nSize);
	void LoadChunkMtl(const MTL_CHUNK_DESC_0746* pChunk, int nSize);
	void LoadChunkMtl(const MTL_CHUNK_DESC_0745* pChunk, int nSize);
	void LoadChunkMtl(const MTL_CHUNK_DESC_0744* pChunk, int nSize);
	void LoadChunkMesh(const MESH_CHUNK_DESC* pChunk, int nSize);
	void LoadChunkNode(const NODE_CHUNK_DESC* pChunk, int nSize);
	void LoadChunkBoneNameList(const BONENAMELIST_CHUNK_DESC_0744* pChunk, int nSize);
	void LoadChunkBoneNameList(const BONENAMELIST_CHUNK_DESC_0745* pChunk, int nSize);
	void LoadChunkBoneAnim(const BONEANIM_CHUNK_DESC* pChunk, int nSize);
	void LoadChunkHelper(const HELPER_CHUNK_DESC* pData, int nSize);
	void LoadChunkLight(const LIGHT_CHUNK_DESC* pData, int nSize);
	void LoadChunkBoneLightBinding(const BONELIGHTBINDING_CHUNK_DESC_0001* pChunk, int nSize);
	void LoadChunkMeshMorphTarget(const MESHMORPHTARGET_CHUNK_DESC_0001* pChunk, int nSize);
	void LoadChunkBoneInitialPos(BONEINITIALPOS_CHUNK_DESC_0001* pChunk, int nSize);
	void LoadChunkSourceInfo(const char* pData, unsigned nSize);
	void LoadChunkSceneProps(const char* pData, unsigned nSize);
	void LoadChunk(int i);
	void LoadCollectedTextures(const char* szFormat);

	bool LoadChunks();

	CMaterialCGF* LoadMaterialFromChunk(int nchunkId);

private:
	char                                        g_szVertexSortOrder[4] = "", g_szFaceSortOrder[4] = "", g_szUVSortOrder[4] = "";
	// print the texture set at the end?
	bool                                        m_bCollectTextures           = true;
	// print as the copy command?
	bool                                        m_bCollectTexturesForCopying = true;
	// used textures: collection
	StringArray                                 g_setTextures;
	// used node names: collection
	StringArray                                 g_setNodeNames;
	StringArray                                 g_setDuplicateNodeNames;
	unsigned                                    g_numUnusedBytes   = 0;
	unsigned                                    g_numUndupedChunks = 0;
	// the reader of the file
	FileReaderPtr                               m_pChunkFile;
	CContentCGF*                                m_pCGF;
	MemoryBlobPtr                               m_FileMapping;
	aiScene*                                    m_pScene;
	typedef std::map<unsigned, std::string>     ChunkTypeNameMap;
	ChunkTypeNameMap                            g_mapChunkTypeName;

	typedef std::map<unsigned, CChunkSizeProps> ChunkCountMap;
	ChunkCountMap                               g_mapChunkCounts;

	unsigned                                    g_nStandardMaterialCounter = 0;
};
