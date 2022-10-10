#include "ClientServer.h"

namespace network
{
	Socket Socket::CreateListen(const char* port)
	{
		int     iResult;

		Socket  Result;
		WSADATA wsaData;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
			printf("WSAStartup failed: %d\n", iResult);
			return Socket();
		}
		////////////////////////////
		struct addrinfo *result = NULL, *ptr = NULL, hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family   = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags    = AI_PASSIVE;

		// Resolve the local address and port to be used by the server
		iResult           = getaddrinfo(NULL, port, &hints, &result);
		if (iResult != 0)
		{
			printf("getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			return Socket();
		}

		// Create a SOCKET for the server to listen for client connections

		Result.m_Socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (Result.m_Socket == INVALID_SOCKET)
		{
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return Socket();
		}

		// Setup the TCP listening socket
		iResult = bind(Result.m_Socket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(result);
			closesocket(Result.m_Socket);
			WSACleanup();
			return Socket();
		}
		freeaddrinfo(result);

		if (listen(Result.m_Socket, SOMAXCONN) == SOCKET_ERROR)
		{
			printf("Listen failed with error: %ld\n", WSAGetLastError());
			closesocket(Result.m_Socket);
			WSACleanup();
			return Socket();
		}

		Result.m_bRunning = true;
		return Result;
	}

	bool Socket::Connect(std::string_view address, std::string_view port)
	{
		int              iResult = 0;
		struct addrinfo *result  = NULL,
		                *ptr     = NULL,
		                hints;

		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family   = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		///

		// Resolve the server address and port
		iResult           = getaddrinfo(address.data(), port.data(), &hints, &result);
		if (iResult != 0)
		{
			printf("getaddrinfo failed: %d\n", iResult);
			return false;
		}

		// Attempt to connect to the first address returned by
		// the call to getaddrinfo
		ptr      = result;

		// Create a SOCKET for connecting to server
		m_Socket = socket(ptr->ai_family, ptr->ai_socktype,
		                  ptr->ai_protocol);

		if (m_Socket == INVALID_SOCKET)
		{
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			return false;
		}

		// Connect to server.
		iResult = connect(m_Socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
		}

		// Should really try the next address returned by getaddrinfo
		// if the connect call failed
		// But for this simple example we just free the resources
		// returned by getaddrinfo and print an error message

		freeaddrinfo(result);

		if (m_Socket == INVALID_SOCKET)
		{
			printf("Unable to connect to server!\n");
			return false;
		}

		m_bRunning   = true;

		m_recvBuffer = CStream(500, &sa);
		m_sendBuffer = CStream(500, &sa);

		return true;
	}

	bool Socket::Disconnect()
	{
		if (m_bRunning)
		{
			m_bRunning = false;
			return true;
		}
		else
		{
			return false;
		}
	}

	void Socket::Term()
	{
		Disconnect();
	}

	void Socket::Update()
	{
		std::lock_guard lock(m_DifferedTasksLock);
		{
			for (auto& task : m_DifferedTasks)
			{
				task();
			}
			m_DifferedTasks.clear();
		}

		if (m_recvBuffer.GetSize() > 0)
		{
			std::lock_guard lock(m_RecvLock);
			OnData(m_recvBuffer);
			m_recvBuffer.Reset();
		}
	}

	void Socket::Send(char* data, size_t len)
	{
		std::lock_guard lock(m_SendLock);

		m_sendBuffer.WriteData(data, len);
	}

	Socket Socket::Acept()
	{
		Socket result;
		if (auto s = accept(m_Socket, NULL, NULL); s != INVALID_SOCKET)
		{
			result = Socket(s);
		}
		else
		{
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(m_Socket);
		}
		result.m_bRunning = true;
		return result;
	}

	void Socket::ListenThread()
	{
		while (m_bRunning)
		{
			FD_SET ReadSet;
			// Prepare the Read and Write socket sets for network I/O notification
			FD_ZERO(&ReadSet);
			FD_SET(m_Socket, &ReadSet);
			//FD_ZERO(&WriteSet);
			timeval tv{0, 1000};
			auto    res = select(0, &ReadSet, nullptr, nullptr, &tv);

			if (res > 0)
			{
				if (FD_ISSET(m_Socket, &ReadSet))
				{
					if (auto Socket = Acept(); Socket.IsValid())
					{
						OnNewConnection(Socket);
					}
				}
			}
		}
		Close();
	}

	void Socket::ThreadFunc()
	{
		//FD_SET WriteSet;
		std::string connectionError;
		auto        OnDisconnectTask = [this](const char* szCause) { /*m_thread.join();*/ this->OnDisconnect(szCause); };
		while (m_bRunning)
		{
			{
				std::lock_guard lock(m_SendLock);
				if (m_sendBuffer.GetSize() > 0)
				{
					auto bufSize = m_sendBuffer.GetSize() / 8;
					auto len     = send(m_Socket, (char*)m_sendBuffer.GetPtr(), bufSize, 0);
					if (len < bufSize)
					{
						assert(false && "Wrong send size");
					}
					m_sendBuffer.Reset();
				}
				//Sleep(15);
			}
			const size_t BUF_LEN = 254;
			char         buf[BUF_LEN];

			FD_SET       ReadSet;
			// Prepare the Read and Write socket sets for network I/O notification
			FD_ZERO(&ReadSet);
			FD_SET(m_Socket, &ReadSet);
			//FD_ZERO(&WriteSet);
			timeval tv{0, 1000};
			auto    res = select(0, &ReadSet, nullptr, nullptr, &tv);
			if (res > 0)
			{
				if (FD_ISSET(m_Socket, &ReadSet))
				{
					CDefaultStreamAllocator sa;
					CStream                 stm(300, &sa);
					std::lock_guard         lock(m_RecvLock);
					while (true)
					{
						auto len = recv(m_Socket, buf, BUF_LEN, 0);
						if (len == 0)
						{
							connectionError = "Connection closed by remote peer";
							m_DifferedTasks.push_back([OnDisconnectTask, connectionError]
							                          { OnDisconnectTask(connectionError.c_str()); });
							m_bRunning = false;
							break;
						}
						else if (len <= BUF_LEN)
						{
							//auto size = stm.GetSize();
							m_recvBuffer.WriteData(buf, len);
							//memcpy(&stm.GetPtr()[size], buf, len);
							break;
						}
						else
						{
							auto error      = WSAGetLastError();
							connectionError = "Unexpected connection error, error code: " + std::to_string(error);
							m_DifferedTasks.push_back([OnDisconnectTask, connectionError]
							                          { OnDisconnectTask(connectionError.c_str()); });
							m_bRunning = false;
							break;
						}
					}
					//Sleep(15);
					//m_recvBuffer = stm;
				}
			}
		}
		Close();
	}

	void Socket::Close()
	{
		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
	}

} // namespace network
