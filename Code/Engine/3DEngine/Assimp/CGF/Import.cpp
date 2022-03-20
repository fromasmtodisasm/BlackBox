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

#include "Import.h"

#if 0
#define PRINT_LOG CryLog;
#else
#define PRINT_LOG
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
			if (CCgfDump::unpackComponent(m_pVertices[nLeft], nCoord) < CCgfDump::unpackComponent(m_pVertices[nRight], nCoord))
				return true;
			else if (CCgfDump::unpackComponent(m_pVertices[nLeft], nCoord) > CCgfDump::unpackComponent(m_pVertices[nRight], nCoord))
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
bool CCgfDump::checkChunk(const Desc* pData, int nSize)
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
void CCgfDump::printSet(const char* szFormat, const std::set<T>& setMtls, const char* szPostfix)
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

CCgfDump::CCgfDump(MemoryBlobPtr fileMapping, aiScene* scene)
    : m_FileMapping(fileMapping)
    , m_pScene(scene)
{
}

void CCgfDump::printOptionHelp(const char* szOption, const char* szDescription)
{
	PRINT_LOG("  % 30s   %s\n", szOption, szDescription);
}

void CCgfDump::addNodeName(const std::string& strName)
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

void CCgfDump::print(const StringArray& arrStrings)
{
	for (StringArray::const_iterator it = arrStrings.begin(); it != arrStrings.end(); ++it)
	{
		if (it != arrStrings.begin())
			PRINT_LOG(", ");
		PRINT_LOG("\"%s\"", it->c_str());
	}
}

void CCgfDump::addChunkToCount(unsigned nChunkType, unsigned nChunkSize)
{
	if (g_mapChunkCounts.find(nChunkType) != g_mapChunkCounts.end())
		g_mapChunkCounts[nChunkType].inc(nChunkSize);
	else
		g_mapChunkCounts.insert(ChunkCountMap::value_type(nChunkType, CChunkSizeProps(nChunkSize)));
}

void CCgfDump::printChunkCounts()
{
	if (!g_mapChunkCounts.empty())
	{
		PRINT_LOG("Chunk counts (%d chunk types encountered):\n\tChunk\tCount\tTotal\tMean\n", g_mapChunkCounts.size());
		for (ChunkCountMap::const_iterator it = g_mapChunkCounts.begin(); it != g_mapChunkCounts.end(); ++it)
			PRINT_LOG("\t%s\t%d\t%d\t%.1f\n", getChunkName(it->first).c_str(), it->second.count(), it->second.size(), it->second.mean());
	}
}

void CCgfDump::InitChunkTypeNameMap()
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

std::string CCgfDump::getChunkName(unsigned nChunkType)
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

const char* CCgfDump::getFileType(unsigned nFileType)
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

void CCgfDump::printChunkUnknown(const void* pChunk, int nSize)
{
	char* pData = (char*)pChunk;
	++g_numUndupedChunks;
}

void CCgfDump::print(const CryIRGB& color)
{
	PRINT_LOG("(%d,%d,%d)", (int)color.r, (int)color.g, (int)color.b);
}

void CCgfDump::print(const char* szName, const TextureMap3& tex)
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

void CCgfDump::print(const char* szName, const TextureMap2& tex)
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

void CCgfDump::print(const char* szName, const TextureMap& tex)
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

void CCgfDump::initVertIndices(std::vector<int>& arrVertIndices, int nVerts)
{
	arrVertIndices.resize(nVerts);
	for (int i = 0; i < nVerts; ++i)
	{
		arrVertIndices[i] = i;
	}
}

float CCgfDump::unpackComponent(const CryVertex& rVector, int nComponent)
{
	return rVector.p[nComponent];
}

float CCgfDump::unpackComponent(const SMeshMorphTargetVertex& rVector, int nComponent)
{
	return rVector.ptVertex[nComponent];
}

void CCgfDump::sortVertIndices(const CryVertex* pVerts, int nVerts, std::vector<int>& arrVertIndices)
{
	initVertIndices(arrVertIndices, nVerts);
	if (g_szVertexSortOrder[0])
		std::sort(arrVertIndices.begin(), arrVertIndices.end(), TVertexSorter<CryVertex>(pVerts, nVerts, g_szVertexSortOrder));
}

