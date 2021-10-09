
//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code 
//	Copyright (c) Crytek 2001-2004
// 
//	File: ScriptObjectStream.h
//
//  Description: 
//		Interface for the CScriptObjectSound class.
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
class CScriptObjectSound :
public _ScriptableEx<CScriptObjectSound>
{
public:
	CScriptObjectSound();
	virtual ~CScriptObjectSound();
public:
	int Init(IScriptSystem *pScriptSystem);
	int LoadSound(IFunctionHandler *pH);
	int SetSoundVolume(IFunctionHandler *pH);
	int SetSoundLoop(IFunctionHandler *pH);
	int PlaySound(IFunctionHandler *pH);
	int StopSound(IFunctionHandler *pH);
	int IsPlaying(IFunctionHandler *pH);
	int LoadStreamSound(IFunctionHandler *pH);
	static void InitializeTemplate(IScriptSystem *pSS);
public:
};


