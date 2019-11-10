#pragma once
#include <BlackBox/INetwork.hpp>

class CClient : public IClient
{
	CClient();
	~CClient();
	// Унаследовано через IClient
	virtual void Connect(const char* szIP, uint16_t wPort, const BYTE* pbAuthorizationID, unsigned int iAuthorizationSize) override;
	virtual void Disconnect(const char* szCause) override;
	virtual void SendReliable(CStream& stm) override;
	virtual void SendUnreliable(CStream& stm) override;
	virtual void ContextReady(CStream& stm) override;
	virtual bool IsReady() override;
	virtual bool Update(unsigned int nTime) override;
	virtual void GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets) override;
	virtual void Release() override;
	virtual unsigned int GetPing() override;
	virtual unsigned int GetRemoteTimestamp(unsigned int nTime) override;
	virtual unsigned int GetPacketsLostCount() override;
	virtual unsigned int GetUnreliablePacketsLostCount() override;
	virtual CIPAddress GetServerIP() const override;
	virtual void InitiateCDKeyAuthorization(const bool inbCDAuthorization) override;
	virtual void OnCDKeyAuthorization(BYTE* pbAuthorizationID) override;
	virtual void SetServerIP(const char* szServerIP) override;
};
