#include "ScriptObjects/ScriptObjectPlayer.hpp"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_DECLARE_SCRIPTABLEEX(CScriptObjectPlayer)

IScriptObject *CScriptObjectPlayer::m_pTempObj = 0;
IScriptObject *CScriptObjectPlayer::m_pTempAng = 0;
IScriptObject *CScriptObjectPlayer::m_pWeaponSlots = 0;
IScriptObject *CScriptObjectPlayer::m_pTempBloodObj = 0;
IScriptObject *CScriptObjectPlayer::m_pBlindScreenPos = 0;
IScriptObject* CScriptObjectPlayer::m_memberSO[SOP_MEMBER_LAST];

//////////////////////////////////////////////////////////////////////
CScriptObjectPlayer::CScriptObjectPlayer():
m_fSpeedRun(0.0f),
m_fSpeedWalk(0.0f),
m_fSpeedCrouch(0.0f),
m_fSpeedProne(0.0f)
{
	m_LastTouchedMaterialID = -1;
}

//////////////////////////////////////////////////////////////////////
CScriptObjectPlayer::~CScriptObjectPlayer()
{
}

//////////////////////////////////////////////////////////////////////
bool CScriptObjectPlayer::Create(IScriptSystem *pScriptSystem)
{
	Init(pScriptSystem,this);
	
	m_pScriptThis->RegisterParent(this);
	m_pCameraOffset.Create( pScriptSystem );
	m_pGetColor.Create( pScriptSystem );
	m_pWeaponInfo.Create(pScriptSystem);
	return true;
}

//////////////////////////////////////////////////////////////////////
void CScriptObjectPlayer::ReleaseTemplate()
{
	SAFE_RELEASE(m_pTempObj);
	SAFE_RELEASE(m_pTempAng);
	SAFE_RELEASE(m_pWeaponSlots);
	SAFE_RELEASE(m_pTempBloodObj);
	SAFE_RELEASE(m_pBlindScreenPos);
	for (int i = 0; i < SOP_MEMBER_LAST; i++)
	{
		SAFE_RELEASE( m_memberSO[i] );
	}
	_ScriptableEx<CScriptObjectPlayer>::ReleaseTemplate();
}

//////////////////////////////////////////////////////////////////////
void CScriptObjectPlayer::SetPlayer(CPlayer *pPlayer)
{
	m_pPlayer=pPlayer;

	if(!EnablePropertiesMapping(pPlayer))
	{
		CryError( "<CryGame> (CScriptObjectPlayer::SetPlayer) failed" );
		return;
	}
}

