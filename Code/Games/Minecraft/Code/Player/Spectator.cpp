
//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code 
//	Copyright (c) Crytek 2001-2004
//
//  File: Spectactor.cpp
//  Description: Multiplayer spectator class.
//
//  History:
//	- Created by Alberto Demichelis and Martin Mittring
//	- February 2005: Modified by Marco Corbetta for SDK release
//	- January 2006: MarcoC, added 1st player spectator mode
//	- October 2006: Modified by Marco Corbetta for SDK 1.4 release
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <IEntitySystem.h>
//#include "Game.h"
#include "Game.hpp"
#include "spectator.h"
//#include "XEntityProcessingCmd.h"
#include "XEntityProcessingCmd.hpp"
//#include "XServer.h"
#include "Server/XServer.hpp"
#include "XPlayer.h"
#include "WeaponClass.h"

//////////////////////////////////////////////////////////////////////////
CSpectator::CSpectator(CXGame *pGame) :m_vAngles(0,0,0)
{
	m_pScriptObject=NULL;
	m_pGame=pGame;
	m_eiHost=0;
	m_fLastTargetSwitch=0.0f;

	m_AreaUser.SetGame( pGame );
	m_AreaUser.SetEntity( GetEntity() );
	m_nSpectatorMode=0;
}

//////////////////////////////////////////////////////////////////////////
CSpectator::~CSpectator(void)
{
	m_pGame->m_XAreaMgr.ExitAllAreas( m_AreaUser );	
	// on the server reset only if this is my spectator
	if (m_pGame->m_pServer)
	{
		if (IsMySpectator())
			ResetFirstPersonView();
	}
	else
		ResetFirstPersonView();
}

