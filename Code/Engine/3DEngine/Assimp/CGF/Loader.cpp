//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code
//	Copyright (c) Crytek 2001-2004
//
//	File:
//
//  Description:
//
//	History:
//
//////////////////////////////////////////////////////////////////////

// CgfDump.cpp : Defines the entry point for the console application.
//

#include "Loader.h"

#if 0
	#define PRINT_LOG(format, ...) CryLog(format, __VA_ARGS__);
#else
	#if 0
		#define printf CryLog
	#else
		#define printf
	#endif
	#define PRINT_LOG printf
#endif

template<typename TVector, typename TComponent, int nComponents>
class TGeomSorter
{
	const TVector* m_pVertices;
	int            m_nVerts;

protected:
	int m_arrSortOrder[nComponents];

public:
	TGeomSorter(const TVector* pVerts, int nVerts);

	void FixSortOrder();

	bool operator()(int nLeft, int nRight) const
	{
		if (nLeft < 0 || nLeft >= m_nVerts || nRight < 0 || nRight >= m_nVerts)
			return true;
		for (int i = 0; i < nComponents; ++i)
		{
			int nCoord = m_arrSortOrder[i];
			if (CLoaderCGF::unpackComponent(m_pVertices[nLeft], nCoord) < CLoaderCGF::unpackComponent(m_pVertices[nRight], nCoord))
				return true;
			else if (CLoaderCGF::unpackComponent(m_pVertices[nLeft], nCoord) > CLoaderCGF::unpackComponent(m_pVertices[nRight], nCoord))
				return false;
		}
		return true;
	}
};

template<typename TVector>
class TVertexSorter : public TGeomSorter<TVector, float, 3>
{
public:
	TVertexSorter(const TVector* pVerts, int nVerts, char* szSortOrder);
};

template<typename TVector, typename TComponent, int nComponents>
TGeomSorter<TVector, TComponent, nComponents>::TGeomSorter(const TVector* pVerts, int nVerts)
    : m_nVerts(nVerts)
    , m_pVertices(pVerts)
{
}

template<typename TVector, typename TComponent, int nComponents>
void TGeomSorter<TVector, TComponent, nComponents>::FixSortOrder()
{
	for (int i = 0; i < nComponents; ++i)
	{
		if (m_arrSortOrder[i] < 0) m_arrSortOrder[i] = 0;
		if (m_arrSortOrder[i] >= nComponents) m_arrSortOrder[i] = nComponents - 1;
	}
}

template<typename TVector>
TVertexSorter<TVector>::TVertexSorter(const TVector* pVerts, int nVerts, char* szSortOrder)
    : TGeomSorter<TVector, float, 3>(pVerts, nVerts)
{
	if (strlen(szSortOrder) != 3)
	{
		m_arrSortOrder[0] = 0;
		m_arrSortOrder[1] = 1;
		m_arrSortOrder[2] = 2;
	}
	else
	{
		for (int i = 0; i < 3; ++i)
			switch (szSortOrder[i])
			{
			case 'X':
			case 'Y':
			case 'Z':
				m_arrSortOrder[i] = szSortOrder[i] - 'X';
				break;

			case 'x':
			case 'y':
			case 'z':
				m_arrSortOrder[i] = szSortOrder[i] - 'x';
				break;

			default:
				m_arrSortOrder[i] = 0;
				break;
			}
	}
	FixSortOrder();
}

// checks whether the given chunk is really the given chunk; returns true if it's valid
template<typename Desc>
bool CLoaderCGF::checkChunk(const Desc* pData, int nSize)
{
	if (nSize < sizeof(*pData))
	{
		PRINT_LOG("Truncated chunk: %d bytes (%d bytes expected)\n", nSize, sizeof(*pData));
		return false;
	}

	if (pData->chdr.ChunkVersion != pData->VERSION)
	{
		PRINT_LOG("Unknown chunk version 0x%04X\n", pData->chdr.ChunkVersion);
		return false;
	}

	if (nSize != sizeof(*pData))
	{
		PRINT_LOG("Unexpected chunk size: %d bytes (%d bytes expected)\n", nSize, sizeof(*pData));
		return true;
	}

	return true;
}

template<typename T>
void CLoaderCGF::printSet(const char* szFormat, const std::set<T>& setMtls, const char* szPostfix)
{
	if (!setMtls.empty())
	{
		PRINT_LOG(" (");
		for (std::set<T>::const_iterator it = setMtls.begin(); it != setMtls.end();)
		{
			if (it != setMtls.begin())
				PRINT_LOG(", ");
			PRINT_LOG(szFormat, *it);
			T nStart = *it;

			++it;

			if (it != setMtls.end() && *it == nStart + 1)
			{
				T nPrev = *it;
				// we've got a region
				while (++it != setMtls.end() && *it == nPrev + 1)
					nPrev = *it;
				if (nPrev == nStart + 1)
					// special case - range of length 1
					PRINT_LOG(",");
				else
					PRINT_LOG("..");
				PRINT_LOG(szFormat, nPrev);
			}
		}
		PRINT_LOG(")");
	}
	PRINT_LOG("%s", szPostfix);
}

CLoaderCGF::CLoaderCGF(MemoryBlobPtr fileMapping, aiScene* scene, CContentCGF* pContentCGF)
    : m_FileMapping(fileMapping)
    , m_pScene(scene)
    , m_pCGF(pContentCGF)
{
}

void CLoaderCGF::printOptionHelp(const char* szOption, const char* szDescription)
{
	PRINT_LOG("  % 30s   %s\n", szOption, szDescription);
}

void CLoaderCGF::addNodeName(const std::string& strName)
{
	if (g_setNodeNames.find(strName) != g_setNodeNames.end())
	{
		g_setDuplicateNodeNames.insert(strName);
	}
	else
	{
		g_setNodeNames.insert(strName);
	}
}

void CLoaderCGF::print(const StringArray& arrStrings)
{
	for (StringArray::const_iterator it = arrStrings.begin(); it != arrStrings.end(); ++it)
	{
		if (it != arrStrings.begin())
			PRINT_LOG(", ");
		PRINT_LOG("\"%s\"", it->c_str());
	}
}

void CLoaderCGF::addChunkToCount(unsigned nChunkType, unsigned nChunkSize)
{
	if (g_mapChunkCounts.find(nChunkType) != g_mapChunkCounts.end())
		g_mapChunkCounts[nChunkType].inc(nChunkSize);
	else
		g_mapChunkCounts.insert(ChunkCountMap::value_type(nChunkType, CChunkSizeProps(nChunkSize)));
}

void CLoaderCGF::printChunkCounts()
{
	if (!g_mapChunkCounts.empty())
	{
		PRINT_LOG("Chunk counts (%d chunk types encountered):\n\tChunk\tCount\tTotal\tMean\n", g_mapChunkCounts.size());
		for (ChunkCountMap::const_iterator it = g_mapChunkCounts.begin(); it != g_mapChunkCounts.end(); ++it)
			PRINT_LOG("\t%s\t%d\t%d\t%.1f\n", getChunkName(it->first).c_str(), it->second.count(), it->second.size(), it->second.mean());
	}
}

void CLoaderCGF::InitChunkTypeNameMap()
{
#define ADD_CHUNK(NAME) g_mapChunkTypeName[ChunkType_##NAME] = "ChunkType_" #NAME;
	ADD_CHUNK(ANY);
	ADD_CHUNK(Mesh);
	ADD_CHUNK(Helper);
	ADD_CHUNK(VertAnim);
	ADD_CHUNK(BoneAnim);
	ADD_CHUNK(GeomNameList);
	ADD_CHUNK(BoneNameList);
	ADD_CHUNK(MtlList);
	ADD_CHUNK(MRM);
	ADD_CHUNK(SceneProps);
	ADD_CHUNK(Light);
	ADD_CHUNK(PatchMesh);
	ADD_CHUNK(Node);
	ADD_CHUNK(Mtl);
	ADD_CHUNK(Controller);
	ADD_CHUNK(Timing);
	ADD_CHUNK(BoneMesh);
	ADD_CHUNK(BoneLightBinding);
	ADD_CHUNK(MeshMorphTarget);
	ADD_CHUNK(BoneInitialPos);
	ADD_CHUNK(SourceInfo);
#undef ADD_CHUNK
}

std::string CLoaderCGF::getChunkName(unsigned nChunkType)
{
	if (g_mapChunkTypeName.find(nChunkType) == g_mapChunkTypeName.end())
	{
		char szBuffer[0x50];
		sprintf(szBuffer, "Unknown(%d)", nChunkType);
		return szBuffer;
	}
	else
		return g_mapChunkTypeName[nChunkType].c_str();
}

const char* CLoaderCGF::getFileType(unsigned nFileType)
{
	switch (nFileType)
	{
	case FileType_Geom:
		return "FileType_Geom";
	case FileType_Anim:
		return "FileType_Anim";
	default:
		return "UNKNOWN";
	}
}

void CLoaderCGF::printChunkUnknown(const void* pChunk, int nSize)
{
	char* pData = (char*)pChunk;
	++g_numUndupedChunks;
}

void CLoaderCGF::print(const CryIRGB& color)
{
	PRINT_LOG("(%d,%d,%d)", (int)color.r, (int)color.g, (int)color.b);
}

void CLoaderCGF::print(const char* szName, const TextureMap3& tex)
{
	if (!tex.name[0])
		return;

	g_setTextures.insert(tex.name);

	PRINT_LOG("%s \"%s\", Amount %d, Type %s (0x%02X) Flags 0x%02X",
	          szName,
	          tex.name,
	          (int)tex.Amount,
	          getTexType(tex.type),
	          (int)tex.type,
	          (int)tex.flags);

	if (tex.flags & TEXMAP_NOMIPMAP)
		PRINT_LOG(" (TEXMAP_NOMIPMAP)");

	PRINT_LOG("\n");

	if (tex.utile || tex.vtile)
	{
		PRINT_LOG("Tiling:");
		if (tex.utile)
			PRINT_LOG(" U");
		if (tex.vtile)
			PRINT_LOG(" V");
		PRINT_LOG("\n");
	}

	if (tex.umirror || tex.vmirror)
	{
		PRINT_LOG("Mirror:");
		if (tex.umirror)
			PRINT_LOG(" U");
		if (tex.vmirror)
			PRINT_LOG(" V");
		PRINT_LOG("\n");
	}

	if (tex.nthFrame)
		PRINT_LOG("Update reflection every %d frame\n", tex.nthFrame);

	if (tex.refBlur || tex.refSize)
		PRINT_LOG("Reflection size %g, blur %g\n", tex.refSize, tex.refBlur);

	PRINT_LOG("UV Scale (%g,%g), UV Offset (%g,%g), UVW Rotate (%g,%g,%g)\n",
	          tex.uscl_val, tex.vscl_val,
	          tex.uoff_val, tex.voff_val,
	          tex.urot_val, tex.vrot_val, tex.wrot_val);

	PRINT_LOG("UV Scale Controller IDs (0x%08X,0x%08X), UV Offset Controller IDs (0x%08X,0x%08X), UVW Rotate Controller IDs (0x%08X,0x%08X,0x%08X)\n",
	          tex.uscl_ctrlID, tex.vscl_ctrlID,
	          tex.uoff_ctrlID, tex.voff_ctrlID,
	          tex.urot_ctrlID, tex.vrot_ctrlID, tex.wrot_ctrlID);
}

