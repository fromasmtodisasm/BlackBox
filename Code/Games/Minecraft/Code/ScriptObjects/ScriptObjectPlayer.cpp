#include "stdafx.h"
#include "ScriptObjects/ScriptObjectPlayer.hpp"
#include "XPlayer.h"


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

void CScriptObjectPlayer::InitializeTemplate(IScriptSystem* pSS)
{
	_ScriptableEx<CScriptObjectPlayer>::InitializeTemplate(pSS);
	m_pTempObj = pSS->CreateObject();
	m_pTempAng = pSS->CreateObject();
	m_pWeaponSlots = pSS->CreateObject();
	m_pTempBloodObj = pSS->CreateObject();
	m_pBlindScreenPos = pSS->CreateObject();
	for (int i = 0; i < SOP_MEMBER_LAST; i++)
	{
		m_memberSO[i] = pSS->CreateObject();
	}

	pSS->SetGlobalValue("BITMASK_PLAYER", BITMASK_PLAYER);
	pSS->SetGlobalValue("BITMASK_WEAPON", BITMASK_WEAPON);
	pSS->SetGlobalValue("BITMASK_OBJECT", BITMASK_OBJECT);

	AllowPropertiesMapping(pSS);
	//PROPERTIES
	RegisterProperty("health", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.health));
	RegisterProperty("max_health", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.maxHealth));
	RegisterProperty("armhealth", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.armHealth));
	RegisterProperty("leghealth", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.legHealth));
	RegisterProperty("armor", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.armor));
	RegisterProperty("max_armor", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.maxArmor));
	RegisterProperty("score", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.score));
	RegisterProperty("deaths", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.deaths));
	RegisterProperty("speedscale", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_stats.fSpeedScale));
	RegisterProperty("firing_grenade", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.firing_grenade));
	RegisterProperty("running", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.running));
	RegisterProperty("jumping", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.jumping));
	RegisterProperty("moving", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.moving));
	RegisterProperty("flying", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.flying));
	RegisterProperty("landing", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.landing));
	RegisterProperty("crouching", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.crouch));
	RegisterProperty("proning", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.prone));
	RegisterProperty("climbing", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.climbing));
	RegisterProperty("use_pressed", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.use_pressed));
	RegisterProperty("first_person", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_bFirstPerson));
	RegisterProperty("ammo_in_clip", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.ammo_in_clip));
	RegisterProperty("ammo", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.ammo));
	RegisterProperty("numofgrenades", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.numofgrenades));
	RegisterProperty("grenadetype", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.grenadetype));
	RegisterProperty("dmgFireAccuracy", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_stats.dmgFireAccuracy));
	RegisterProperty("holding_breath", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.holding_breath));
	RegisterProperty("melee_attack", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.melee_attack));
	RegisterProperty("has_flashlight", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.has_flashlight));
	RegisterProperty("has_binoculars", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.has_binoculars));
	RegisterProperty("AnimationSystemEnabled", PROPERTY_TYPE_INT, offsetof(CPlayer, m_AnimationSystemEnabled));
	RegisterProperty("concentration", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.concentration));
	RegisterProperty("bForceWalk", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.bForceWalk));
	RegisterProperty("model", PROPERTY_TYPE_STRING, offsetof(CPlayer, m_strModel));
	RegisterProperty("reloading", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.reloading));
	RegisterProperty("weapon_busy", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_stats.weapon_busy));
	RegisterProperty("drawfpweapon", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.drawfpweapon));
	RegisterProperty("underwater", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_stats.underwater));
	RegisterProperty("lock_weapon", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.lock_weapon));
	RegisterProperty("firemode", PROPERTY_TYPE_INT, offsetof(CPlayer, m_stats.firemode));
	RegisterProperty("canfire", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.canfire));
	RegisterProperty("accuracy", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_stats.accuracy));
	RegisterProperty("melee_distance", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_stats.melee_distance));
	RegisterProperty("aiming", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.aiming));
	RegisterProperty("fire_rate_busy", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.fire_rate_busy));
	RegisterProperty("vel", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_stats.fVel));
	RegisterProperty("blinding_value", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_stats.curBlindingValue));
	RegisterProperty("firing", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.firing));
	RegisterProperty("stamina", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_StaminaTable.StaminaHUD));
	RegisterProperty("breath", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_StaminaTable.BreathHUD));
	RegisterProperty("modelhidden", PROPERTY_TYPE_BOOL, offsetof(CPlayer, m_stats.bModelHidden));
	RegisterProperty("SafeMntAngZ", PROPERTY_TYPE_FLOAT, offsetof(CPlayer, m_vSafeAngAtMountedWeapon.z));


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

