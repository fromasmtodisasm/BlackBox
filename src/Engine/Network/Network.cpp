#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Network/Network.hpp>
#include <BlackBox/Network/Client.hpp>
#include <BlackBox/Network/Server.hpp>

#include <SDL2/SDL_net.h>

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
  gEnv->pLog->Log("-- Compiled with SDL_net version: %d.%d.%d",
    compile_version.major,
    compile_version.minor,
    compile_version.patch);
  gEnv->pLog->Log("-- Running with SDL_net version: %d.%d.%d",
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
  return new CNetworkClient;
}

IServer* CNetwork::CreateServer(IServerSlotFactory* pFactory, uint16_t nPort, bool local)
{
  return new CNetworkServer;
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
  return nullptr;
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

INetwork* CreateNetwork(ISystem* pSystem)
{
  CNetwork* pNetwork = new CNetwork(pSystem);
  if (!pNetwork->Init())
  {
    pNetwork->Release();
    pNetwork = nullptr;
  }
  return pNetwork;
}