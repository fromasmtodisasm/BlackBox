#include <BlackBox/Network/Client.hpp>
#include <BlackBox/Network/IPAddress.hpp>

#include <SDL2/SDL_net.h>

CNetworkClient::CNetworkClient()
{
}

CNetworkClient::~CNetworkClient()
{
}

void CNetworkClient::Connect(const char* szIP, uint16_t wPort, const uint8_t* pbAuthorizationID, unsigned int iAuthorizationSize)
{
  IPaddress ip;
  if (SDLNet_ResolveHost(&ip, szIP, wPort))
  {
  }
}

void CNetworkClient::Disconnect(const char* szCause)
{
}

void CNetworkClient::SendReliable(CStream& stm)
{
}

void CNetworkClient::SendUnreliable(CStream& stm)
{
}

void CNetworkClient::ContextReady(CStream& stm)
{
}

bool CNetworkClient::IsReady()
{
  return false;
}

bool CNetworkClient::Update(unsigned int nTime)
{
  return false;
}

void CNetworkClient::GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets)
{
}

void CNetworkClient::Release()
{
}

unsigned int CNetworkClient::GetPing()
{
  return 0;
}

unsigned int CNetworkClient::GetRemoteTimestamp(unsigned int nTime)
{
  return 0;
}

unsigned int CNetworkClient::GetPacketsLostCount()
{
  return 0;
}

unsigned int CNetworkClient::GetUnreliablePacketsLostCount()
{
  return 0;
}

CIPAddress CNetworkClient::GetServerIP() const
{
  return CIPAddress();
}

void CNetworkClient::InitiateCDKeyAuthorization(const bool inbCDAuthorization)
{
}

void CNetworkClient::OnCDKeyAuthorization(uint8_t* pbAuthorizationID)
{
}

void CNetworkClient::SetServerIP(const char* szServerIP)
{
}
