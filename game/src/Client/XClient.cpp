#include <Client/XClient.hpp>

//////////////////////////////////////////////////////////////////////
/** Parameters for cut-scene cameras
*/
struct SCameraParams
{
	SCameraParams()
	{
		//cameraNode = 0;
		nCameraId = 0;
		fFOV	  = 0.0f;
	}
	//IAnimNode* cameraNode;
	unsigned short nCameraId;
	float fFOV;
};

//////////////////////////////////////////////////////////////////////
CXClient::CXClient()
{
	m_bConnected		  = 0;
	m_CameraParams		  = 0;
	m_pScriptObjectClient = 0;
	m_pIActionMapManager  = 0;
	//m_pEntitySystem = 0;
	m_pISystem = 0;
	m_pIClient = 0;
	m_pISystem = 0;
	//m_wPlayerID = INVALID_WID;
	m_pGame = 0;

	cl_explShakeDCoef = 0.07f;
	cl_explShakeAmplH = 0.001f;
	cl_explShakeAmplV = 0.001f;
	cl_explShakeFreq  = 11.73f;
	cl_explShakeTime  = 1.73f;

	m_fFrontSound		= 0;
	m_fBackSound		= 0;
	m_fLeftSound		= 0;
	m_fRightSound		= 0;
	m_pClientStuff		= 0;
	bDoSwitch			= false;
	m_pTimer			= 0;
	m_bSelfDestruct		= false; //  to make sure the client is only released in one place
	m_pSavedConsoleVars = 0;
	m_bLazyChannelState = false; // start with false on client and serverslot side
}

CXClient::~CXClient()
{
}

bool CXClient::Init(CGame* pGame, bool bLocal)
{
	m_fLastClientStringTime = 0;
	m_bDisplayHud			= true;
	m_bMapConnecting		= false;
	m_bRecordingDemo		= false;
	m_bPlaybackDemo			= false;
	//m_wPlayerID = INVALID_WID;
	m_pISystem				= NULL;
	m_bLocalHost			= false;
	m_bLinkListenerToCamera = true;
	m_pGame					= pGame;
	m_pScriptSystem			= m_pGame->GetScriptSystem();
	//m_pEntitySystem = m_pGame->GetSystem()->GetIEntitySystem();
	m_pLog = m_pGame->m_pLog;

	m_sopMsgNormal.Create(m_pScriptSystem);
	m_sopMsgPos.Create(m_pScriptSystem);

	m_pTimer = m_pGame->GetSystem()->GetITimer();
	// Create the client
	m_pIClient = m_pGame->CreateClient(this, bLocal);

	//m_PrevPlayerProcessingCmd = m_PlayerProcessingCmd;

	// Set the first System interface.
	UpdateISystem();

	m_pIActionMapManager = m_pGame->GetActionMapManager();
	if (m_pIActionMapManager)
		m_pIActionMapManager->SetSink(this);

	// Create the console variables
	CreateConsoleVariables();

	m_nGameState			= CGS_INTERMISSION; // until we get first update from the mod
	m_nGameLastTime			= 0;
	m_fGameLastTimeReceived = 0;
	m_pScriptObjectClient	= new CScriptObjectClient;
	m_pScriptObjectClient->Create(pGame->GetScriptSystem(), pGame, this);

	m_CameraParams		 = new SCameraParams;
	m_pClientStuff		 = m_pScriptSystem->CreateEmptyObject();
	m_iPhysicalWorldTime = 0;
	m_bIgnoreSnapshot	 = false;

	return true;
}

void CXClient::OnXConnect()
{
	//sound sources
	m_fFrontSound = 0;
	m_fBackSound  = 0;
	m_fLeftSound  = 0;
	m_fRightSound = 0;
	m_lstSounds.clear();
	m_nDiscardedPackets		   = 0;
	m_fLastRemoteAsyncCurrTime = 0;
	m_fLastScoreBoardTime	   = 0;
	TRACE("CXClient::OnXConnect");
	LoadPlayerDesc();
}

