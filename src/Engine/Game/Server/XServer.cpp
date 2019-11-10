#include "..\..\..\..\include\BlackBox\Game\Server\XServer.hpp"

CXServer::CXServer(CGame* pGame, WORD nPort, const char* szName, bool listen)
{
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