//////////////////////////////////////////////////////////////////////////
void CSpectator::ResetFirstPersonView()
{ 
	//if (m_pGame->m_pServer)
	{	
		if(m_eiHost!=0)
		{				
			IEntity *pHost= m_pGame->GetSystem()->GetIEntitySystem()->GetEntity(m_eiHost);
			if(pHost)
			{
				IEntityContainer *pHostCnt=pHost->GetContainer();

				if (pHostCnt)
				{
					CPlayer *pPlayerHost;

					if (pHostCnt->QueryContainerInterface(CIT_IPLAYER,(void **) &pPlayerHost))
					{						
						pPlayerHost->m_bFirstPerson=false;								
						pPlayerHost->UpdateCamera();						
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Initialize the spectator-container.
bool CSpectator::Init()
{
	IEntity *entity = GetEntity();

	entity->GetScriptObject()->SetValue("type", "spectator");
	// set camera
	entity->SetCamera(m_pGame->GetSystem()->GetIEntitySystem()->CreateEntityCamera());
	entity->GetCamera()->GetCamera().Init(m_pGame->GetSystem()->GetIRenderer()->GetWidth(),m_pGame->GetSystem()->GetIRenderer()->GetHeight());	
	m_pTimer=m_pGame->GetSystem()->GetITimer();
	entity->SetNeedUpdate(true);

	entity->SetNetPresence(true);			// needed to sync m_eiHost

	m_roll = 0;

	float fMass=100.0f;
	float fHeight=1.2f;
	float fEyeHeight=1.7f;
	float sphere_height=0.6f;
	float radius=0.6f;
	float fGravity=0.0f;

	entity->CreateLivingEntity(fMass,fHeight,fEyeHeight,sphere_height,radius,-1,fGravity,true,true);

	pe_player_dynamics pd;
	pd.bActive = 1;
	pd.kAirControl=1.0f;
	pd.bSwimming=1;
	entity->GetPhysics()->SetParams(&pd);

	{
		pe_params_flags pf;

		pf.flagsAND=~(lef_push_players | lef_push_objects);

		entity->GetPhysics()->SetParams(&pf);
	}

	{
		pe_params_part pp;

		pp.ipart=0;
		pp.flagsAND=0;

		entity->GetPhysics()->SetParams(&pp);
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Update the camera.
void CSpectator::Update()
{
	IEntity *pEntity=GetEntity();		

	//FIXME:!!!
	#if 0
	m_pGame->ConstrainToSandbox(pEntity);			// limit in the world boundaries
	#endif

	if(m_pGame->m_pServer)			// detect if player was removed (not 100% accurate, might switch to different player)
	{
		bool bReset=false;

		IEntity *pHost = m_pGame->GetSystem()->GetIEntitySystem()->GetEntity(m_eiHost);

		if(pHost) 
		{
			IEntityContainer *pICnt=pHost->GetContainer();

			if(pICnt)
			{
				CPlayer *pPlayer;

				if(!pICnt->QueryContainerInterface(CIT_IPLAYER,(void **)&pPlayer))
					bReset=true;
			}
			else bReset=true;
		}
		else bReset=true;

		if (bReset)
		{
			ResetHostId();					// host entity was removed			
			m_nSpectatorMode=0;			// reset to fly mode...			
		}
		else
		{
			// [marco] if the host is there - verify if it is driving a vehicle and force 3rd person
			// spectator mode
			IEntityContainer *pICnt=pHost->GetContainer();
			if (pICnt)
			{
				CPlayer *pPlayer;
				if(pICnt->QueryContainerInterface(CIT_IPLAYER,(void **)&pPlayer))
				{
					if (pPlayer->GetVehicle())
					{
						if (m_nSpectatorMode==2)													
							m_nSpectatorMode=1;						
					}
				}
			}
		}
	} // server

	// only a client has to set the camera
	// [marco] added an additional check for IsMySpectator - since this code
	// is executed on the server too (but the key press is not), and we are 
	// forcing 1st/3rd person mode for the camera, this may cause the server's view
	// to switch to 3rd person if a remote client is spectating
	// a player playing on the server!
	if(m_pGame->IsClient() && IsMySpectator())
	{
		IEntityCamera *pEC;
		Vec3d v3Angles;

		//////////////////////////////////////////////////////////////////////////		
		if(m_eiHost!=0)
		{				
			IEntity *pHost= m_pGame->GetSystem()->GetIEntitySystem()->GetEntity(m_eiHost);
			if(pHost)
			{
				IEntityContainer *pHostCnt=pHost->GetContainer();

				if(pHostCnt)
				{
					CPlayer *pPlayerHost;

					if(pHostCnt->QueryContainerInterface(CIT_IPLAYER,(void **) &pPlayerHost))
					{
						// [marco] switch to the desired spectator mode													
						if (m_pGame->g_first_person_spectator->GetIVal() && (m_nSpectatorMode==2) && (!pPlayerHost->GetVehicle()))
						{						
							// spectator mode 2, 1st person
							//m_pLog->Log("pPlayerHost=%s",pPlayerHost->GetName());
							//pPlayerHost->SetViewMode(false);

							// force first person mode, the above doesnt work
							pPlayerHost->m_bFirstPerson=true;								

							// update camera position
							pPlayerHost->UpdateCamera();

							// update first person view
							bool bdrawfpweapon=pPlayerHost->m_stats.drawfpweapon;
							pPlayerHost->m_stats.drawfpweapon=true;
							pPlayerHost->UpdateFirstPersonView();
							pPlayerHost->m_stats.drawfpweapon=bdrawfpweapon;

							// retrieve and set camera pos
							IEntityCamera *pEntCam=pHost->GetCamera();
							pEC=pEntity->GetCamera();
							if(pEC)
							{
								pEC->SetAngles(pEntCam->GetAngles());
								pEC->SetPos(pEntCam->GetPos());		
							}												
						}
						else
						{
							// spectator mode 1, 3rd person - this 
							// works like before - but make sure to remove 
							// first person mode
							pPlayerHost->m_bFirstPerson=false;								
							pPlayerHost->UpdateCamera();
						}
					}
				}
			}			
			return;
		}
		//////////////////////////////////////////////////////////////////////////


		#if 0
		pEC=pEntity->GetCamera();
		if(pEC)
		{
			pEC->SetAngles(m_vAngles);
			pEC->SetPos(pEntity->GetPos()+Vec3(0,0,0.6f));		// 1.7m eye height
		}
		#endif

		// update areas only for local spectator
		if(IsMySpectator())
		{
			m_AreaUser.SetEntity(GetEntity());
			m_pGame->m_XAreaMgr.UpdatePlayer( m_AreaUser );
		}

	}
}

//////////////////////////////////////////////////////////////////////////
EntityId CSpectator::GetHostId() const
{
	return m_eiHost;
}

//////////////////////////////////////////////////////////////////////////
void CSpectator::ResetHostId()
{
	m_eiHost=0;
}

//////////////////////////////////////////////////////////////////////////
void CSpectator::OnSetAngles( const Vec3d &ang ) {}

//////////////////////////////////////////////////////////////////////////
IScriptObject *CSpectator::GetScriptObject()
{
	return m_pScriptObject;
}

//////////////////////////////////////////////////////////////////////////
void CSpectator::SetScriptObject(IScriptObject *object)
{
	m_pScriptObject=object;
}

//////////////////////////////////////////////////////////////////////////
// Save upcast.
bool CSpectator::QueryContainerInterface(ContainerInterfaceType desired_interface, void **ppInterface )
{
	if (desired_interface == CIT_ISPECTATOR)
	{
		*ppInterface = (void *) this;
		return true;
	}
	else
	{
		*ppInterface = 0;
		return false;
	}		
}

//////////////////////////////////////////////////////////////////////////
void CSpectator::GetEntityDesc( CEntityDesc &desc ) const 
{
}

//////////////////////////////////////////////////////////////////////////
// Process input.
void CSpectator::ProcessKeys(CXEntityProcessingCmd &epc)
{
	if(epc.CheckAction(ACTION_FIRE0))
	{
		if(m_pGame->m_pServer && (m_pTimer->GetCurrTime()-m_fLastTargetSwitch>1))
		{
			// [marco] this is changed on the server only so it needs to be sent (see read/write)
			epc.RemoveAction(ACTION_FIRE0);

			if (m_pGame->g_first_person_spectator->GetIVal())
			{			
				if (m_eiHost)
				{			
					// verify it isnt trying to change when driving a vehicle
					IEntity *pHost = m_pGame->GetSystem()->GetIEntitySystem()->GetEntity(m_eiHost);
					if(pHost) 
					{
						IEntityContainer *pICnt=pHost->GetContainer();
						if(pICnt)
						{
							CPlayer *pPlayer;
							if(pICnt->QueryContainerInterface(CIT_IPLAYER,(void **)&pPlayer))
							{
								if (pPlayer->GetVehicle())
								{
									if (m_nSpectatorMode==1)
									{								
										// we can only change between 3rd and fly mode
										// when the player is driving a vehicle
										m_nSpectatorMode=2;	// setting to 2 will cause below to switch																	
									}
								}
							}
						}
					}
				}

				//m_pGame->m_pLog->Log("SERVER:Spectator mode=%d,host=%d",m_nSpectatorMode,m_eiHost);
				// [marco] when in fly spectator mode (0) or in 1st person spectator mode (2), switch container
				// in mode 1 (3rd person) works like before
				if (m_nSpectatorMode==0 || m_nSpectatorMode==2)
				{									 
					// make sure first person gets always reset. 
					// If needed it gets changed later.

					// avoid to cause the server's view
					// to switch to 3rd person if a remote client is spectating
					// a player playing on the server!
					if(m_pGame->IsClient() && IsMySpectator())
						ResetFirstPersonView();					

					//m_pGame->m_pLog->Log("SERVER:changing=%d,host=%d",m_nSpectatorMode,m_eiHost);
					m_pGame->m_pServer->m_ServerRules.OnSpectatorSwitchModeRequest(m_pEntity);	
				}

				m_nSpectatorMode++;
				if (m_nSpectatorMode>2)
				{
					m_nSpectatorMode=1; // change player , go to 3rd person on the next one
				}												
			}
			else
			{
				m_pGame->m_pServer->m_ServerRules.OnSpectatorSwitchModeRequest(m_pEntity);				
				m_nSpectatorMode=0; // will switch back to fly mode. Reset every time in case there is a sudden cvar change.
			}
			m_fLastTargetSwitch=m_pTimer->GetCurrTime();			
		}
		//if(!m_pGame->m_pServer)
		//	m_pGame->m_pLog->Log("CLIENT:spectatormode=%d,host=%d",m_nSpectatorMode,m_eiHost);
	}

	if(m_eiHost!=0) 
		return;

	// fly-mode
	#if 0
	Vec3d angles=epc.GetDeltaAngles();

	m_vAngles=angles;

	m_vAngles.x = max(m_vAngles.x,-65);
	m_vAngles.x = min(m_vAngles.x,65);

	epc.SetDeltaAngles(m_vAngles);

	Vec3d pos=m_pEntity->GetPos();
	#endif

	int strafe = 0, move = 0;
	if(epc.CheckAction(ACTION_MOVE_FORWARD))  move = 1;
	if(epc.CheckAction(ACTION_MOVE_BACKWARD)) move = -1;
	if(epc.CheckAction(ACTION_MOVE_LEFT))     strafe = -1;
	if(epc.CheckAction(ACTION_MOVE_RIGHT))    strafe = 1;


	Vec3d d;

	d.x = (float)(move*cry_cosf(DEG2RAD(m_vAngles.z-90)));
	d.y = (float)(move*cry_sinf(DEG2RAD(m_vAngles.z-90)));

	d.x *= (float)cry_cosf(DEG2RAD(-m_vAngles.x));
	d.y *= (float)cry_cosf(DEG2RAD(-m_vAngles.x));
	d.z = (float)(move*cry_sinf(DEG2RAD(-m_vAngles.x)));

	d.x += (float)(strafe*cry_cosf(DEG2RAD(m_vAngles.z-180)));
	d.y += (float)(strafe*cry_sinf(DEG2RAD(m_vAngles.z-180)));

	float curtime = m_pGame->m_pSystem->GetITimer()->GetFrameTime();

	if (strafe==0) 
	{
		m_roll = m_roll/(1+(float)cry_sqrtf(curtime*1000.0f)/25);
	}
	else
	{
		m_roll += strafe*curtime/-0.03f;
		float maxroll = 3.0f;
		if(m_roll>maxroll)  m_roll = (float)maxroll;
		if(m_roll<-maxroll) m_roll = (float)-maxroll;
	};

	m_vAngles.y = m_roll;

	// move entity by physics
	if(curtime)
	{
		pe_action_move motion;

		motion.dt=curtime;
		motion.dir=d*12.0f;			// spectator speed is 12x

		m_pEntity->GetPhysics()->Action(&motion);
	}
}

//////////////////////////////////////////////////////////////////////////
bool CSpectator::Write(CStream &stm,EntityCloneState *cs)
{
	stm.Write(m_eiHost);
	// [marco]
	stm.Write(m_nSpectatorMode);
	return (true);
}

//////////////////////////////////////////////////////////////////////////
bool CSpectator::Read(CStream &stm)
{
	// [marco]
	EntityId nHost;
	stm.Read(nHost);
	int nSpectatorMode;
	stm.Read(nSpectatorMode);

	if (m_pGame->g_first_person_spectator->GetIVal())
	{	
		// avoid graphics bugs, make sure to reset
		if ((m_pGame->IsClient() && IsMySpectator()))
		{
			if ((nSpectatorMode!=m_nSpectatorMode) || (nHost==0))
				ResetFirstPersonView();
		}
	}	

	m_eiHost=nHost;
	m_nSpectatorMode=nSpectatorMode;

	return (true);
}



//////////////////////////////////////////////////////////////////////////
bool CSpectator::IsMySpectator() const
{
	if(!m_pGame->m_pClient) return false;
	return (m_pEntity->GetId()==m_pGame->m_pClient->GetPlayerId());
}


//////////////////////////////////////////////////////////////////////////
void CSpectator::OnEntityNetworkUpdate( const EntityId &idViewerEntity, const Vec3d &v3dViewer, uint32 &inoutPriority, 
																			 EntityCloneState &inoutCloneState ) const
{
	bool bLocalPlayer = inoutCloneState.m_bLocalplayer;

	if(!bLocalPlayer)								// spectators are not sent to other player
		inoutPriority=0;							// no update at all
}
