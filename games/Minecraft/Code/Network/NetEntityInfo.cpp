
//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code 
//	Copyright (c) Crytek 2001-2004
//
//  File: NetEntityInfo.cpp
//  Description: Network entity info.
//
//  History:
//  - Created by Alberto Demichelis and Martin Mittring
//	- February 2005: Modified by Marco Corbetta for SDK release
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NetEntityInfo.h"
#include "XPlayer.h"
#include "XVehicle.h"
#include "XSystemBase.h"
#include <ITimer.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
CNetEntityInfo::CNetEntityInfo()
{
	m_pEntity=0;
	m_fLastUpdate=0;
	m_pTimer=NULL;
	m_nScore=0;
	m_cState=0;
	m_dwBitSizeEstimate=40;		// initial guessed value
}

//////////////////////////////////////////////////////////////////////
CNetEntityInfo::CNetEntityInfo(const CNetEntityInfo& nei)
{
	m_pEntity=nei.m_pEntity;
	m_fLastUpdate=nei.m_fLastUpdate;
	m_pTimer=nei.m_pTimer;
	m_nPriority=nei.m_nPriority;
	m_nScore=nei.m_nScore;
	m_cState=nei.m_cState;
	m_ecsClone=nei.m_ecsClone;
	m_dwBitSizeEstimate=nei.m_dwBitSizeEstimate;
}

//////////////////////////////////////////////////////////////////////
CNetEntityInfo::CNetEntityInfo(CXServerSlot *pServerSlot,ITimer *pTimer,IEntity* pEntity)
{
	m_pEntity=pEntity;
	m_fLastUpdate=0;
	m_pTimer=pTimer;
	m_nScore=0;
	m_cState=0;
	#if 0
	m_ecsClone.m_v3Angles.Set(1E10f,1E10f,1E10f);	
	#endif
	m_ecsClone.m_pServerSlot=pServerSlot;
	m_dwBitSizeEstimate=40;		// iniital guessed value
}

//////////////////////////////////////////////////////////////////////
CNetEntityInfo::~CNetEntityInfo() 
{
}

//////////////////////////////////////////////////////////////////////
bool CNetEntityInfo::Write( CXServer *pServer, CStream &stm)
{
	assert(pServer);

	bool bRes;
	m_ecsClone.m_fWriteStepBack = GetXServerSlot()->GetPlayerWriteStepBack();
	m_ecsClone.m_bOffSync = GetXServerSlot()->IsEntityOffSync(m_pEntity->GetId());

	size_t dwPos = stm.GetSize();

	bRes=m_pEntity->Write(stm,&m_ecsClone);

	m_dwBitSizeEstimate = 5+9 + (uint32)(stm.GetSize()-dwPos);	// 5 for XSERVERMSG_UPDATEENTITY, 9 for EntityId

	return bRes;
}

//////////////////////////////////////////////////////////////////////
void CNetEntityInfo::Reset()
{
	m_fLastUpdate=m_pTimer->GetCurrTime();

	CXServer *pServer = GetXServerSlot()->GetServer();

	if(pServer->m_pGame->GetMyPlayer())
	if((pServer->sv_netstats->GetIVal() > 1) &&
		(pServer->GetServerSlotByEntityId(pServer->m_pGame->GetMyPlayer()->GetId()) == GetXServerSlot()))
	{
		int i;

		if(GetISystem()->GetIEntitySystem()->IsDynamicEntityId(m_pEntity->GetId()))
			i=(int)(0xffff-m_pEntity->GetId());
			else
			i=1000-(int)(m_pEntity->GetId());

		if(i>=1000)i=1000-1;		// clamp in max range
		if(i<0)i=0;							// clamp in min range

		GetXServerSlot()->GetServer()->m_NetStats.AddToSumGraph(i, 1);
	}
}

//////////////////////////////////////////////////////////////////////////
uint32 CNetEntityInfo::CalcEstimatedSize()
{
	return m_dwBitSizeEstimate;
}

