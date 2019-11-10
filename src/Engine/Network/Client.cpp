#include <BlackBox/Network/Client.hpp>
#include <BlackBox/Network/IPAddress.hpp>

void CClient::Connect(const char* szIP, uint16_t wPort, const BYTE* pbAuthorizationID, unsigned int iAuthorizationSize)
{
}

void CClient::Disconnect(const char* szCause)
{
}

void CClient::SendReliable(CStream& stm)
{
}

void CClient::SendUnreliable(CStream& stm)
{
}

void CClient::ContextReady(CStream& stm)
{
}

bool CClient::IsReady()
{
	return false;
}

bool CClient::Update(unsigned int nTime)
{
	return false;
}

void CClient::GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets)
{
}

void CClient::Release()
{
}

unsigned int CClient::GetPing()
{
	return 0;
}

unsigned int CClient::GetRemoteTimestamp(unsigned int nTime)
{
	return 0;
}

unsigned int CClient::GetPacketsLostCount()
{
	return 0;
}

unsigned int CClient::GetUnreliablePacketsLostCount()
{
	return 0;
}

CIPAddress CClient::GetServerIP() const
{
	return CIPAddress();
}

void CClient::InitiateCDKeyAuthorization(const bool inbCDAuthorization)
{
}

void CClient::OnCDKeyAuthorization(BYTE* pbAuthorizationID)
{
}

void CClient::SetServerIP(const char* szServerIP)
{
}