void CCgfDump::sortVertIndices(const SMeshMorphTargetVertex* pVerts, int nVerts, std::vector<int>& arrVertIndices)
{
	initVertIndices(arrVertIndices, nVerts);
	if (g_szVertexSortOrder[0])
		std::sort(arrVertIndices.begin(), arrVertIndices.end(), TVertexSorter<SMeshMorphTargetVertex>(pVerts, nVerts, g_szVertexSortOrder));
}

void CCgfDump::printChunkMtl(const MTL_CHUNK_DESC_0746* pChunk, int nSize)
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

void CCgfDump::printChunkMtl(const MTL_CHUNK_DESC_0745* pChunk, int nSize)
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

void CCgfDump::printChunkMtl(const MTL_CHUNK_DESC_0744* pChunk, int nSize)
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

const char* CCgfDump::getCtrlName(unsigned nCtrl)
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

const char* CCgfDump::getBoneName(const std::vector<const char*>& arrNames, int nBone, BoneIdentificationEnum nIdentity)
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

void CCgfDump::getBoneNames(const BONENAMELIST_CHUNK_DESC_0744* pChunk, unsigned nSize, std::vector<const char*>& arrNames)
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

const char* CCgfDump::getBoneName0744(const BONENAMELIST_CHUNK_DESC_0744* pChunk, unsigned nSize, int nBone, BoneIdentificationEnum nIdentity)
{
	if (!pChunk)
		return "#NO CHUNK#";
	std::vector<const char*> arrNames;
	getBoneNames(pChunk, nSize, arrNames);

	return getBoneName(arrNames, nBone, nIdentity);
}

void CCgfDump::getBoneNames(const BONENAMELIST_CHUNK_DESC_0745* pNameChunk, unsigned nChunkSize, std::vector<const char*>& arrGeomBoneNameTable)
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

const char* CCgfDump::getBoneName0745(const BONENAMELIST_CHUNK_DESC_0745* pNameChunk, unsigned nChunkSize, int nBone, BoneIdentificationEnum nIdentity)
{
	std::vector<const char*> arrGeomBoneNameTable;
	getBoneNames(pNameChunk, nChunkSize, arrGeomBoneNameTable);

	return getBoneName(arrGeomBoneNameTable, nBone, nIdentity);
}

