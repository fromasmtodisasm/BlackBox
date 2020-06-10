#include <Game.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <Server/XServer.hpp>
#include <IXSystem.hpp>
#include <Server/XServerSlot.hpp>

//////////////////////////////////////////////////////////////////////////////////////////////
void CXServer::OnSpawnContainer( CEntityDesc &ed,IEntity *pEntity )
{
	m_pISystem->OnSpawnContainer(ed,pEntity);
}

//////////////////////////////////////////////////////////////////////
void CXServer::OnSpawn(IEntity *ent, CEntityDesc & ed )
{
  #if 0
	m_pISystem->OnSpawn(ent,ed);

	bool bSend = true;
	
	bSend=!m_bIsLoadingLevel;		// during loading we don't sync entities

	XSlotMap::iterator i = m_mapXSlots.begin();
	while(i != m_mapXSlots.end())
	{
		i->second->OnSpawnEntity(ed,ent,bSend);
		++i;
	}
  #endif
}

//////////////////////////////////////////////////////////////////////
void CXServer::OnRemove(IEntity *ent)
{
  #if 0
	XSlotMap::iterator i = m_mapXSlots.begin();
	while(i != m_mapXSlots.end())
	{
		//TRACE("CXServer::OnRemove [%d]",ent->GetId());
		i->second->OnRemoveEntity(ent);
		++i;
	}
  #endif
}

CXServer::CXServer(CGame* pGame, WORD nPort, const char* szName, bool listen)
{
  assert(pGame);

  m_pGame = pGame;
  m_pTimer = pGame->m_pSystem->GetITimer();
  IConsole* pConsole = m_pGame->GetSystem()->GetIConsole();			assert(pConsole);

  sv_name = pConsole->GetCVar("sv_name");

  sv_password = pConsole->GetCVar("sv_password");
  sv_maxplayers = pConsole->GetCVar("sv_maxplayers");
  sv_maxupdaterate = pConsole->GetCVar("sv_maxupdaterate");

  sv_maxrate = pConsole->GetCVar("sv_maxrate");
  sv_maxrate_lan = pConsole->GetCVar("sv_maxrate_lan");

  sv_netstats = pConsole->GetCVar("sv_netstats");
  sv_max_scheduling_delay = pConsole->GetCVar("sv_max_scheduling_delay");
  sv_min_scheduling_delay = pConsole->GetCVar("sv_min_scheduling_delay");
  m_bIsLoadingLevel = false;

  m_bListen = listen;

  m_mapXSlots.clear();

  float	timeout = m_pGame->sv_timeout->GetFVal() * 1000;

  // create the entity system sink
  //m_pGame->GetSystem()->GetIEntitySystem()->SetSink(this);

  // create the system interface
  #if 0
  m_pISystem = new CXSystemServer(this, m_pGame, m_pGame->m_pLog);
#endif

  // get this info before we set the server
  #if 0
  m_pGame->GetSystem()->SetForceNonDevMode(!m_pGame->IsDevModeEnable());
#endif

  INetwork* pNet = pGame->m_pSystem->GetINetwork();
  pNet->SetUBIGameServerIP(NULL);

  // fill m_ServerInfo structure
  GetServerInfo();

  // create the server
  m_pIServer = m_pGame->CreateServer(this, nPort, m_bListen);
  if (!m_pIServer)
  {
    m_pGame->m_pLog->Log("!!---------Server creation failed---------!!");
    m_bOK = false;
    return;
  }
  else
    m_bOK = true;

  m_pIServer->SetSecuritySink(this);
  m_pIServer->SetVariable(cnvDataStreamTimeout, (unsigned int)timeout);

  m_ServerInfos.nPort = nPort;
  m_ServerInfos.VersionInfo = GetISystem()->GetFileVersion();

  // initialise the game context
  m_GameContext.dwNetworkVersion = NETWORK_FORMAT_VERSION;
  m_GameContext.strMission = "";

  m_ScriptObjectServer.Create(m_pGame->GetScriptSystem(), m_pISystem, m_pGame);
  m_ScriptObjectServer.SetServer(this);
  m_bInDestruction = false;

  LoadBanList();

  IScriptSystem* pScriptSystem = GetISystem()->GetIScriptSystem();
  assert(pScriptSystem);

  SmartScriptObject pMapCycle(pScriptSystem);
  pScriptSystem->GetGlobalValue("MapCycle", pMapCycle);

  if (((IScriptObject*)pMapCycle) != 0)
  {
    HSCRIPTFUNCTION pfnInit = 0;
    if (pMapCycle->GetValue("Init", pfnInit))
    {
      pScriptSystem->BeginCall(pfnInit);
      pScriptSystem->PushFuncParam((IScriptObject*)pMapCycle);
      pScriptSystem->EndCall();

#ifndef _DEBUG
      pScriptSystem->ReleaseFunc(pfnInit);
#endif
    }
  }
}

CXServer::~CXServer()
{
}

void CXServer::DrawNetStats(IRenderer* pRenderer)
{
}

bool CXServer::IsInDestruction() const
{
  return false;
}

bool CXServer::IsIPBanned(const unsigned int dwIP)
{
  return false;
}

void CXServer::BanIP(const unsigned int dwIP)
{
}

void CXServer::UnbanIP(const unsigned int dwIP)
{
}

