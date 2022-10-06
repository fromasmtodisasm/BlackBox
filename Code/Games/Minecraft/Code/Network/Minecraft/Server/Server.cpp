#include "Server.h"

#include <fstream>
#include <string>
#include <regex>
#include <BlackBox/System/File/CryFile.h>

namespace std
{
	bool getline(CCryFile& file, std::string& str)
	{
		if (file.IsEof()) return false;

		str.clear();

		while (!file.IsEof())
		{
			char c;
			file.Read(&c, 1);
			if (c == '\n')
			{
				return true;
			}
			else
			{
				str.push_back(c);
			}
		}
		return str.size() != 0;
	}
} // namespace std

Server::Server(int numPlayers)
    : NumPlayers(numPlayers)
{
}
Server::~Server()
{
	Stop();
}

void Server::DisconnectPending()
{
	//std::lock_guard lock(m_ClientsLock);
	for (size_t i = 0; i < m_toDisconnect.size(); i++)
	{
		auto slot = m_toDisconnect[i];
		CryLog("Connection closed  for %s", slot->name.c_str());
		slot->Stop();

		OnDisconnectServerSlot(slot);

		slot->m_thread.join();
		delete slot;
	}
	m_toDisconnect.clear();
}
void Server::Stop()
{
	//m_bRunning = false;
	for (auto& c : clients)
	{
		c->Disconnect();
	}
	DisconnectPending();

	m_Socket.Disconnect();
	m_thread.join();
}
bool Server::Start()
{
	m_Socket = std::move(network::Socket::CreateListen());
	if (m_Socket.IsValid())
	{
		m_Socket.OnNewConnection = [this](network::Socket& s)
		{ OnNewConnection(s); };
		m_thread = std::thread([this]
		                       { ThreadFunc(); });
		return true;
	}
	return false;
}
void Server::OnNewConnection(network::Socket& client)
{
	auto              slot = new ServerSlot(client, this);

	///
	std::stringstream ss;
	//ss << "New user connected with id: " << clients.size() << "\n";
	//for (auto& c : clients)
	//{
	//	send(c->m_socket, ss.str().c_str(), ss.str().size(), 0);
	//}

	CryLog("%s", ss.str().c_str());
	///

	if (!OnCreateServerSlot(slot))
	{
		//OnDisconnect(slot, "Server full");
		delete slot;
		return;
	}

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
	m_Socket.ListenThread();
}
void Server::Update()
{
	DisconnectPending();
	for (auto& slot : clients)
	{
		slot->Update();
	}
}
//////////////////////////////////////
ServerSlot::~ServerSlot()
{
}
ServerSlot::ServerSlot(network::Socket& socket, Server* server)
    : m_Socket(std::move(socket))
    , m_server(server)
{
}

void ServerSlot::Start()
{
	m_Socket.OnData       = std::move(OnData);
	m_Socket.OnDisconnect = [this]
	{ m_server->OnDisconnect(this, "WTF??"); };
	m_thread = std::thread([this]
	                       { m_Socket.ThreadFunc(); });
}
void ServerSlot::Stop()
{
	m_bRunning = false;
}
void ServerSlot::Disconnect()
{
	m_server->OnDisconnect(this, "wtf");
	Stop();
}
void ServerSlot::Update()
{
	m_Socket.Update();
}
void ServerSlot::Send(CStream& stm)
{
	m_Socket.Send((char*)stm.GetPtr(), stm.GetSize() / 8);
}
//////////////////////////////////////
GameServer::GameServer()
    : server(10)
{
	isStarted = server.Start();
	if (!isStarted)
		return;
	LoadUsers();
	server.OnCreateServerSlot = [this](ServerSlot* slot)
	{
		auto gs = new GameServerSlot(slot, this);
		gs->id  = m_Slots.size();
		m_Slots.push_back(gs);

		return true;
	};
	server.OnDisconnectServerSlot = [this](ServerSlot* slot)
	{
		if (auto it = std::find_if(m_Slots.begin(), m_Slots.end(), [slot](GameServerSlot* other)
		                           {
			if (other->slot == slot)
			{
				return true;
			}
			return false; });
		    it != m_Slots.end())
		{
			auto    msg = network::msg::Server::DISCONNECT;
			auto    id  = (*it)->id;
			CStream stm;
			stm.Write(0);
			stm.Write(uint8(msg));
			stm.Write(id);
			BroadCast(stm, (*it));

			delete *it;
			m_Slots.erase(it);
		}
		return true;
	};
}

