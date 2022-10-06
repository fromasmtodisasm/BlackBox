#include "Server.h"
#include "Server.h"
#include "Server.h"


Server::Server(int numPlayers) : NumPlayers(numPlayers) {

}

void Server::Stop()
{
	m_bRunning = false;
	m_thread.join();
}
bool Server::Start()
{
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return false;
	}
	////////////////////////////
#define DEFAULT_PORT "27015"

	struct addrinfo* result = NULL, * ptr = NULL, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return false;
	}

	// Create a SOCKET for the server to listen for client connections

	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}
	freeaddrinfo(result);

	if (listen(ListenSocket, NumPlayers) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return false;
	}


	m_thread = std::thread([this] {ThreadFunc(); });
	return true;
}

void Server::OnNewConnection(SOCKET client)
{
	auto slot = new ServerSlot(client, this);

	///
	std::stringstream ss;
	ss << "New user connected with id: " << clients.size() << "\n";
	for (auto& c : clients)
	{
		send(c->m_socket, ss.str().c_str(), ss.str().size(), 0);
	}

	printf("%s", ss.str().c_str());
	///

	slot->Start();
	std::lock_guard lock(m_ClientsLock);
	clients.emplace_back(slot);

}

void Server::OnDisconnect(ServerSlot* slot, const char* why)
{
	std::lock_guard lock(m_ClientsLock);
	m_toDisconnect.push_back(slot);
	if (auto it = std::find(clients.begin(), clients.end(), slot); it != clients.end())
	{
		clients.erase(it);
	}

}

void Server::CheckConnections()
{

}

void Server::ThreadFunc()
{
	while (m_bRunning)
	{
		if (clients.size() < NumPlayers)
		{

			SOCKET ClientSocket;
			ClientSocket = INVALID_SOCKET;

			FD_SET ReadSet;
			// Prepare the Read and Write socket sets for network I/O notification
			FD_ZERO(&ReadSet);
			FD_SET(ListenSocket, &ReadSet);
			//FD_ZERO(&WriteSet);
			timeval tv{ 0, 1000 };
			auto res = select(0, &ReadSet, nullptr, nullptr, &tv);

			if (res > 0)
			{
				if (FD_ISSET(ListenSocket, &ReadSet))
				{
					ClientSocket = accept(ListenSocket, NULL, NULL);
					if (ClientSocket == INVALID_SOCKET) {
						printf("accept failed: %d\n", WSAGetLastError());
						closesocket(ListenSocket);
						WSACleanup();
						return;
					}
					else
					{
						OnNewConnection(ClientSocket);
					}
				}
			}
			// Accept a client socket
		}

	}


}

void Server::Update()
{
	{
		std::lock_guard lock(m_ClientsLock);
		for (size_t i = 0; i < m_toDisconnect.size(); i++)
		{
			auto slot = m_toDisconnect[i];
			printf("Connection closed  for %s", slot->name.c_str());
			slot->Stop();
			slot->m_thread.join();
			delete m_toDisconnect[i];
		}
		m_toDisconnect.clear();
	}
	ImGui::Begin("Clients");                          // Create a window called "Hello, world!" and append into it.
	for (auto& c : clients)
	{

	}
	ImGui::End();
}

ServerSlot::ServerSlot(SOCKET socket, Server* server) :m_socket(socket), m_server(server)
{

}

void ServerSlot::Start()
{
	m_thread = std::thread([this] {ThreadFunc(); });
}

void ServerSlot::ThreadFunc()
{
	while (m_bRunning)
	{
		{
			std::lock_guard lock(m_SendLock);
			if (m_sendBuffer.size() > 0)
			{
				send(m_socket, m_sendBuffer.data(), m_sendBuffer.size(), 0);
			}

		}

		const size_t BUF_LEN = 254;
		char buf[BUF_LEN];

		FD_SET ReadSet;
		// Prepare the Read and Write socket sets for network I/O notification
		FD_ZERO(&ReadSet);
		FD_SET(m_socket, &ReadSet);
		//FD_ZERO(&WriteSet);
		timeval tv{ 0, 1000 };

		auto res = select(0, &ReadSet, nullptr, nullptr, &tv);
		if (res > 0)
		{
			int recv_size = 0;
			while (true)
			{
				auto len = recv(m_socket, buf, BUF_LEN, 0);
				if (len == 0)
				{
					m_server->OnDisconnect(this, "Connection closed");
					break;
				}
				else if (len <= BUF_LEN)
				{
					// copy data
					std::lock_guard lock(m_RecvLock);
					auto size = m_recvBuffer.size();
					m_recvBuffer.resize(size + len);
					memcpy(&m_recvBuffer[size], buf, len);
				}
				else
				{
					//auto error = WSAGetLastError();
					//printf("recv error: %d", error);
					//closesocket(ConnectSocket);
					m_server->OnDisconnect(this, "Network error");
					break;
				}

			}
		}

	}
}

///////////////////////

void ServerSlot::Send(char* data, size_t len)
{
	m_Socket.Send(data, len);
}

// Main code
int main(int, char**)
{
	Server server(10);
	if (server.Start())
	{
		MainLoop mainLoop{ {"Server"}, [&server] {server.Update(); } };
		mainLoop.Start();
	}
	return 0;
}