void CLoaderCGF::print(const char* szName, const TextureMap2& tex)
{
	if (!tex.name[0])
		return;
	g_setTextures.insert(tex.name);

	PRINT_LOG("%s \"%s\", Amount %d, Type %s (0x%02X) Flags 0x%02X",
	          szName,
	          tex.name,
	          (int)tex.Amount,
	          getTexType(tex.type),
	          (int)tex.type,
	          (int)tex.flags);

	if (tex.flags & TEXMAP_NOMIPMAP)
		PRINT_LOG(" (TEXMAP_NOMIPMAP)");

	PRINT_LOG("\n");

	if (tex.utile || tex.vtile)
	{
		PRINT_LOG("Tiling:");
		if (tex.utile)
			PRINT_LOG(" U");
		if (tex.vtile)
			PRINT_LOG(" V");
		PRINT_LOG("\n");
	}

	if (tex.umirror || tex.vmirror)
	{
		PRINT_LOG("Mirror:");
		if (tex.umirror)
			PRINT_LOG(" U");
		if (tex.vmirror)
			PRINT_LOG(" V");
		PRINT_LOG("\n");
	}

	if (tex.nthFrame)
		PRINT_LOG("Update reflection every %d frame\n", tex.nthFrame);

	if (tex.refBlur || tex.refSize)
		PRINT_LOG("Reflection size %g, blur %g\n", tex.refSize, tex.refBlur);

	PRINT_LOG("UV Scale (%g,%g), UV Offset (%g,%g), UVW Rotate (%g,%g,%g)\n",
	          tex.uscl_val, tex.vscl_val,
	          tex.uoff_val, tex.voff_val,
	          tex.urot_val, tex.vrot_val, tex.wrot_val);

	PRINT_LOG("UV Scale Controller IDs (0x%08X,0x%08X), UV Offset Controller IDs (0x%08X,0x%08X), UVW Rotate Controller IDs (0x%08X,0x%08X,0x%08X)\n",
	          tex.uscl_ctrlID, tex.vscl_ctrlID,
	          tex.uoff_ctrlID, tex.voff_ctrlID,
	          tex.urot_ctrlID, tex.vrot_ctrlID, tex.wrot_ctrlID);
}

void CLoaderCGF::print(const char* szName, const TextureMap& tex)
{
	if (!tex.name[0])
		return;
	g_setTextures.insert(tex.name);

	PRINT_LOG("%s \"%s\"\n",
	          szName,
	          tex.name);

	if (tex.utile || tex.vtile)
	{
		PRINT_LOG("Tiling:");
		if (tex.utile)
			PRINT_LOG(" U");
		if (tex.vtile)
			PRINT_LOG(" V");
		PRINT_LOG("\n");
	}

	if (tex.umirror || tex.vmirror)
	{
		PRINT_LOG("Mirror:");
		if (tex.umirror)
			PRINT_LOG(" U");
		if (tex.vmirror)
			PRINT_LOG(" V");
		PRINT_LOG("\n");
	}

	PRINT_LOG("UV Scale (%g,%g), UV Offset (%g,%g), UVW Rotate (%g,%g,%g)\n",
	          tex.uscl_val, tex.vscl_val,
	          tex.uoff_val, tex.voff_val,
	          tex.urot_val, tex.vrot_val, tex.wrot_val);

	PRINT_LOG("UV Scale Controller IDs (0x%08X,0x%08X), UV Offset Controller IDs (0x%08X,0x%08X), UVW Rotate Controller IDs (0x%08X,0x%08X,0x%08X)\n",
	          tex.uscl_ctrlID, tex.vscl_ctrlID,
	          tex.uoff_ctrlID, tex.voff_ctrlID,
	          tex.urot_ctrlID, tex.vrot_ctrlID, tex.wrot_ctrlID);
}

void CLoaderCGF::initVertIndices(std::vector<int>& arrVertIndices, int nVerts)
{
	arrVertIndices.resize(nVerts);
	for (int i = 0; i < nVerts; ++i)
	{
		arrVertIndices[i] = i;
	}
}

float CLoaderCGF::unpackComponent(const CryVertex& rVector, int nComponent)
{
	return rVector.p[nComponent];
}

float CLoaderCGF::unpackComponent(const SMeshMorphTargetVertex& rVector, int nComponent)
{
	return rVector.ptVertex[nComponent];
}

void CLoaderCGF::sortVertIndices(const CryVertex* pVerts, int nVerts, std::vector<int>& arrVertIndices)
{
	initVertIndices(arrVertIndices, nVerts);
	if (g_szVertexSortOrder[0])
		std::sort(arrVertIndices.begin(), arrVertIndices.end(), TVertexSorter<CryVertex>(pVerts, nVerts, g_szVertexSortOrder));
}

void CLoaderCGF::sortVertIndices(const SMeshMorphTargetVertex* pVerts, int nVerts, std::vector<int>& arrVertIndices)
{
	initVertIndices(arrVertIndices, nVerts);
	if (g_szVertexSortOrder[0])
		std::sort(arrVertIndices.begin(), arrVertIndices.end(), TVertexSorter<SMeshMorphTargetVertex>(pVerts, nVerts, g_szVertexSortOrder));
}

void CLoaderCGF::LoadChunkMtl(const MTL_CHUNK_DESC_0746* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	if (pChunk->chdr.ChunkVersion != pChunk->VERSION)
	{
		PRINT_LOG("Unexpected version %04X (expected %04X)", pChunk->chdr.ChunkVersion, MTL_CHUNK_DESC::VERSION);
		return;
	}

	PRINT_LOG("%s Material \"%s\"", getMtlType(pChunk->MtlType), pChunk->name, g_nStandardMaterialCounter);
	if (pChunk->MtlType == MTL_STANDARD)
		PRINT_LOG(" #%u", g_nStandardMaterialCounter++);
	PRINT_LOG("\n");
	switch (pChunk->MtlType)
	{
	case MTL_STANDARD:
		PRINT_LOG("Colors: diffuse ");
		print(pChunk->std.col_d);
		PRINT_LOG(", specular ");
		print(pChunk->std.col_s);
		PRINT_LOG(", ambient ");
		print(pChunk->std.col_a);
		PRINT_LOG("\nSpecLevel %.2f, SpecShininess %.2f, SelfIllumination %.2f, Opacity %.2f\n", pChunk->std.specLevel, pChunk->std.specShininess, pChunk->std.selfIllum, pChunk->std.opacity);
		print("Ambient", pChunk->std.tex_a);
		print("Diffuse", pChunk->std.tex_d);
		print("Specular", pChunk->std.tex_s);
		print("Opacity", pChunk->std.tex_o);
		print("Bump", pChunk->std.tex_b);
		print("Gloss", pChunk->std.tex_g);
		print("Filter", pChunk->std.tex_fl);
		print("Reflection", pChunk->std.tex_rl);
		print("Subsurface", pChunk->std.tex_subsurf);
		print("Detail", pChunk->std.tex_det);
		PRINT_LOG("Flags %08X %s ", pChunk->std.flags, getMtlFlags(pChunk->std.flags).c_str());
		PRINT_LOG("Dyn: Bounce %.3f, StaticFriction %.3f, SlidingFriction %.3f\n", pChunk->std.Dyn_Bounce, pChunk->std.Dyn_StaticFriction, pChunk->std.Dyn_SlidingFriction);
		break;

	case MTL_MULTI:
		PRINT_LOG("%d children\n", pChunk->multi.nChildren);
		break;
	}
}

void CLoaderCGF::LoadChunkMtl(const MTL_CHUNK_DESC_0745* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	if (pChunk->chdr.ChunkVersion != pChunk->VERSION)
	{
		PRINT_LOG("Unexpected version %04X (expected %04X)", pChunk->chdr.ChunkVersion, MTL_CHUNK_DESC::VERSION);
		return;
	}

	PRINT_LOG("%s Material \"%s\"", getMtlType(pChunk->MtlType), pChunk->name);
	if (pChunk->MtlType == MTL_STANDARD)
		PRINT_LOG(" #%u", g_nStandardMaterialCounter++);
	PRINT_LOG("\n");
	switch (pChunk->MtlType)
	{
	case MTL_STANDARD:
		PRINT_LOG("Colors: diffuse ");
		print(pChunk->std.col_d);
		PRINT_LOG(", specular ");
		print(pChunk->std.col_s);
		PRINT_LOG(", ambient ");
		print(pChunk->std.col_a);
		PRINT_LOG("\nSpecLevel %.2f, SpecShininess %.2f, SelfIllumination %.2f, Opacity %.2f\n", pChunk->std.specLevel, pChunk->std.specShininess, pChunk->std.selfIllum, pChunk->std.opacity);
		print("Ambient", pChunk->std.tex_a);
		print("Diffuse", pChunk->std.tex_d);
		print("Specular", pChunk->std.tex_s);
		print("Opacity", pChunk->std.tex_o);
		print("Bump", pChunk->std.tex_b);
		print("Gloss", pChunk->std.tex_g);
		print("Reflection", pChunk->std.tex_rl);
		print("Subsurface", pChunk->std.tex_subsurf);
		print("Detail", pChunk->std.tex_det);
		PRINT_LOG("Flags %08X %s ", pChunk->std.flags, getMtlFlags(pChunk->std.flags).c_str());
		PRINT_LOG("Dyn: Bounce %.3f, StaticFriction %.3f, SlidingFriction %.3f\n", pChunk->std.Dyn_Bounce, pChunk->std.Dyn_StaticFriction, pChunk->std.Dyn_SlidingFriction);
		break;

	case MTL_MULTI:
		PRINT_LOG("%d children\n", pChunk->multi.nChildren);
		break;
	}
}

void CLoaderCGF::LoadChunkMtl(const MTL_CHUNK_DESC_0744* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	if (pChunk->chdr.ChunkVersion != pChunk->VERSION)
	{
		PRINT_LOG("Unexpected version %04X (expected %04X)", pChunk->chdr.ChunkVersion, MTL_CHUNK_DESC::VERSION);
		return;
	}

	PRINT_LOG("%s Material \"%s\"", getMtlType(pChunk->MtlType), pChunk->name);
	if (pChunk->MtlType == MTL_STANDARD)
		PRINT_LOG(" #%u", g_nStandardMaterialCounter++);
	PRINT_LOG("\n");
	switch (pChunk->MtlType)
	{
	case MTL_STANDARD:
		PRINT_LOG("Colors: diffuse ");
		print(pChunk->std.col_d);
		PRINT_LOG(", specular ");
		print(pChunk->std.col_s);
		PRINT_LOG(", ambient ");
		print(pChunk->std.col_a);
		print("Diffuse", pChunk->std.tex_d);
		print("Opacity", pChunk->std.tex_o);
		print("Bump", pChunk->std.tex_b);
		PRINT_LOG("Dyn: Bounce %.3f, StaticFriction %.3f, SlidingFriction %.3f\n", pChunk->std.Dyn_Bounce, pChunk->std.Dyn_StaticFriction, pChunk->std.Dyn_SlidingFriction);
		break;

	case MTL_MULTI:
		PRINT_LOG("%d children\n", pChunk->multi.nChildren);
		break;
	}
}

