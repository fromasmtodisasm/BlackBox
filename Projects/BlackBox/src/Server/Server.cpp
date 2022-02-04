#include <Server/Server.hpp>

void CServer::OnXServerSlotConnect(const uint8_t* pbAuthorizationID, unsigned int uiAuthorizationSize)
{
}

void CServer::OnXServerSlotDisconnect(const char* szCause)
{
}

void CServer::OnContextReady(CStream& stm)
{
}

void CServer::OnData(CStream& stm)
{
}

void CServer::OnXPlayerAuthorization(bool bAllow, const char* szError, const uint8_t* pGlobalID, unsigned int uiGlobalIDSize)
{
}
