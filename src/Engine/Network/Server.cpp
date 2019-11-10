#include <BlackBox/Network/Server.hpp>

void CServer::Update(unsigned int nTime)
{
}

void CServer::Release()
{
}

void CServer::SetVariable(CryNetworkVarible eVarName, unsigned int nValue)
{
}

void CServer::GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets)
{
}

const char* CServer::GetHostName()
{
	return nullptr;
}

void CServer::RegisterPacketSink(const unsigned char inPacketID, INetworkPacketSink* inpSink)
{
}

void CServer::SetSecuritySink(IServerSecuritySink* pSecuritySink)
{
}

bool CServer::IsIPBanned(const unsigned int dwIP)
{
	return false;
}

void CServer::BanIP(const unsigned int dwIP)
{
}

void CServer::UnbanIP(const unsigned int dwIP)
{
}

IServerSlot* CServer::GetServerSlotbyID(const unsigned char ucId) const
{
	return nullptr;
}

uint8_t CServer::GetMaxClientID() const
{
	return uint8_t();
}

EMPServerType CServer::GetServerType() const
{
	return EMPServerType();
}