const char* CLoaderCGF::getCtrlName(unsigned nCtrl)
{
	switch (nCtrl)
	{
	case CTRL_NONE:
		return "None";
	case CTRL_LINEER1:
		return "Linear 1D";
	case CTRL_LINEER3:
		return "Linear 3D";
	case CTRL_LINEERQ:
		return "Linear Quat";
	case CTRL_BEZIER1:
		return "Bezier 1D";
	case CTRL_BEZIER3:
		return "Bezier 3D";
	case CTRL_BEZIERQ:
		return "Bezier Quat";
	case CTRL_TCB1:
		return "TCB 3D";
	case CTRL_TCBQ:
		return "TCB Quat";
	case CTRL_CONST:
		return "Const";
	case CTRL_CRYBONE:
		return "CryBone";
	case CTRL_BSPLINE_2O:
		return "BSpline2O";
	case CTRL_BSPLINE_2C:
		return "BSpline2C";
	case CTRL_BSPLINE_1O:
		return "BSpline1O";
	case CTRL_BSPLINE_1C:
		return "BSpline1C";
	default:
	{
		static char szBuffer[32];
		sprintf(szBuffer, "Uknown(%d)", nCtrl);
		return szBuffer;
	}
	}
}

const char* CLoaderCGF::getBoneName(const std::vector<const char*>& arrNames, int nBone, BoneIdentificationEnum nIdentity)
{
	switch (nIdentity)
	{
	case kBoneID:
		if (!(nBone >= 0 && nBone < (int)arrNames.size()))
			return "#OUT OF RANGE#";

		return arrNames[nBone];
		break;

	case kBoneCtrlID:
		for (int i = 0; i < (int)arrNames.size(); ++i)
		{
			unsigned nCtrlID = Crc32Gen::GetCRC32(arrNames[i]);
			if (nCtrlID == (unsigned)nBone)
				return arrNames[i];
		}
		break;
	}
	return "#UNKNOWN#";
}

void CLoaderCGF::getBoneNames(const BONENAMELIST_CHUNK_DESC_0744* pChunk, unsigned nSize, std::vector<const char*>& arrNames)
{
	const NAME_ENTITY* pBones = (const NAME_ENTITY*)(pChunk + 1);
	arrNames.resize(pChunk->nEntities, "#TRUNCATED#");
	for (int n = 0; n < pChunk->nEntities; ++n)
	{
		if ((const char*)(pBones + n + 1) <= (const char*)pChunk + nSize)
		{
			arrNames[n] = pBones[n].name;
		}
		else
			break;
	}
}

const char* CLoaderCGF::getBoneName0744(const BONENAMELIST_CHUNK_DESC_0744* pChunk, unsigned nSize, int nBone, BoneIdentificationEnum nIdentity)
{
	if (!pChunk)
		return "#NO CHUNK#";
	std::vector<const char*> arrNames;
	getBoneNames(pChunk, nSize, arrNames);

	return getBoneName(arrNames, nBone, nIdentity);
}

void CLoaderCGF::getBoneNames(const BONENAMELIST_CHUNK_DESC_0745* pNameChunk, unsigned nChunkSize, std::vector<const char*>& arrGeomBoneNameTable)
{
	int nGeomBones = pNameChunk->numEntities;
	arrGeomBoneNameTable.resize(nGeomBones, "#TRUNCATED#");

	const char* pNameListEnd = ((const char*)pNameChunk) + nChunkSize;
	const char* pName        = (const char*)(pNameChunk + 1);
	int         nName        = 0;
	while (*pName && pName < pNameListEnd && nName < nGeomBones)
	{
		arrGeomBoneNameTable[nName] = pName;
		pName += strlen(pName) + 1;
		++nName;
	}
}

const char* CLoaderCGF::getBoneName0745(const BONENAMELIST_CHUNK_DESC_0745* pNameChunk, unsigned nChunkSize, int nBone, BoneIdentificationEnum nIdentity)
{
	std::vector<const char*> arrGeomBoneNameTable;
	getBoneNames(pNameChunk, nChunkSize, arrGeomBoneNameTable);

	return getBoneName(arrGeomBoneNameTable, nBone, nIdentity);
}

const char* CLoaderCGF::getBoneName(int nBone, BoneIdentificationEnum nIdentity)
{
	const BONENAMELIST_CHUNK_DESC_0744* pChunk744 = NULL;
	const BONENAMELIST_CHUNK_DESC_0745* pChunk    = NULL;
	unsigned                            i, nSize = 0;

	for (i = 0; !pChunk && i < (unsigned)m_pChunkFile->numChunks(); ++i)
	{
		const CHUNK_HEADER& chunkHeader = m_pChunkFile->getChunkHeader(i);
		if (chunkHeader.ChunkType == ChunkType_BoneNameList)
		{
			switch (chunkHeader.ChunkVersion)
			{
			case BONENAMELIST_CHUNK_DESC_0744::VERSION:
				pChunk744 = (const BONENAMELIST_CHUNK_DESC_0744*)m_pChunkFile->getChunkData(i);
				break;
			case BONENAMELIST_CHUNK_DESC_0745::VERSION:
				pChunk = (const BONENAMELIST_CHUNK_DESC_0745*)m_pChunkFile->getChunkData(i);
				break;
			}
			nSize = m_pChunkFile->getChunkSize(i);
		}
	}

	if (pChunk744)
		return getBoneName0744(pChunk744, nSize, nBone, nIdentity);
	if (pChunk)
		return getBoneName0745(pChunk, nSize, nBone, nIdentity);
	return "#NO CHUNK#";
}

// searches the given object's node (the node with the object id set to this id)
// and returns its name

const char* CLoaderCGF::getObjectName(int nChunkId)
{
	for (unsigned i = 0; i < (unsigned)m_pChunkFile->numChunks(); ++i)
	{
		const CHUNK_HEADER& chunkHeader = m_pChunkFile->getChunkHeader(i);
		if (chunkHeader.ChunkType == ChunkType_Node)
		{
			switch (chunkHeader.ChunkVersion)
			{
			case NODE_CHUNK_DESC::VERSION:
			{
				const NODE_CHUNK_DESC* pChunk = (const NODE_CHUNK_DESC*)m_pChunkFile->getChunkData(i);
				if (pChunk->ObjectID == nChunkId)
					return pChunk->name;
			}
			break;
			}
		}
	}
	return "";
}

// searches for the given material node and returns its name

const char* CLoaderCGF::getMaterialName(int nMtlId)
{
	int nStdMtlCounter = 0;
	for (unsigned i = 0; i < (unsigned)m_pChunkFile->numChunks(); ++i)
	{
		const CHUNK_HEADER& chunkHeader = m_pChunkFile->getChunkHeader(i);
		const void*         pData       = m_pChunkFile->getChunkData(i);
		if (chunkHeader.ChunkType == ChunkType_Mtl)
		{
			switch (chunkHeader.ChunkVersion)
			{
			case MTL_CHUNK_DESC_0744::VERSION:
			{
				const MTL_CHUNK_DESC_0744* pChunk = (const MTL_CHUNK_DESC_0744*)pData;
				if (pChunk->MtlType == MTL_STANDARD)
				{
					if ((nStdMtlCounter++) == nMtlId)
						return pChunk->name;
				}
			}
			break;
			case MTL_CHUNK_DESC_0745::VERSION:
			{
				const MTL_CHUNK_DESC_0745* pChunk = (const MTL_CHUNK_DESC_0745*)pData;
				if (pChunk->MtlType == MTL_STANDARD)
				{
					if ((nStdMtlCounter++) == nMtlId)
						return pChunk->name;
				}
			}
			break;
			case MTL_CHUNK_DESC_0746::VERSION:
			{
				const MTL_CHUNK_DESC_0746* pChunk = (const MTL_CHUNK_DESC_0746*)pData;
				if (pChunk->MtlType == MTL_STANDARD)
				{
					if ((nStdMtlCounter++) == nMtlId)
						return pChunk->name;
				}
			}
			break;
			}
		}
	}
	return "";
}

void CLoaderCGF::printMtlSet(const char* szFormat, const std::set<unsigned>& setMtls, const char* szPostfix)
{
	if (!setMtls.empty())
	{
		PRINT_LOG(" (");
		for (std::set<unsigned>::const_iterator it = setMtls.begin(); it != setMtls.end();)
		{
			if (it != setMtls.begin())
				PRINT_LOG(", ");
			PRINT_LOG(szFormat, *it, getMaterialName(*it));
			unsigned nStart = *it;

			++it;

			if (it != setMtls.end() && *it == nStart + 1)
			{
				unsigned nPrev = *it;
				// we've got a region
				while (++it != setMtls.end() && *it == nPrev + 1)
					nPrev = *it;
				if (nPrev == nStart + 1)
					// special case - range of length 1
					PRINT_LOG(",");
				else
					PRINT_LOG("..");
				PRINT_LOG(szFormat, nPrev, getMaterialName(nPrev));
			}
		}
		PRINT_LOG(")");
	}
	PRINT_LOG("%s", szPostfix);
}

std::string CLoaderCGF::getHelperType(HelperTypes nType)
{
	switch (nType)
	{
	case HP_POINT:
		return "Point";
	case HP_DUMMY:
		return "Dummy";
	case HP_XREF:
		return "XRef";
	case HP_CAMERA:
		return "Camera";
	default:
	{
		char szBuffer[0x50];
		sprintf(szBuffer, "Unknown(%d)", nType);
		return szBuffer;
	}
	}
}

