// ClientServer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "ClientServer.h"

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

	void OnNewConnection(SOCKET socket);

	void OnDisconnect(ServerSlot* slot, const char* why);

	void CheckConnections();

	void ThreadFunc();

	void Update();

	WSADATA wsaData;
	SOCKET ListenSocket = INVALID_SOCKET;
	int NumPlayers;

	std::vector<ServerSlot*> clients;


	bool m_bRunning = true;
	std::thread m_thread;


	std::mutex m_ClientsLock;
	std::vector<ServerSlot*> m_toDisconnect;

};

struct ServerSlot
{
	ServerSlot() = default;
	~ServerSlot()
	{
		if (m_bRunning)
		{
			m_thread.join();
		}
		closesocket(m_socket);
	}

	ServerSlot(SOCKET socket, Server* server);

	void Start();
	void Stop()
	{
		m_bRunning = false;
	}


	void ThreadFunc();

	SOCKET m_socket;
	Server* m_server;
	std::thread m_thread;
	std::mutex m_lock;

	std::mutex m_disconnectLock;

	bool m_bRunning = true;

	std::string name;
	size_t id;

	std::mutex m_RecvLock;
	std::mutex m_SendLock;

	std::vector<char> m_recvBuffer;
	std::vector<char> m_sendBuffer;

};


