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

class CLoaderCGF
{
public:
	using TMemoryBlob   = IMemoryBlob;
	using MemoryBlobPtr = _smart_ptr<TMemoryBlob>;
	using FileReader    = CChunkFileReader<TMemoryBlob>;
	using FileReaderPtr = _smart_ptr<FileReader>;

	CLoaderCGF(MemoryBlobPtr fileMapping, aiScene* scene);

	char                          g_szVertexSortOrder[4] = "", g_szFaceSortOrder[4] = "", g_szUVSortOrder[4] = "";

	// print the texture set at the end?
	bool                          m_bCollectTextures           = true;
	// print as the copy command?
	bool                          m_bCollectTexturesForCopying = true;

	typedef std::set<std::string> StringArray;
	// used textures: collection
	StringArray                   g_setTextures;

	// used node names: collection
	StringArray                   g_setNodeNames;
	StringArray                   g_setDuplicateNodeNames;

	void                          printOptionHelp(const char* szOption, const char* szDescription);

	void                          addNodeName(const std::string& strName);

	unsigned                      g_numUnusedBytes   = 0;

	unsigned                      g_numUndupedChunks = 0;

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

	typedef std::map<unsigned, CChunkSizeProps> ChunkCountMap;
	ChunkCountMap                               g_mapChunkCounts;

	//std::string getChunkName (unsigned nChunkType);
public:
	void                                    print(const StringArray& arrStrings);

	void                                    addChunkToCount(unsigned nChunkType, unsigned nChunkSize);

	void                                    printChunkCounts();

	// the reader of the file
	FileReaderPtr                           m_pReader;
	MemoryBlobPtr                           m_FileMapping;

	aiScene*                                m_pScene;

	typedef std::map<unsigned, std::string> ChunkTypeNameMap;
	ChunkTypeNameMap                        g_mapChunkTypeName;

public:
	void         InitChunkTypeNameMap();

	std::string  getChunkName(unsigned nChunkType);

	const char*  getFileType(unsigned nFileType);

	void         printChunkUnknown(const void* pChunk, int nSize);

	void         print(const CryIRGB& color);

	void         print(const char* szName, const TextureMap3& tex);

	void         print(const char* szName, const TextureMap2& tex);

	void         print(const char* szName, const TextureMap& tex);

	unsigned     g_nStandardMaterialCounter = 0;

	void         initVertIndices(std::vector<int>& arrVertIndices, int nVerts);

	static float unpackComponent(const CryVertex& rVector, int nComponent);
	static float unpackComponent(const SMeshMorphTargetVertex& rVector, int nComponent);

	void         sortVertIndices(const CryVertex* pVerts, int nVerts, std::vector<int>& arrVertIndices);

	void         sortVertIndices(const SMeshMorphTargetVertex* pVerts, int nVerts, std::vector<int>& arrVertIndices);

	const char*  getCtrlName(unsigned nCtrl);

	enum BoneIdentificationEnum
	{
		kBoneID,
		kBoneCtrlID
	};

	const char* getBoneName(const std::vector<const char*>& arrNames, int nBone, BoneIdentificationEnum nIdentity = kBoneID);

	void        getBoneNames(const BONENAMELIST_CHUNK_DESC_0744* pChunk, unsigned nSize, std::vector<const char*>& arrNames);

	const char* getBoneName0744(const BONENAMELIST_CHUNK_DESC_0744* pChunk, unsigned nSize, int nBone, BoneIdentificationEnum nIdentity = kBoneID);

	void        getBoneNames(const BONENAMELIST_CHUNK_DESC_0745* pNameChunk, unsigned nChunkSize, std::vector<const char*>& arrGeomBoneNameTable);

	const char* getBoneName0745(const BONENAMELIST_CHUNK_DESC_0745* pNameChunk, unsigned nChunkSize, int nBone, BoneIdentificationEnum nIdentity = kBoneID);

	const char* getBoneName(int nBone, BoneIdentificationEnum nIdentity = kBoneID);

	// searches the given object's node (the node with the object id set to this id)
	// and returns its name
	const char* getObjectName(int nChunkId);

	struct CMeshData
	{
		CMeshData(unsigned nChunkId, FileReaderPtr m_pReader);

		unsigned         nVerts;
		const CryVertex* pVerts;
	};

	// searches for the given material node and returns its name
	const char* getMaterialName(int nMtlId);

	void        printMtlSet(const char* szFormat, const std::set<unsigned>& setMtls, const char* szPostfix = "");

	// checks whether the given chunk is really the given chunk; returns true if it's valid
	template<typename Desc>
	bool        checkChunk(const Desc* pData, int nSize);

	std::string getHelperType(HelperTypes nType);

	template<typename T>
	void printSet(const char* szFormat, const std::set<T>& setMtls, const char* szPostfix = "");

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

	typedef std::map<Vec3d, unsigned, CVec3dOrder>           VertexWelderMap;
	typedef std::set<Vec3d, CVec3dOrder>                     VertexWelderSet;
	typedef std::map<Vec3d, std::set<unsigned>, CVec3dOrder> VertexWelderMapSet;

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
	void Load(const char* filename);
};