void CLoaderCGF::LoadChunkController(const CONTROLLER_CHUNK_DESC_0826* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	if (pChunk->VERSION != pChunk->chdr.ChunkVersion)
	{
		PRINT_LOG("Unexpected version 0x%04X (expected 0x%04X)\n", pChunk->chdr.ChunkVersion, pChunk->VERSION);
		return;
	}

	PRINT_LOG("nControllerId: 0x%08X \"%s\"\n", pChunk->nControllerId, getBoneName(pChunk->nControllerId, kBoneCtrlID));
	PRINT_LOG("%d %s keys", pChunk->nKeys, getCtrlName(pChunk->type));

	if (pChunk->type == CTRL_CRYBONE)
	{
		int               i, numKeys = pChunk->nKeys;
		const CryBoneKey* pKeys = (const CryBoneKey*)(pChunk + 1);
		//FIXME:
#if 0
		if (m_bDumpKeys)
#else
		if (true)
#endif
		{
			PRINT_LOG(":");
			for (i = 0; i < numKeys; ++i)
			{
				PRINT_LOG("\n");
				const CryBoneKey& k = pKeys[i];
				const CryQuat&    q = k.relquat;
				const Vec3d&      p = k.relpos;
				AngleAxis         qAngleAxis(q);
				Ang3              ptEuler;
				ptEuler.SetAnglesXYZ(Matrix33(q));
				Vec3d ql = log(q).v;
				PRINT_LOG("% 3d. frame %4.1f pos (%.3f,%.3f,%.3f)  quat (%.3f;%.3f,%.3f,%.3f) = 2*qlog (%.3fpi,%.3fpi,%.3fpi) = rotAA (%.1f deg axis %.3f,%.3f,%.3f) = euler (%.1f,%.1f,%.1f)",
				          i, k.time / 160.0f,
				          p.x, p.y, p.z,
				          q.w, q.v.x, q.v.y, q.v.z,
				          2 * ql.x / M_PI, 2 * ql.y / M_PI, 2 * ql.z / M_PI,
				          qAngleAxis.angle * 180 / M_PI, qAngleAxis.axis.x, qAngleAxis.axis.y, qAngleAxis.axis.z,
				          ptEuler.x * 180 / M_PI, ptEuler.y * 180 / M_PI, ptEuler.z * 180 / M_PI);
			}
		}
		else
		{
			PRINT_LOG(" at frames (frame=tick/160): ");
			for (i = 0; i < numKeys; ++i)
			{
				if (i) PRINT_LOG(", ");
				PRINT_LOG("%.1f", pKeys[i].time / 160.0f);
			}
		}
	}
	PRINT_LOG("\n");
}

void CLoaderCGF::LoadChunkController(const CONTROLLER_CHUNK_DESC_0827* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	PRINT_LOG("nControllerId: 0x%08X \"%s\"\n", pChunk->nControllerId, getBoneName(pChunk->nControllerId, kBoneCtrlID));
	PRINT_LOG("%d v827 plain Pos/RotLog keys", pChunk->numKeys);

	int                i, numKeys = pChunk->numKeys;
	const CryKeyPQLog* pKeys = (const CryKeyPQLog*)(pChunk + 1);

#if 0
		if (m_bDumpKeys)
#else
	if (true)
#endif
	{
		PRINT_LOG(":");
		for (i = 0; i < numKeys; ++i)
		{
			PRINT_LOG("\n");
			const CryKeyPQLog& k  = pKeys[i];
			const Vec3d&       ql = k.vRotLog;
			CryQuat            q  = exp(CryQuat(0, ql));
			const Vec3d&       p  = k.vPos;
			AngleAxis          qAngleAxis(q);
			Ang3               ptEuler;
			ptEuler.SetAnglesXYZ(Matrix33(q));
			PRINT_LOG("% 3d. frame %4.1f pos (%.3f,%.3f,%.3f)  quat (%.3f;%.3f,%.3f,%.3f) = 2*qlog (%.3fpi,%.3fpi,%.3fpi) = rotAA (%.1f deg axis %.3f,%.3f,%.3f) = euler (%.1f,%.1f,%.1f)",
			          i, k.nTime / 160.0f,
			          p.x, p.y, p.z,
			          q.w, q.v.x, q.v.y, q.v.z,
			          2 * ql.x / M_PI, 2 * ql.y / M_PI, 2 * ql.z / M_PI,
			          qAngleAxis.angle * 180 / M_PI, qAngleAxis.axis.x, qAngleAxis.axis.y, qAngleAxis.axis.z,
			          ptEuler.x * 180 / M_PI, ptEuler.y * 180 / M_PI, ptEuler.z * 180 / M_PI);
		}
	}
	else
	{
		PRINT_LOG(" at frames (frame=tick/160): ");
		for (i = 0; i < numKeys; ++i)
		{
			if (i) PRINT_LOG(", ");
			PRINT_LOG("%.1f", pKeys[i].nTime / 160.0f);
		}
	}
	PRINT_LOG("\n");
}

void CLoaderCGF::LoadChunkTiming(const TIMING_CHUNK_DESC* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	double fSecsPerFrame = double(pChunk->SecsPerTick) * pChunk->TicksPerFrame;

	PRINT_LOG("Global range: %d .. %d frames (%g sec .. %g sec, length %g seconds)\n",
	          pChunk->global_range.start, pChunk->global_range.end,
	          pChunk->global_range.start * fSecsPerFrame, pChunk->global_range.end * fSecsPerFrame,
	          (pChunk->global_range.end - pChunk->global_range.start) * fSecsPerFrame);
	PRINT_LOG("%.3f MilliSecs/Tick, %d Ticks/Frame\n", pChunk->SecsPerTick * 1000, pChunk->TicksPerFrame);
	const RANGE_ENTITY* pSubrange = (const RANGE_ENTITY*)(pChunk + 1);
	for (int i = 0; i < pChunk->nSubRanges; ++i)
	{
		char name[sizeof(pSubrange->name) + 1];
		name[sizeof(name) - 1] = '\0';
		memcpy(name, pSubrange[i].name, sizeof(pSubrange->name));

		PRINT_LOG("Subrange %d \"%s\": %d .. %d", i, name, pSubrange[i].start, pSubrange[i].end);
	}
}

