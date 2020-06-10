#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Core/ICompressionHelper.hpp>
#include <BlackBox/Network/Network.hpp>
#include <BlackBox/Network/Client.hpp>
#include <BlackBox/Network/Server.hpp>
#include <BlackBox/Network/IPAddress.hpp>

#include <SDL2/SDL_net.h>

class CCompressionHelper : public ICompressionHelper
{
public:
	CCompressionHelper(CNetwork* pNetwork)
		: m_pNetwork(pNetwork)
  {
     
  }

public:
  CNetwork* m_pNetwork;

  // Inherited via ICompressionHelper
  virtual bool Write(CStream& outStream, const unsigned char inChar) override
  {
	  assert(0);
	  return false;
  }
  virtual bool Read(CStream& inStream, unsigned char& outChar) override
  {
	  assert(0);
	  return false;
  }
  virtual bool Write(CStream& outStream, const char* inszString) override
  {
	  auto len = strlen(inszString) + 1;
		return outStream.WriteBits((BYTE*)inszString, BYTES2BITS(len));
  }
  virtual bool Read(CStream& inStream, char* outszString, const DWORD indwStringSize) override
  {
		DWORD i = 0; 
    bool ok = false;
	  for (; i < indwStringSize && !inStream.EOS() ; i++)
	  {
      if (inStream.ReadBits((BYTE*)(outszString + i), BYTES2BITS(sizeof(char))))
      {
				if (outszString[i] == 0)
				{
					outszString[i + 1] = 0;
					ok = true;
					break; 
        }
      }
			else
			{
				return false; 
      }
	  }
	  return ok;
  }
};

class CTmpNetworkClient : public IClient
{
public:
  CTmpNetworkClient()
  {

  }
  ~CTmpNetworkClient()
  {

  }
  // Inherited via IClient
  virtual void Connect(const char* szIP, uint16_t wPort, const uint8_t* pbAuthorizationID, unsigned int iAuthorizationSize) override
  {
  }
  virtual void Disconnect(const char* szCause) override
  {
  }
  virtual void SendReliable(CStream& stm) override
  {
  }
  virtual void SendUnreliable(CStream& stm) override
  {
  }
  virtual void ContextReady(CStream& stm) override
  {
  }
  virtual bool IsReady() override
  {
    return false;
  }
  virtual bool Update(unsigned int nTime) override
  {
    return false;
  }
  virtual void GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets) override
  {
  }
  virtual void Release() override
  {
  }
  virtual unsigned int GetPing() override
  {
    return 0;
  }
  virtual unsigned int GetRemoteTimestamp(unsigned int nTime) override
  {
    return 0;
  }
  virtual unsigned int GetPacketsLostCount() override
  {
    return 0;
  }
  virtual unsigned int GetUnreliablePacketsLostCount() override
  {
    return 0;
  }
  virtual CIPAddress GetServerIP() const override
  {
    return CIPAddress();
  }
  virtual void InitiateCDKeyAuthorization(const bool inbCDAuthorization) override
  {
  }
  virtual void OnCDKeyAuthorization(uint8_t* pbAuthorizationID) override
  {
  }
  virtual void SetServerIP(const char* szServerIP) override
  {
  }
};