void CXClient::OnXClientDisconnect(const char* szCause)
{
	SmartScriptObject pClientStuff(m_pScriptSystem, true);
	if (m_pScriptSystem->GetGlobalValue("ClientStuff", pClientStuff)) // call ClientStuff:OnShutdown()
	{
		m_pScriptSystem->BeginCall("ClientStuff", "OnShutdown");
		m_pScriptSystem->PushFuncParam(pClientStuff);
		m_pScriptSystem->EndCall();
	}

	TRACE(szCause);
	//SetPlayerID(0);
	// <<FIXME>> Should cleanup the stuff with a new function of the IXSystem interface

	m_bConnected = 0;

	if (m_pISystem)
		void(0); // m_pISystem->Disconnected(szCause);

	m_pScriptSystem->BeginCall("ClientOnDisconnect");
	m_pScriptSystem->PushFuncParam(szCause);
	m_pScriptSystem->EndCall();

	m_pGame->MarkClientForDestruct(); // to make sure the client is only released in one place

	// hide console and reset progress bar after a disconnection
	//GetISystem()->GetIConsole()->ResetProgressBar(0);
	//GetISystem()->GetIConsole()->SetScrollMax(600 / 2);
	GetISystem()->GetIConsole()->ShowConsole(0);
}

void CXClient::OnXContextSetup(CStream& stmContex)
{
#if 0
  GetISystem()->GetILog()->Log("CXClient::OnXContextSetup");

  SetPlayerID(INVALID_WID);
  UpdateISystem();

  m_GameContext.Read(stm);		// Read the sended game context

  if (m_pGame->IsMultiplayer())
  {
    if (m_GameContext.bInternetServer)
      if (!GetISystem()->GetINetwork()->VerifyMultiplayerOverInternet())
        return;
  }

  if (!m_pGame->m_bEditor)
  {
    HSCRIPTFUNCTION pfnOnConnectEstablished = m_pScriptSystem->GetFunctionPtr("Game", "OnConnectEstablished");

    if (pfnOnConnectEstablished)
    {
      m_pScriptSystem->BeginCall(pfnOnConnectEstablished);
      m_pScriptSystem->PushFuncParam(m_pGame->GetScriptObject());
      m_pScriptSystem->EndCall();

      m_pScriptSystem->ReleaseFunc(pfnOnConnectEstablished);
    }
  }

  IGameMods* pModInterface = m_pGame->GetModsInterface();

  assert(pModInterface);		// otherwise the Game::Init failed

  if (stricmp(m_GameContext.strMod.c_str(), pModInterface->GetCurrentMod()) != 0)
  {
    m_pLog->LogError("Wrong Mod: CurrentMod='%s' RequestedMod='%s'", pModInterface->GetCurrentMod(), m_GameContext.strMod.c_str());

    XDisconnect("@GameVersionError");
    return;
  }

  if (!m_GameContext.IsVersionOk())
  {
    m_pLog->LogError("CXClient::OnXContextSetup - Versions do not match.  Server version: %i.%d  Client version: %i.%d",
      m_GameContext.dwNetworkVersion, (int)m_GameContext.ucServerInfoVersion, NETWORK_FORMAT_VERSION, (int)SERVERINFO_FORMAT_VERSION);

    XDisconnect("@GameVersionError");
    return;
  }

  {
    IConsole* pConsole = m_pGame->GetSystem()->GetIConsole();
    static CDefaultStreamAllocator sa;

    RestoreServerSyncedVars();	// restore VF_REQUIRE_NET_SYNC marked console vars

    if (!stm.EOS())
    {
      m_pSavedConsoleVars = new CStream(1024, &sa); // saved console variable state (to restore the VF_REQUIRE_NET_SYNC marked vars)

      string varname, val;
      while (!stm.EOS() && stm.Read(varname))
      {
        stm.Read(val);

        ICVar* pVar = pConsole->GetCVar(varname.c_str());

        if (pVar)
        {
          m_pSavedConsoleVars->Write(varname.c_str());
          m_pSavedConsoleVars->Write(pVar->GetString());
        }

        m_pISystem->SetVariable(varname.c_str(), val.c_str());
      }
    }
  }

  m_pGame->g_GameType->Set(m_GameContext.strGameType.c_str());
  m_Snapshot.Reset();

  m_pLog->Log("CXClient::OnXContextSetup - map : %s\n", m_GameContext.strMapFolder.c_str());

  if (!m_pISystem->LoadLevel(m_GameContext.strMapFolder.c_str(), m_GameContext.strMission.c_str(), false))
  {
    m_pLog->LogError("CXClient::OnXContextSetup ERROR LOADING LEVEL: %s\n", m_GameContext.strMapFolder.c_str());

    LoadingError("@LoadLevelError");

    return;
  }

  if ((!m_pGame->m_bEditor)
    && (!m_pGame->IsServer())
    && (m_pISystem->GetLevelDataCheckSum() != m_GameContext.wLevelDataCheckSum))
  {
    m_pLog->LogError("CXClient::OnXContextSetup ERROR LOADING LEVEL: %s [INVALID CHECKSUM]\n", m_GameContext.strMapFolder.c_str());

    LoadingError("@LevelVersionError");

    return;
  }

  _SmartScriptObject pClientStuff(m_pScriptSystem, true);
  if (m_pScriptSystem->GetGlobalValue("ClientStuff", pClientStuff))				// call ClientStuff:OnShutdown()
  {
    m_pScriptSystem->BeginCall("ClientStuff", "OnShutdown");
    m_pScriptSystem->PushFuncParam(pClientStuff);
    m_pScriptSystem->EndCall();
  }

  if (!m_pGame->m_bDedicatedServer)								// don't load ClientStuff on dedicated server
  {
    if (!m_pGame->ExecuteScript("scripts/$GT$/ClientStuff.lua", true))
    {
      DebugBreak();
    }
    m_pScriptSystem->GetGlobalValue("ClientStuff", m_pClientStuff);
    m_pScriptSystem->BeginCall("ClientStuff", "OnInit");
    m_pScriptSystem->PushFuncParam(m_pClientStuff);
    m_pScriptSystem->EndCall();
  }

  // Write the stream to send to ContextReady
  {
    //m_pLog->Log("Write the stream to send to ContextReady");
    stm.Reset();

    // bLocalHost
    stm.Write(m_bLocalHost);

    // p_name
    m_pLog->Log("p_name=%s", m_pGame->p_name->GetString());
    stm.Write(m_pGame->p_name->GetString());

    // p_model, mp_model
    {
      ICVar* model;

      if (m_pGame->IsMultiplayer())
        model = m_pGame->mp_model;		// multiplayer model
      else
        model = m_pGame->p_model;			// single player model

      if (!model->GetString())
        stm.Write("");
      else
      {
        m_pLog->Log("p_model=%s", model->GetString());
        stm.Write(model->GetString());
      }
    }

    // p_color
    {
      ICVar* color = m_pGame->p_color;			// player's color in non team base multiplayer mods

      if (!color->GetString())
        stm.Write("");
      else
      {
        m_pLog->Log("p_color=%s", color->GetString());
        stm.Write(color->GetString());
      }
    }

    // player classid
    stm.Write(cl_playerclassid->GetIVal());

    m_pLog->Log("SEND ContextReady");
    m_pIClient->ContextReady(stm);
  }

  // fade in when loading a new map
  if (!m_pGame->m_bEditor)
  {
    m_pGame->m_p3DEngine->SetScreenFx("ScreenFade", 1);
    float fFadeTime = -2.5f;
    m_pGame->m_p3DEngine->SetScreenFxParam("ScreenFade", "ScreenFadeTime", &fFadeTime);
    float fPreFade = 5.0f;
    m_pGame->m_p3DEngine->SetScreenFxParam("ScreenFade", "ScreenPreFadeTime", &fPreFade);
  }

  if (!m_pGame->m_bEditor)
  {
    m_pGame->m_pSystem->SetIProcess(m_pGame->m_p3DEngine);
    m_pGame->m_pSystem->GetIProcess()->SetFlags(PROC_3DENGINE);
  }

  m_pGame->m_pUIHud->Reset();

  // We have to tell Ubisoft that the client has successfully connected
  // If ubisoft is not running this won't do anything.
  GetISystem()->GetINetwork()->Client_ReJoinGameServer();

  m_bConnected = 1;

  m_pGame->GetSystem()->SetForceNonDevMode(m_GameContext.bForceNonDevMode);

  // clean up all the sounds that might have been started before the first
  // frame to avoid problems with sloppy/bogus vis areas
  // this calls RecomputeSoundOcclusion

  if (m_pGame->GetSystem()->GetISoundSystem())
    m_pGame->GetSystem()->GetISoundSystem()->Silence();
  if (m_pGame->m_pSystem->GetIMusicSystem())
    m_pGame->m_pSystem->GetIMusicSystem()->Silence();

  if (!m_pGame->m_bIsLoadingLevelFromFile)
  {
    if (m_pGame->IsMultiplayer())
    {
      m_pGame->GotoMenu(true);
    }
    else
    {
      m_pGame->GotoGame(true);
    }
  }

  if (!m_pGame->m_bIsLoadingLevelFromFile)
  {
    //		m_pLog->Log("HIDE CONSOLE");
    m_pGame->GetSystem()->GetIConsole()->ResetProgressBar(0);
    m_pGame->m_pSystem->GetIConsole()->ShowConsole(false);
    m_pGame->m_pSystem->GetIConsole()->SetScrollMax(600 / 2);

    //if (m_pGame->IsMultiplayer())
    m_pGame->GetSystem()->GetIRenderer()->ClearColorBuffer(Vec3(0, 0, 0));
  }
#endif
}

