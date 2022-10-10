#include "Server.hpp"
#include "Network.hpp"

#include <Common/Socket.h>

CNetworkServer::CNetworkServer(CNetwork& pParent, IServerSlotFactory* pFactory, uint16_t nPort, bool bLocal)
    : m_pParent(pParent)
    , m_pFactory(pFactory)
    , m_nPort(nPort)
    , m_bLocal(bLocal)
{
}

CNetworkServer::~CNetworkServer()
{
	m_Socket->Term();
	if (m_thread.joinable())
		m_thread.join();
	m_pParent.UnregisterServer(this);
}

void CNetworkServer::Update(unsigned int nTime)
{
	m_LastUpdateTime = nTime;

	for (auto& [id, slot] : m_Slots)
	{
		slot->Update();
	}
}

void CNetworkServer::Release()
{
	delete this;
}

void CNetworkServer::SetVariable(enum CryNetworkVarible eVarName, unsigned int nValue)
{
}

void CNetworkServer::GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets)
{
}

const char* CNetworkServer::GetHostName()
{
	return m_HostName.c_str();
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

bool CNetworkServer::Start()
{
	m_Socket = std::move(network::Socket::CreateListen(m_szPort));
	if (m_Socket->IsValid())
	{
		m_Socket->OnNewConnection = [this](network::Socket::Ptr s)
		{
			OnNewConnection(std::move(s));
		};
		m_thread = std::thread([this]
		                       { ThreadFunc(); });
		return true;
	}
	return false;
}

void CNetworkServer::OnNewConnection(network::Socket::Ptr client)
{
	auto id   = m_Slots.size();
	auto slot = new CServerSlot(*this, uchar(id), std::move(client));

	///

	if (!m_pFactory->CreateServerSlot(slot))
	{
		//OnDisconnect(slot, "Server full");
		delete slot;
		return;
	}
	
	slot->Start();
	std::lock_guard lock(m_SlotsLock);
	m_Slots[id] = (slot);
}

void CNetworkServer::ThreadFunc()
{
	m_Socket->ListenThread();
}
void CNetworkServer::OnDisconnect(CServerSlot* slot, const char* why)
{
	std::lock_guard lock(m_SlotsLock);
	if (auto it = std::find_if(m_Slots.begin(), m_Slots.end(), [slot](const decltype(m_Slots)::value_type& val)
	                           { return val.second == slot; });
	    it != m_Slots.end())
	{
		it->second->MarkForDestruct();
	}
}
// Inherited via IServerSlot

CServerSlot::CServerSlot(CNetworkServer& parent, uchar ucID, network::Socket::Ptr socket)
    : m_Parent(parent)
    , m_ID(ucID)
    , m_Socket(std::move(socket))
{
}

CServerSlot::~CServerSlot()
{
}

void CServerSlot::Advise(IServerSlotSink* pSink)
{
	m_pSink = pSink;
}

void CServerSlot::Disconnect(const char* szCause)
{
	m_Socket->Disconnect();
}

bool CServerSlot::ContextSetup(CStream& stm)
{
	return false;
}

void CServerSlot::SendReliable(CStream& stm, bool bSecondaryChannel)
{
	m_Socket->Send(stm);
}

void CServerSlot::SendUnreliable(CStream& stm)
{
	m_Socket->Send(stm);
}

bool CServerSlot::IsReady()
{
	return m_bIsReady;
}

unsigned char CServerSlot::GetID()
{
	return m_ID;
}

unsigned int CServerSlot::GetClientIP() const
{
	return m_ClientIP.GetAsUINT();
}

void CServerSlot::Release()
{
	delete this;
}

unsigned int CServerSlot::GetPing()
{
	return 0;
}

unsigned int CServerSlot::GetPacketsLostCount()
{
	return 0;
}

unsigned int CServerSlot::GetUnreliablePacketsLostCount()
{
	return 0;
}

void CServerSlot::ResetBandwidthStats()
{
}

void CServerSlot::GetBandwidthStats(SServerSlotBandwidthStats& out) const
{
}

void CServerSlot::OnPlayerAuthorization(bool bAllow, const char* szError, const uint8_t* pGlobalID, unsigned int uiGlobalIDSize)
{
}
void CServerSlot::Update()
{
	m_Socket->Update();
}

void CServerSlot::Start()
{
	assert(m_pSink != nullptr);
	m_Socket->OnData = [this](CStream& stm)
	{ m_pSink->OnData(stm); };

	m_Socket->OnDisconnect = [this](const char* szCause)
	{ m_Parent.OnDisconnect(this, szCause); };


	m_thread = std::thread([this]
	                       { m_Socket->ThreadFunc(); });
}
