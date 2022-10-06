#include "Client.h"

constexpr std::string_view DEFAULT_PORT = "27015";
struct Client
{

	bool Connect(std::string_view address, std::string_view port = DEFAULT_PORT)
	{
		WSADATA wsaData;
		int iResult = 0;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed: %d\n", iResult);
			return false;
		}

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
			WSACleanup();
			return false;
		}

		// Attempt to connect to the first address returned by
// the call to getaddrinfo
		ptr = result;

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Error at socket(): %ld\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
		}

		// Should really try the next address returned by getaddrinfo
		// if the connect call failed
		// But for this simple example we just free the resources
		// returned by getaddrinfo and print an error message

		freeaddrinfo(result);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Unable to connect to server!\n");
			WSACleanup();
			return false;
		}

		// Change the socket mode on the listening socket from blocking to

   // non-block so the application will not block waiting for requests

		//int NonBlock = 1;
		//if (ioctlsocket(ConnectSocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
		//{
		//	printf("ioctlsocket() failed with error %d\n", WSAGetLastError());
		//	return false;
		//}
		//else
		//{
		//	printf("ioctlsocket() is OK!\n");
		//}


		m_bRunning = true;
		return true;

	}

	void Disconnect()
	{
		if (m_bRunning)
		{
			m_bRunning = false;
			m_thread.join();
		}
	}

	void Update()
	{
		if (m_recvBuffer.size() > 0)
		{
			OnData(&m_recvBuffer[0], m_recvBuffer.size());
			std::lock_guard lock(m_RecvLock);
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

	void Start()
	{
		m_thread = std::thread([this] {ThreadFunc(); });
	}

	void ThreadFunc()
	{

		//FD_SET WriteSet;
		while (m_bRunning)
		{
			{
				std::lock_guard lock(m_SendLock);
				if (m_sendBuffer.size() > 0)
				{
					send(ConnectSocket, m_sendBuffer.data(), m_sendBuffer.size(), 0);
				}

			}
			const size_t BUF_LEN = 254;
			char buf[BUF_LEN];

			FD_SET ReadSet;
			// Prepare the Read and Write socket sets for network I/O notification
			FD_ZERO(&ReadSet);
			FD_SET(ConnectSocket, &ReadSet);
			//FD_ZERO(&WriteSet);
			timeval tv{ 0, 1000 };
			auto res = select(0, &ReadSet, nullptr, nullptr, &tv);
			if (res > 0)
			{
				if (FD_ISSET(ConnectSocket, &ReadSet))
				{
					while (true)
					{
						auto len = recv(ConnectSocket, buf, BUF_LEN, 0);
						if (len == 0)
						{
							break;
						}
						else if (len <= BUF_LEN)
						{
							std::lock_guard lock(m_RecvLock);
							auto size = m_recvBuffer.size();
							m_recvBuffer.resize(size + len);
							memcpy(&m_recvBuffer[size], buf, len);
						}
						else
						{
							auto error = WSAGetLastError();
							printf("recv error: %d", error);
							break;
						}

					}

				}
			}

		}
		shutdown(ConnectSocket, SD_BOTH);
		closesocket(ConnectSocket);
	}

	SOCKET ConnectSocket = INVALID_SOCKET;
	std::string name;
	size_t id;

	std::function<void(char* data, size_t len)> OnData;
	std::function<void()> OnDisconnect;

	bool m_bRunning = false;
	std::mutex m_RecvLock;
	std::mutex m_SendLock;
	std::thread m_thread;

	std::vector<char> m_recvBuffer;
	std::vector<char> m_sendBuffer;
};

struct Friend
{
	std::string name;
	size_t id;
};

struct Message
{
	std::string from;
	std::string text;
};

struct ConnectDialog
{
	std::function<bool(ConnectDialog&)> OnConnect;

	ConnectDialog()
	{
		address.reserve(ADDR_LEN);
		port.reserve(PORT_LEN);
		address = "127.0.0.1";
		port = "27015";
	}

	void Update()
	{
		ImGui::Begin("Connect to Server");
		if (ImGui::InputText("Address", address.data(), ADDR_LEN))
		{
			printf("address: %s", address.c_str());
		}
		if (ImGui::InputText("Port", port.data(), PORT_LEN))
		{
			printf("port: %s", port.c_str());
		}
		if (ImGui::Button("Connect"))
		{
			OnConnect(*this);
		}


		ImGui::End();
	}

	static const size_t ADDR_LEN = 32 + 1;
	static const  size_t PORT_LEN = 5 + 1;
	std::string address;
	std::string port;
};

struct Chat
{
	enum State
	{
		NotConnected,
		Connecting,
		Connected,
	};
	Chat()
	{
		m_friends.push_back(Friend{ "HackMan", 1 });
		m_connectDialog.OnConnect = [this](ConnectDialog& cd) { return OnConnect(&cd); };
	}
	~Chat()
	{
		m_client.Disconnect();
	}
	void DrawChat()
	{

	}
	void ConnectWindow()
	{
		m_connectDialog.Update();
	}
	void ChatWindow()
	{
		UpdateClient();
		ImGui::Begin("Chat");

		ImGui::ListBox("Friends", &m_currentFriend,
			[](void* data, int idx, const char** out_text) {
			auto& f = *(std::vector<Friend>*)data;

			*out_text = f[idx].name.c_str();
			return true;
		},
			&m_friends, m_friends.size());


		static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
		ImGui::InputTextMultiline("##source", text.data(), text.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);

		ImGui::End();

	}
	void UpdateClient()
	{
		m_client.Update();
	}
	void Update()
	{
		switch (m_state)
		{
		case Chat::NotConnected:
			ConnectWindow();
			break;
		case Chat::Connecting:
			break;
		case Chat::Connected:
			ChatWindow();
			break;
		default:
			break;
		}
	}

	bool OnConnect(ConnectDialog* cd)
	{
		if (auto result = m_client.Connect(cd->address, cd->port); result)
		{
			m_state = Connected;
			m_client.OnData = [this](char* data, size_t len) {
				text.insert(text.size(), data, len);
			};
			m_client.Start();
			return true;
		}
		return false;
	}

	Client m_client;

	int m_currentFriend;
	std::vector<Friend> m_friends;

	std::vector<Message> m_messages;
	State m_state = NotConnected;

	ConnectDialog m_connectDialog;
	// Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
// and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.
	std::string text = "text";


};

// Main code
int main(int, char**)
{
	Chat testChat;
	MainLoop mainLoop{ {"Client"}, [&testChat] {testChat.Update(); }};
	mainLoop.Start();
	return 0;
}

