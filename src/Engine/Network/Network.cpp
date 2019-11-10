#include <BlackBox/Network/Network.hpp>

CNetwork::CNetwork(ISystem* pSystem)
{
}

CNetwork::~CNetwork()
{
}

uint32_t CNetwork::GetLocalIP() const
{
	return uint32_t();
}

void CNetwork::SetLocalIP(const char* szLocalIP)
{
}

IClient* CNetwork::CreateClient(IClientSink* pSink, bool bLocal)
{
	return nullptr;
}

IServer* CNetwork::CreateServer(IServerSlotFactory* pFactory, uint16_t nPort, bool local)
{
	return nullptr;
}

IRConSystem* CNetwork::CreateRConSystem()
{
	return nullptr;
}

INETServerSnooper* CNetwork::CreateNETServerSnooper(INETServerSnooperSink* pSink)
{
	return nullptr;
}

IServerSnooper* CNetwork::CreateServerSnooper(IServerSnooperSink* pSink)
{
	return nullptr;
}

const char* CNetwork::EnumerateError(uint32_t err)
{
	return nullptr;
}

void CNetwork::Release()
{
}

void CNetwork::GetMemoryStatistics(ICrySizer* pSizer)
{
}

ICompressionHelper* CNetwork::GetCompressionHelper()
{
	return nullptr;
}

void CNetwork::ClearProtectedFiles()
{
}

void CNetwork::AddProtectedFile(const char* sFilename)
{
}

IServer* CNetwork::GetServerByPort(const uint16_t wPort)
{
	return nullptr;
}

void CNetwork::UpdateNetwork()
{
}

void CNetwork::OnAfterServerLoadLevel(const char* szServerName, const uint32_t dwPlayerCount, const uint16_t wPort)
{
}

bool CNetwork::VerifyMultiplayerOverInternet()
{
	return false;
}

void CNetwork::Client_ReJoinGameServer()
{
}

IClient* CNetwork::GetClient()
{
	return nullptr;
}

void CNetwork::SetUBIGameServerIP(const char* szAddress)
{
}

const char* CNetwork::GetUBIGameServerIP(bool bLan)
{
	return nullptr;
}


INetwork* CreateNetwork(ISystem *pSystem)
{
	return new CNetwork(pSystem);
}
