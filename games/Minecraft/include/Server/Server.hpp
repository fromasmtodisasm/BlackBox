#pragma once
#include <Network/XEntityProcessingCmd.hpp>
#include <Network/XNetwork.hpp>	

class CServer
	: public IServerSlotSink
{
	CServer(CXGame* pGame);

	// Inherited via IServerSlotSink
	virtual void OnXServerSlotConnect(const uint8_t* pbAuthorizationID, unsigned int uiAuthorizationSize) override;
	virtual void OnXServerSlotDisconnect(const char* szCause) override;
	virtual void OnContextReady(CStream& stm) override;
	virtual void OnData(CStream& stm) override;
	virtual void OnXPlayerAuthorization(bool bAllow, const char* szError, const uint8_t* pGlobalID, unsigned int uiGlobalIDSize) override;
};