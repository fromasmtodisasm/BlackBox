
//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code 
//	Copyright (c) Crytek 2001-2004
// 
//	File: ScriptObjectSound.cpp
//
//  Description: 
//		Implementation of the CScriptObjectSound class.
//
//	History: 
//	- File Created by Alberto Demichelis, Martin Mittring
//	- February 2005: Modified by Marco Corbetta for SDK release
//
//////////////////////////////////////////////////////////////////////

#include <BlackBox/ScriptSystem/ScriptObjectSound.hpp>
enum
{
	SOUND_LOOP,
	SOUND_MUSIC,
	SOUND_UNSCALABLE
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
_DECLARE_SCRIPTABLEEX(CScriptObjectSound)

//////////////////////////////////////////////////////////////////////
CScriptObjectSound::CScriptObjectSound()
{
}

//////////////////////////////////////////////////////////////////////
CScriptObjectSound::~CScriptObjectSound()
{
}

//////////////////////////////////////////////////////////////////////
void CScriptObjectSound::InitializeTemplate(IScriptSystem* pSS)
{
	_ScriptableEx<CScriptObjectSound>::InitializeTemplate(pSS);
	REG_FUNC(CScriptObjectSound, LoadSound);
	REG_FUNC(CScriptObjectSound, SetSoundVolume);
	REG_FUNC(CScriptObjectSound, SetSoundLoop);
	REG_FUNC(CScriptObjectSound, PlaySound);
	REG_FUNC(CScriptObjectSound, StopSound);
	REG_FUNC(CScriptObjectSound, LoadStreamSound);
}

int CScriptObjectSound::Init(IScriptSystem* pScriptSystem)
{
	InitGlobal(pScriptSystem,"Sound",this);

	m_pScriptSystem->SetGlobalValue("SOUND_LOOP", SOUND_LOOP );
	m_pScriptSystem->SetGlobalValue("SOUND_MUSIC", SOUND_MUSIC );
	m_pScriptSystem->SetGlobalValue("SOUND_UNSCALABLE", SOUND_UNSCALABLE );

	return 0;
}

int CScriptObjectSound::LoadSound(IFunctionHandler* pH)
{
	return pH->EndFunction();
}

int CScriptObjectSound::SetSoundVolume(IFunctionHandler* pH)
{
	return pH->EndFunction();
}

int CScriptObjectSound::SetSoundLoop(IFunctionHandler* pH)
{
	return pH->EndFunction();
}

int CScriptObjectSound::PlaySound(IFunctionHandler* pH)
{
	return pH->EndFunction();
}

int CScriptObjectSound::StopSound(IFunctionHandler* pH)
{
	return pH->EndFunction();
}

int CScriptObjectSound::LoadStreamSound(IFunctionHandler* pH)
{
	return pH->EndFunction();
}

int CScriptObjectSound::IsPlaying(IFunctionHandler* pH)
{
	return pH->EndFunction();
}

