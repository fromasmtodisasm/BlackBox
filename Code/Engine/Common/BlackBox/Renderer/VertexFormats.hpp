//////////////////////////////////////////////////////////////////////////
//
//	Crytek Source code
//	Copyright (c) Crytek 2001-2004
//
//	File: VertexFormats.h
//  Description: Definition of different vertex formats used in the engine.
//
//	History:
//	- Feb 23,2001:Created by Marco Corbetta
//	- Modified by Andrey Khonich
//	- February 2005: Modified by Marco Corbetta for SDK release
//
//////////////////////////////////////////////////////////////////////////

#ifndef VERTEXFORMATS_H
#define VERTEXFORMATS_H

#if _MSC_VER > 1000
	#pragma once
#endif

//////////////////////////////////////////////////////////////////////
struct InputLayoutHandle
{
	typedef uint8_t ValueType;
	ValueType       value;

	constexpr InputLayoutHandle()
	    : value(Unspecified)
	{
	}
	constexpr InputLayoutHandle(ValueType v)
	    : value(v)
	{
	}

	// Test operators
	template<typename T>
	bool operator==(const T other) const
	{
		return value == other;
	}
	template<typename T>
	bool operator!=(const T other) const
	{
		return value != other;
	}
	// Range operators
	template<typename T>
	bool operator<=(const T other) const
	{
		return value <= other;
	}
	template<typename T>
	bool operator>=(const T other) const
	{
		return value >= other;
	}
	// Sorting operators
	template<typename T>
	bool operator<(const T other) const
	{
		return value < other;
	}
	template<typename T>
	bool operator>(const T other) const
	{
		return value > other;
	}

	// Auto cast for array access operator []
	                           operator ValueType() const { return value; }

	// Not an enum, because of SWIG
	static constexpr ValueType Unspecified = ValueType(~0);
};

//////////////////////////////////////////////////////////////////////////
// If you change this you also have to change gBufOffsTable in CRendElement.cpp
enum eVertexFormat
{
	VERTEX_FORMAT_P3F               = 1,  // shadow volumes (12 bytes)
	VERTEX_FORMAT_P3F_C4B           = 2,  // usually terrain (16 bytes)
	VERTEX_FORMAT_P3F_T2F           = 3,  // everything else (20 bytes)
	VERTEX_FORMAT_P3F_C4B_T2F       = 4,  // usually plants (24 bytes)
	VERTEX_FORMAT_TRP3F_C4B_T2F     = 5,  // fonts (28 bytes)
	VERTEX_FORMAT_P3F_C4B_C4B       = 6,  // terrain with detail layers (20 bytes)
	VERTEX_FORMAT_P3F_N             = 7,  // (24 bytes)
	VERTEX_FORMAT_P3F_N_C4B         = 8,  // (28 bytes)
	VERTEX_FORMAT_P3F_N_T2F         = 9,  // (32 bytes)
	VERTEX_FORMAT_P3F_N_C4B_T2F     = 10, // (36 bytes)
	VERTEX_FORMAT_P3F_N_C4B_C4B     = 11, // terrain with detail layers (32 bytes)
	VERTEX_FORMAT_P3F_C4B_C4B_T2F   = 12, // usually plants (28 bytes)
	VERTEX_FORMAT_P3F_N_C4B_C4B_T2F = 13, // usually plants (40 bytes)
	VERTEX_FORMAT_T3F_B3F_N3F       = 14, // tangent space (36 bytes)
	VERTEX_FORMAT_T2F               = 15, // light maps TC (8 bytes)
	VERTEX_FORMAT_P3F_C4B_T2F_T2F   = 16, // used for multitextured drawing
	VERTEX_FORMAT_NUMS              = 17, // number of vertex formats
};

