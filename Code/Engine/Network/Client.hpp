#pragma once
#include <BlackBox/Network/INetwork.hpp>

#include "Common/ClientServer.h"
#include <vector>
#include <functional>

class CNetwork;

class CNetworkClient : public IClient
{
public:
	CNetworkClient(CNetwork* parent, IClientSink* sink, bool isLocal);
	~CNetworkClient();

	virtual void                       Connect(const char* szIP, uint16_t wPort, const uint8_t* pbAuthorizationID, unsigned int iAuthorizationSize) override;
	virtual void                       Disconnect(const char* szCause) override;
	virtual void                       SendReliable(CStream& stm) override;
	virtual void                       SendUnreliable(CStream& stm) override;
	virtual void                       ContextReady(CStream& stm) override;
	virtual bool                       IsReady() override;
	virtual bool                       Update(unsigned int nTime) override;
	virtual void                       GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets) override;
	virtual void                       Release() override;
	virtual unsigned int               GetPing() override;
	virtual unsigned int               GetRemoteTimestamp(unsigned int nTime) override;
	virtual unsigned int               GetPacketsLostCount() override;
	virtual unsigned int               GetUnreliablePacketsLostCount() override;
	virtual CIPAddress                 GetServerIP() const override;
	virtual void                       InitiateCDKeyAuthorization(const bool inbCDAuthorization) override;
	virtual void                       OnCDKeyAuthorization(uint8_t* pbAuthorizationID) override;
	virtual void                       SetServerIP(const char* szServerIP) override;

	CNetwork*                          m_pParent = nullptr;
	IClientSink*                       m_pSink   = nullptr;
	CIPAddress                         m_ServerIP;
	uint16_t                           m_ServerPort = DEFAULT_SERVERPORT;
	bool                               m_bIsLocal   = false;
	network::Socket                    m_Socket;
	bool                               m_bReady         = false;
	unsigned int                       m_LastUpdateTime = 0;
	bool                               m_bConnected     = false;

	size_t                             id               = 0;
	std::thread                        m_thread;

	std::mutex                         m_DeferedTasksLock;
	std::vector<std::function<void()>> m_DeferedTasks;
};