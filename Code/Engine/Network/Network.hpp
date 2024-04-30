#pragma once

#include <BlackBox/Network/INetwork.hpp>

//#include <SDL2/SDL_net.h>

class CCompressionHelper;
class CTmpNetworkClient;
class CTmpNetworkServer;

class CNetwork : public INetwork
{
public:
  CNetwork();
  ~CNetwork();

  bool Init(ISystem* pSystem);

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
  virtual void GetMemoryUsage(ICrySizer* pSizer) override;
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

  void UnregisterClient(CTmpNetworkClient* pClient);
  void UnregisterServer(CTmpNetworkServer* pServer);

  void CDKeyAuth(IClient* pClient);

private:
  ISystem* m_pSystem;

  std::string m_LocalIP;

  std::vector<CTmpNetworkServer*> m_Servers;
  std::vector<IClient*> m_Clients;

  CCompressionHelper* m_pCompressionHelper{};
};

///////////////////////////////////////////////
// Game context (sent to the client when connecting to a server)
struct SXGameContext
{
  unsigned char		ucServerInfoVersion;	//!< SERVERINFO_FORMAT_VERSION (needed to prevent connects from old clients)
  string					strMapFolder;					//!<
  string					strMod;								//!< e.g. "FarCry", "Counterstrike" current TCM(Total Conversion Mod), specified with -MOD ...
  string					strGameType;					//!< e.g. "ASSAULT", "FFA", "TDM"
  string					strMission;						//!<
  DWORD						dwNetworkVersion;			//!< NETWORK_FORMAT_VERSION
  WORD						wLevelDataCheckSum;		//!<
  bool						bForceNonDevMode;			//!< client is forced not to use the Devmode
  bool						bInternetServer;			//!< true=requires UBI login, false=LAN
  BYTE						nComputerType;        //!< HI:CPUType, LO:OSType

  //! constructor
  SXGameContext();

  //!
  bool Write(class CStream& stm);
  //!
  bool Read(class CStream& stm);
  //!
  bool IsVersionOk() const;
  //! \return 0 if unknown, zero terminated string otherwise
  const char* GetCPUTarget() const;
  //! \return 0 if unknown, zero terminated string otherwise
  const char* GetOSTarget() const;
};

