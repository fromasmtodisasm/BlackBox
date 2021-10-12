
//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code 
//	Copyright (c) Crytek 2001-2004
// 
//	File: ScriptObjectStream.h
//
//  Description: 
//		Interface for the CScriptObjectStream class.
//		This class implements script-functions for exposing the bit stream functionalities
//
//		REMARKS:
//		this object doesn't have a global mapping(is not present as global variable into the script state)
//
//		IMPLEMENTATIONS NOTES:
//		These function will never be called from C-Code. They're script-exclusive.
//
//	History: 
//	- File Created by Alberto Demichelis, Martin Mittring
//	- February 2005: Modified by Marco Corbetta for SDK release
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>

class CStream;

//////////////////////////////////////////////////////////////////////
class CScriptObjectStream :
public _ScriptableEx<CScriptObjectStream>
{
public:
	CScriptObjectStream();
	virtual ~CScriptObjectStream();
	bool Create(IScriptSystem *pScriptSystem);
	void Attach(CStream *pStm)
	{
		m_pStm=pStm;
	}
public:
	int WriteInt(IFunctionHandler *pH);
	int WriteShort(IFunctionHandler *pH);
	int WriteByte(IFunctionHandler *pH);
	int WriteFloat(IFunctionHandler *pH);
	int WriteString(IFunctionHandler *pH);
	int WriteBool(IFunctionHandler *pH);
	int WriteNumberInBits(IFunctionHandler *pH);

	int ReadInt(IFunctionHandler *pH);
	int ReadShort(IFunctionHandler *pH);
	int ReadByte(IFunctionHandler *pH);
	int ReadFloat(IFunctionHandler *pH);
	int ReadString(IFunctionHandler *pH);
	int ReadBool(IFunctionHandler *pH);
	int ReadNumberInBits(IFunctionHandler *pH);
	static void InitializeTemplate(IScriptSystem *pSS);
public:
	CStream *m_pStm;
};

