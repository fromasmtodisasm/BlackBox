
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

#include "ScriptObjectSound.hpp"
enum
{
	SOUND_LOOP       = FLAG_SOUND_LOOP,
	SOUND_MUSIC      = FLAG_SOUND_MUSIC,
	SOUND_UNSCALABLE = FLAG_SOUND_UNSCALABLE
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
	REG_FUNC(CScriptObjectSound, Load3DSound);
	REG_FUNC(CScriptObjectSound, SetSoundVolume);
	REG_FUNC(CScriptObjectSound, SetSoundLoop);
	REG_FUNC(CScriptObjectSound, PlaySound);
	REG_FUNC(CScriptObjectSound, StopSound);
	REG_FUNC(CScriptObjectSound, LoadStreamSound);
	REG_FUNC(CScriptObjectSound, RemoveFromScaleGroup);
}

int CScriptObjectSound::Init(IScriptSystem* pScriptSystem)
{
	InitGlobal(pScriptSystem, "Sound", this);

	m_pScriptSystem->SetGlobalValue("SOUND_LOOP", SOUND_LOOP);
	m_pScriptSystem->SetGlobalValue("SOUND_MUSIC", SOUND_MUSIC);
	m_pScriptSystem->SetGlobalValue("SOUND_UNSCALABLE", SOUND_UNSCALABLE);

	m_pScriptSystem->SetGlobalValue("SOUNDSCALE_MASTER", SOUNDSCALE_MASTER);
	m_pScriptSystem->SetGlobalValue("SOUNDSCALE_SCALEABLE", SOUNDSCALE_SCALEABLE);
	m_pScriptSystem->SetGlobalValue("SOUNDSCALE_DEAFNESS", SOUNDSCALE_DEAFNESS);
	m_pScriptSystem->SetGlobalValue("SOUNDSCALE_UNDERWATER", SOUNDSCALE_UNDERWATER);
	m_pScriptSystem->SetGlobalValue("SOUNDSCALE_MISSIONHINT", SOUNDSCALE_MISSIONHINT);

	return 0;
}

int CScriptObjectSound::LoadSound(IFunctionHandler* pH)
{
	//CHECK_PARAMETERS(2);
	const char* szFileName{};
	int         Flags{0};

	if (pH->GetParam(1, szFileName))
	{
		pH->GetParam(2, Flags);
		auto sound = gEnv->pSoundSystem->LoadSound(szFileName, Flags);
		if (sound)
		{
			USER_DATA pUserData = gEnv->pScriptSystem->CreateUserData((INT_PTR)sound, USER_DATA_SOUND);
			return pH->EndFunction(pUserData);
		}
	}

	return pH->EndFunctionNull();
}

int CScriptObjectSound::Load3DSound(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(5);
	const char* szFileName{};
	int         Flags{0};

	if (pH->GetParam(1, szFileName))
	{
		if (pH->GetParam(2, Flags))
		{
			auto sound = gEnv->pSoundSystem->LoadSound(szFileName, Flags);
			if (sound)
			{
				USER_DATA pUserData = gEnv->pScriptSystem->CreateUserData((int)sound, USER_DATA_SOUND);
				return pH->EndFunction(pUserData);
			}
		}
	}

	return pH->EndFunctionNull();
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
	CHECK_PARAMETERS(1);
	USER_DATA pSound;
	if (pH->GetParam(1, pSound))
	{
		auto Sound = (ISound*)pSound;
		Sound->Play();
	}
	else
	{
		CryError("Passed incorrect sound to PlaySound");
	}
	return pH->EndFunction();
}

int CScriptObjectSound::StopSound(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(1);
	USER_DATA pSound;
	if (pH->GetParam(1, pSound))
	{
		auto Sound = (ISound*)pSound;
		Sound->Stop();
	}
	else
	{
		CryError("Passed incorrect sound to StopSound");
	}
	return pH->EndFunction();
}

int CScriptObjectSound::LoadStreamSound(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(2);
	const char* szFileName{};
	int         Flags{0};

	if (pH->GetParam(1, szFileName))
	{
		if (pH->GetParam(2, Flags))
		{
			auto sound = gEnv->pSoundSystem->LoadSound(szFileName, Flags | FLAG_SOUND_STREAM);
			if (sound)
			{
				USER_DATA pUserData = gEnv->pScriptSystem->CreateUserData((INT_PTR)sound, USER_DATA_SOUND);
				return pH->EndFunction(pUserData);
			}
		}
	}

	return pH->EndFunctionNull();
}

int CScriptObjectSound::IsPlaying(IFunctionHandler* pH)
{
	return pH->EndFunction(true);
}

int CScriptObjectSound::RemoveFromScaleGroup(IFunctionHandler* pH)
{
	return pH->EndFunction();
}