void CLoaderCGF::LoadChunkMesh(const MESH_CHUNK_DESC* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	if (pChunk->VERSION != pChunk->chdr.ChunkVersion)
	{
		PRINT_LOG("Unexpected version 0x%04X (expected 0x%04X)\n", pChunk->chdr.ChunkVersion, pChunk->VERSION);
		return;
	}

	const char* pChunkEnd = (const char*)pChunk + nSize;

	PRINT_LOG("\"%s\" %d Verts, %d UVs, %d Faces", getObjectName(pChunk->chdr.ChunkID), pChunk->nVerts, pChunk->nTVerts, pChunk->nFaces);

//FIXME:
#if 0
	if (pChunk->HasBoneInfo)
		PRINT_LOG(", Has BONE INFO");
	else if (m_bDumpMeshBones)
		PRINT_LOG(", Has NO bone info");
#endif

	if (pChunk->HasVertexCol)
		PRINT_LOG(", Has VERTEX COLORS");

	PRINT_LOG("  (%d bytes useful)", pChunk->nVerts * 12 + pChunk->nTVerts * 8 + pChunk->nFaces * 8);

	PRINT_LOG("\n");

	const CryVertex* pVerts = (const CryVertex*)(pChunk + 1);

	if ((const char*)(pVerts + pChunk->nVerts) > pChunkEnd)
	{
		PRINT_LOG("Error: chunk trunkated on %d-th vertex\n", int(pChunkEnd - (const char*)pVerts) / sizeof(*pVerts));
		//return;
	}

	const CryFace* pFaces = (const CryFace*)(pVerts + pChunk->nVerts);

	if ((const char*)(pFaces + pChunk->nFaces) > pChunkEnd)
	{
		PRINT_LOG("Error: chunk trunkated on %d-th face, vertices are ok\n", int(pChunkEnd - (const char*)pFaces) / sizeof(*pFaces));
		//return;
	}

	const CryUV* pUVs = (const CryUV*)(pFaces + pChunk->nFaces);

	if ((const char*)(pUVs + pChunk->nTVerts) > pChunkEnd)
	{
		PRINT_LOG("Error: chunk trunkated on %d-th UV, vertices and faces are ok\n", int(pChunkEnd - (const char*)pUVs) / sizeof(*pUVs));
		//return;
	}

	const CryTexFace* pTexFaces = (const CryTexFace*)(pUVs + pChunk->nTVerts);

	if ((const char*)(pTexFaces + (pChunk->nTVerts ? pChunk->nFaces : 0)) > pChunkEnd)
	{
		PRINT_LOG("Error: chunk trunkated on %d-th texture face, vertices, faces and UVs are ok\n", int(pChunkEnd - (const char*)pTexFaces) / sizeof(*pTexFaces));
		//return;
	}

	const void* pBoneInfo = pTexFaces + (pChunk->nTVerts ? pChunk->nFaces : 0);

	Vec3d       ptMin, ptMax;
	ptMin = ptMax = pVerts[0].p;
	int i, j;
//FIXME:
#if 0
	if (m_bDumpMeshVerts)
		PRINT_LOG("Vertices: %d\n", pChunk->nVerts);
#endif

	std::vector<int> arrVertIndices;
	if ((const char*)(pVerts + pChunk->nVerts) > pChunkEnd)
		initVertIndices(arrVertIndices, pChunk->nVerts);
	else
		sortVertIndices(pVerts, pChunk->nVerts, arrVertIndices);

	VertexWelderSet    setVertices, setNormals;
	VertexWelderMapSet mapWeldedVertices;

	unsigned           numDuplicateVertices = 0, numDuplicateNormals = 0;
	for (j = 0; j < pChunk->nVerts; ++j)
	{
		i = arrVertIndices[j];
		if ((const char*)(pVerts + i + 1) > pChunkEnd)
		{
			PRINT_LOG("Error: trunkated chunk (vertex %d).\n", i);
			return;
		}
		const CryVertex& v = pVerts[i];
		for (int nCoord = 0; nCoord < 3; ++nCoord)
		{
			if (ptMin[nCoord] > pVerts[i].p[nCoord])
				ptMin[nCoord] = v.p[nCoord];
			else if (ptMax[nCoord] < pVerts[i].p[nCoord])
				ptMax[nCoord] = v.p[nCoord];
		}

		if (setVertices.find(v.p) != setVertices.end())
			++numDuplicateVertices;
		else
			setVertices.insert(v.p);

		mapWeldedVertices[v.p].insert(i);

		if (setNormals.find(v.n) != setNormals.end())
			++numDuplicateNormals;
		else
			setNormals.insert(v.n);

// FIXME:
#if 0
		if (m_bDumpMeshVerts)
			PRINT_LOG("\t% 4d\tp=(%8.3f,%8.3f,%8.3f) n=(%7.3f,%7.3f,%7.3f)\n", i, v.p.x, v.p.y, v.p.z, v.n.x, v.n.y, v.n.z);
#endif
	}

	if (numDuplicateVertices)
	{
		PRINT_LOG("%u Duplicate vertices; ", numDuplicateVertices);
// FIXME:
#if 0
		if (m_bDumpMeshVerts)
#endif
		for (VertexWelderMapSet::iterator it = mapWeldedVertices.begin(); it != mapWeldedVertices.end(); ++it)
			if (it->second.size() > 1)
				PRINT_LOG("\t%s {%g,%g,%g}\n", CryStringUtils::toString(it->second, "%d").c_str(), it->first.x, it->first.y, it->first.z);
	}

	if (numDuplicateNormals)
		PRINT_LOG("%u Duplicate normals; ", numDuplicateNormals);

	PRINT_LOG("BBox {%g .. %g, %g .. %g, %g .. %g}\n", ptMin.x, ptMax.x, ptMin.y, ptMax.y, ptMin.z, ptMax.z);

	int                nSmGroupsUsed = 0;
	std::set<unsigned> setSmGroups, setMtls;
	std::set<int>      setUnusedVerts;
	for (i = 0; i < pChunk->nVerts; ++i)
		setUnusedVerts.insert(i);

	//if (m_bDumpMeshFaces)
	PRINT_LOG("Faces: %d\n", pChunk->nFaces);
	for (i = 0; i < pChunk->nFaces; ++i)
	{
		if ((const char*)(pFaces + i + 1) > pChunkEnd)
		{
			PRINT_LOG("Error: trunkated chunk (face %d).\n", i);
			return;
		}
		const CryFace& f = pFaces[i];
		nSmGroupsUsed |= f.SmGroup;
		setSmGroups.insert(f.SmGroup);
		setMtls.insert(f.MatID);
		setUnusedVerts.erase(f.v0);
		setUnusedVerts.erase(f.v1);
		setUnusedVerts.erase(f.v2);

		// FIXME:
		//if (m_bDumpMeshFaces)
		PRINT_LOG("\t% 4d\tv=(% 4d,% 4d,% 4d) Smooth=0x%X MatID=%d\n", i, f.v0, f.v1, f.v2, f.SmGroup, f.MatID);
	}

	if (!setUnusedVerts.empty())
	{
		unsigned numUnused = (unsigned)setUnusedVerts.size();
		PRINT_LOG("%d unused vertices: ", numUnused);
		printSet("%d", setUnusedVerts, "\n");

		g_numUnusedBytes += numUnused * sizeof(CryVertex);
	}

	PRINT_LOG("SmoothGroups: ORed: 0x%08X, Used: %d",
	          nSmGroupsUsed, setSmGroups.size());
	printSet("0x%X", setSmGroups);

	if (setMtls.size() == 1)
	{
		PRINT_LOG(" Single material %u \"%s\" is used\n", *setMtls.begin(), getMaterialName(*setMtls.begin()));
	}
	else
	{
		PRINT_LOG(" Materials used: %d", setMtls.size());
		printMtlSet("%u \"%s\"", setMtls, "\n");
	}

	if (pChunk->nTVerts)
	{
		// FIXME:
		//if (m_bDumpMeshUVs)
		PRINT_LOG("%d Texture Vertices\n", pChunk->nTVerts);

		CryUV uvMin, uvMax;
		uvMin = uvMax = pUVs[0];

		for (i = 0; i < pChunk->nTVerts; ++i)
		{
			if ((const char*)(pUVs + i + 1) > pChunkEnd)
			{
				PRINT_LOG("Error: trunkated chunk (TVert %d).\n", i);
				return;
			}
			const CryUV& uv = pUVs[i];

			if (uvMin.u > uv.u)
				uvMin.u = uv.u;
			else if (uvMax.u < uv.u)
				uvMax.u = uv.u;

			if (uvMin.v > uv.v)
				uvMin.v = uv.v;
			else if (uvMax.v < uv.v)
				uvMax.v = uv.v;

			//if (m_bDumpMeshUVs)
			PRINT_LOG("\t% 4d\t(%.4f, %.4f)\n", i, uv.u, uv.v);
		}
		PRINT_LOG("UV BBox {%g..%g, %g..%g}\n", uvMin.u, uvMax.u, uvMin.v, uvMax.v);

		// FIXME:
		//if (m_bDumpMeshTexFaces)
		PRINT_LOG("%d Texture Faces\n", pChunk->nFaces);

		std::set<int> setUnunsedUVs;
		for (i = 0; i < pChunk->nTVerts; ++i)
			setUnunsedUVs.insert(i);

		for (i = 0; i < pChunk->nFaces; ++i)
		{
			if ((const char*)(pTexFaces + i + 1) > pChunkEnd)
			{
				PRINT_LOG("Error: trunkated chunk (tex face %d).\n", i);
				return;
			}
			const CryTexFace& tf = pTexFaces[i];
			setUnunsedUVs.erase(tf.t0);
			setUnunsedUVs.erase(tf.t1);
			setUnunsedUVs.erase(tf.t2);

			// FIXME:
			//if (m_bDumpMeshTexFaces)
			PRINT_LOG("\t% 4d\t% 4d,% 4d,% 4d\n", i, tf.t0, tf.t1, tf.t2);
		}

		if (!setUnunsedUVs.empty())
		{
			unsigned numUnused = (unsigned)setUnunsedUVs.size();
			PRINT_LOG("%d Ununsed Texture Vertices: ", numUnused);
			printSet("%d", setUnunsedUVs);
			PRINT_LOG("\n");
			g_numUnusedBytes += numUnused * sizeof(CryUV);
		}
	}

	if (pChunk->HasBoneInfo)
	{
		// hystogram: #Weights:#Vertices
		std::vector<unsigned> arrWVH;

		//if (m_bDumpMeshBones)
		PRINT_LOG("%d Bone Links:\n", pChunk->nVerts);
		for (i = 0; i < pChunk->nVerts; ++i)
		{
			// FIXME:
			//if (m_bDumpMeshBones)
			PRINT_LOG("\t% 4d  ", i);

			const unsigned* pnumLinks = (const unsigned*)pBoneInfo;

			if ((const char*)(pnumLinks + 1) > pChunkEnd)
			{
				PRINT_LOG("Error: trunkated chunk (bone info (numLinks) %d).\n", i);
				return;
			}
			unsigned numLinks = *pnumLinks;

			CryLink* pLinks   = (CryLink*)((unsigned*)pBoneInfo + 1);

			if ((const char*)(pLinks + numLinks) > pChunkEnd)
			{
				PRINT_LOG("Error: trunkated chunk (bone info (arrLinks) %d).\n", i);
				return;
			}

			float fSumWeight = 0;
			for (unsigned nLink = 0; nLink < numLinks; ++nLink)
			{
				//if (m_bDumpMeshBones && nLink)
				PRINT_LOG(" ");
				const CryLink& l = pLinks[nLink];
				fSumWeight += l.Blending;
				//if (m_bDumpMeshBones)
				PRINT_LOG("w[%2d] = %.3f * TM (%-6.3f,%-6.3f,%-6.3f)", l.BoneID, l.Blending, l.offset.x, l.offset.y, l.offset.z);
			}
			// FIXME:
			//if (m_bDumpMeshBones)
			PRINT_LOG("\n");

			if (fSumWeight < 0.98f || fSumWeight > 1.02f)
				PRINT_LOG("Warning: weights don't sum to 1: vertex %d, sum %.3f\n", i, fSumWeight);

			assert(numLinks < 100);

			if (arrWVH.size() < numLinks + 1)
				arrWVH.resize(numLinks + 1, 0);
			arrWVH[numLinks]++;

			pBoneInfo = pLinks + numLinks;
		}

		PRINT_LOG("#Weights:#Verts hystogram: ");
		bool     bFirst     = true;
		unsigned nSkinVerts = 0; // vertex structures that are likely to be allocated for skinning by the new skinning algorithm
		for (unsigned i = 0; i < arrWVH.size(); ++i)
			if (arrWVH[i] > 0)
			{
				if (bFirst)
					bFirst = false;
				else
					PRINT_LOG("   ");
				PRINT_LOG("%u:%u", i, arrWVH[i]);

				nSkinVerts += i * arrWVH[i];
			}

		PRINT_LOG(";  skinner redundancy: ~%d%%\n", int(((float(nSkinVerts) / float(pChunk->nVerts)) - 1) * 100));
	}

	if (pChunk->HasVertexCol)
	{
		// FIXME:
		//if (m_bDumpMeshVertCols)
		PRINT_LOG("%d Vertex Colors:\n", pChunk->nVerts);
		CryIRGB* pVertColor = (CryIRGB*)pBoneInfo;

		for (i = 0; i < pChunk->nVerts; ++i)
		{
			if ((const char*)(pVertColor + i + 1) > pChunkEnd)
			{
				PRINT_LOG("Error: trunkated chunk (vertex color %d).\n", i);
				return;
			}

			const CryIRGB& c = pVertColor[i];
			// FIXME:
			//if (m_bDumpMeshVertCols)
			PRINT_LOG("\t% 4d  (% 3u,% 3u,% 3u)\n", i, (unsigned)c.r, (unsigned)c.g, (unsigned)c.b);
		}
		pBoneInfo = pVertColor + pChunk->nVerts;
	}

	unsigned nUsedSize = (unsigned)pBoneInfo - (unsigned)pChunk;
	if (nUsedSize != nSize)
		PRINT_LOG("Unexpected length of data in chunk: %d used instead of %d available (%d bytes extra)", nUsedSize, nSize, nSize - nUsedSize);

	PRINT_LOG("\n");
}

