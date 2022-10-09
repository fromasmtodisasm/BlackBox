#pragma once

#include <BlackBox/Network/INetwork.hpp>

//#include <SDL2/SDL_net.h>

class CCompressionHelper;
class CNetworkClient;
class CNetworkServer;

class CNetwork : public INetwork
{
public:
	CNetwork();
	~CNetwork();

	bool                        Init(ISystem* pSystem);

	// Унаследовано через INetwork
	virtual uint32_t            GetLocalIP() const override;
	virtual void                SetLocalIP(const char* szLocalIP) override;
	virtual IClient*            CreateClient(IClientSink* pSink, bool bLocal = false) override;
	virtual IServer*            CreateServer(IServerSlotFactory* pFactory, uint16_t nPort, bool local = false) override;
	virtual IRConSystem*        CreateRConSystem() override;
	virtual INETServerSnooper*  CreateNETServerSnooper(INETServerSnooperSink* pSink) override;
	virtual IServerSnooper*     CreateServerSnooper(IServerSnooperSink* pSink) override;
	virtual const char*         EnumerateError(uint32_t err) override;
	virtual void                Release() override;
	virtual void                GetMemoryUsage(ICrySizer* pSizer) override;
	virtual ICompressionHelper* GetCompressionHelper() override;
	virtual void                ClearProtectedFiles() override;
	virtual void                AddProtectedFile(const char* sFilename) override;
	virtual IServer*            GetServerByPort(const uint16_t wPort) override;
	virtual void                UpdateNetwork() override;
	virtual void                OnAfterServerLoadLevel(const char* szServerName, const uint32_t dwPlayerCount, const uint16_t wPort) override;
	virtual bool                VerifyMultiplayerOverInternet() override;
	virtual void                Client_ReJoinGameServer() override;
	virtual IClient*            GetClient() override;
	virtual void                SetUBIGameServerIP(const char* szAddress) override;
	virtual const char*         GetUBIGameServerIP(bool bLan) override;

	void                        UnregisterClient(CNetworkClient* pClient);
	void                        UnregisterServer(CNetworkServer* pServer);

private:
	ISystem*                          m_pSystem;
	CIPAddress                        m_LocalIP;
	WORD                              m_LocalPort;

	CNetworkServer*                   m_pServer;
	CNetworkClient*                   m_pClient;

	CCompressionHelper*               m_pCompressionHelper{};

	std::map<uint16, CNetworkServer*> m_PortToServerMap;
};