//////////////////////////////////////////////////////////////////////////
_inline int VertFormatForComponents(bool bNeedCol, bool bNeedSecCol, bool bNeedNormals, bool bHasTC)
{
	int RequestedVertFormat;

	if (!bNeedCol && !bHasTC && !bNeedNormals)
		RequestedVertFormat = VERTEX_FORMAT_P3F;
	else if (!bNeedCol && !bHasTC && bNeedNormals)
		RequestedVertFormat = VERTEX_FORMAT_P3F_N;
	else if (bNeedCol && bNeedSecCol && bNeedNormals && !bHasTC)
		RequestedVertFormat = VERTEX_FORMAT_P3F_N_C4B_C4B;
	else if (bNeedCol && bNeedSecCol && bNeedNormals && bHasTC)
		RequestedVertFormat = VERTEX_FORMAT_P3F_N_C4B_C4B_T2F;
	else if (bNeedCol && bNeedSecCol && !bNeedNormals && bHasTC)
		RequestedVertFormat = VERTEX_FORMAT_P3F_C4B_C4B_T2F;
	else if (bNeedCol && bNeedSecCol && !bHasTC)
		RequestedVertFormat = VERTEX_FORMAT_P3F_C4B_C4B;
	else if (bNeedCol && !bHasTC && !bNeedNormals)
		RequestedVertFormat = VERTEX_FORMAT_P3F_C4B;
	else if (bNeedCol && !bHasTC && bNeedNormals)
		RequestedVertFormat = VERTEX_FORMAT_P3F_N_C4B;
	else if (!bNeedCol && bHasTC && !bNeedNormals)
		RequestedVertFormat = VERTEX_FORMAT_P3F_T2F;
	else if (bNeedCol && bHasTC && !bNeedNormals)
		RequestedVertFormat = VERTEX_FORMAT_P3F_C4B_T2F;
	else if (!bNeedCol && bHasTC && bNeedNormals)
		RequestedVertFormat = VERTEX_FORMAT_P3F_N_T2F;
	else
		RequestedVertFormat = VERTEX_FORMAT_P3F_N_C4B_T2F;

	return RequestedVertFormat;
}

//////////////////////////////////////////////////////////////////////////
struct SBufInfoTable
{
	INT_PTR OffsTC;
	INT_PTR OffsColor;
	INT_PTR OffsSecColor;
	INT_PTR OffsNormal;
};
//////////////////////////////////////////////////////////////////////
struct SVertBufComps
{
	bool m_bHasTC;
	bool m_bHasColors;
	bool m_bHasSecColors;
	bool m_bHasNormals;
};

union UCol
{
	union
	{
		uint32 dcolor;
		uint8  bcolor[4];

