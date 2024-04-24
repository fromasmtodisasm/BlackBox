#include "pch.hpp"
#include "Client.hpp"
#include "Network.hpp"
#include "Server.hpp"

#include <BlackBox/Core/ICompressionHelper.hpp>
#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/Network/IPAddress.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <SDL2/SDL_net.h>
#include <functional>
#include <map>
#include <sstream>

#include <BlackBox/Core/Utils.hpp>

CNetwork g_Network;

class CTmpServerSlot : public IServerSlot
{
	// Inherited via IServerSlot
	virtual void Advise(IServerSlotSink* pSink) override
	{
	}
	virtual void Disconnect(const char* szCause) override
	{
	}
	virtual bool ContextSetup(CStream& stm) override
	{
		return false;
	}
	virtual void SendReliable(CStream& stm, bool bSecondaryChannel = false) override
	{
	}
	virtual void SendUnreliable(CStream& stm) override
	{
	}
	virtual bool IsReady() override
	{
		return false;
	}
	virtual unsigned char GetID() override
	{
		return 0;
	}
	virtual unsigned int GetClientIP() const override
	{
		return 0;
	}
	virtual void Release() override
	{
	}
	virtual unsigned int GetPing() override
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
	virtual void ResetBandwidthStats() override
	{
	}
	virtual void GetBandwidthStats(SServerSlotBandwidthStats& out) const override
	{
	}
	virtual void OnPlayerAuthorization(bool bAllow, const char* szError, const uint8_t* pGlobalID, unsigned int uiGlobalIDSize) override
	{
	}
};

class CTmpNetworkServer : public IServer
{
	IServerSlotFactory*    m_pFactory = nullptr;
	uint16_t               m_nPort;
	bool                   m_bLocal;
	IPaddress              m_IP;
	TCPsocket              m_Socket;
	std::vector<TCPsocket> m_ClientSockes;
	//std::map<string, std::function<void(std::stringstream& ss, const string& args)>> handlers;

public:
	CTmpNetworkServer(IServerSlotFactory* pFactory, uint16_t nPort, bool local)
	    : m_pFactory(pFactory)
	    , m_nPort(nPort)
	    , m_bLocal(local)
	{
		Env::Log()->Log("NetworkServer Constructed");
#if 0
		handlers["info"] = [this](std::stringstream& ss, const string& args) {
			auto& content = ss;
			content << "CTmpNetworkServer: " << this << "</br>";
			content << "ISystem: " << Env::System() << "</br>";
			content << "IConsole: " << Env::Console() << "</br>";
			content << "IRenderer: " << Env::Renderer() << "</br>";
			content << "ILog: " << Env::Log() << "</br>";
			content << R"(<a href="gaben">��� ���!!!</a>)";
			content <<
				R"(
<form action="/cmd" method="get">
	<p>Remote command<input type="text" name="remote_command"></p>
	<p><input type="submit" value="���������"></p>
</form>
)";
		};
		handlers["main"] = [this](std::stringstream& ss, const string& args) {
			ss << "Main Page";
		};
		handlers["cmd"] = [this](std::stringstream& ss, const string& args) {
			Env::Console()->ExecuteString("remote_command = \"\"");
			Env::Console()->ExecuteString(args.c_str());
			auto cmd = Env::Console()->GetCVar("remote_command");
			if (cmd)
			{
				Env::Console()->ExecuteString(cmd->GetString());
			}
			ss <<
				R"(
<form action="/cmd" method="get">
	<p>Remote command<input type="text" name="remote_command"></p>
	<p><input type="submit" value="���������"></p>
</form>
)";
		};
		handlers[""] = [this](std::stringstream& ss, const string& args) {
			ss << "Main Page!!!";
		};
		handlers["gaben"] = [this](std::stringstream& ss, const string& args) {
			ss << R"(
      <h1>Gabe, give me money!!!</h1>
      <a href="info">Info<a>
)";
		};