void CLoaderCGF::LoadChunkNode(const NODE_CHUNK_DESC* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	if (pChunk->VERSION != pChunk->chdr.ChunkVersion)
	{
		PRINT_LOG("Unexpected version 0x%04X (expected 0x%04X)\n", pChunk->chdr.ChunkVersion, pChunk->VERSION);
		return;
	}

	if (nSize != sizeof(*pChunk) + pChunk->PropStrLen + sizeof(int) * pChunk->nChildren)
	{
		PRINT_LOG("Unexpected chunk length %d instead of %d\n", nSize, sizeof(*pChunk) + pChunk->PropStrLen + sizeof(int) * pChunk->nChildren);
		return;
	}

	PRINT_LOG("\"%s\"\n", pChunk->name);
	PRINT_LOG("ObjectID: 0x%08X\tParentID: 0x%08X\n", pChunk->ObjectID, pChunk->ParentID);

	auto pNodeCGF = new CNodeCGF(pChunk->name);
	m_pCGF->AddNode(pNodeCGF);

	pNodeCGF->mParent = nullptr;

	memcpy(&pNodeCGF->mTransformation, &pChunk->tm, sizeof Matrix44);

	if (pChunk->MatID > 0)
	{
		auto mat = LoadMaterialFromChunk(pChunk->MatID);
	}

	auto header = m_pChunkFile->getChunkHeader(pChunk->ObjectID);
	if (header.ChunkType == ChunkType_Mesh)
	{
		auto meshChunk       = (MESH_CHUNK_DESC*)m_pChunkFile->getChunkData(pChunk->ObjectID);

		m_pScene->mMeshes    = new CMesh*[1];
		m_pScene->mNumMeshes = 1;
		{
			pNodeCGF->mNumMeshes     = 1;
			pNodeCGF->mMeshes        = new unsigned int[1];

			auto assimpMesh          = new CMesh;
			assimpMesh->mName        = getObjectName(pChunk->ObjectID);

			assimpMesh->mNumVertices = meshChunk->nVerts;
			assimpMesh->mVertices    = new aiVector3D[assimpMesh->mNumVertices];
			assimpMesh->mNormals     = new aiVector3D[assimpMesh->mNumVertices];

			auto verts               = (CryVertex*)(meshChunk + 1);

			for (size_t i = 0; i < assimpMesh->mNumVertices; i++)
			{
				memcpy(&assimpMesh->mVertices[i], &verts[i].p, sizeof aiVector3D);
				memcpy(&assimpMesh->mNormals[i], &verts[i].n, sizeof aiVector3D);
			}

			assimpMesh->mNumFaces = meshChunk->nFaces;
			assimpMesh->mFaces    = new aiFace[assimpMesh->mNumFaces];

			auto faces            = (CryFace*)(verts + meshChunk->nVerts);
			for (size_t i = 0; i < assimpMesh->mNumFaces; i++)
			{
				auto& face       = assimpMesh->mFaces[i];
				face.mNumIndices = 3;
				face.mIndices    = new unsigned int[3];

				memcpy(face.mIndices, &faces[i], 3 * sizeof(unsigned int));
			}

			if (meshChunk->nTVerts != 0)
			{
				assimpMesh->mNumUVComponents[0] = 2;
				assimpMesh->mTextureCoords[0]   = new aiVector3D[assimpMesh->mNumVertices];
				auto* uvs                       = (CryUV*)(faces + meshChunk->nFaces);
				auto* tfaces                    = (CryTexFace*)(uvs + meshChunk->nTVerts);

				auto* UVs                       = &assimpMesh->mTextureCoords[0];
				//if (meshChunk->nTVerts != meshChunk->nVerts)
				//{
				//	for (size_t i = 0; i < meshChunk->nFaces; i++)
				//	{
				//		aiVector3D* UVs  = assimpMesh->mTextureCoords[0];
				//		CryTexFace* tface  = &tfaces[i];
				//		CryUV*      face[] = {
                //            &uvs[tface->t0],
                //            &uvs[tface->t1],
                //            &uvs[tface->t2]};
				//		aiVector3D* dstUv[] = {
				//		    &UVs[tface->t0],
				//		    &UVs[tface->t1],
				//		    &UVs[tface->t2]};
				//		dstUv[0]->x = face[0]->u;
				//		dstUv[0]->y = face[0]->v;
				//		dstUv[0]->z = 0.f;

				//		dstUv[1]->x = face[1]->u;
				//		dstUv[1]->y = face[1]->v;
				//		dstUv[1]->z = 0.f;

				//		dstUv[2]->x = face[2]->u;
				//		dstUv[2]->y = face[2]->v;
				//		dstUv[2]->z = 0.f;
				//	}
				//}
				//else
				{
					for (size_t i = 0; i < meshChunk->nTVerts; i++)
					{
						auto* uv = &UVs[0][i];

						uv->x    = uvs[i].u;
						uv->y    = uvs[i].v;
						uv->z    = 0.f;
						break;
					}
				}
			}

			m_pScene->mMeshes[0] = assimpMesh;
		}
		//assimpMesh->mNumVertices
	}

	if (pChunk->nChildren)
	{
		PRINT_LOG("Children: %d", pChunk->nChildren);
		std::set<int> setChildren;
		const int*    pChildren = (const int*)((const char*)(pChunk + 1) + pChunk->PropStrLen);
		for (int i = 0; i < pChunk->nChildren; ++i)
			setChildren.insert(pChildren[i]);
		printSet("%d", setChildren);
	}
	else
		PRINT_LOG("No Children");
	PRINT_LOG("\tMatID: 0x%08X\n", pChunk->MatID);

	if (pChunk->PropStrLen)
	{
		std::string strProp((const char*)(pChunk + 1), pChunk->PropStrLen);
		PRINT_LOG("Property string: \"%s\"\n", strProp.c_str());
	}

	CryQuat qRot = pChunk->rot;
	if (fabs((qRot | qRot) - 1) > 0.01)
	{
		PRINT_LOG("*WARNING*: rotation quaternion is not normalized {w=%g,v=(%g,%g,%g)}", qRot.w, qRot.v.x, qRot.v.y, qRot.v.z);
		qRot.Normalize();
		PRINT_LOG(" assuming {w=%g,v=(%g,%g,%g)}\n", qRot.w, qRot.v.x, qRot.v.y, qRot.v.z);
	}

	AngleAxis aaRot(qRot);
	Ang3      ptEul;
	ptEul.SetAnglesXYZ(Matrix33(qRot));
	PRINT_LOG("pos (%g,%g,%g) scale (%g,%g,%g) rot(%g degrees around %g,%g,%g) == euler (%g,%g,%g) == quaternion (w=%g,x=%g,y=%g,z=%g)\n",
	          pChunk->pos.x, pChunk->pos.y, pChunk->pos.z,
	          pChunk->scl.x, pChunk->scl.y, pChunk->scl.z,
	          aaRot.angle * 180 / M_PI, aaRot.axis.x, aaRot.axis.y, aaRot.axis.z,
	          ptEul.x * 180 / M_PI, ptEul.y * 180 / M_PI, ptEul.z * 180 / M_PI,
	          qRot.w, qRot.v.x, qRot.v.y, qRot.v.z);

	for (int i = 0; i < 4; ++i)
	{
		if (!i)
			PRINT_LOG("TM: ");
		else
			PRINT_LOG("    ");

		PRINT_LOG("%-7.3f  %- 7.3f  %- 7.3f  %- 7.3f\n", pChunk->tm[i][0], pChunk->tm[i][1], pChunk->tm[i][2], pChunk->tm[i][3]);
	}

	PRINT_LOG("CtrlID: pos: 0x%08X,  rot: 0x%08X,  scl: 0x%08X",
	          pChunk->pos_cont_id, pChunk->rot_cont_id, pChunk->scl_cont_id);

	PRINT_LOG("\n");
}

void CLoaderCGF::LoadChunkBoneNameList(const BONENAMELIST_CHUNK_DESC_0744* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	if (pChunk->VERSION != pChunk->chdr.ChunkVersion)
	{
		PRINT_LOG("Unexpected version 0x%04X (expected 0x%04X)\n", pChunk->chdr.ChunkVersion, pChunk->VERSION);
		return;
	}

	PRINT_LOG("%d entities\n", pChunk->nEntities);
	int nExpectedSize = pChunk->nEntities * sizeof(NAME_ENTITY) + sizeof(*pChunk);
	if (nExpectedSize != nSize)
	{
		PRINT_LOG("Unexpected chunk size: expected %d bytes length\n", nExpectedSize);
	}
	const NAME_ENTITY* pNames = (const NAME_ENTITY*)(pChunk + 1);
	for (int i = 0; i < pChunk->nEntities; ++i)
	{
		char szName[sizeof(pNames->name) + 1] = "#TRUNCATED#";
		szName[sizeof(szName) - 1]            = '\0';
		if ((const char*)(pNames + i + 1) <= (const char*)pChunk + nSize)
			memcpy(szName, pNames[i].name, sizeof(pNames[i].name));
		PRINT_LOG("%2d. \"%s\"\n", i, szName);
	}
}

void CLoaderCGF::LoadChunkBoneNameList(const BONENAMELIST_CHUNK_DESC_0745* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	PRINT_LOG("%d entities\n", pChunk->numEntities);
	const char* pNameListEnd = ((const char*)pChunk) + nSize;
	int         nBoneId      = 0;
	for (const char* p = (const char*)(pChunk + 1); p < pNameListEnd && *p;)
	{
		PRINT_LOG("%2d. \"%s\"\n", nBoneId, p);
		++nBoneId;
		while (p < pNameListEnd && *p)
			++p;
		++p;
	}

	if (nBoneId != pChunk->numEntities)
		PRINT_LOG("Warning: Declared number of entites (%d) doesn't match the real number of substrings in the chunk (%d)\n", pChunk->numEntities, nBoneId);
}

void CLoaderCGF::LoadChunkBoneAnim(const BONEANIM_CHUNK_DESC* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	if (pChunk->VERSION != pChunk->chdr.ChunkVersion)
	{
		PRINT_LOG("Unexpected version 0x%04X (expected 0x%04X)\n", pChunk->chdr.ChunkVersion, pChunk->VERSION);
		//return;
	}

	PRINT_LOG("%d Bones\n", pChunk->nBones);
	const BONE_ENTITY* pBoneEntities  = (const BONE_ENTITY*)(pChunk + 1);
	const BONE_ENTITY* pBoneEntityEnd = (const BONE_ENTITY*)(((const char*)pChunk) + nSize);
	if (pBoneEntities + pChunk->nBones != pBoneEntityEnd)
	{
		PRINT_LOG("*WARNING* Unexpected chunk size %d, expected %d\n", nSize, sizeof(*pChunk) + pChunk->nBones * sizeof(BONE_ENTITY));
	}

	std::vector<int> stackParents;
	for (const BONE_ENTITY* pBoneEntity = pBoneEntities; pBoneEntity < pBoneEntityEnd; ++pBoneEntity)
	{
		unsigned i;
		char     szTabs[0x20];
		for (i = 0; i < stackParents.size() && i < sizeof(szTabs) - 1; ++i)
			szTabs[i] = '\t';
		szTabs[i] = '\0';

		PRINT_LOG("%s", szTabs);

		char szProps[sizeof(pBoneEntity->prop) + 1];
		szProps[sizeof(szProps) - 1] = '\0';
		memcpy(szProps, pBoneEntity->prop, sizeof(pBoneEntity->prop));
		PRINT_LOG("Bone %d \"%s\": Parent %d, %d children, CtrlID 0x%08X",
		          pBoneEntity->BoneID,
		          getBoneName(pBoneEntity->BoneID),
		          pBoneEntity->ParentID,
		          pBoneEntity->nChildren,
		          pBoneEntity->ControllerID);

		if (szProps[0])
			PRINT_LOG(", props: %s", szProps);

		BONE_PHYSICS phys;
		CopyPhysInfo(phys, pBoneEntity->phys);
		if (phys.flags != -1)
		{
			PRINT_LOG(", DOF:");
			if ((phys.flags & (JNT_XACTIVE * 7)) == 0)
				PRINT_LOG(" NONE");
			for (int i = 0; i < 3; ++i)
				if (phys.flags & (JNT_XACTIVE << i))
					PRINT_LOG(" %c", int('X' + i));
		}

		PRINT_LOG("\n");

		if (phys.flags != -1)
		{
			if (phys.flags & (JNT_XLIMITED * 7))
			{
				PRINT_LOG("%sLimits:", szTabs);
				for (int i = 0; i < 3; ++i)
					if (phys.flags & (JNT_XLIMITED << i))
					{
						PRINT_LOG(" %c {%g..%g}", int('X' + i), phys.min[i], phys.max[i]);
					};
				PRINT_LOG("\n");
			}
			PRINT_LOG("%sSpring angle: (%g,%g,%g), tension: (%g,%g,%g), damping: (%g,%g,%g)\n",
			          szTabs,
			          phys.spring_angle[0], phys.spring_angle[1], phys.spring_angle[2],
			          phys.spring_tension[0], phys.spring_tension[1], phys.spring_tension[2],
			          phys.damping[0], phys.damping[1], phys.damping[2]);
			for (int i = 0; i < 3; ++i)
			{
				if (!i)
					PRINT_LOG("%sFrame Matrix: ", szTabs);
				else
					PRINT_LOG("%s              ", szTabs);
				PRINT_LOG("%9.4f  %9.4f  %9.4f\n", phys.framemtx[i][0], phys.framemtx[i][1], phys.framemtx[i][2]);
			}
		}

		if (pBoneEntity->nChildren)
			stackParents.push_back(pBoneEntity->nChildren);
		else
			while (!stackParents.empty() && --stackParents.back() <= 0)
				stackParents.pop_back();
	}
}

