#include "Client.hpp"
#include <BlackBox/Network/IPAddress.hpp>

#include <SDL2/SDL_net.h>

CNetworkClient::CNetworkClient(CNetwork* parent, IClientSink* sink, bool isLocal)
    : m_pParent(parent)
    , m_pSink(sink)
    , m_bIsLocal(isLocal)
{
}

CNetworkClient::~CNetworkClient()
{
}

void CNetworkClient::Connect(const char* szIP, uint16_t wPort, const uint8_t* pbAuthorizationID, unsigned int iAuthorizationSize)
{
	m_Socket.OnData = [this](CStream& stm)
	{ m_pSink->OnXData(stm); };
	m_Socket.OnDisconnect = [this](const char* szCause)
	{ m_pSink->OnXClientDisconnect(szCause); };

	m_thread = std::thread([this, szIP, wPort]
	                       { 
		if (m_Socket.Connect(szIP, std::to_string(wPort)))
		{
			{
				std::unique_lock lock(m_DeferedTasksLock);
				m_DeferedTasks.push_back([this]
										 { m_pSink->OnXConnect(); });
			}
			m_Socket.ThreadFunc(); 
		} });
}

void CNetworkClient::Disconnect(const char* szCause)
{
}

void CNetworkClient::SendReliable(CStream& stm)
{
	m_Socket.Send(stm);
}

void CNetworkClient::SendUnreliable(CStream& stm)
{
	m_Socket.Send(stm);
}

void CNetworkClient::ContextReady(CStream& stm)
{
	m_pSink->OnXContextSetup(stm);
	m_bReady = true;
}

bool CNetworkClient::IsReady()
{
	return m_bReady;
}

bool CNetworkClient::Update(unsigned int nTime)
{
	{
		std::unique_lock lock(m_DeferedTasksLock);
		for (auto& task : m_DeferedTasks)
		{
			task();
		}
	}
	m_Socket.Update();
#if 1
	return true;
#else
	return false;
#endif
}

void CNetworkClient::GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets)
{
	assert(0 && "Not implemented");
}

void CNetworkClient::Release()
{
	delete this;
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
	return m_ServerIP;
}

void CNetworkClient::InitiateCDKeyAuthorization(const bool inbCDAuthorization)
{
	Connect(m_ServerIP.GetAsString(), m_ServerPort, nullptr, 0);
}

void CNetworkClient::OnCDKeyAuthorization(uint8_t* pbAuthorizationID)
{
}

void CNetworkClient::SetServerIP(const char* szServerIP)
{
	m_ServerIP.Set(m_ServerPort, (char*)szServerIP);
}
