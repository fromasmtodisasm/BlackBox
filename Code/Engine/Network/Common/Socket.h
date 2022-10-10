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
		using Ptr            = std::unique_ptr<network::Socket>;
		using OnDataCb       = std::function<void(CStream& stm)>;
		using OnDisconnectCB = std::function<void(const char* szCause)>;
		using OnConnectCB    = std::function<void(Socket::Ptr s)>;
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
		static Socket::Ptr CreateListen(const char* port);

		bool               Connect(std::string_view address, std::string_view port);
		bool               Disconnect();
		void               Term();

		void               Update();

		void               Send(char* data, size_t len);
		void               Send(CStream& stm)
		{
			Send((char*)stm.GetPtr(), stm.GetSize() / 8);
		}

		bool        IsValid() { return m_Socket != INVALID_SOCKET; }

		Socket::Ptr Acept();
		void        ListenThread();
		void        ThreadFunc();

	protected:
		void Close();

	public:
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