//////////////////////////////////////////////////////////////////////////
void CNetEntityInfo::Update(Legacy::Vec3 v3dViewer)
{
	FUNCTION_PROFILER( PROFILE_GAME );

	CXServer *pServer = GetXServerSlot()->GetServer();

	IBitStream *pBitStream = pServer->m_pGame->GetIBitStream();			// compression helper

	m_nPriority=1;								// lowest update level
	
	if(!m_pEntity)
	{
		assert(m_pEntity);
		m_nPriority=0;							// no update at all
		return;
	}

	//////////////////////////////////////////////////////////////////////
	unsigned char cState=(unsigned char)m_pEntity->GetStateIdx();

	if(!m_pEntity->IsStateClientside()			// state changes need sync
		&& m_cState!=cState)										// state has changed
	{
		// send it to the client
		CStream stm;
		//if there were no previous state - this is the first time state is set 
		bool	bFirstTime = (m_cState==0);
		m_cState=cState;
		WRITE_COOKIE(stm);
		pBitStream->WriteBitStream(stm,(EntityId)m_pEntity->GetId(),eEntityId);
		//		stm.Write((EntityId)m_pEntity->GetId());
		ASSERT(m_pEntity->IsInState(cState));
		stm.Write(cState);
		WRITE_COOKIE(stm);
		GetXServerSlot()->SendReliableMsg(XSERVERMSG_SETENTITYSTATE,stm,false,m_pEntity->GetName());
		//we need to know if this is the first time - then make it execute OnBind on client 
		if(bFirstTime)
			m_pEntity->SinkRebind( GetXServerSlot()->GetServer() );
	}

	if(m_pEntity->IsGarbage())
	{
		m_nPriority=0;							// no update at all
		return;
	}

	/////////////////////////////////////////////////
	if(!m_pEntity->GetNetPresence())
	{
		m_nPriority=0;							// no update at all
		return;
	}

	bool bLocalPlayer = GetXServerSlot()->GetPlayerId()==m_pEntity->GetId();

	/////////////////////////////////////////////////
	if(bLocalPlayer)
		m_nPriority+=1000000;


	IEntityContainer *pC=m_pEntity->GetContainer();

	m_ecsClone.m_bSyncAngles = true;
	m_ecsClone.m_bSyncYAngle = true;
	m_ecsClone.m_bLocalplayer = bLocalPlayer;			// different from earlier method (was only true for a Player, not for spectators)

	auto v3This=m_pEntity->GetPos();

	// lower priority for more distant entities
	{
		float fDistance2=0;

		#if 0
		if(!IsEquivalent(v3dViewer,v3This))
			fDistance2 = (v3dViewer-(const Legacy::Vec3)v3This).len2();
		#endif

		float fVisibleRadius2=500*500;

		if(fDistance2>fVisibleRadius2)
			m_nPriority=100;		// almost no udate
		else
			m_nPriority+=(unsigned int)cry_sqrtf(fVisibleRadius2-fDistance2);
	}

	// container gets the change to change the m_nPriority
	if(pC)
		pC->OnEntityNetworkUpdate(GetXServerSlot()->GetPlayerId(),v3dViewer,m_nPriority,m_ecsClone);

	if(!m_nPriority)
		return;								// no update at all

	CPlayer *pPlayer=0;

	if(pC)
		pC->QueryContainerInterface(CIT_IPLAYER,(void **)&pPlayer);

	if(pPlayer)
	{
		//if is the slot's player
		if(bLocalPlayer)
		{
			if(pPlayer->m_stats.score!=m_nScore) 
			{
				CStream stm;

				m_nScore=pPlayer->m_stats.score;
				WRITE_COOKIE(stm);
				stm.Write(m_pEntity->GetId());
				stm.Write(m_nScore);
				WRITE_COOKIE(stm);
				GetXServerSlot()->SendReliableMsg(XSERVERMSG_SETPLAYERSCORE,stm,true);
			}
		}
	}

	if(m_nUpdateNumber==0)
	{
		m_nPriority=0x0000FFFF;
		m_nUpdateNumber++;
		return;
	}
	
	m_nUpdateNumber++;
}

//////////////////////////////////////////////////////////////////////////
float CNetEntityInfo::GetTimeAffectedPriority()
{
	float fCurrentTime = m_pTimer->GetCurrTime();
	float fDeltaTime=fCurrentTime-m_fLastUpdate;

	if(fDeltaTime<0)
	{
		m_fLastUpdate=fCurrentTime;		// timer was reset
		fDeltaTime=0;
	}

	return (float)m_nPriority*fDeltaTime;
}

//////////////////////////////////////////////////////////////////////////
float CNetEntityInfo::GetDistanceTo( const Legacy::Vec3 &vPos )
{
	assert(m_pEntity);	

	if(!m_pEntity)
		return 0;

	auto v3This=m_pEntity->GetPos();

	#if 0
	return (vPos-(const Vec3d)v3This).len2();
	#else
	return 0.f;
	#endif
}

//////////////////////////////////////////////////////////////////////////
bool CNetEntityInfo::operator ==(EntityId id) const
{
	if(!m_pEntity)
		return false;
	if(m_pEntity->GetId()==id)
		return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
CXServerSlot *CNetEntityInfo::GetXServerSlot()
{
	return m_ecsClone.m_pServerSlot;
}