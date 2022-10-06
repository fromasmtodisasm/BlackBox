#include "Server.h"

#include <fstream>
#include <string>
#include <regex>

Server::Server(int numPlayers) : NumPlayers(numPlayers) {

}
void Server::DisconnectPending()
{
	//std::lock_guard lock(m_ClientsLock);
	for (size_t i = 0; i < m_toDisconnect.size(); i++)
	{
		auto slot = m_toDisconnect[i];
		printf("Connection closed  for %s", slot->name.c_str());
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
		m_Socket.OnNewConnection = [this](network::Socket& s) {OnNewConnection(s); };
		m_thread = std::thread([this] {ThreadFunc(); });
		return true;
	}
	return false;
}
void Server::OnNewConnection(network::Socket& client)
{
	auto slot = new ServerSlot(client, this);


	///
	std::stringstream ss;
	//ss << "New user connected with id: " << clients.size() << "\n";
	//for (auto& c : clients)
	//{
	//	send(c->m_socket, ss.str().c_str(), ss.str().size(), 0);
	//}

	printf("%s", ss.str().c_str());
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
ServerSlot::ServerSlot(network::Socket& socket, Server* server) : m_Socket(std::move(socket)), m_server(server)
{

}
void ServerSlot::Start()
{
	m_Socket.OnData = std::move(OnData);
	m_Socket.OnDisconnect = [this] {m_server->OnDisconnect(this, "WTF??"); };
	m_thread = std::thread([this] {m_Socket.ThreadFunc(); });

}
void GameServer::Update()
{
	server.Update();
}
GameServer::GameServer() :server(10)
{
	isStarted = server.Start();
	if (!isStarted)
		return;
	LoadUsers();
	server.OnCreateServerSlot = [this](ServerSlot* slot) {

		auto gs = new GameServerSlot(slot, this);
		gs->id = m_Slots.size();
		m_Slots.push_back(gs);

		return true;
	};
	server.OnDisconnectServerSlot = [this](ServerSlot* slot) {

		if (auto it = std::find_if(m_Slots.begin(), m_Slots.end(), [slot](GameServerSlot* other) {
			if (other->slot == slot)
			{
				return true;
			}
			return false;
		}); it != m_Slots.end())
		{
			auto msg = network::Server::DISCONNECT;
			auto id = (*it)->id;
			Writer wr(sizeof msg + sizeof id);
			wr.Write(msg);
			wr.Write(id);
			BroadCast(wr, (*it));

			delete* it;
			m_Slots.erase(it);
		}
		return true;
	};
}
void GameServer::LoadUsers()
{
	std::ifstream users("users.txt");
	if (users.is_open())
	{
		std::string line;
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
				m_Users.push_back({ name,mail,pass });
			}

		}
	}
	//m_Users = {
	//	{"HackMan", "test@mail.com", "123456"},
	//	{"Joe", "joe@mail.com", "123654"},
	//	{"Rose", "rose@mail.com", "1111"},
	//};
}

void GameServerSlot::NotifyConnect()
{
	auto msg = network::Server::CONNECT;
	auto PrepareConnect = [&msg](Writer& wr, size_t id, std::string_view name) {
		wr.Write(msg);
		wr.Write(id);
		wr.Write(name);
	};
	{
		Writer wr(sizeof msg + sizeof id + sizeof(size_t) + name.length() + 1);
		PrepareConnect(wr, id, name);
		parent->BroadCast(wr, this);

	}
	{
		auto msg = network::Server::CONNECTED;
		Writer wr(sizeof msg);
		wr.Write(msg);
		Send(wr);
	}
	for (auto s : parent->m_Slots)
	{
		if (s == this) continue;
		auto id = s->id;
		auto& name = s->name;
		Writer wr(sizeof msg + sizeof id + sizeof(size_t) + name.length() + 1);
		PrepareConnect(wr, id, name);
		Send(wr);
	}



}

void GameServerSlot::OnAuth(Stream& stm)
{
	std::string_view mail;
	if (!stm.Read(mail))
	{
		return;
	}
	std::string_view password;
	if (!stm.Read(password))
	{
		return;
	}

	if (auto it = std::find_if(parent->m_Users.begin(), parent->m_Users.end(),
		[&](const User& u)
	{
		if (u.mail == mail)
			return true;
		return false;
	});
		it != parent->m_Users.end())
	{
		printf("Client auth:\n\tmail: %s\n\tpassword: %s\n", mail.data(), password.data());
		if (it->password != password)
		{
			printf("Wrong password or email\n");
			slot->Disconnect();
		}
		else
		{
			printf("\tSuccess\n");
			name = it->name;
			isReady = true;
			NotifyConnect();

		}
	}
	else
	{
		printf("Wrong password or email\n");
		slot->Disconnect();
	}


}
void GameServerSlot::Send(Writer& w)
{
	slot->Send(&w.data[0], w.len);
}
void GameServerSlot::OnMessage(Stream& stm)
{
	std::string_view text;
	if (stm.Read(text))
	{
		auto msg = network::Server::MESSAGE;
		Writer wr(sizeof msg + sizeof id + sizeof(size_t) + text.length() + 1);
		wr.Write(msg);
		wr.Write(id);
		auto txt = std::string_view(text);
		wr.Write(txt);
		parent->BroadCast(wr, this);
		printf("client message: [%s]\n", text.data());
	}

}
void GameServer::BroadCast(Writer& w, GameServerSlot* exclude)
{
	for (auto s : m_Slots)
	{
		if (s != exclude && s->isReady)
			s->Send(w);
	}
}

// Main code
const auto DEFAULT_WIDTH = 640;
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