class CTmpNetworkServer : public IServer
{
  IServerSlotFactory* m_pFactory = nullptr;
  uint16_t m_nPort;
  bool m_bLocal;
  IPaddress m_IP;
  TCPsocket m_Socket;
  std::vector<TCPsocket> m_ClientSockes;
public:
  CTmpNetworkServer(IServerSlotFactory* pFactory, uint16_t nPort, bool local)
    :
    m_pFactory(pFactory),
    m_nPort(nPort),
    m_bLocal(local)
  {
    gEnv->pLog->Log("NetworkServer Constructed");

  }
  ~CTmpNetworkServer()
  {
    gEnv->pLog->Log("NetworkServer Desctruected");
  }
  bool Init()
  {
    if (SDLNet_ResolveHost(&m_IP, nullptr, m_nPort) == -1) {
      gEnv->pLog->Log("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
      return false;
    }

    m_Socket = SDLNet_TCP_Open(&m_IP);
    if (!m_Socket) {
      gEnv->pLog->Log("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
      return false;
    }
    gEnv->pLog->Log("Conntection Opened");
    return true;
  }
  // Inherited via IServer
  virtual void Update(unsigned int nTime) override
  {
    // accept a connection coming in on server_tcpsock
    TCPsocket new_tcpsock;

    while (true)
    {
      new_tcpsock=SDLNet_TCP_Accept(m_Socket);
      if(!new_tcpsock) {
        break;// gEnv->pLog->Log("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
      }
      else {
        gEnv->pLog->Log("New connection");
        //m_ClientSockes.push_back(new_tcpsock);
        size_t len, result;
        auto* msg = R"(
HTTP/1.1 200 OK
Host: site.com
Content-Type: text/html;
Connection: close
Content-Length: 21

<h1>Test page...</h1>)";
        static char buf[1024];
        int reslen = 0;
        if ((reslen = SDLNet_TCP_Recv(new_tcpsock, buf, 1000)) <= 0)
        {
          gEnv->pLog->Log("Error of read");
        }
        else
        {
          gEnv->pLog->Log("Response %.*s:\n", reslen, buf);
        }

        len = strlen(msg) + 1; // add one for the terminating NULL
        result = SDLNet_TCP_Send(new_tcpsock, msg, len);
        SDLNet_TCP_Close(new_tcpsock);

        if (result < len) {
           printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
          // It may be good to disconnect sock because it is likely invalid now.
        }
      }
    }
    for (auto c : m_ClientSockes)
    {
    }
  }
  virtual void Release() override
  {
    delete this;
  }
  virtual void SetVariable(CryNetworkVarible eVarName, unsigned int nValue) override
  {
  }
  virtual void GetBandwidth(float& fIncomingKbPerSec, float& fOutgoinKbPerSec, uint32_t& nIncomingPackets, uint32_t& nOutgoingPackets) override
  {
  }
  virtual const char* GetHostName() override
  {
    return nullptr;
  }
  virtual void RegisterPacketSink(const unsigned char inPacketID, INetworkPacketSink* inpSink) override
  {
  }
  virtual void SetSecuritySink(IServerSecuritySink* pSecuritySink) override
  {
  }
  virtual bool IsIPBanned(const unsigned int dwIP) override
  {
    return false;
  }
  virtual void BanIP(const unsigned int dwIP) override
  {
  }
  virtual void UnbanIP(const unsigned int dwIP) override
  {
  }
  virtual IServerSlot* GetServerSlotbyID(const unsigned char ucId) const override
  {
    return nullptr;
  }
  virtual uint8_t GetMaxClientID() const override
  {
    return uint8_t();
  }
  virtual EMPServerType GetServerType() const override
  {
    return EMPServerType();
  }
};

CNetwork::CNetwork(ISystem* pSystem)
  :
  m_pSystem(pSystem)
{
}

CNetwork::~CNetwork()
{
  SDLNet_Quit();
}

bool CNetwork::Init()
{
  SDL_version compile_version;
  const SDL_version* link_version = SDLNet_Linked_Version();
  SDL_NET_VERSION(&compile_version);
  gEnv->pLog->Log("Compiled with SDL_net version: %d.%d.%d",
    compile_version.major,
    compile_version.minor,
    compile_version.patch);
  gEnv->pLog->Log("Running with SDL_net version: %d.%d.%d",
    link_version->major,
    link_version->minor,
    link_version->patch);
  bool res = true;
  if (SDL_Init(0) != -1) {
    if (SDLNet_Init() == -1) {
      gEnv->pLog->Log("SDLNet_Init: %s\n", SDLNet_GetError());
      res = false;
    }
  }
  else
  {
    gEnv->pLog->Log("SDL_Init: %s\n", SDL_GetError());
    res = false;
  }

  m_pCompressionHelper = new CCompressionHelper(this);
  return res;
}

uint32_t CNetwork::GetLocalIP() const
{
  return uint32_t();
}

void CNetwork::SetLocalIP(const char* szLocalIP)
{
  m_LocalIP = szLocalIP;
}

IClient* CNetwork::CreateClient(IClientSink* pSink, bool bLocal)
{
  auto result = new CTmpNetworkClient;
  m_Clients.push_back(result);
  return result;
}

IServer* CNetwork::CreateServer(IServerSlotFactory* pFactory, uint16_t nPort, bool local)
{
  auto result = new CTmpNetworkServer(pFactory, nPort, local);
  if (!result->Init())
    return nullptr;
  m_Servers.push_back(result);
  return result;
}

IRConSystem* CNetwork::CreateRConSystem()
{
  return nullptr;
}

INETServerSnooper* CNetwork::CreateNETServerSnooper(INETServerSnooperSink* pSink)
{
  return nullptr;
}

IServerSnooper* CNetwork::CreateServerSnooper(IServerSnooperSink* pSink)
{
  return nullptr;
}

const char* CNetwork::EnumerateError(uint32_t err)
{
  return nullptr;
}

void CNetwork::Release()
{
  delete this;
}

void CNetwork::GetMemoryStatistics(ICrySizer* pSizer)
{
}

ICompressionHelper* CNetwork::GetCompressionHelper()
{
  return m_pCompressionHelper;
}

void CNetwork::ClearProtectedFiles()
{
}

void CNetwork::AddProtectedFile(const char* sFilename)
{
}

IServer* CNetwork::GetServerByPort(const uint16_t wPort)
{
  return nullptr;
}

void CNetwork::UpdateNetwork()
{
  for (auto &server : m_Servers)
  {
    server->Update(0);
  }
  for (auto &client : m_Clients)
  {
    client->Update(0);
  }
}

void CNetwork::OnAfterServerLoadLevel(const char* szServerName, const uint32_t dwPlayerCount, const uint16_t wPort)
{
}

bool CNetwork::VerifyMultiplayerOverInternet()
{
  return false;
}

void CNetwork::Client_ReJoinGameServer()
{
}

IClient* CNetwork::GetClient()
{
  return nullptr;
}

void CNetwork::SetUBIGameServerIP(const char* szAddress)
{
}

const char* CNetwork::GetUBIGameServerIP(bool bLan)
{
  return nullptr;
}

NETWORK_API INetwork* CreateNetwork(ISystem* pSystem)
{
  CNetwork* pNetwork = new CNetwork(pSystem);
  if (!pNetwork->Init())
  {
    pNetwork->Release();
    pNetwork = nullptr;
  }
  return pNetwork;
}
