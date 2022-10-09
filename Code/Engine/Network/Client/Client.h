// ClientServer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <Network/Minecraft/Common/ClientServer.h>

// TODO: Reference additional headers your program requires here.

namespace network
{
	struct Client
	{
		static constexpr std::string_view DEFAULT_PORT = "27015";
		Client();
		~Client();
		bool                            Connect(std::string_view address, std::string_view port = DEFAULT_PORT);
		void                            Disconnect();
		void                            Update();
		void                            Send(CStream& stm);
		void                            Send(char* data, size_t len);
		void                            Start();

		std::string                     mail     = "test@mail.com";
		std::string                     password = "123456";

		network::Socket                 m_Socket;
		network::System                 m_NetworkSystem;

		size_t                          id;
		std::thread                     m_thread;

		network::Socket::OnDataCb       OnData;
		network::Socket::OnDisconnectCB OnDisconnect;
	};

} // namespace network
struct Friend
{
	int         id;
	std::string name;
};

struct Message
{
	std::string from;
	std::string text;
};

struct Chat
{
	//std::future<
	enum State
	{
		NotConnected,
		Connecting,
		Connected,
	};

	std::future<void> m_ConnectingFuture;
	Chat();
	~Chat();
	void                 DrawChat();
	void                 ChatWindow();
	void                 UpdateClient();
	void                 OnConnecting();

	void                 Update();
	void                 SendMessageTo(std::string_view msg);
	void                 Auth(std::string_view mail, std::string_view password);
	void                 OnConnect(CStream& stm);
	void                 OnDisconnect(CStream& stm);
	void                 OnMessage(CStream& stm);
	void                 ProcessIncomming(CStream& stm);

	bool                 OnConnect();

	network::Client      m_client;

	int                  m_currentFriend;
	std::vector<Friend>  m_online;
	std::vector<Message> m_messages;
	State                m_state = NotConnected;

	// Note: we are using a fixed-sized buffer for simplicity here. See ImGuiInputTextFlags_CallbackResize
	// and the code in misc/cpp/imgui_stdlib.h for how to setup InputText() for dynamically resizing strings.
	std::string          text    = "text";
};
