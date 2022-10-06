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
	~Server();

	bool                                  Start();
	void                                  Stop();

	void                                  OnNewConnection(network::Socket& socket);

	void                                  OnDisconnect(ServerSlot* slot, const char* why);

	void                                  DisconnectPending();

	void                                  CheckConnections();

	void                                  ThreadFunc();

	void                                  Update();

	std::function<bool(ServerSlot* slot)> OnCreateServerSlot;
	std::function<bool(ServerSlot* slot)> OnDisconnectServerSlot;

	network::Socket                       m_Socket;

	int                                   NumPlayers;

	std::vector<ServerSlot*>              clients;

	//bool m_bRunning = true;
	std::thread                           m_thread;

	std::mutex                            m_ClientsLock;
	std::vector<ServerSlot*>              m_toDisconnect;
};

struct ServerSlot
{
	ServerSlot() = default;
	~ServerSlot();

	ServerSlot(network::Socket& socket, Server* server);

	void                              Start();
	void                              Stop();
	void                              Disconnect();

	void                              Update();

	///////////////////////
	void                              ThreadFunc();
	///////////////////////
	void                              Send(CStream& stm);
	///////////////////////
	std::function<void(CStream& stm)> OnData;
	///////////////////////

	network::Socket                   m_Socket;
	Server*                           m_server;
	bool                              m_bRunning = true;
	std::string                       name;
	size_t                            id;
	std::thread                       m_thread;
};

struct GameServer;
struct GameServerSlot
{
	GameServerSlot(ServerSlot* slot, GameServer* parent);

	~GameServerSlot();

	void        OnMessage(CStream& stm);
	void        NotifyConnect();
	void        OnAuth(CStream& stm);

	void        Send(CStream& w);
	void        ProcessIncomming(CStream& stm);

	std::string name;
	size_t      id = 0;
	ServerSlot* slot;
	GameServer* parent;

	bool        isReady = false;
};

struct GameServer
{
	GameServer();
	void                         Update();
	void                         LoadUsers();
	void                         BroadCast(CStream& w, GameServerSlot* exclude = nullptr);

	Server                       server;
	bool                         isStarted;

	std::vector<GameServerSlot*> m_Slots;

	std::vector<User>            m_Users;
};