void CXServer::CheaterFound(const unsigned int dwIP, int type, const char* sMsg)
{
}

bool CXServer::GetSlotInfo(const unsigned int dwIP, SSlotInfo& info, int nameOnly)
{
  return false;
}

bool CXServer::GetServerInfo()
{
  return false;
}

const char* CXServer::GetMsgName(XSERVERMSG inValue)
{
  return nullptr;
}

void CXServer::SaveBanList(bool bSaveID, bool bSaveIP)
{
}

void CXServer::LoadBanList(bool bLoadID, bool bLoadIP)
{
}

CXServerSlot* CXServer::GetServerSlotByIP(unsigned int clientIP) const
{
  return nullptr;
}

bool CXServer::CreateServerSlot(IServerSlot* pIServerSlot)
{
  return false;
}

bool CXServer::GetServerInfoStatus(std::string& szServerStatus)
{
  return false;
}

bool CXServer::GetServerInfoStatus(std::string& szName, std::string& szGameType, std::string& szMap, std::string& szVersion, bool* pbPassword, int* piPlayers, int* piMaxPlayers)
{
  return false;
}

bool CXServer::GetServerInfoRules(std::string& szServerRules)
{
  return false;
}

bool CXServer::GetServerInfoPlayers(std::string* vszStrings[4], int& nStrings)
{
  return false;
}

bool CXServer::ProcessXMLInfoRequest(const char* sRequest, const char* sRespone, int nResponseMaxLength)
{
  return false;
}

void CXServer::RegisterSlot(CXServerSlot* pSlot)
{
}

void CXServer::UnregisterXSlot(DWORD nClientID)
{
}

void CXServer::ClearSlots()
{
}

int CXServer::GetNumPlayers()
{
  return 0;
}

void CXServer::Update()
{
}

void CXServer::UpdateXServerNetwork()
{
}

void CXServer::BroadcastUnreliable(XSERVERMSG msg, CStream& stmIn, int nExclude)
{
}

void CXServer::BroadcastReliable(XSERVERMSG msg, CStream& stmIn, bool bSecondaryChannel)
{
}

void CXServer::BroadcastText(const char* sText, float fLifeTime)
{
}

void CXServer::BroadcastCommand(const char* sCmd)
{
}

void CXServer::SyncVariable(ICVar* p)
{
}

void CXServer::SyncAIState(void)
{
}

bool CXServer::GetContext(SXGameContext& ctxOut)
{
  return false;
}

void CXServer::AddRespawnPoint(ITagPoint* pPoint)
{
}

void CXServer::RemoveRespawnPoint(ITagPoint* pPoint)
{
}

ITagPoint* CXServer::GetFirstRespawnPoint()
{
  return nullptr;
}

ITagPoint* CXServer::GetNextRespawnPoint()
{
  return nullptr;
}

ITagPoint* CXServer::GetPrevRespawnPoint()
{
  return nullptr;
}

ITagPoint* CXServer::GetRespawnPoint(char* name)
{
  return nullptr;
}

ITagPoint* CXServer::GetRandomRespawnPoint(const char* sFilter)
{
  return nullptr;
}

void CXServer::AddToTeam(const char* sTeam, int eid)
{
}

void CXServer::RemoveFromTeam(int eid)
{
}

void CXServer::AddTeam(const char* sTeam)
{
}

void CXServer::RemoveTeam(const char* sTeam)
{
}

void CXServer::SetTeamScore(const char* sTeam, int score)
{
}

void CXServer::SetTeamFlags(const char* sTeam, int flags)
{
}

unsigned CXServer::MemStats()
{
  return 0;
}

unsigned int CXServer::GetSchedulingDelay()
{
  return 0;
}

unsigned int CXServer::GetMaxUpdateRate() const
{
  return 0;
}

//////////////////////////////////////////////////////////////////////
void CXServer::BindEntity(EntityId idParent,EntityId idChild,unsigned char cParam)
{
#if 0
	IEntity *pChild=m_pISystem->GetEntity(idChild);
	IEntity *pParent=m_pISystem->GetEntity(idParent);
	CStream stm;
	stm.Write(idParent);
	stm.Write(idChild);
	stm.Write(cParam);
	stm.Write(true);
	stm.Write(pParent->GetAngles(1));
	stm.Write(pChild->GetAngles(1));
	BroadcastReliable(XSERVERMSG_BINDENTITY,stm,false);
#endif
}

//////////////////////////////////////////////////////////////////////
void CXServer::UnbindEntity(EntityId idParent,EntityId idChild,unsigned char cParam)
{
#if 0
	IEntity *pChild=m_pISystem->GetEntity(idChild);
	IEntity *pParent=m_pISystem->GetEntity(idParent);
	CStream stm;
	stm.Write(idParent);
	stm.Write(idChild);
	stm.Write(cParam);
	stm.Write(false);
	stm.Write(pParent->GetAngles(1));
	stm.Write(pChild->GetAngles(1));
	BroadcastReliable(XSERVERMSG_BINDENTITY,stm,false);
#endif
}

void CXServer::OnMapChanged()
{
}

bool CXServer::IsIDBanned(const BannedID& ID)
{
  return false;
}

void CXServer::BanID(const BannedID& ID)
{
}

void CXServer::UnbanID(const BannedID& ID)
{
}