void CLoaderCGF::LoadChunkHelper(const HELPER_CHUNK_DESC* pData, int nSize)
{
	if (!checkChunk(pData, nSize))
		return;

	PRINT_LOG("%s \"%s\", ", getHelperType(pData->type).c_str(), getObjectName(pData->chdr.ChunkID));
	PRINT_LOG("Size: {%.3f, %.3f, %.3f}\n", pData->size.x, pData->size.y, pData->size.z);
}

void CLoaderCGF::LoadChunkLight(const LIGHT_CHUNK_DESC* pData, int nSize)
{
	if (!checkChunk(pData, nSize))
		return;

	PRINT_LOG("%s light \"%s\", %s %.2f*", getLightType(pData->type).c_str(), getObjectName(pData->chdr.ChunkID), pData->on ? "ON" : "OFF", pData->intens);
	print(pData->color);
	PRINT_LOG(" Shadows: %s", pData->shadow ? "ON" : "OFF");
	if (pData->szLightImage[0])
		PRINT_LOG(" Image: \"%s\"", pData->szLightImage);
	PRINT_LOG("\n");
	PRINT_LOG("Direction: {%-4.1f,%-4.1f,%-4.1f}\n", pData->vDirection.x * 180 / M_PI, pData->vDirection.y * 180 / M_PI, pData->vDirection.z * 180 / M_PI);

	switch (pData->type)
	{
	case LT_SPOT:
	case LT_DIRECT:
		PRINT_LOG("hotspot %.3f, falloff %.3f\n", pData->hotsize * 180 / M_PI, pData->fallsize * 180 / M_PI);
		break;
	}
	if (pData->useNearAtten)
		PRINT_LOG("Near Attenuation ON: [%g..%g]\n", pData->nearAttenStart, pData->nearAttenEnd);

	if (pData->useAtten)
		PRINT_LOG("Far Attenuation ON: [%g..%g]\n", pData->attenStart, pData->attenEnd);
}

void CLoaderCGF::LoadChunkBoneLightBinding(const BONELIGHTBINDING_CHUNK_DESC_0001* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	const SBoneLightBind* pBinds = (const SBoneLightBind*)(pChunk + 1);
	PRINT_LOG("%d bone-light bindings\n", pChunk->numBindings);
	for (unsigned i = 0; i < pChunk->numBindings; ++i)
	{
		PRINT_LOG("%2d. ", i);
		if ((const char*)(pBinds + i) > (const char*)pChunk + nSize)
		{
			PRINT_LOG("*WARNING* Truncated\n");
			break;
		}

		const SBoneLightBind& bind = pBinds[i];
		// print each binding
		PRINT_LOG("Bone %2d \"%s\" Light 0x%08X \"%s\" offset (%.1f,%.1f,%.1f) rotation (%.2f,%.2f,%.2f) (%.1f degrees)\n",
		          bind.nBoneId, getBoneName(bind.nBoneId),
		          bind.nLightChunkId, getObjectName(bind.nLightChunkId),
		          bind.vLightOffset.x, bind.vLightOffset.y, bind.vLightOffset.z,
		          bind.vRotLightOrientation.x, bind.vRotLightOrientation.y, bind.vRotLightOrientation.z, bind.vRotLightOrientation.Length() * 180 / (M_PI * 2));
	}
}

void CLoaderCGF::LoadChunkMeshMorphTarget(const MESHMORPHTARGET_CHUNK_DESC_0001* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	const char*                   pChunkEnd    = (const char*)pChunk + nSize;

	const char*                   pMeshName    = getObjectName(pChunk->nChunkIdMesh);
	const SMeshMorphTargetVertex* pMorphVertex = (const SMeshMorphTargetVertex*)(pChunk + 1);
	const char*                   pMorphName   = (const char*)(pMorphVertex + pChunk->numMorphVertices);
	if (pMorphName >= pChunkEnd)
		PRINT_LOG("NULL (morph name unknown) ");
	else
	{
		PRINT_LOG("\"%s\" ", std::string(pMorphName, pChunkEnd).c_str());
	}

	std::vector<int> arrVertIndices;
	sortVertIndices(pMorphVertex, pChunk->numMorphVertices, arrVertIndices);

	PRINT_LOG(", Mesh id 0x%08X ", pChunk->nChunkIdMesh);
	if (pMeshName)
		PRINT_LOG("\"%s\"", pMeshName);
	else
		PRINT_LOG("NULL (mesh name unknown)");
	PRINT_LOG(", %u morphed vertices\n", pChunk->numMorphVertices);

	if (nSize < (int)(pChunk->numMorphVertices * sizeof(SMeshMorphTargetVertex) + sizeof(MESHMORPHTARGET_CHUNK_DESC_0001)))
	{
		PRINT_LOG("ERROR: Trunkaged Chunk: not all morph vertices present\n");
		return;
	}

	CMeshData     Mesh(pChunk->nChunkIdMesh, m_pChunkFile);

	std::set<int> setMorphedVertices;
	Vec3d         ptMin, ptMax;
	for (int j = 0; j < (int)pChunk->numMorphVertices; ++j)
	{
		int                           i       = arrVertIndices[j];
		const SMeshMorphTargetVertex& rVertex = pMorphVertex[i];

		if (i == 0)
			ptMax = ptMin = rVertex.ptVertex;
		else
			; // AddToBounds(rVertex.ptVertex, ptMin, ptMax);

// FIXME:
#if 0
		if (m_bDumpMeshVerts)
#else
		if (true)
#endif
		{
			PRINT_LOG("%4d. (%8.3f,%8.3f,%8.3f)",
			          rVertex.nVertexId,
			          rVertex.ptVertex.x, rVertex.ptVertex.y, rVertex.ptVertex.z);

			if (Mesh.pVerts)
			{
				if (rVertex.nVertexId < Mesh.nVerts)
				{
					Vec3d ptMeshVertex = Mesh.pVerts[rVertex.nVertexId].p;
					Vec3d ptOffset     = rVertex.ptVertex - ptMeshVertex;
					PRINT_LOG(" offset (%8.3f,%8.3f,%8.3f) from vtx %4d",
					          ptOffset.x, ptOffset.y, ptOffset.z,
					          rVertex.nVertexId);
				}
				else
					PRINT_LOG(" mesh vertex (%d) is out of range [0..%d)", rVertex.nVertexId, Mesh.nVerts);
			}

			PRINT_LOG("\n");
		}
		else setMorphedVertices.insert(rVertex.nVertexId);
	}

	if (!setMorphedVertices.empty())
	{
		PRINT_LOG("%d vertices morphed:", setMorphedVertices.size());
		printSet("%d", setMorphedVertices, "");
		PRINT_LOG("\nBBox {%g .. %g, %g .. %g, %g .. %g}\n", ptMin.x, ptMax.x, ptMin.y, ptMax.y, ptMin.z, ptMax.z);
	}
}

void CLoaderCGF::LoadChunkBoneInitialPos(BONEINITIALPOS_CHUNK_DESC_0001* pChunk, int nSize)
{
	if (nSize < sizeof(*pChunk))
	{
		PRINT_LOG("Trunkated Chunk: expected at least %d byte descriptor\n", sizeof(*pChunk));
		return;
	}

	const char*               pChunkEnd = (const char*)pChunk + nSize;

	const SBoneInitPosMatrix* pBones    = (const SBoneInitPosMatrix*)(pChunk + 1);
	const SBoneInitPosMatrix* pBonesEnd = pBones + pChunk->numBones;

	if (pChunkEnd > (const char*)pBonesEnd)
		PRINT_LOG("*WARNING* The chunk is overgrown by %d bytes\n", pChunkEnd - (const char*)pBonesEnd);

	if (pChunkEnd < (const char*)pBonesEnd)
		PRINT_LOG("*ERROR* The chunk is truncated by %d bytes\n", (const char*)pBonesEnd - pChunkEnd);

	PRINT_LOG("%d bones. Bound to mesh ", pChunk->numBones);
	const char* szMeshName = getObjectName(pChunk->nChunkIdMesh);
	if (szMeshName[0])
		PRINT_LOG("\"%s\"\n", szMeshName);
	else
		PRINT_LOG("ChunkId 0x%08X\n", pChunk->nChunkIdMesh);

	// FIXME:
	//if (m_bDumpMeshBones || m_bDumpBoneInitPos)
	for (unsigned nBone = 0; nBone < pChunk->numBones; ++nBone)
	{
		const SBoneInitPosMatrix& matBone = pBones[nBone];
		PRINT_LOG("%2d. \"%s\"", nBone, getBoneName(nBone));
		// check for orthogonality and parity of the matrix
		Vec3d            vX = matBone.getOrt(0);
		Vec3d            vY = matBone.getOrt(1);
		Vec3d            vZ = matBone.getOrt(2);
		SBasisProperties BasisProps(vX, vY, vZ);

		if (BasisProps.bMatrixDegraded)
			PRINT_LOG(" Matrix Degraded");
		else
		{
			if (BasisProps.fErrorDeg > 0.5)
				PRINT_LOG(" NOT Orthogonal (error=%4.1f°)", BasisProps.fErrorDeg);

			if (BasisProps.bLeftHanded)
				PRINT_LOG(" Left-handed %2d%%", (int)BasisProps.getLeftHandednessPercentage());
		}

		PRINT_LOG("\n");

		for (int nRow = 0; nRow < 4; ++nRow)
		{
			PRINT_LOG("        ");
			for (int nColumn = 0; nColumn < 3; ++nColumn)
				PRINT_LOG("%11.4f", matBone[nRow][nColumn]);
			PRINT_LOG("\n");
		}
	}
}

void CLoaderCGF::LoadChunkSourceInfo(const char* pData, unsigned nSize)
{
	const char* pDataEnd = pData + nSize;
	while (pData < pDataEnd)
	{
		PRINT_LOG("  %s\n", pData);
		pData += strlen(pData) + 1;
	}
}

void CLoaderCGF::LoadChunkSceneProps(const char* pData, unsigned nSize)
{
	if (nSize < sizeof(SCENEPROPS_CHUNK_DESC))
	{
		PRINT_LOG(" Truncated SCENEPROPS_CHUNK_DESC (%d bytes)\n", nSize);
		return;
	}

	const SCENEPROPS_CHUNK_DESC* pDesc = (const SCENEPROPS_CHUNK_DESC*)pData;

	if (nSize != sizeof(*pDesc) + sizeof(SCENEPROP_ENTITY) * pDesc->nProps)
	{
		PRINT_LOG(" Unexpected chunk size: %d scene props declared, which is %d bytes total, but %d bytes are actually in the chunk",
		          pDesc->nProps, sizeof(*pDesc) + sizeof(SCENEPROP_ENTITY) * pDesc->nProps, nSize);
	}

	for (int i = 0; i < pDesc->nProps; ++i)
	{
		const SCENEPROP_ENTITY& Prop = ((const SCENEPROP_ENTITY*)(pDesc + 1))[i];
		PRINT_LOG("%32s = \"%s\"\n", Prop.name, Prop.value);
	}
}

