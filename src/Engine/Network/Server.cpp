#include <BlackBox/Network/Server.hpp>

CNetworkServer::CNetworkServer()
{
}

CNetworkServer::~CNetworkServer()
{
}

void CNetworkServer::Update(unsigned int nTime)
{
}

void CNetworkServer::Release()
{
}

void CNetworkServer::SetVariable(enum CryNetworkVarible eVarName, unsigned int nValue)
{
}

void CNetworkServer::GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets)
{
}

const char* CNetworkServer::GetHostName()
{
  return nullptr;
}

void CNetworkServer::RegisterPacketSink(const unsigned char inPacketID, INetworkPacketSink* inpSink)
{
}

void CNetworkServer::SetSecuritySink(IServerSecuritySink* pSecuritySink)
{
}

bool CNetworkServer::IsIPBanned(const unsigned int dwIP)
{
  return false;
}

void CNetworkServer::BanIP(const unsigned int dwIP)
{
}

void CNetworkServer::UnbanIP(const unsigned int dwIP)
{
}

IServerSlot* CNetworkServer::GetServerSlotbyID(const unsigned char ucId) const
{
  return nullptr;
}

uint8_t CNetworkServer::GetMaxClientID() const
{
  return uint8_t();
}

EMPServerType CNetworkServer::GetServerType() const
{
  return EMPServerType();
}