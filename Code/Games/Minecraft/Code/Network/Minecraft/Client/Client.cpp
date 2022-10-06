#include "Client.h"
#include <fstream>
#include <regex>

namespace stl
{
	//template<typename ValueType, typename  Container<ValueType>>
	//auto find(const typename Container<ValueType>& c, auto fn);

}

// Main code
const auto DEFAULT_WIDTH  = 640;
const auto DEFAULT_HEIGHT = 480;
#if 0
int main(int, char**)
{
	Chat testChat;
	MainLoop mainLoop{ {"Client", DEFAULT_WIDTH, DEFAULT_HEIGHT}, [&testChat] {testChat.Update(); } };
	mainLoop.Start();
	return 0;
}
#endif

namespace network
{
	Client::Client()
	{
	}

	Client::~Client()
	{
	}

	bool Client::Connect(std::string_view address, std::string_view port)
	{
		return m_Socket.Connect(address, port);
	}

	void Client::Disconnect()
	{
		if (m_Socket.Disconnect())
		{
			m_thread.join();
		}
	}

	void Client::Update()
	{
		m_Socket.Update();
	}

	void Client::Send(CStream& stm)
	{
		m_Socket.Send(stm);
	}

	void Client::Send(char* data, size_t len)
	{
		m_Socket.Send(data, len);
	}

	void Client::Start()
	{
		m_Socket.OnData       = std::move(OnData);
		m_Socket.OnDisconnect = std::move(OnDisconnect);
		m_thread              = std::thread([this]
                               { m_Socket.ThreadFunc(); });
	}
} // namespace network

Chat::Chat()
{
}

Chat::~Chat()
{
	m_client.Disconnect();
}

void Chat::DrawChat()
{
}

void Chat::ChatWindow()
{
}

void Chat::UpdateClient()
{
	m_client.Update();
}

void Chat::OnConnecting()
{
}

void Chat::Update()
{
	UpdateClient();
	switch (m_state)
	{
	case Chat::NotConnected:
		//ConnectWindow();
		break;
	case Chat::Connecting:
		OnConnecting();
		break;
	case Chat::Connected:
		ChatWindow();
		break;
	default:
		break;
	}
}

void Chat::SendMessageTo(std::string_view msg)
{
	auto             message = network::msg::Client::MESSAGE;
	std::string_view data    = msg;

	CStream          wr;
	wr.Write(msg_type(message));
	wr.Write(data);

	m_client.Send(wr);
}

void Chat::Auth(std::string_view mail, std::string_view password)
{
#if 1
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#endif
#if 1
	auto    message = network::msg::Client::AUTH;

	CStream wr;
	wr.Write(msg_type(message));
	wr.Write(mail);
	wr.Write(password);

	m_client.Send(wr);
#endif
}

void Chat::OnConnect(CStream& stm)
{
	Friend f;
	stm.Read(f.id);
	stm.Read(f.name);

	m_online.push_back(f);

	CryLog("%s connected", f.name.c_str());
}

void Chat::OnDisconnect(CStream& stm)
{
	size_t id;
	stm.Read(id);

	if (auto it = std::find_if(m_online.begin(), m_online.end(),
	                           [id](const Friend& f)
	                           {
		                           return f.id == id;
	                           });
	    it != m_online.end())
	{
		CryLog("%s disconnected", it->name.c_str());
		m_online.erase(it);
	}
}

void Chat::OnMessage(CStream& stm)
{
	size_t      id;
	std::string text;
	stm.Read(id);
	stm.Read(text);

	if (auto it = std::find_if(m_online.begin(), m_online.end(), [id](const Friend& f)
	                           { return f.id == id; });
	    it != m_online.end())
	{
		CryLog("%s: %s", it->name.c_str(), text.data());
	}
}

void Chat::ProcessIncomming(CStream& stm)
{
#define HANDLE_MESSAGE(m, fn)     \
	case network::msg::Server::m: \
		(fn(stm));                \
		break;
	auto EmtpyHandle = [](CStream& stm)
	{ assert(0); };

	msg_type message;
	if (!stm.Read(message))
		return;
	switch (network::msg::Server(message))
	{
		HANDLE_MESSAGE(MESSAGE, OnMessage);
		HANDLE_MESSAGE(NAME, EmtpyHandle);
		HANDLE_MESSAGE(JOIN, EmtpyHandle);
		HANDLE_MESSAGE(CONNECT, OnConnect);
		HANDLE_MESSAGE(CONNECTED, [this](CStream& stm)
		               { this->m_state = Chat::Connected; });
		HANDLE_MESSAGE(DISCONNECT, OnDisconnect);
	default:
		break;
	}
}

bool Chat::OnConnect()
{
#if 0
		m_state = Connecting;
		if (auto result = m_client.Connect(cd->address, cd->port); result)
		{
			m_client.OnData = [this](char* data, size_t len) {
				Stream stm(data, len);
				ProcessIncomming(stm);
			};
			m_client.OnDisconnect = [this] {
				m_state = NotConnected;
				printf("Disconnection occur\n");
			};
			m_client.Start();

			Auth(cd->mail, cd->password);
			return true;
		}
#endif
	return false;
}