void GameServer::Update()
{
	server.Update();
}
void GameServer::LoadUsers()
{
	CCryFile users;
	if (users.Open("users.txt", "r"))
	{
		std::string             line;
		static const std::regex re(R"((\w+),([a-zA-Z1-9@.]+),(.*))");
		while (std::getline(users, line))
		{
			std::string name;
			std::string mail;
			std::string pass;

			std::smatch m;

			if (std::regex_search(line, m, re))
			{
				name = m[1];
				mail = m[2];
				pass = m[3];
				m_Users.push_back({name, mail, pass});
			}
		}
	}
	//m_Users = {
	//	{"HackMan", "test@mail.com", "123456"},
	//	{"Joe", "joe@mail.com", "123654"},
	//	{"Rose", "rose@mail.com", "1111"},
	//};
}

//////////////////////////////////////
GameServerSlot::GameServerSlot(ServerSlot* slot, GameServer* parent)
    : slot(slot)
    , parent(parent)
{
	slot->OnData = [this](CStream& stm)
	{ ProcessIncomming(stm); };
}
GameServerSlot::~GameServerSlot()
{
}

void GameServerSlot::NotifyConnect()
{
	auto msg            = network::msg::Server::CONNECT;
	auto PrepareConnect = [&msg](CStream& wr, size_t id, std::string_view name)
	{
		wr.Write(msg_type(msg));
		wr.Write(id);
		auto n = std::string(name);
		wr.Write(n);
	};
	{
		CStream wr;
		PrepareConnect(wr, id, name);
		parent->BroadCast(wr, this);
	}
	{
		auto    msg = network::msg::Server::CONNECTED;
		CStream stm;
		stm.Write(msg_type(msg));
		stm.Write(id);
		Send(stm);
	}
	for (auto s : parent->m_Slots)
	{
		if (s == this) continue;
		auto    id   = s->id;
		auto&   name = s->name;
		CStream wr;
		PrepareConnect(wr, id, name);
		Send(wr);
	}
}
void GameServerSlot::OnAuth(CStream& stm)
{
	std::string mail;
	if (!stm.Read(mail))
	{
		return;
	}
	std::string password;
	if (!stm.Read(password))
	{
		return;
	}

	bool isTestUser = false;

	if (auto it = std::find_if(parent->m_Users.begin(), parent->m_Users.end(),
	                           [&](const User& u)
	                           {
		                           if (u.mail == "test@mail.com")
		                           {
			                           isTestUser = true;
			                           return true;
		                           }

		                           if (u.mail == mail)
			                           return true;
		                           return false;
	                           });
	    it != parent->m_Users.end())
	{
		CryLog("Client auth:\n\tmail: %s\n\tpassword: %s\n", mail.data(), password.data());
		if (isTestUser || it->password == password)
		{
			CryLog("\tSuccess\n");
			name    = it->name;
			isReady = true;
			NotifyConnect();
		}
		else
		{
			CryLog("Wrong password or email\n");
			slot->Disconnect();
		}
	}
	else
	{
		CryLog("Wrong password or email\n");
		slot->Disconnect();
	}
}
void GameServerSlot::Send(CStream& stm)
{
	slot->Send(stm);
}
void GameServerSlot::ProcessIncomming(CStream& stm)
{
#if 1
	do
	{
		msg_type message;
		if (!stm.Read(message))
			return;
		switch (network::msg::Client(message))
		{
		case network::msg::Client::AUTH:
			OnAuth(stm);
			break;
		case network::msg::Client::MESSAGE:
		{
			OnMessage(stm);
			break;
		}
		break;
		default:
			break;
		}
	} while (!stm.EOS());
#endif
}
void GameServerSlot::OnMessage(CStream& stm)
{
	std::string text;
	if (stm.Read(text))
	{
		auto    msg = network::msg::Server::MESSAGE;
		CStream wr;
		wr.Write(msg_type(msg));
		wr.Write(id);
		auto txt = std::string_view(text);
		wr.Write(txt);
		parent->BroadCast(wr, this);
		CryLog("client message: [%s]\n", text.data());
	}
}
void GameServer::BroadCast(CStream& w, GameServerSlot* exclude)
{
	for (auto s : m_Slots)
	{
		if (s != exclude && s->isReady)
			s->Send(w);
	}
}

// Main code
const auto DEFAULT_WIDTH  = 640;
const auto DEFAULT_HEIGHT = 480;
#if 0
int main(int, char**)
{
	GameServer server;
	if (server.isStarted)
	{
		MainLoop mainLoop{ {"Server", DEFAULT_WIDTH, DEFAULT_HEIGHT}, [&server] {server.Update(); } };
		mainLoop.Start();
	}
	return 0;
}

#endif
