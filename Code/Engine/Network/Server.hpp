#pragma once
#include <BlackBox/Network/INetwork.hpp>
#include <Common/ClientServer.h>

class CNetwork;
class CNetworkServer;

class CServerSlot : public IServerSlot
{
public:
	CServerSlot(CNetworkServer& parent, uchar ucID, network::Socket& socket);
	~CServerSlot();

	// Inherited via IServerSlot
	virtual void          Advise(IServerSlotSink* pSink) override;
	virtual void          Disconnect(const char* szCause) override;
	virtual bool          ContextSetup(CStream& stm) override;
	virtual void          SendReliable(CStream& stm, bool bSecondaryChannel) override;
	virtual void          SendUnreliable(CStream& stm) override;
	virtual bool          IsReady() override;
	virtual unsigned char GetID() override;
	virtual unsigned int  GetClientIP() const override;
	virtual void          Release() override;
	virtual unsigned int  GetPing() override;
	virtual unsigned int  GetPacketsLostCount() override;
	virtual unsigned int  GetUnreliablePacketsLostCount() override;
	virtual void          ResetBandwidthStats() override;
	virtual void          GetBandwidthStats(SServerSlotBandwidthStats& out) const override;
	virtual void          OnPlayerAuthorization(bool bAllow, const char* szError, const uint8_t* pGlobalID, unsigned int uiGlobalIDSize) override;

	void                  MarkForDestruct() { m_bSelfDestruct = true; }

	void                  Update();
	void                  Start();

	CNetworkServer&       m_Parent;
	CIPAddress            m_ClientIP;
	network::Socket       m_Socket;
	IServerSlotSink*      m_pSink    = nullptr;
	uchar                 m_ID       = 0;
	bool                  m_bIsReady = false;

	bool                  m_bSelfDestruct = false;

	std::thread           m_thread;
};

class CNetworkServer : public IServer
{
public:
	CNetworkServer(CNetwork& pParent, IServerSlotFactory* pFactory, uint16_t nPort, bool local);
	~CNetworkServer();

	virtual void                   Update(unsigned int nTime) override;
	virtual void                   Release() override;
	virtual void                   SetVariable(enum CryNetworkVarible eVarName, unsigned int nValue) override;
	virtual void                   GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets) override;
	virtual const char*            GetHostName() override;
	virtual void                   RegisterPacketSink(const unsigned char inPacketID, INetworkPacketSink* inpSink) override;
	virtual void                   SetSecuritySink(IServerSecuritySink* pSecuritySink) override;
	virtual bool                   IsIPBanned(const unsigned int dwIP) override;
	virtual void                   BanIP(const unsigned int dwIP) override;
	virtual void                   UnbanIP(const unsigned int dwIP) override;
	virtual IServerSlot*           GetServerSlotbyID(const unsigned char ucId) const override;
	virtual uint8_t                GetMaxClientID() const override;
	virtual EMPServerType          GetServerType() const override;
	//////////
	bool                           Start();
	void                           OnNewConnection(network::Socket& client);
	void                           ThreadFunc();
	void                           OnDisconnect(CServerSlot* slot, const char* why);

	CNetwork&                      m_pParent;
	network::Socket                m_Socket;
	IServerSlotFactory*            m_pFactory = nullptr;
	uint16_t                       m_nPort    = DEFAULT_SERVERPORT;
	const char*                    m_szPort   = DEFAULT_SERVERPORT_STR;
	bool                           m_bLocal   = false;
	string                         m_HostName;
	std::map<uint16, CServerSlot*> m_Slots;
	unsigned int                   m_LastUpdateTime = 0;
	////////////////////////////////
	std::thread                    m_thread;

	std::mutex                     m_SlotsLock;
};