		struct
		{
			uint8 b, g, r, a;
		};
		struct
		{
			uint8 z, y, x, w;
		};
	};
	/*
	UCol& operator=(Legacy::Vec4& v)
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
	UCol(const Legacy::Vec4& v)
	{
		bcolor[0] = static_cast<char>(v[3] * 255.f);
		bcolor[1] = static_cast<char>(v[0] * 255.f);
		bcolor[2] = static_cast<char>(v[1] * 255.f);
		bcolor[3] = static_cast<char>(v[2] * 255.f);
	}
	UCol(const Legacy::Vec3& v)
	    : UCol(Legacy::Vec4(v, 1))
	{
	}
	UCol(const uint8 v[4])
	    : bcolor{v[0], v[1], v[2], v[3]}
	{
	}
	UCol(uint8 v0, uint8 v1, uint8 v2, uint8 v3)
	    : bcolor{v0, v1, v2, v3}
	{
	}
};

//////////////////////////////////////////////////////////////////////////
struct SVF_P3F // 12 bytes
{
	Legacy::Vec3 xyz;
};
struct SVF_P3F_C4B
{
	Legacy::Vec3 xyz;
	UCol         color;
};
struct SVF_P3F_N // 24 bytes
{
	Legacy::Vec3 xyz;
	Legacy::Vec3 normal;
};
struct SVF_T2F // 8 bytes
{
	Legacy::Vec2 st;
};

struct SVF_P3F_N_C4B
{
	Legacy::Vec3 xyz;
	Legacy::Vec3 normal;
	UCol         color;
};

struct SVF_P3F_T2F
{
	Legacy::Vec3 xyz;
	Legacy::Vec2 st;
};
struct SVF_P3F_N_T2F
{
	Legacy::Vec3 xyz;
	Legacy::Vec3 normal;
	Legacy::Vec2 st;
};

struct SVF_P3F_C4B_T2F
{
	Legacy::Vec3 xyz;
	UCol         color;
	Legacy::Vec2 st;
};
struct SVF_P3F_C4B_C4B_T2F
{
	Legacy::Vec3 xyz;
	UCol         color;
	UCol         seccolor;
	Legacy::Vec2 st;
};
struct SVF_P3F_N_C4B_C4B_T2F
{
	Legacy::Vec3 xyz;
	Legacy::Vec3 normal;
	UCol         color;
	UCol         seccolor;
	Legacy::Vec2 st;
};
struct SVF_P3F_C4B_T2F_T2F
{
	Legacy::Vec3 xyz;
	UCol         color;
	Legacy::Vec2 st0[2];
	Legacy::Vec2 st1[2];
};

struct SVF_P3F_N_C4B_T2F
{
	Legacy::Vec3 xyz;
	Legacy::Vec3 normal;
	UCol         color;
	Legacy::Vec2 st;

	bool         operator==(SVF_P3F_N_C4B_T2F& other);
};

struct SVF_TRP3F_C4B_T2F
{
	float        x, y, z, rhw;
	UCol         color;
	Legacy::Vec2 st;
};

struct SVF_P3F_C4B_C4B
{
	Legacy::Vec3 xyz;
	UCol         color;
	UCol         seccolor;
};

struct SVF_P3F_N_C4B_C4B
{
	Legacy::Vec3 xyz;
	Legacy::Vec3 normal;
	UCol         color;
	UCol         seccolor;
};

struct SPipTangents
{
	Legacy::Vec3 m_Tangent;
	Legacy::Vec3 m_Binormal;
	Legacy::Vec3 m_TNormal;
};

//////////////////////////////////////////////////////////////////////////
_inline void* CreateVertexBuffer(int nFormat, int nVerts)
{
	switch (nFormat)
	{
	case VERTEX_FORMAT_P3F:
		return new SVF_P3F[nVerts];

	case VERTEX_FORMAT_P3F_N:
		return new SVF_P3F_N[nVerts];

	case VERTEX_FORMAT_P3F_C4B_T2F:
		return new SVF_P3F_C4B_T2F[nVerts];

	case VERTEX_FORMAT_P3F_C4B_C4B_T2F:
		return new SVF_P3F_C4B_C4B_T2F[nVerts];

	case VERTEX_FORMAT_P3F_N_C4B_C4B_T2F:
		return new SVF_P3F_N_C4B_C4B_T2F[nVerts];

	case VERTEX_FORMAT_P3F_C4B_T2F_T2F:
		return new SVF_P3F_C4B_T2F_T2F[nVerts];

	case VERTEX_FORMAT_P3F_N_C4B_T2F:
		return new SVF_P3F_N_C4B_T2F[nVerts];

	case VERTEX_FORMAT_P3F_T2F:
		return new SVF_P3F_T2F[nVerts];

	case VERTEX_FORMAT_P3F_N_T2F:
		return new SVF_P3F_N_T2F[nVerts];

	case VERTEX_FORMAT_P3F_C4B:
		return new SVF_P3F_C4B[nVerts];

	case VERTEX_FORMAT_P3F_C4B_C4B:
		return new SVF_P3F_C4B_C4B[nVerts];

	case VERTEX_FORMAT_P3F_N_C4B_C4B:
		return new SVF_P3F_N_C4B_C4B[nVerts];

	case VERTEX_FORMAT_P3F_N_C4B:
		return new SVF_P3F_N_C4B[nVerts];

	case VERTEX_FORMAT_TRP3F_C4B_T2F:
		return new SVF_TRP3F_C4B_T2F[nVerts];

	case VERTEX_FORMAT_T2F:
		return new SVF_T2F[nVerts];

	default:
		assert(0);
	}
	return NULL;
}

#ifdef WIN64
	// we don't care about truncation of the struct member offset, because
	// it's a very small integer (even fits into a signed byte)
	#pragma warning(push)
	#pragma warning(disable : 4311)
	#pragma warning(disable : 4302)
#endif

//////////////////////////////////////////////////////////////////////////
// Vertex Sizes
const int gVertexSize[] =
    {
        0,
        sizeof(SVF_P3F),
        sizeof(SVF_P3F_C4B),
        sizeof(SVF_P3F_T2F),
        sizeof(SVF_P3F_C4B_T2F),
        sizeof(SVF_TRP3F_C4B_T2F),
        sizeof(SVF_P3F_C4B_C4B),
        sizeof(SVF_P3F_N),
        sizeof(SVF_P3F_N_C4B),
        sizeof(SVF_P3F_N_T2F),
        sizeof(SVF_P3F_N_C4B_T2F),
        sizeof(SVF_P3F_N_C4B_C4B),
        sizeof(SVF_P3F_C4B_C4B_T2F),
        sizeof(SVF_P3F_N_C4B_C4B_T2F),
        sizeof(SPipTangents),
        sizeof(SVF_T2F),
        sizeof(SVF_P3F_C4B_T2F_T2F),
};

//////////////////////////////////////////////////////////////////////////
// this is the table of offsets of UVs relative to the start of the structure
// -1 means there's no UVs in this format
// This is required by Animation and must be kept intact with the vertex format enumeration
const INT_PTR g_VertFormatUVOffsets[] =
    {
        -1,                                               // no UVs in this format - invalid format
        -1,                                               // VERTEX_FORMAT_P3F=1,                // shadow volumes (12 bytes)
        -1,                                               // VERTEX_FORMAT_P3F_C4B=2,         // usually terrain (16 bytes)
        (INT_PTR) & (((SVF_P3F_T2F*)0)->st[0]),           // VERTEX_FORMAT_P3F_T2F=3,          // everything else (20 bytes)
        (INT_PTR) & (((SVF_P3F_C4B_T2F*)0)->st[0]),       // VERTEX_FORMAT_P3F_C4B_T2F=4,   // usually plants (24 bytes)
        (INT_PTR) & (((SVF_TRP3F_C4B_T2F*)0)->st[0]),     // VERTEX_FORMAT_TRP3F_C4B_T2F=5, // fonts (28 bytes)
        -1,                                               // VERTEX_FORMAT_P3F_C4B_C4B=1,
        -1,                                               // VERTEX_FORMAT_P3F_N=1,
        -1,                                               // VERTEX_FORMAT_P3F_N_C4B=1,
        (INT_PTR) & (((SVF_P3F_N_T2F*)0)->st[0]),         // VERTEX_FORMAT_P3F_N_T2F=3,          // everything else (20 bytes)
        (INT_PTR) & (((SVF_P3F_N_C4B_T2F*)0)->st[0])      // VERTEX_FORMAT_P3F_N_C4B_T2F=4,   // usually plants (24 bytes)
                        - 1,                              // VERTEX_FORMAT_P3F_N_C4B_C4B=1,
        (INT_PTR) & (((SVF_P3F_C4B_C4B_T2F*)0)->st[0]),   // VERTEX_FORMAT_P3F_C4B_C4B_T2F=4,   // usually plants (24 bytes)
        (INT_PTR) & (((SVF_P3F_N_C4B_C4B_T2F*)0)->st[0]), // VERTEX_FORMAT_P3F_N_C4B_C4B_T2F=4,   // usually plants (24 bytes)
};

//////////////////////////////////////////////////////////////////////////
// this is the table of offsets of colors relative to the start of the structure
// -1 means there's no colors in this format
// This is required by Animation and must be kept in tact with the vertex format enumeration
const INT_PTR g_VertFormatRGBAOffsets[] =
    {
        -1, // invalid format
        -1,
        (INT_PTR) & (((SVF_P3F_C4B*)0)->color.dcolor),
        -1,
        (INT_PTR) & (((SVF_P3F_C4B_T2F*)0)->color.dcolor),
        (INT_PTR) & (((SVF_TRP3F_C4B_T2F*)0)->color.dcolor),
        (INT_PTR) & (((SVF_P3F_C4B_C4B*)0)->color.dcolor),
        -1,
        (INT_PTR) & (((SVF_P3F_N_C4B*)0)->color.dcolor),
        -1,
        (INT_PTR) & (((SVF_P3F_N_C4B_T2F*)0)->color.dcolor),
        (INT_PTR) & (((SVF_P3F_N_C4B_C4B*)0)->color.dcolor),
        (INT_PTR) & (((SVF_P3F_C4B_C4B_T2F*)0)->color.dcolor),
        (INT_PTR) & (((SVF_P3F_N_C4B_C4B_T2F*)0)->color.dcolor),
};

//////////////////////////////////////////////////////////////////////////
// this is the table of offsets of normals relative to the start of the structure
// -1 means there's no colors in this format
// This is required by Animation and must be kept in tact with the vertex format enumeration
const INT_PTR g_VertFormatNormalOffsets[] =
    {
        -1,                                            // invalid format
        -1,                                            // VERTEX_FORMAT_P3F=1
        -1,                                            // VERTEX_FORMAT_P3F_C4B=2,
        -1,                                            // VERTEX_FORMAT_P3F_T2F=3,
        -1,                                            // VERTEX_FORMAT_P3F_C4B_T2F=4,
        -1,                                            // VERTEX_FORMAT_TRP3F_C4B_T2F=5,
        -1,                                            // VERTEX_FORMAT_P3F_C4B_C4B,
        (INT_PTR) & (((SVF_P3F_N*)0)->normal),         // VERTEX_FORMAT_P3F_N=1,
        (INT_PTR) & (((SVF_P3F_N_C4B*)0)->normal),     // VERTEX_FORMAT_P3F_N_C4B=1,
        (INT_PTR) & (((SVF_P3F_N_T2F*)0)->normal),     // VERTEX_FORMAT_P3F_N_T2F=3,          // everything else (20 bytes)
        (INT_PTR) & (((SVF_P3F_N_C4B_T2F*)0)->normal), // VERTEX_FORMAT_P3F_N_C4B_T2F=4,   // usually plants (24 bytes)
        (INT_PTR) & (((SVF_P3F_N_C4B_C4B*)0)->normal),
        -1,
        (INT_PTR) & (((SVF_P3F_N_C4B_C4B_T2F*)0)->normal),
};

static struct SBufInfoTable gBufInfoTable[] =
    {
        {0},
        {//VERTEX_FORMAT_P3F
#define OOFS(x) (INT_PTR) & (((SVF_P3F*)0)->x)
         0
#undef OOFS
        },
        {
//VERTEX_FORMAT_P3F_C4B
#define OOFS(x) (INT_PTR) & (((SVF_P3F_C4B*)0)->x)
            0,
            OOFS(color.dcolor),
#undef OOFS
        },
        {//VERTEX_FORMAT_P3F_T2F
#define OOFS(x) (INT_PTR) & (((SVF_P3F_T2F*)0)->x)
         OOFS(st[0])
#undef OOFS
        },
        {//VERTEX_FORMAT_P3F_C4B_T2F
#define OOFS(x) (INT_PTR) & (((SVF_P3F_C4B_T2F*)0)->x)
         OOFS(st[0]),
         OOFS(color.dcolor)
#undef OOFS
        },
        {
//VERTEX_FORMAT_TRP3F_C4B_T2F
#define OOFS(x) (INT_PTR) & (((SVF_TRP3F_C4B_T2F*)0)->x)
            OOFS(st[0]),
            OOFS(color.dcolor),
#undef OOFS
        },
        {
//VERTEX_FORMAT_P3F_C4B_C4B
#define OOFS(x) (INT_PTR) & (((SVF_P3F_C4B_C4B*)0)->x)
            0,
            OOFS(color.dcolor),
            OOFS(seccolor.dcolor),
#undef OOFS
        },
        {
//VERTEX_FORMAT_P3F_N
#define OOFS(x) (INT_PTR) & (((SVF_P3F_N*)0)->x)
            0,
            0,
            0,
            OOFS(normal.x),
#undef OOFS
        },
        {
//VERTEX_FORMAT_P3F_N_C4B
#define OOFS(x) (INT_PTR) & (((SVF_P3F_N_C4B*)0)->x)
            0,
            OOFS(color.dcolor),
            0,
            OOFS(normal.x),
#undef OOFS
        },
        {
//VERTEX_FORMAT_P3F_N_T2F
#define OOFS(x) (INT_PTR) & (((SVF_P3F_N_T2F*)0)->x)
            OOFS(st[0]),
            0,
            0,
            OOFS(normal.x),
#undef OOFS
        },
        {
//VERTEX_FORMAT_P3F_N_C4B_T2F
#define OOFS(x) (INT_PTR) & (((SVF_P3F_N_C4B_T2F*)0)->x)
            OOFS(st[0]),
            OOFS(color.dcolor),
            0,
            OOFS(normal.x),
#undef OOFS
        },
        {
//VERTEX_FORMAT_P3F_N_C4B_C4B
#define OOFS(x) (INT_PTR) & (((SVF_P3F_N_C4B_C4B*)0)->x)
            0,
            OOFS(color.dcolor),
            OOFS(seccolor.dcolor),
            OOFS(normal.x),
#undef OOFS
        },
        {
//VERTEX_FORMAT_P3F_C4B_C4B_T2F
#define OOFS(x) (INT_PTR) & (((SVF_P3F_C4B_C4B_T2F*)0)->x)
            OOFS(st[0]),
            OOFS(color.dcolor),
            OOFS(seccolor.dcolor),
            0,
#undef OOFS
        },
        {
//VERTEX_FORMAT_P3F_N_C4B_C4B_T2F
#define OOFS(x) (INT_PTR) & (((SVF_P3F_N_C4B_C4B_T2F*)0)->x)
            OOFS(st[0]),
            OOFS(color.dcolor),
            OOFS(seccolor.dcolor),
            OOFS(normal.x),
#undef OOFS
        },
};

#ifdef WIN64
	#pragma warning(pop)
#endif

_inline void GetVertBufComps(SVertBufComps* Comps, int Format)
{
	memset(Comps, 0, sizeof(SVertBufComps));
	if (gBufInfoTable[Format].OffsTC)
		Comps->m_bHasTC = true;
	if (gBufInfoTable[Format].OffsColor)
		Comps->m_bHasColors = true;
	if (gBufInfoTable[Format].OffsSecColor)
		Comps->m_bHasSecColors = true;
	if (gBufInfoTable[Format].OffsNormal)
		Comps->m_bHasNormals = true;
}

typedef SVF_P3F_C4B_T2F SAuxVertex;

class DynVertexFormat
{
public:
	enum DataType
	{
		Float,
		UInt,
		SInt,
		FVec2,
		UVec2,
		SVec2,
		FVec3,
		UVec3,
		SVec3,
		FVec4,
		UVec4,
		SVec4,
	};

	static size_t typeSize(DataType type)
	{
		switch (type)
		{
		case Float:
			return sizeof(float);
		case UInt:
			return sizeof(glm::uint32);
		case SInt:
			return sizeof(glm::int32);
		case FVec2:
			return sizeof(glm::vec2);
		case UVec2:
			return sizeof(glm::uvec2);
		case SVec2:
			return sizeof(glm::ivec2);
		case FVec3:
			return sizeof(glm::vec3);
		case UVec3:
			return sizeof(glm::uvec3);
		case SVec3:
			return sizeof(glm::ivec3);
		case FVec4:
			return sizeof(glm::vec4);
		case UVec4:
			return sizeof(glm::uvec4);
		case SVec4:
			return sizeof(glm::ivec4);
		default:
			assert(0);
			return 0;
		}
	}

	void add(std::string_view name, DataType type)
	{
		members.push_back(FieldInfo{std::hash<std::string_view>()(name), size, type});
		size += typeSize(type);
	}

	void* get(std::string_view name, void* pData, size_t index = 0)
	{
		if (auto f = findField(name); f != nullptr)
		{
			return (byte*)pData + size * index + f->offset;
		}
		return nullptr;
	}

	template<typename FnT>
	bool apply(std::string_view name, void* pData, FnT f)
	{
		if (auto f = findField(name); f != nullptr)
		{
			DataType type = f->type;
			auto     mem  = (byte*)pData + f->offset;

			switch (type)
			{
			case Float:
				return iterateField<float>(f, mem);
			case UInt:
				return iterateField<glm::uint32>(f, mem);
			case SInt:
				return iterateField<glm::int32>(f, mem);
			case FVec2:
				return iterateField<glm::vec2>(f, mem);
			case UVec2:
				return iterateField<glm::uvec2>(f, mem);
			case SVec2:
				return iterateField<glm::ivec2>(f, mem);
			case FVec3:
				return iterateField<glm::vec3>(f, mem);
			case UVec3:
				return iterateField<glm::uvec3>(f, mem);
			case SVec3:
				return iterateField<glm::ivec3>(f, mem);
			case FVec4:
				return iterateField<glm::vec4>(f, mem);
			case UVec4:
				return iterateField<glm::uvec4>(f, mem);
			case SVec4:
				return iterateField<glm::ivec4>(f, mem);
			}
		}
		return false;
	}

private:
	template<typename T, typename F>
	bool iterateField(F f, byte* data)
	{
		for (size_t offset = 0; offset < size; offset += size)
		{
			f(reinterpret_cast<T*>(data + offset, offset / size));
		}
		return true;
	}

	struct FieldInfo
	{
		unsigned long long id;
		size_t             offset;
		DataType           type;
	};

	FieldInfo* findField(std::string_view name)
	{
		auto id = std::hash<std::string_view>()(name);
		for (auto f : members)
		{
			if (f.id == id)
			{
				return &f;
			}
		}
		return nullptr;
	}

	std::vector<FieldInfo> members;
	size_t                 size = 0;
};

#endif
