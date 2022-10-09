#pragma once
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
		auto    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
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
		using OnDataCb       = std::function<void(CStream& stm)>;
		using OnDisconnectCB = std::function<void(const char *szCause)>;
		using OnConnectCB    = std::function<void(network::Socket& s)>;
		enum Type
		{
			TCP,
			UDP
		};
		/// //
		Socket()
		    : m_recvBuffer(500, &sa)
		    , m_sendBuffer(500, &sa)
		{
		}
		Socket(SOCKET s)
		    : m_Socket(s)
		    , m_recvBuffer(500, &sa)
		    , m_sendBuffer(500, &sa)
		{
		}
		~Socket()
		{
			Disconnect();
		}
		static Socket CreateListen(const char* port);

		bool          Connect(std::string_view address, std::string_view port);
		bool          Disconnect();
		void          Term();

		void          Update();

		void          Send(char* data, size_t len);
		void          Send(CStream& stm)
		{
			Send((char*)stm.GetPtr(), stm.GetSize() / 8);
		}

		bool   IsValid() { return m_Socket != INVALID_SOCKET; }

		Socket Acept();
		void   ListenThread();
		void   ThreadFunc();
		//////////////////////////////////////////////
		Socket(Socket&& other) noexcept
		{
			m_Socket        = other.m_Socket;
			m_Initialized   = other.m_Initialized;
			m_bRunning      = other.m_bRunning;
			m_DifferedTasks = other.m_DifferedTasks;
			m_DifferedTasks = std::move(other.m_DifferedTasks);

//m_DifferedTasksLock = other.m_DifferedTasksLock;

//m_RecvLock = std::move(other.m_RecvLock);
//m_SendLock = std::move(other.m_SendLock);

//FIXME: rewrite this!!!
#if 0
			m_recvBuffer    = std::move(other.m_recvBuffer);
			m_sendBuffer    = std::move(other.m_sendBuffer);
#else
			m_recvBuffer = CStream(500, &sa);
			m_sendBuffer = CStream(500, &sa);
#endif

			OnData          = std::move(other.OnData);
			OnDisconnect    = std::move(other.OnDisconnect);
			OnNewConnection = std::move(other.OnNewConnection);
		}

		Socket& operator=(Socket&& other)
		{
			m_Socket        = other.m_Socket;
			m_Initialized   = other.m_Initialized;
			m_bRunning      = other.m_bRunning;
			m_DifferedTasks = other.m_DifferedTasks;
			m_DifferedTasks = std::move(other.m_DifferedTasks);

			//m_DifferedTasksLock = other.m_DifferedTasksLock;

			//m_RecvLock = std::move(other.m_RecvLock);
			//m_SendLock = std::move(other.m_SendLock);

#if 0
			m_recvBuffer    = std::move(other.m_recvBuffer);
			m_sendBuffer    = std::move(other.m_sendBuffer);
#else
			m_recvBuffer = CStream(500, &sa);
			m_sendBuffer = CStream(500, &sa);
#endif

			OnData          = std::move(other.OnData);
			OnDisconnect    = std::move(other.OnDisconnect);
			OnNewConnection = std::move(other.OnNewConnection);
			return *this;
		}

		/// <summary>
		/// /////////////////////////////////////////////////////////////////////////////
		/// </summary>
		SOCKET                             m_Socket      = INVALID_SOCKET;
		bool                               m_Initialized = false;
		bool                               m_bRunning    = false;

		std::vector<std::function<void()>> m_DifferedTasks;
		std::mutex                         m_DifferedTasksLock;

		std::mutex                         m_RecvLock;
		std::mutex                         m_SendLock;

		CDefaultStreamAllocator            sa;
		CStream                            m_recvBuffer;
		CStream                            m_sendBuffer;

		OnDataCb                           OnData;
		OnDisconnectCB                     OnDisconnect;
		OnConnectCB                        OnNewConnection;
	};
} // namespace network
