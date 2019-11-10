#if 0

#include <BlackBox/Game/Game.hpp>
#include <BlackBox/IConsole.hpp>

#include <SDL2/SDL_net.h>
#include <memory>
#include <algorithm>

class CClient : public IClient
{
public:
	CClient(CGame *pGame)
		:
		m_pGame(pGame)
	{

	}
	~CClient()
	{
		if (connected)
		{
			SDLNet_TCP_Close(m_pServer);
		}
	}
	// Унаследовано через IClient
	virtual bool Init() override
	{
		m_Buffer.resize(1024);
		return true;
	}
	virtual bool Connect(const char* addr, int port) override
	{
		IPaddress ipaddress;
		if (SDLNet_ResolveHost(&ipaddress, addr, port) != 0)
			return false;
		m_pGame->SetHostType(CGame::CLIENT);
		m_pServer = SDLNet_TCP_Open(&ipaddress);
		if (!m_pServer)
			return false;
		connected = true;
		return true;
	}
	virtual bool Disconnect() override
	{
		SDLNet_TCP_Close(m_pServer);
		return true;
	}
	virtual bool Send(std::string const& str) override
	{
		if (!connected)
			return false;
		if (SDLNet_TCP_Send(m_pServer, str.c_str(), str.size()) < str.size())
		{
			GetISystem()->GetIConsole()->PrintLine("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
			return false;
		}
		return true;
	}
	virtual std::string const& Response() override
	{

		int result;
		if (!connected)
			return "";
		m_Message = "";
		result = SDLNet_TCP_Recv(m_pServer, &m_Buffer[0], m_Buffer.size());
		if (result <= 0)
		{
			// TCP Connection is broken. (because of error or closure)
			SDLNet_TCP_Close(m_pServer);
			connected = false;
			m_Message = "";
		}
		else
		{
			m_Message = m_Buffer;
			//std::copy(m_Buffer.begin(), m_Buffer.end(), m_Message.begin());
		}
		auto err = SDLNet_GetError();
		return m_Message;
	}
	virtual bool Update() override
	{
		return false;
	}
	bool connected = false;
	TCPsocket m_pClient;
	TCPsocket m_pServer;
	
	std::string m_Message;
	std::string m_Buffer;

	CGame* m_pGame;
};

class CServer : public IServer
{
public:
	CServer(CGame *pGame)
		:
		m_pGame(pGame)
	{
		m_pConsole = GetISystem()->GetIConsole();
		m_Buffer.resize(1024);
	}
	~CServer()
	{
		if (connected)
		{
			SDLNet_TCP_Close(m_pClient);
		}
	}
	// Унаследовано через IServer
	virtual bool Create(int port) override
	{
		IPaddress ipaddress;
		if (SDLNet_ResolveHost(&ipaddress, NULL, port) != 0)
			return false;
		m_pServer = SDLNet_TCP_Open(&ipaddress);
		m_pGame->SetHostType(CGame::SERVER);
		return true;
	}
	virtual bool Send(std::string const& str) override
	{
		return SDLNet_TCP_Send(m_pClient, str.c_str(), str.size()) == str.size();
	}
	virtual std::string const& Response() override
	{
		int result;
		if (!connected)
			return "";
		result = SDLNet_TCP_Recv(m_pClient, &m_Buffer[0], m_Buffer.size());
		if (result <= 0)
		{
			// TCP Connection is broken. (because of error or closure)
			SDLNet_TCP_Close(m_pClient);
			connected = false;
			m_Message = "";
		}
		else
		{
			m_Message = m_Buffer;
		}
		return m_Message;
	}
	virtual bool Update() override
	{
		if (!connected)
		{
			Accept();
		}
		if (Response().size() > 0)
		{
			m_pConsole->PrintLine(m_Message.c_str());
			Send(m_Message);
			//m_Message = "";
		}
		return true;
	}

	void Accept()
	{
		TCPsocket new_tcpsock;
		std::string resp  = "Test String";

		new_tcpsock = SDLNet_TCP_Accept(m_pServer);
		if (!new_tcpsock) {
			;// printf("SDLNet_TCP_Accept: %s\n", SDLNet_GetError());
		}
		else {
			// communicate over new_tcpsock
			m_pClient = new_tcpsock;
			connected = true;
		}
	}
	bool connected = false;
	bool started = false;
	TCPsocket m_pClient;
	TCPsocket m_pServer;
	std::string m_Message;
	std::string m_Buffer;

	CGame* m_pGame;
	IConsole* m_pConsole;

	// Унаследовано через IServer
	virtual bool Start() override
	{
		return true;
	}

	// Унаследовано через IServer
	virtual bool Stop() override
	{
		return true;
	}
};

class CNetwork : public INetwork
{
public:
	CNetwork(CGame *pGame)
		:
		m_pGame(pGame)
	{
		
	}
	~CNetwork()
	{
		SDLNet_Quit();
	}
	// Унаследовано через INetwork
	virtual bool Init() override
	{
		if (SDLNet_Init() < 0) {
			std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
			return false;
		}
		else
			return true;
	}
	virtual IClient* CreateClient() override
	{
		return new CClient(m_pGame);
	}
	virtual IServer* CreateServer() override
	{
		return new CServer(m_pGame);
	}
	virtual bool Update() override
	{
		return false;
	}
	CGame* m_pGame;
};

INetwork* CreateNetwork(ISystem *pSystem)
{
	return new CNetwork(dynamic_cast<CGame*>(pSystem->GetIGame()));
}

#endif
