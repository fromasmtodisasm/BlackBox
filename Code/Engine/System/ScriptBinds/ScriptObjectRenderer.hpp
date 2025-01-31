
//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code
//	Copyright (c) Crytek 2001-2004
//
//	File: ScriptObjectRender.h
//
//  Description:
//		ScriptObjectRender.cpp: Interface for the CScriptObjectRender class.
//
//	History:
//	- File Created by Marco Corbetta
//	- February 2005: Modified by Marco Corbetta for SDK release
//
//////////////////////////////////////////////////////////////////////

#ifndef _SCRIPT_OBJECT_RENDERER_H_
#define _SCRIPT_OBJECT_RENDERER_H_

#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>

#include <vector>

struct IRenderer;
class CVertexBuffer;
#define USER_DATA_SCRIPTOBJRENDERER 0x100

//////////////////////////////////////////////////////////////////////
typedef union tag_Clr
{
	unsigned int  c;
	unsigned char cc[4];
} _Clr;

//////////////////////////////////////////////////////////////////////
typedef struct tag_Vtx
{
	float         x, y, z;
	unsigned char cc[4];
	float         u, v;
} _Vtx;

typedef std::vector<_Vtx>           _VtxVec;
typedef std::vector<unsigned short> _IdxBuf;

//////////////////////////////////////////////////////////////////////
class CScriptObjectRenderer :
    public _ScriptableEx<CScriptObjectRenderer>
{
public:
	CScriptObjectRenderer(void);
	virtual ~CScriptObjectRenderer(void);
	static void    InitializeTemplate(IScriptSystem* pSS);
	IScriptObject* Create(IScriptSystem* pSS, IRenderer* pRen);
	int            Reset(IFunctionHandler* pH);
	int            PushQuad(IFunctionHandler* pH);
	int            Draw(IFunctionHandler* pH);

private:
	_VtxVec    m_vBuffer;
	_IdxBuf    m_vIdxBuf;
	IRenderer* m_pRenderer;
};

#endif
