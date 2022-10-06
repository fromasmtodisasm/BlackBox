// ClientServer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <Network/Minecraft/Common/ClientServer.h>

// TODO: Reference additional headers your program requires here.

struct ServerSlot;

struct Server
{

	Server(int numPlayers);
	~Server()
	{
		Stop();
	}


	bool Start();
	void Stop();

	void OnNewConnection(network::Socket& socket);

	void OnDisconnect(ServerSlot* slot, const char* why);

	void DisconnectPending();

	void CheckConnections();

	void ThreadFunc();

	void Update();

	std::function<bool(ServerSlot* slot)> OnCreateServerSlot;
	std::function<bool(ServerSlot* slot)> OnDisconnectServerSlot;

	network::Socket m_Socket;

	int NumPlayers;

	std::vector<ServerSlot*> clients;


	//bool m_bRunning = true;
	std::thread m_thread;


	std::mutex m_ClientsLock;
	std::vector<ServerSlot*> m_toDisconnect;

};

struct ServerSlot
{
	ServerSlot() = default;
	~ServerSlot()
	{
	}

	ServerSlot(network::Socket& socket, Server* server);

	void Start();
	void Stop()
	{
		m_bRunning = false;
	}
	void Disconnect()
	{
		m_server->OnDisconnect(this, "wtf");
		Stop();
	}

	void Update()
	{
		m_Socket.Update();
	}

	///////////////////////
	void ThreadFunc();
	///////////////////////
	void Send(char* data, size_t len)
	{
		m_Socket.Send(data, len);
	}
	///////////////////////
	std::function<void(char* data, size_t len)> OnData;
	///////////////////////


	network::Socket m_Socket;
	Server* m_server;
	bool m_bRunning = true;
	std::string name;
	size_t id;
	std::thread m_thread;

};

struct GameServer;
struct GameServerSlot
{
	GameServerSlot(ServerSlot* slot, GameServer* parent) : slot(slot), parent(parent)
	{
		slot->OnData = [this](char* data, size_t len) { ProcessIncomming(data, len); };
	}

	~GameServerSlot()
	{
	}

	void OnMessage(Stream& stm);
	void NotifyConnect();
	void OnAuth(Stream& stm);

	void Send(Writer& w);
	void ProcessIncomming(char* data, size_t len)
	{
		Stream stm(data, len);
#if 1
		network::Client message;
		if (!stm.Read(message))
			return;
		switch (message)
		{
		case network::Client::AUTH:
			OnAuth(stm);
			break;
		case network::Client::MESSAGE:
		{
			OnMessage(stm);
		}
		break;
		default:
			break;
		}
#endif
	}

	std::string name;
	size_t id = 0;
	ServerSlot* slot;
	GameServer* parent;

	bool isReady = false;
};

struct GameServer
{
	GameServer();
	void Update();
	void LoadUsers();
	void BroadCast(Writer& w, GameServerSlot* exclude = nullptr);

	Server server;
	bool isStarted;

	std::vector<GameServerSlot*> m_Slots;

	std::vector<User> m_Users;
};


