#include "ClientServer.h"


namespace network
{
	Socket Socket::CreateListen()
	{
		int iResult;

		Socket Result;
		WSADATA wsaData;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed: %d\n", iResult);
			return Socket();
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
			return Socket();
		}

		// Create a SOCKET for the server to listen for client connections

		Result.m_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (Result.m_Socket == INVALID_SOCKET) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return Socket();
		}

		// Setup the TCP listening socket
		iResult = bind(Result.m_Socket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(Result.m_Socket);
			WSACleanup();
			return Socket();
		}
		freeaddrinfo(result);

		if (listen(Result.m_Socket, SOMAXCONN) == SOCKET_ERROR) {
			printf("Listen failed with error: %ld\n", WSAGetLastError());
			closesocket(Result.m_Socket);
			WSACleanup();
			return Socket();
		}


		Result.m_bRunning = true;
		return Result;
	}

}