void CLoaderCGF::LoadChunk(int i)
{
}

void CLoaderCGF::LoadCollectedTextures(const char* szFormat)
{
	for (std::set<std::string>::const_iterator it = g_setTextures.begin(); it != g_setTextures.end(); ++it)
		PRINT_LOG(szFormat, it->c_str(), it->c_str(), it->c_str());
}

void CLoaderCGF::Load(const char* filename)
{
	const char* szFileName = filename;

	// create the object that reads the file
	//
	m_pChunkFile           = new FileReader();
	if (!m_pChunkFile->open(m_FileMapping))
	{
		PRINT_LOG("Cannot open %s: unrecognized file format or corrupted file\n", szFileName);
		return;
	}

	// prepare the mapping to verbose representation of chunk ids
	//
	InitChunkTypeNameMap();

	// print the file header
	//
	PRINT_LOG("File: %s\n", szFileName);
	getFileType(m_pChunkFile->getFileHeader().FileType);
	PRINT_LOG("  ChunkTableOffset = 0x%08X\t%s\tVersion = 0x%08X\t#Chunks = %d\n",
	          m_pChunkFile->getFileHeader().ChunkTableOffset,
	          getFileType(m_pChunkFile->getFileHeader().FileType),
	          m_pChunkFile->getFileHeader().Version,
	          m_pChunkFile->numChunks());
	if (!LoadChunks())
	{
		CryFatalError("Cannot load chunks of %s", szFileName);
	}

	// Set node parents.
	for (int i = 0; i < m_pCGF->GetNodeCount(); ++i)
	{
		CryLog("i = %d", i);
		//if (m_pCGF->GetNode(i)->nParentChunkId > 0)
		//{
		//	for (int j = 0; j < m_pCGF->GetNodeCount(); ++j)
		//	{
		//		if (m_pCGF->GetNode(i)->nParentChunkId == m_pCGF->GetNode(j)->nChunkId)
		//		{
		//			m_pCGF->GetNode(i)->pParent = m_pCGF->GetNode(j);
		//			break;
		//		}
		//	}
		//}
	}
	m_pScene->mRootNode = m_pCGF->GetNode(0);

	if (m_bCollectTextures)
	{
		PRINT_LOG("---------------------------------------------------\n");
		if (g_setTextures.empty())
			PRINT_LOG("NO Textures in use\n");
		else
		{
			PRINT_LOG("%d Textures in use:\n", g_setTextures.size());
			LoadCollectedTextures(m_bCollectTexturesForCopying ? "copy \"C:\\MasterCD\\%s\" \".\\%s\"\n" : "%s\n");
			PRINT_LOG("\n");
		}
	}
}

bool CLoaderCGF::LoadChunks()
{
	// load all chunks in sequence
	//
	for (int i = 0; i < m_pChunkFile->numChunks(); ++i)
	{
		const CHUNK_HEADER& ch    = m_pChunkFile->getChunkHeader(i);
		int                 nSize = m_pChunkFile->getChunkSize(i);

		const void*         pData = m_pChunkFile->getChunkData(i);

		addChunkToCount(ch.ChunkType, nSize);

		switch (ch.ChunkType)
		{
		case ChunkType_Mtl:
			switch (ch.ChunkVersion)
			{
			case 0x744:
				LoadChunkMtl((const MTL_CHUNK_DESC_0744*)pData, nSize);
				break;
			case 0x745:
				LoadChunkMtl((const MTL_CHUNK_DESC_0745*)pData, nSize);
				break;
			case 0x746:
				LoadChunkMtl((const MTL_CHUNK_DESC_0746*)pData, nSize);
				break;
			}
			break;
		case ChunkType_Controller:
			switch (ch.ChunkVersion)
			{
			case 0x826:
				LoadChunkController((const CONTROLLER_CHUNK_DESC_0826*)pData, nSize);
				break;
			case 0x827:
				LoadChunkController((const CONTROLLER_CHUNK_DESC_0827*)pData, nSize);
				break;
			}
			break;
		case ChunkType_Timing:
			LoadChunkTiming((const TIMING_CHUNK_DESC*)pData, nSize);
			break;
		case ChunkType_Mesh:
			LoadChunkMesh((const MESH_CHUNK_DESC*)pData, nSize);
			break;
		case ChunkType_Node:
			LoadChunkNode((const NODE_CHUNK_DESC*)pData, nSize);
			break;

		case ChunkType_BoneNameList:
		{
			switch (ch.ChunkVersion)
			{
			case BONENAMELIST_CHUNK_DESC_0744::VERSION:
				LoadChunkBoneNameList((const BONENAMELIST_CHUNK_DESC_0744*)pData, nSize);
				break;
			case BONENAMELIST_CHUNK_DESC_0745::VERSION:
				LoadChunkBoneNameList((const BONENAMELIST_CHUNK_DESC_0745*)pData, nSize);
				break;
			}
		}
		break;

		case ChunkType_BoneInitialPos:
			LoadChunkBoneInitialPos((BONEINITIALPOS_CHUNK_DESC_0001*)pData, nSize);
			break;

		case ChunkType_BoneAnim:
			LoadChunkBoneAnim((const BONEANIM_CHUNK_DESC*)pData, nSize);
			break;

		case ChunkType_BoneMesh:
			LoadChunkMesh((const MESH_CHUNK_DESC*)pData, nSize);
			break;

		case ChunkType_Light:
			LoadChunkLight((const LIGHT_CHUNK_DESC*)pData, nSize);
			break;

		case ChunkType_Helper:
			LoadChunkHelper((const HELPER_CHUNK_DESC*)pData, nSize);
			break;

		case ChunkType_BoneLightBinding:
		{
			switch (ch.ChunkVersion)
			{
			case BONELIGHTBINDING_CHUNK_DESC_0001::VERSION:
				LoadChunkBoneLightBinding((const BONELIGHTBINDING_CHUNK_DESC_0001*)pData, nSize);
				break;
			}
		};
		break;

		case ChunkType_MeshMorphTarget:
		{
			switch (ch.ChunkVersion)
			{
			case MESHMORPHTARGET_CHUNK_DESC_0001::VERSION:
				LoadChunkMeshMorphTarget((const MESHMORPHTARGET_CHUNK_DESC_0001*)pData, nSize);
				break;
			}
		}
		break;

		case ChunkType_SourceInfo:
			LoadChunkSourceInfo((const char*)pData, nSize);
			break;

		case ChunkType_SceneProps:
			LoadChunkSceneProps((const char*)pData, nSize);
			break;

		default:
			printChunkUnknown(pData, nSize);
			break;
		}
	}

	return true;
}

CMaterialCGF* CLoaderCGF::LoadMaterialFromChunk(int nchunkId)
{
	auto& header = m_pChunkFile->getChunkHeader(nchunkId);
	assert(header.ChunkVersion == MTL_CHUNK_DESC_0746::VERSION && "Bad version of material!");
	auto*            pChunk = (MTL_CHUNK_DESC_0746*)m_pChunkFile->getChunkData(nchunkId);
	std::vector<int> mtlIds;
	if (pChunk->MtlType == MTL_MULTI)
	{
		mtlIds.resize(pChunk->multi.nChildren);
		memcpy(mtlIds.data(), (pChunk + 1), pChunk->multi.nChildren * sizeof(int));
	}
	for (size_t i = 0, length = pChunk->multi.nChildren; i < length; i++)
	{
		pChunk = (MTL_CHUNK_DESC_0746*)m_pChunkFile->getChunkData(mtlIds[i]);
		switch (pChunk->MtlType)
		{
		case MTL_STANDARD:
			PRINT_LOG("Colors: diffuse ");
			print(pChunk->std.col_d);
			PRINT_LOG(", specular ");
			print(pChunk->std.col_s);
			PRINT_LOG(", ambient ");
			print(pChunk->std.col_a);
			PRINT_LOG("\nSpecLevel %.2f, SpecShininess %.2f, SelfIllumination %.2f, Opacity %.2f\n", pChunk->std.specLevel, pChunk->std.specShininess, pChunk->std.selfIllum, pChunk->std.opacity);
			print("Ambient", pChunk->std.tex_a);
			print("Diffuse", pChunk->std.tex_d);
			print("Specular", pChunk->std.tex_s);
			print("Opacity", pChunk->std.tex_o);
			print("Bump", pChunk->std.tex_b);
			print("Gloss", pChunk->std.tex_g);
			print("Reflection", pChunk->std.tex_rl);
			print("Subsurface", pChunk->std.tex_subsurf);
			print("Detail", pChunk->std.tex_det);
			PRINT_LOG("Flags %08X %s ", pChunk->std.flags, getMtlFlags(pChunk->std.flags).c_str());
			PRINT_LOG("Dyn: Bounce %.3f, StaticFriction %.3f, SlidingFriction %.3f\n", pChunk->std.Dyn_Bounce, pChunk->std.Dyn_StaticFriction, pChunk->std.Dyn_SlidingFriction);
			break;

		case MTL_MULTI:
			PRINT_LOG("%d children\n", pChunk->multi.nChildren);
			break;
		}
	}

	return nullptr;
}

CLoaderCGF::CChunkSizeProps::CChunkSizeProps()
    : m_nCount(0)
    , m_nSize(0)
{
}

CLoaderCGF::CChunkSizeProps::CChunkSizeProps(unsigned nSize)
    : m_nCount(1)
    , m_nSize(nSize)
{
}

void CLoaderCGF::CChunkSizeProps::inc(unsigned nSize)
{
	m_nSize += nSize;
	++m_nCount;
}

unsigned CLoaderCGF::CChunkSizeProps::size() const { return m_nSize; }

unsigned CLoaderCGF::CChunkSizeProps::count() const { return m_nCount; }

double   CLoaderCGF::CChunkSizeProps::mean() const { return double(m_nSize) / m_nCount; }

CLoaderCGF::CMeshData::CMeshData(unsigned nChunkId, FileReaderPtr pChunkFile)
{
	nVerts = 0;
	pVerts = 0;

	for (unsigned i = 0; i < (unsigned)pChunkFile->numChunks(); ++i)
	{
		const CHUNK_HEADER& chunkHeader = pChunkFile->getChunkHeader(i);
		if (chunkHeader.ChunkType == ChunkType_Mesh && chunkHeader.ChunkID == nChunkId)
		{
			switch (chunkHeader.ChunkVersion)
			{
			case MESH_CHUNK_DESC::VERSION:
			{
				const MESH_CHUNK_DESC* pChunk = (const MESH_CHUNK_DESC*)pChunkFile->getChunkData(i);
				this->nVerts                  = pChunk->nVerts;
				this->pVerts                  = (const CryVertex*)(pChunk + 1);
			}
				return;
			}
		}
	}
}