void CXClient::OnXData(CStream& stm)
{
}

void CXClient::OnXServerTimeout()
{
}

void CXClient::OnXServerRessurect()
{
}

unsigned int CXClient::GetTimeoutCompensation()
{
	return 0;
}

void CXClient::MarkForDestruct()
{
}

bool CXClient::DestructIfMarked()
{
	return false;
}

bool CXClient::CreateConsoleVariables()
{
	IConsole* pConsole = m_pGame->GetSystem()->GetIConsole();
	cl_runroll		   = pConsole->CreateVariable("cl_runroll", "0", 0,
										  "\n"
										  "Usage: cl_runroll ?\n"
										  "Default is 0.");
	cl_runpitch		   = pConsole->CreateVariable("cl_runpitch", "0.4", 0,
											  "\n"
											  "Usage: cl_runpitch 0.4\n"
											  "Default is 0.4.");
	cl_runheight	   = pConsole->CreateVariable("cl_runheight", "0.03", 0,
											  "\n"
											  "Usage: cl_runheight 0.03\n"
											  "Default is 0.03.");
	cl_runheightspeed  = pConsole->CreateVariable("cl_runheightspeed", "1.5", 0,
												  "\n"
												  "Usage: cl_runheightspeed 1.5\n"
												  "Default is 1.5.");
	cl_playerclassid   = pConsole->CreateVariable("cl_playerclassid", "1", 0,
												  "Sets the player class.\n"
												  "Usage: cl_playerclassid #\n"
												  "Default is 1.");
	cl_netstats		   = pConsole->CreateVariable("cl_netstats", "0", 0,
											  "Toggles client network statistics.\n"
											  "Usage: cl_netstats [0/1]\n"
											  "Default is 0 (off). Set to 1 to display network statistics.");
	cl_cmdrate		   = pConsole->CreateVariable("cl_cmdrate", "40", 0,
										  "Specify the max client network command rate\n"
										  "(less is better for bandwidth, more is better for response,\n"
										  "the actual rate is limited by frame rate as well)\n"
										  "Usage: cl_cmdrate [5..100]\n"
										  "Default is 40");

	cl_sound_detection_max_distance = pConsole->CreateVariable("cl_sound_detection_max_distance", "50", 0);
	cl_sound_detection_min_distance = pConsole->CreateVariable("cl_sound_detection_min_distance", "2", 0);
	cl_sound_event_radius			= pConsole->CreateVariable("cl_sound_event_radius", "50", 0);
	cl_sound_event_timeout			= pConsole->CreateVariable("cl_sound_event_timeout", "1", 0);

	pConsole->AddCommand("say", "Client:Say(%line)", VF_NULL, "");
	pConsole->AddCommand("sayteam", "Client:SayTeam(%line)", VF_NULL, "");
	pConsole->AddCommand("sayone", "Client:SayOne(%%)", VF_NULL, "");
	pConsole->AddCommand("tell", "Client:SayOne(%%)", VF_NULL, "");
	pConsole->AddCommand("team", "Client:JoinTeamRequest(%%)", 0,
						 "Sends a request to join a team.\n"
						 "Usage: team teamname\n");
	pConsole->AddCommand("ready", "Client:CallVote(\"ready\")", 0,
						 "Asks if other players are ready.\n"
						 "Usage: ready\n"
						 "Works by sending a request to players.\n"
						 "Players respond y or n.\n");
	pConsole->AddCommand("callvote", "Client:CallVote(%%)", 0,
						 "Asks players to vote on a command.\n"
						 "Usage: callvote commandname arg\n"
						 "Sends a request to players to vote on 'commandname arg'.\n"
						 "Players respond y or n.\n");
	pConsole->AddCommand("vote", "Client:Vote(%1)", 0,
						 "Used to vote on suggestions from other players.\n"
						 "Usage: vote [y/n]\n"
						 "Vote y for yes or n for no.");
	pConsole->AddCommand("name", "Client:SetName(%line)", VF_NULL, "");
	pConsole->AddCommand("kill", "Client:Kill()", 0,
						 "Kills the player.\n"
						 "Usage: kill\n"
						 "Player respawns as normal.");
	pConsole->AddCommand("cl_maxrate", "Client:SetBitsPerSecond(%1)", 0,
						 "Sets client maximum download bandwidth\n"
						 "(the actual rate is limited by server setting as well)\n"
						 "Usage: cl_maxrate 28800\n"
						 "Sets bits per second the server is allowed to send to you (this client).");
	pConsole->AddCommand("cl_updaterate", "Client:SetUpdateRate(%1)", 0,
						 "Specify the max server network update rate\n"
						 "(less is better for bandwidth, more is better for response,\n"
						 "the actual rate is limited by frame/update rate and the server setting as well)\n"
						 "Usage: cl_updaterate [5..100]\n"
						 "Default is 20");
	return true;
}

void CXClient::XConnect(const char* szAddr, const bool inbDoLateSwitch, const bool inbCDAuthorization)
{
	m_pIClient->SetServerIP(szAddr);
	//bDoSwitch = _bDoLateSwitch;

	m_pIClient->InitiateCDKeyAuthorization(inbCDAuthorization);

	m_pGame->m_szLastAddress = szAddr;
	//m_pGame->m_bLastDoLateSwitch = _bDoLateSwitch;
	m_pGame->m_bLastCDAuthentication = inbCDAuthorization;
}

void CXClient::XDisconnect(const char* szCause)
{
	if (m_pIClient)
		m_pIClient->Disconnect(szCause);
}

void CXClient::LoadPlayerDesc()
{
}

void CXClient::UpdateISystem()
{
}