const char* CCgfDump::getBoneName(int nBone, BoneIdentificationEnum nIdentity)
{
	const BONENAMELIST_CHUNK_DESC_0744* pChunk744 = NULL;
	const BONENAMELIST_CHUNK_DESC_0745* pChunk    = NULL;
	unsigned                            i, nSize = 0;

	for (i = 0; !pChunk && i < (unsigned)m_pReader->numChunks(); ++i)
	{
		const CHUNK_HEADER& chunkHeader = m_pReader->getChunkHeader(i);
		if (chunkHeader.ChunkType == ChunkType_BoneNameList)
		{
			switch (chunkHeader.ChunkVersion)
			{
			case BONENAMELIST_CHUNK_DESC_0744::VERSION:
				pChunk744 = (const BONENAMELIST_CHUNK_DESC_0744*)m_pReader->getChunkData(i);
				break;
			case BONENAMELIST_CHUNK_DESC_0745::VERSION:
				pChunk = (const BONENAMELIST_CHUNK_DESC_0745*)m_pReader->getChunkData(i);
				break;
			}
			nSize = m_pReader->getChunkSize(i);
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

const char* CCgfDump::getObjectName(int nChunkId)
{
	for (unsigned i = 0; i < (unsigned)m_pReader->numChunks(); ++i)
	{
		const CHUNK_HEADER& chunkHeader = m_pReader->getChunkHeader(i);
		if (chunkHeader.ChunkType == ChunkType_Node)
		{
			switch (chunkHeader.ChunkVersion)
			{
			case NODE_CHUNK_DESC::VERSION:
			{
				const NODE_CHUNK_DESC* pChunk = (const NODE_CHUNK_DESC*)m_pReader->getChunkData(i);
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

const char* CCgfDump::getMaterialName(int nMtlId)
{
	int nStdMtlCounter = 0;
	for (unsigned i = 0; i < (unsigned)m_pReader->numChunks(); ++i)
	{
		const CHUNK_HEADER& chunkHeader = m_pReader->getChunkHeader(i);
		const void*         pData       = m_pReader->getChunkData(i);
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

void CCgfDump::printMtlSet(const char* szFormat, const std::set<unsigned>& setMtls, const char* szPostfix)
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

std::string CCgfDump::getHelperType(HelperTypes nType)
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

void CCgfDump::printChunkController(const CONTROLLER_CHUNK_DESC_0826* pChunk, int nSize)
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
		if (m_bDumpKeys)
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

void CCgfDump::printChunkController(const CONTROLLER_CHUNK_DESC_0827* pChunk, int nSize)
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
	if (m_bDumpKeys)
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

void CCgfDump::printChunkTiming(const TIMING_CHUNK_DESC* pChunk, int nSize)
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

void CCgfDump::printChunkMesh(const MESH_CHUNK_DESC* pChunk, int nSize)
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

	if (pChunk->HasBoneInfo)
		PRINT_LOG(", Has BONE INFO");
	else if (m_bDumpMeshBones)
		PRINT_LOG(", Has NO bone info");

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
	if (m_bDumpMeshVerts)
		PRINT_LOG("Vertices: %d\n", pChunk->nVerts);

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

		if (m_bDumpMeshVerts)
			PRINT_LOG("\t% 4d\tp=(%8.3f,%8.3f,%8.3f) n=(%7.3f,%7.3f,%7.3f)\n", i, v.p.x, v.p.y, v.p.z, v.n.x, v.n.y, v.n.z);
	}

	if (numDuplicateVertices)
	{
		PRINT_LOG("%u Duplicate vertices; ", numDuplicateVertices);
		if (m_bDumpMeshVerts)
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

	if (m_bDumpMeshFaces)
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

		if (m_bDumpMeshFaces)
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
		if (m_bDumpMeshUVs)
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

			if (m_bDumpMeshUVs)
				PRINT_LOG("\t% 4d\t(%.4f, %.4f)\n", i, uv.u, uv.v);
		}
		PRINT_LOG("UV BBox {%g..%g, %g..%g}\n", uvMin.u, uvMax.u, uvMin.v, uvMax.v);

		if (m_bDumpMeshTexFaces)
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

			if (m_bDumpMeshTexFaces)
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

		if (m_bDumpMeshBones)
			PRINT_LOG("%d Bone Links:\n", pChunk->nVerts);
		for (i = 0; i < pChunk->nVerts; ++i)
		{
			if (m_bDumpMeshBones)
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
				if (m_bDumpMeshBones && nLink)
					PRINT_LOG(" ");
				const CryLink& l = pLinks[nLink];
				fSumWeight += l.Blending;
				if (m_bDumpMeshBones)
					PRINT_LOG("w[%2d] = %.3f * TM (%-6.3f,%-6.3f,%-6.3f)", l.BoneID, l.Blending, l.offset.x, l.offset.y, l.offset.z);
			}
			if (m_bDumpMeshBones)
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
		if (m_bDumpMeshVertCols)
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
			if (m_bDumpMeshVertCols)
				PRINT_LOG("\t% 4d  (% 3u,% 3u,% 3u)\n", i, (unsigned)c.r, (unsigned)c.g, (unsigned)c.b);
		}
		pBoneInfo = pVertColor + pChunk->nVerts;
	}

	unsigned nUsedSize = (unsigned)pBoneInfo - (unsigned)pChunk;
	if (nUsedSize != nSize)
		PRINT_LOG("Unexpected length of data in chunk: %d used instead of %d available (%d bytes extra)", nUsedSize, nSize, nSize - nUsedSize);

	PRINT_LOG("\n");
}

void CCgfDump::printChunkNode(const NODE_CHUNK_DESC* pChunk, int nSize)
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

	//
	// if (pChunk->ParentID == -1)
	{
		m_pScene->mRootNode   = new aiNode(pChunk->name);
		auto root             = m_pScene->mRootNode;
		root->mParent         = nullptr;

		memcpy(&root->mTransformation, &pChunk->tm, sizeof Matrix44);

		auto header           = m_pReader->getChunkHeader(pChunk->ObjectID);
		if (header.ChunkType == ChunkType_Mesh)
		{
			auto mesh_chunk      = (MESH_CHUNK_DESC*)m_pReader->getChunkData(pChunk->ObjectID);

			m_pScene->mMeshes    = new aiMesh*[1];
			m_pScene->mNumMeshes = 1;
			{
				root->mNumMeshes   = 1;
				root->mMeshes      = new unsigned int[1];

				auto mesh          = new aiMesh;
				mesh->mName        = getObjectName(pChunk->ObjectID);

				mesh->mNumVertices = mesh_chunk->nVerts;
				mesh->mVertices    = new aiVector3D[mesh->mNumVertices];
				mesh->mNormals     = new aiVector3D[mesh->mNumVertices];

				auto verts         = (CryVertex*)(mesh_chunk + 1);

				for (size_t i = 0; i < mesh->mNumVertices; i++)
				{
					memcpy(&mesh->mVertices[i], &verts[i].p, sizeof aiVector3D);
					memcpy(&mesh->mNormals[i], &verts[i].n, sizeof aiVector3D);
				}

				mesh->mNumFaces = mesh_chunk->nFaces;
				mesh->mFaces    = new aiFace[mesh->mNumFaces];

				auto faces      = (CryFace*)(verts + mesh_chunk->nVerts);
				for (size_t i = 0; i < mesh->mNumFaces; i++)
				{
					auto& face       = mesh->mFaces[i];
					face.mNumIndices = 3;
					face.mIndices    = new unsigned int[3];

					memcpy(face.mIndices, &faces[i], 3 * sizeof(unsigned int));
				}

				if (mesh_chunk->nTVerts != 0)
				{
					mesh->mNumUVComponents[0] = 2;
					mesh->mTextureCoords[0]   = new aiVector3D[mesh->mNumVertices];

					auto tfaces               = (CryUV*)(faces + mesh_chunk->nFaces);
					for (size_t i = 0; i < mesh->mNumVertices; i++)
					{
						auto& face = mesh->mTextureCoords[0];

						face->x    = tfaces[i].u;
						face->y    = tfaces[i].v;
						face->z    = 0.f;
					}
				}

				m_pScene->mMeshes[0] = mesh;
			}
			//mesh->mNumVertices
		}
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

void CCgfDump::printChunkBoneNameList(const BONENAMELIST_CHUNK_DESC_0744* pChunk, int nSize)
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

void CCgfDump::printChunkBoneNameList(const BONENAMELIST_CHUNK_DESC_0745* pChunk, int nSize)
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

void CCgfDump::printChunkBoneAnim(const BONEANIM_CHUNK_DESC* pChunk, int nSize)
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

void CCgfDump::printChunkHelper(const HELPER_CHUNK_DESC* pData, int nSize)
{
	if (!checkChunk(pData, nSize))
		return;

	PRINT_LOG("%s \"%s\", ", getHelperType(pData->type).c_str(), getObjectName(pData->chdr.ChunkID));
	PRINT_LOG("Size: {%.3f, %.3f, %.3f}\n", pData->size.x, pData->size.y, pData->size.z);
}

void CCgfDump::printChunkLight(const LIGHT_CHUNK_DESC* pData, int nSize)
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

void CCgfDump::printChunkBoneLightBinding(const BONELIGHTBINDING_CHUNK_DESC_0001* pChunk, int nSize)
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

void CCgfDump::printChunkMeshMorphTarget(const MESHMORPHTARGET_CHUNK_DESC_0001* pChunk, int nSize)
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

	CMeshData     Mesh(pChunk->nChunkIdMesh, m_pReader);

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

		if (m_bDumpMeshVerts)
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
		else
			setMorphedVertices.insert(rVertex.nVertexId);
	}

	if (!setMorphedVertices.empty())
	{
		PRINT_LOG("%d vertices morphed:", setMorphedVertices.size());
		printSet("%d", setMorphedVertices, "");
		PRINT_LOG("\nBBox {%g .. %g, %g .. %g, %g .. %g}\n", ptMin.x, ptMax.x, ptMin.y, ptMax.y, ptMin.z, ptMax.z);
	}
}

void CCgfDump::printChunkBoneInitialPos(BONEINITIALPOS_CHUNK_DESC_0001* pChunk, int nSize)
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

	if (m_bDumpMeshBones || m_bDumpBoneInitPos)
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

void CCgfDump::printChunkSourceInfo(const char* pData, unsigned nSize)
{
	const char* pDataEnd = pData + nSize;
	while (pData < pDataEnd)
	{
		PRINT_LOG("  %s\n", pData);
		pData += strlen(pData) + 1;
	}
}

void CCgfDump::printChunkSceneProps(const char* pData, unsigned nSize)
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

void CCgfDump::printChunk(int i)
{
	const CHUNK_HEADER& ch    = m_pReader->getChunkHeader(i);
	int                 nSize = m_pReader->getChunkSize(i);

	if (!m_bBriefInfo || ch.ChunkType == ChunkType_Timing)
		PRINT_LOG("-------------------------------------------------------------------\n%s\tVersion: %04X\tChunkID: 0x%08X (%d)\tSize: %d bytes\tFileOffset: 0x%08X\n",
		       getChunkName(ch.ChunkType).c_str(),
		       ch.ChunkVersion,
		       ch.ChunkID, ch.ChunkID,
		       nSize,
		       ch.FileOffset);

	const void* pData = m_pReader->getChunkData(i);

	addChunkToCount(ch.ChunkType, nSize);

	switch (ch.ChunkType)
	{
	case ChunkType_Mtl:
		if (!m_bBriefInfo)
			switch (ch.ChunkVersion)
			{
			case 0x744:
				printChunkMtl((const MTL_CHUNK_DESC_0744*)pData, nSize);
				break;
			case 0x745:
				printChunkMtl((const MTL_CHUNK_DESC_0745*)pData, nSize);
				break;
			case 0x746:
				printChunkMtl((const MTL_CHUNK_DESC_0746*)pData, nSize);
				break;
			}
		break;
	case ChunkType_Controller:
		if (!m_bBriefInfo)
			switch (ch.ChunkVersion)
			{
			case 0x826:
				printChunkController((const CONTROLLER_CHUNK_DESC_0826*)pData, nSize);
				break;
			case 0x827:
				printChunkController((const CONTROLLER_CHUNK_DESC_0827*)pData, nSize);
				break;
			}
		break;
	case ChunkType_Timing:
		printChunkTiming((const TIMING_CHUNK_DESC*)pData, nSize);
		break;
	case ChunkType_Mesh:
		if (!m_bBriefInfo)
			printChunkMesh((const MESH_CHUNK_DESC*)pData, nSize);
		break;
	case ChunkType_Node:
		if (!m_bBriefInfo)
			printChunkNode((const NODE_CHUNK_DESC*)pData, nSize);
		break;

	case ChunkType_BoneNameList:
		if (!m_bBriefInfo)
		{
			switch (ch.ChunkVersion)
			{
			case BONENAMELIST_CHUNK_DESC_0744::VERSION:
				printChunkBoneNameList((const BONENAMELIST_CHUNK_DESC_0744*)pData, nSize);
				break;
			case BONENAMELIST_CHUNK_DESC_0745::VERSION:
				printChunkBoneNameList((const BONENAMELIST_CHUNK_DESC_0745*)pData, nSize);
				break;
			}
		}
		break;

	case ChunkType_BoneInitialPos:
		printChunkBoneInitialPos((BONEINITIALPOS_CHUNK_DESC_0001*)pData, nSize);
		break;

	case ChunkType_BoneAnim:
		if (!m_bBriefInfo)
			printChunkBoneAnim((const BONEANIM_CHUNK_DESC*)pData, nSize);
		break;

	case ChunkType_BoneMesh:
		if (!m_bBriefInfo)
			printChunkMesh((const MESH_CHUNK_DESC*)pData, nSize);
		break;

	case ChunkType_Light:
		if (!m_bBriefInfo)
			printChunkLight((const LIGHT_CHUNK_DESC*)pData, nSize);
		break;

	case ChunkType_Helper:
		if (!m_bBriefInfo)
			printChunkHelper((const HELPER_CHUNK_DESC*)pData, nSize);
		break;

	case ChunkType_BoneLightBinding:
		if (!m_bBriefInfo)
		{
			switch (ch.ChunkVersion)
			{
			case BONELIGHTBINDING_CHUNK_DESC_0001::VERSION:
				printChunkBoneLightBinding((const BONELIGHTBINDING_CHUNK_DESC_0001*)pData, nSize);
				break;
			}
		};
		break;

	case ChunkType_MeshMorphTarget:
		if (!m_bBriefInfo)
		{
			switch (ch.ChunkVersion)
			{
			case MESHMORPHTARGET_CHUNK_DESC_0001::VERSION:
				printChunkMeshMorphTarget((const MESHMORPHTARGET_CHUNK_DESC_0001*)pData, nSize);
				break;
			}
		}
		break;

	case ChunkType_SourceInfo:
		printChunkSourceInfo((const char*)pData, nSize);
		break;

	case ChunkType_SceneProps:
		printChunkSceneProps((const char*)pData, nSize);
		break;

	default:
		printChunkUnknown(pData, nSize);
		break;
	}
}

void CCgfDump::printCollectedTextures(const char* szFormat)
{
	for (std::set<std::string>::const_iterator it = g_setTextures.begin(); it != g_setTextures.end(); ++it)
		PRINT_LOG(szFormat, it->c_str(), it->c_str(), it->c_str());
}

void CCgfDump::Dump(int argc, char* argv[])
{
	const char* szFileName = NULL;
	if (argc < 2)
	{
		PRINT_LOG("Usage: %s -option1 -option2 ... file-name.cgf(caf) > Output-file-name.txt\n", argv[0]);
		PRINT_LOG("Options:\n");
		printOptionHelp("-briefInfo", "prints only the summary information about the file and the timing very essential chunks (timing)");
		printOptionHelp("-keys", "prints detailed information about keys");
		printOptionHelp("-mesh", "prints all detailed information about mesh.");
		printOptionHelp("-meshVerts", "prints mesh chunk vertices");
		printOptionHelp("-meshFaces", "prints mesh chunk face lists");
		printOptionHelp("-meshUVs", "prints mesh chunk UV coordinate lists");
		printOptionHelp("-meshTexFaces", "prints texture face lists");
		printOptionHelp("-meshBones", "prints bone binding info");
		printOptionHelp("-boneInitPos", "prints the initial bone position matrices, if the BoneInitialPos chunk is available");
		printOptionHelp("-meshVertCols", "prints vertex color lists");
		printOptionHelp("-collectTextures", "observes all textures used by cgf file and lists them at the bottom of the output");
		printOptionHelp("-collectTexturesForCopying", "Outputs textures prepared to copy somewhere as a set of commands");
		return;
	}

	// Detect the flags
	//
	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] != '/' && argv[i][0] != '-')
		{
			if (szFileName)
			{
				PRINT_LOG("Only one file name is allowed\n");
				return;
			}
			szFileName = argv[i];
		}
		else
		{
			const char* szOption = argv[i] + 1;
			if (!strcmpi(szOption, "keys"))
				m_bDumpKeys = true;
			else if (!strcmpi(szOption, "mesh"))
			{
				m_bDumpMeshVerts    = true;
				m_bDumpMeshFaces    = true;
				m_bDumpMeshUVs      = true;
				m_bDumpMeshTexFaces = true;
				m_bDumpMeshBones    = true;
				m_bDumpMeshVertCols = true;
			}
			else if (!strcmpi(szOption, "boneInitPos"))
			{
				m_bDumpBoneInitPos = true;
			}
			else if (!strcmpi(szOption, "meshVerts"))
			{
				m_bDumpMeshVerts = true;
				if (i < argc - 1 && argv[i + 1][0] >= 'X' && argv[i + 1][0] <= 'Z')
					strcpy(g_szVertexSortOrder, argv[++i]);
			}
			else if (!strcmpi(szOption, "meshFaces"))
			{
				m_bDumpMeshFaces = true;
				if (i < argc - 1 && argv[i + 1][0] >= '0' && argv[i + 1][0] <= '2')
					strcpy(g_szFaceSortOrder, argv[++i]);
			}
			else if (!strcmpi(szOption, "meshUVs"))
			{
				m_bDumpMeshUVs = true;
				if (i < argc - 1 && argv[i + 1][0] >= 'U' && argv[i + 1][0] <= 'V')
					strcpy(g_szUVSortOrder, argv[++i]);
			}
			else if (!strcmpi(szOption, "meshTexFaces"))
			{
				m_bDumpMeshTexFaces = true;
			}
			else if (!strcmpi(szOption, "meshBones"))
			{
				m_bDumpMeshBones = true;
			}
			else if (!strcmpi(szOption, "meshVertCols"))
			{
				m_bDumpMeshVertCols = true;
			}
			else if (!strcmpi(szOption, "collectTextures"))
			{
				m_bCollectTextures = true;
			}
			else if (!strcmpi(szOption, "collectTexturesForCopying"))
			{
				m_bCollectTextures           = true;
				m_bCollectTexturesForCopying = true;
			}
			else if (!strcmpi(szOption, "briefInfo"))
			{
				m_bBriefInfo = true;
			}
		}
	}

	if (!szFileName)
	{
		PRINT_LOG("You must give me at least one file name, not only options\n");
		return;
	}

	// create the object that reads the file
	//
	m_pReader = new FileReader();
	if (!m_pReader->open(m_FileMapping))
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
	getFileType(m_pReader->getFileHeader().FileType);
	PRINT_LOG("  ChunkTableOffset = 0x%08X\t%s\tVersion = 0x%08X\t#Chunks = %d\n",
	       m_pReader->getFileHeader().ChunkTableOffset,
	       getFileType(m_pReader->getFileHeader().FileType),
	       m_pReader->getFileHeader().Version,
	       m_pReader->numChunks());

	// print all chunks in sequence
	//
	for (int i = 0; i < m_pReader->numChunks(); ++i)
		printChunk(i);

	if (!m_bBriefInfo)
		PRINT_LOG("------------------------------------------------------------------------------------\nSummary:\n");
	else
		PRINT_LOG("\n");

	if (!g_setDuplicateNodeNames.empty())
	{
		PRINT_LOG("WARNING: duplicate node names {");
		print(g_setDuplicateNodeNames);
		PRINT_LOG("}\n");
	}

	if (g_numUnusedBytes)
		PRINT_LOG("%u wasted bytes\n", g_numUnusedBytes);

	if (!m_bBriefInfo)
		printChunkCounts();

	if (g_numUndupedChunks)
		PRINT_LOG("%u undumped chunks\n", g_numUndupedChunks);

	if (m_bCollectTextures)
	{
		PRINT_LOG("---------------------------------------------------\n");
		if (g_setTextures.empty())
			PRINT_LOG("NO Textures in use\n");
		else
		{
			PRINT_LOG("%d Textures in use:\n", g_setTextures.size());
			printCollectedTextures(m_bCollectTexturesForCopying ? "copy \"C:\\MasterCD\\%s\" \".\\%s\"\n" : "%s\n");
			PRINT_LOG("\n");
		}
	}
}

CCgfDump::CChunkSizeProps::CChunkSizeProps()
    : m_nCount(0)
    , m_nSize(0)
{
}

CCgfDump::CChunkSizeProps::CChunkSizeProps(unsigned nSize)
    : m_nCount(1)
    , m_nSize(nSize)
{
}

void CCgfDump::CChunkSizeProps::inc(unsigned nSize)
{
	m_nSize += nSize;
	++m_nCount;
}

unsigned CCgfDump::CChunkSizeProps::size() const { return m_nSize; }

unsigned CCgfDump::CChunkSizeProps::count() const { return m_nCount; }

double   CCgfDump::CChunkSizeProps::mean() const { return double(m_nSize) / m_nCount; }

CCgfDump::CMeshData::CMeshData(unsigned nChunkId, FileReaderPtr m_pReader)
{
	nVerts = 0;
	pVerts = 0;

	for (unsigned i = 0; i < (unsigned)m_pReader->numChunks(); ++i)
	{
		const CHUNK_HEADER& chunkHeader = m_pReader->getChunkHeader(i);
		if (chunkHeader.ChunkType == ChunkType_Mesh && chunkHeader.ChunkID == nChunkId)
		{
			switch (chunkHeader.ChunkVersion)
			{
			case MESH_CHUNK_DESC::VERSION:
			{
				const MESH_CHUNK_DESC* pChunk = (const MESH_CHUNK_DESC*)m_pReader->getChunkData(i);
				this->nVerts                  = pChunk->nVerts;
				this->pVerts                  = (const CryVertex*)(pChunk + 1);
			}
				return;
			}
		}
	}
}