#endif
	}
	~CTmpNetworkServer()
	{
		SDLNet_TCP_Close(m_Socket);
		Env::Log()->Log("NetworkServer Desctruected");
		g_Network.UnregisterServer(this);
	}
	bool Init()
	{
		if (SDLNet_ResolveHost(&m_IP, nullptr, m_nPort) == -1)
		{
			Env::Log()->Log("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
			return false;
		}

		m_Socket = SDLNet_TCP_Open(&m_IP);
		if (!m_Socket)
		{
			Env::Log()->Log("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
			return false;
		}
		Env::Log()->Log("Conntection Opened");
		return true;
	}
	// Inherited via IServer
	virtual void Update(unsigned int nTime) override
	{
		// accept a connection coming in on server_tcpsock
		TCPsocket new_tcpsock;

		while (true)
		{
			new_tcpsock = SDLNet_TCP_Accept(m_Socket);
			if (!new_tcpsock)
			{
				break; // Env::Log()->Log("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
			}
			else
			{
				Env::Log()->Log("New connection");
				//m_ClientSockes.push_back(new_tcpsock);
				size_t            length = 0, result;
				std::stringstream response;
				static char       buf[1024];
				int               reslen = 0;
				if ((reslen = SDLNet_TCP_Recv(new_tcpsock, buf, 1000)) <= 0)
				{
					Env::Log()->Log("Error of read");
				}
				else
				{
					std::stringstream content;
					content << R"(
<html>
	<head>
    <title>
      BlackBox
    </title>
		<meta http-equiv="Content-Type" content="text/html; charset=windows-1251">
	</head>
  <body>
		<h1>BlackBox</h1>
)";
					//Env::Log()->Log("Response %.*s:\n", reslen, buf);
					std::stringstream ss(buf);
					//Env::Log()->Log("Response %s:\n", ss.str().data());
					string            line;
					std::getline(ss, line);
					const int len = line.size() - 14; // GET / HTTP/1.1
					string    location;
					string    args;
					location.resize(len);
					string tmp = line;
					urldecode2(tmp.data(), line.data());
					line     = tmp;
					//auto c = sscanf(line.data(), "GET /%s HTTP/1.1", location.data());
					location = line.substr(5, line.size() - (5 + 10));
					Env::Log()->Log("Location: %s", location.data());

					auto pos = location.find('?');
					auto s   = location.size();
					if (pos != string::npos)
					{
						args = location.substr(pos + 1);
						location.resize(pos);
					}

#if 0
					if (auto it = handlers.find(location.c_str()); it != handlers.end())
					{
						it->second(content, args);
					}
#endif
					content << R"(
  </body>
</html>
)";
					response <<
					    R"(
	HTTP/1.1 200 OK
	Host: site.com
	Content-Type: text/html;
	Connection: close
	Content-Length: )";
					response << content.str().length() << "\r\n\r\n"
					         << content.str();

					length = response.str().length(); // add one for the terminating NULL
					result = SDLNet_TCP_Send(new_tcpsock, response.str().data(), length);
					SDLNet_TCP_Close(new_tcpsock);

					if (result < len)
					{
						printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
						// It may be good to disconnect sock because it is likely invalid now.
					}
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

	bool ConnectTo(CTmpNetworkClient* pClient)
	{
		auto ss = new CTmpServerSlot;
		m_pFactory->CreateServerSlot(ss);
		return true;
	}
};

class CCompressionHelper : public ICompressionHelper
{
public:
	CCompressionHelper(CNetwork* pNetwork)
	    : m_pNetwork(pNetwork)
	{
	}

public:
	CNetwork*    m_pNetwork;

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
		DWORD i  = 0;
		bool  ok = false;
		for (; i < indwStringSize && !inStream.EOS(); i++)
		{
			if (inStream.ReadBits((BYTE*)(outszString + i), BYTES2BITS(sizeof(char))))
			{
				if (outszString[i] == 0)
				{
					outszString[i + 1] = 0;
					ok                 = true;
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
	CTmpNetworkClient(CNetwork* pNetwork)
	    : m_pNetwork(pNetwork)
	{
	}
	~CTmpNetworkClient()
	{
	}
	// Inherited via IClient
	virtual void Connect(const char* szIP, uint16_t wPort, const uint8_t* pbAuthorizationID, unsigned int iAuthorizationSize) override
	{
		CryLog("connect to: ip %s", szIP);
	}
	virtual void Disconnect(const char* szCause) override
	{
		m_pNetwork->UnregisterClient(this);
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
//FIXME:
#if 0
		return false;
#else
		return true;
#endif
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
		if (this)
			delete this;
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
		g_Network.CDKeyAuth(this);
	}
	virtual void OnCDKeyAuthorization(uint8_t* pbAuthorizationID) override
	{
	}
	virtual void SetServerIP(const char* szServerIP) override
	{
	}
	CNetwork* m_pNetwork{};
};

CNetwork::CNetwork()
    : m_pSystem(nullptr)
{
}

CNetwork::~CNetwork()
{
	for (int i = 0; i < m_Servers.size(); i++)
	{
		m_Servers[i]->Release();
	}
	for (int i = 0; i < m_Clients.size(); i++)
	{
		m_Clients[i]->Release();
	}
	SAFE_DELETE(m_pCompressionHelper);
	SDLNet_Quit();
}

bool CNetwork::Init(ISystem* pSystem)
{
	SDL_version        compile_version;
	const SDL_version* link_version = SDLNet_Linked_Version();
	SDL_NET_VERSION(&compile_version);
	Env::Log()->Log("Compiled with SDL_net version: %d.%d.%d",
	                compile_version.major,
	                compile_version.minor,
	                compile_version.patch);
	Env::Log()->Log("Running with SDL_net version: %d.%d.%d",
	                link_version->major,
	                link_version->minor,
	                link_version->patch);
	bool res = true;
	if (SDL_Init(0) != -1)
	{
		if (SDLNet_Init() == -1)
		{
			Env::Log()->Log("SDLNet_Init: %s\n", SDLNet_GetError());
			res = false;
		}
	}
	else
	{
		Env::Log()->Log("SDL_Init: %s\n", SDL_GetError());
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
	auto result = new CTmpNetworkClient(this);
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
	assert(this != nullptr);
	delete this;
}

void CNetwork::GetMemoryUsage(ICrySizer* pSizer)
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
	for (auto& server : m_Servers)
	{
		server->Update(0);
	}
	for (auto& client : m_Clients)
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

void CNetwork::UnregisterClient(CTmpNetworkClient* pClient)
{
	if (auto it = std::find_if(
	        m_Clients.begin(), m_Clients.end(), [pClient](IClient* cl)
	        { return cl == pClient; });
	    it != m_Clients.end())
	{
		m_Clients.erase(it);
	}
}

void CNetwork::UnregisterServer(CTmpNetworkServer* pServer)
{
	if (auto it = std::find_if(
	        m_Servers.begin(), m_Servers.end(), [pServer](IServer* cl)
	        { return cl == pServer; });
	    it != m_Servers.end())
	{
		m_Servers.erase(it);
	}
}

void CNetwork::CDKeyAuth(IClient* pClient)
{
	m_Servers[0]->ConnectTo((CTmpNetworkClient*)pClient);
}

NETWORK_API INetwork* CreateNetwork(ISystem* pSystem)
{
	if (!g_Network.Init(pSystem))
	{
		g_Network.Release();
	}
	return &g_Network;
}
