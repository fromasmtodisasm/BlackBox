#pragma once

#include <BlackBox/Network/INetwork.hpp>

//#include <SDL2/SDL_net.h>

class CNetwork : public INetwork
{
public:
  CNetwork(ISystem* pSystem);
  ~CNetwork();

  bool Init();

  // Унаследовано через INetwork
  virtual uint32_t GetLocalIP() const override;
  virtual void SetLocalIP(const char* szLocalIP) override;
  virtual IClient* CreateClient(IClientSink* pSink, bool bLocal = false) override;
  virtual IServer* CreateServer(IServerSlotFactory* pFactory, uint16_t nPort, bool local = false) override;
  virtual IRConSystem* CreateRConSystem() override;
  virtual INETServerSnooper* CreateNETServerSnooper(INETServerSnooperSink* pSink) override;
  virtual IServerSnooper* CreateServerSnooper(IServerSnooperSink* pSink) override;
  virtual const char* EnumerateError(uint32_t err) override;
  virtual void Release() override;
  virtual void GetMemoryStatistics(ICrySizer* pSizer) override;
  virtual ICompressionHelper* GetCompressionHelper() override;
  virtual void ClearProtectedFiles() override;
  virtual void AddProtectedFile(const char* sFilename) override;
  virtual IServer* GetServerByPort(const uint16_t wPort) override;
  virtual void UpdateNetwork() override;
  virtual void OnAfterServerLoadLevel(const char* szServerName, const uint32_t dwPlayerCount, const uint16_t wPort) override;
  virtual bool VerifyMultiplayerOverInternet() override;
  virtual void Client_ReJoinGameServer() override;
  virtual IClient* GetClient() override;
  virtual void SetUBIGameServerIP(const char* szAddress) override;
  virtual const char* GetUBIGameServerIP(bool bLan) override;
private:
  ISystem* m_pSystem;

  std::string m_LocalIP;

  std::vector<IServer*> m_Servers;
  std::vector<IClient*> m_Clients;

  ICompressionHelper* m_pCompressionHelper{};
};