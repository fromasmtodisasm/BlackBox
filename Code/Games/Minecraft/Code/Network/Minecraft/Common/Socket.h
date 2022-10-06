#include <winsock2.h>
#include <ws2tcpip.h>

#include <assert.h>
#include <algorithm>

namespace network
{
	inline bool InitSockets()
	{
		WSADATA wsaData;
		// Initialize Winsock
		auto iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed: %d\n", iResult);
			return false;
		}
		return true;

	}
	inline void ShutdownSockets()
	{
		WSACleanup();
	}

	struct Socket
	{
		using OnDataCb = std::function<void(char* data, size_t len)>;
		using OnDisconnectCB = std::function<void()>;
		using OnConnectCB = std::function<void(network::Socket& s)>;
		enum Type
		{
			TCP,
			UDP
		};
		/// //
		Socket()
		{
		}
		Socket(SOCKET s) : m_Socket(s)
		{

		}
		~Socket()
		{
			Disconnect();
		}
		static Socket CreateListen();

		bool Connect(std::string_view address, std::string_view port)
		{
			int iResult = 0;
			struct addrinfo* result = NULL,
				* ptr = NULL,
				hints;

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_UNSPEC;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
			///

			// Resolve the server address and port
			iResult = getaddrinfo(address.data(), port.data(), &hints, &result);
			if (iResult != 0) {
				printf("getaddrinfo failed: %d\n", iResult);
				return false;
			}

			// Attempt to connect to the first address returned by
			// the call to getaddrinfo
			ptr = result;

			// Create a SOCKET for connecting to server
			m_Socket = socket(ptr->ai_family, ptr->ai_socktype,
				ptr->ai_protocol);

			if (m_Socket == INVALID_SOCKET) {
				printf("Error at socket(): %ld\n", WSAGetLastError());
				freeaddrinfo(result);
				return false;
			}

			// Connect to server.
			iResult = connect(m_Socket, ptr->ai_addr, (int)ptr->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(m_Socket);
				m_Socket = INVALID_SOCKET;
			}

			// Should really try the next address returned by getaddrinfo
			// if the connect call failed
			// But for this simple example we just free the resources
			// returned by getaddrinfo and print an error message

			freeaddrinfo(result);

			if (m_Socket == INVALID_SOCKET) {
				printf("Unable to connect to server!\n");
				return false;
			}

			m_bRunning = true;
			return true;

		}
		bool Disconnect()
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
		void Term()
		{
			WSACleanup();
		}

		void Update()
		{
			std::lock_guard lock(m_DifferedTasksLock);
			{
				for (auto& task : m_DifferedTasks)
				{
					task();
				}
				m_DifferedTasks.clear();
			}

			if (m_recvBuffer.size() > 0)
			{
				std::lock_guard lock(m_RecvLock);
				OnData(&m_recvBuffer[0], m_recvBuffer.size());
				m_recvBuffer.resize(0);
			}
		}

		void Send(char* data, size_t len)
		{
			std::lock_guard lock(m_SendLock);

			auto size = m_sendBuffer.size();
			m_sendBuffer.resize(size + len);
			memcpy(&m_sendBuffer[size], data, len);
		}

		bool IsValid() { return m_Socket != INVALID_SOCKET; }

		Socket Acept()
		{
			Socket result;
			if (auto s = accept(m_Socket, NULL, NULL); s != INVALID_SOCKET) {
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
		void ListenThread()
		{
			while (m_bRunning)
			{

				FD_SET ReadSet;
				// Prepare the Read and Write socket sets for network I/O notification
				FD_ZERO(&ReadSet);
				FD_SET(m_Socket, &ReadSet);
				//FD_ZERO(&WriteSet);
				timeval tv{ 0, 1000 };
				auto res = select(0, &ReadSet, nullptr, nullptr, &tv);

				if (res > 0)
				{
					if (FD_ISSET(m_Socket, &ReadSet))
					{
						if (auto Socket = Acept(); Socket.IsValid()) {
							OnNewConnection(Socket);
						}
					}
				}
			}
		}
		void ThreadFunc()
		{

			//FD_SET WriteSet;
			auto OnDisconnect = [this] { /*m_thread.join();*/ this->OnDisconnect(); };
			while (m_bRunning)
			{
				{
					std::lock_guard lock(m_SendLock);
					if (m_sendBuffer.size() > 0)
					{
						auto len = send(m_Socket, m_sendBuffer.data(), m_sendBuffer.size(), 0);
						if (len < m_sendBuffer.size())
						{
							assert(false && "Wrong send size");
						}
						m_sendBuffer.clear();
					}

				}
				const size_t BUF_LEN = 254;
				char buf[BUF_LEN];

				FD_SET ReadSet;
				// Prepare the Read and Write socket sets for network I/O notification
				FD_ZERO(&ReadSet);
				FD_SET(m_Socket, &ReadSet);
				//FD_ZERO(&WriteSet);
				timeval tv{ 0, 1000 };
				auto res = select(0, &ReadSet, nullptr, nullptr, &tv);
				if (res > 0)
				{
					if (FD_ISSET(m_Socket, &ReadSet))
					{
						while (true)
						{
							auto len = recv(m_Socket, buf, BUF_LEN, 0);
							if (len == 0)
							{
								m_DifferedTasks.push_back(OnDisconnect);
								m_bRunning = false;
								break;
							}
							else if (len <= BUF_LEN)
							{
								std::lock_guard lock(m_RecvLock);
								auto size = m_recvBuffer.size();
								m_recvBuffer.resize(size + len);
								memcpy(&m_recvBuffer[size], buf, len);
								break;
							}
							else
							{
								auto error = WSAGetLastError();
								printf("recv error: %d", error);
								m_DifferedTasks.push_back(OnDisconnect);
								m_bRunning = false;
								break;
							}

						}

					}
				}

			}
			shutdown(m_Socket, SD_BOTH);
			closesocket(m_Socket);
		}
		//////////////////////////////////////////////
		Socket(Socket&& other) noexcept
		{
			m_Socket = other.m_Socket;
			m_Initialized = other.m_Initialized;
			m_bRunning = other.m_bRunning;
			m_DifferedTasks = other.m_DifferedTasks;
			m_DifferedTasks = std::move(other.m_DifferedTasks);

			//m_DifferedTasksLock = other.m_DifferedTasksLock;

			//m_RecvLock = std::move(other.m_RecvLock);
			//m_SendLock = std::move(other.m_SendLock);

			m_recvBuffer = std::move(other.m_recvBuffer);
			m_sendBuffer = std::move(other.m_sendBuffer);

			OnData = std::move(other.OnData);
			OnDisconnect = std::move(other.OnDisconnect);
			OnNewConnection = std::move(other.OnNewConnection);
		}

		Socket& operator=(Socket&& other)
		{
			m_Socket = other.m_Socket;
			m_Initialized = other.m_Initialized;
			m_bRunning = other.m_bRunning;
			m_DifferedTasks = other.m_DifferedTasks;
			m_DifferedTasks = std::move(other.m_DifferedTasks);

			//m_DifferedTasksLock = other.m_DifferedTasksLock;

			//m_RecvLock = std::move(other.m_RecvLock);
			//m_SendLock = std::move(other.m_SendLock);

			m_recvBuffer = std::move(other.m_recvBuffer);
			m_sendBuffer = std::move(other.m_sendBuffer);

			OnData = std::move(other.OnData);
			OnDisconnect = std::move(other.OnDisconnect);
			OnNewConnection = std::move(other.OnNewConnection);
			return *this;
		}

		/// <summary>
		/// /////////////////////////////////////////////////////////////////////////////
		/// </summary>
		SOCKET m_Socket = INVALID_SOCKET;
		bool m_Initialized = false;
		bool m_bRunning = false;

		std::vector<std::function<void()>> m_DifferedTasks;
		std::mutex m_DifferedTasksLock;


		std::mutex m_RecvLock;
		std::mutex m_SendLock;

		std::vector<char> m_recvBuffer;
		std::vector<char> m_sendBuffer;

		OnDataCb OnData;
		OnDisconnectCB OnDisconnect;
		OnConnectCB OnNewConnection;
	};
}
