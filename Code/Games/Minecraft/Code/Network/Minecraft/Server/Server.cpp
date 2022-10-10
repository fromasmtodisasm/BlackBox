#include "Server.h"

#include <fstream>
#include <string>
#include <regex>
#include <array>
#include <BlackBox/System/File/CryFile.h>

#include "Game.hpp"
#include "GameFiles/Minecraft.h"
#include "GameFiles/Snake.h"

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

//////////////////////////////////////
GameServer::GameServer(class Minecraft* pGame, WORD nPort, const char* szName, bool listen)
{
	m_bListen  = listen;
	// create the server
	m_pIServer = ((CXGame*)(Env::System()->GetIGame()))->CreateServer(this, nPort, m_bListen);
	if (!m_pIServer)
	{
		CryLog("!!---------Server creation failed---------!!");
		m_bOK = false;
		return;
	}
	else
		m_bOK = true;

	LoadUsers();
}

GameServer::~GameServer()
{
	// update the network, to process any pending disconnect
	UpdateXServerNetwork();

	// shut down server game rules. (incorrect place but left for SP stability)
	//m_ServerRules.ShutDown();

	// delele the entity system sink
	//m_pGame->GetSystem()->GetIEntitySystem()->RemoveSink(this);

	// remove the slots which are still connected
	ClearSlots();

	// update the network, to process any pending disconnect
	UpdateXServerNetwork();

	// shut down server game rules. (correct place)
	// m_ServerRules.ShutDown();
	//m_pGame->GetScriptSystem()->SetGlobalToNull("GameRules"); // workaround to minimize risk

	// release the IServer interface
	SAFE_RELEASE(m_pIServer);

	// release the system interface
}

void GameServer::Update()
{
	UpdateXServerNetwork();

	for (auto& player : minecraft->Players)
	{
		player.second.snake->Update();

		CStream stm;
		auto    msg = network::msg::Server::UPDATE_SNAKE;
		stm.Write(msg_type(msg));
		stm.Write(player.first);
		auto p = player.second.snake->GetHead()->GetPos();
		stm.Write(p.x);
		stm.Write(p.z);

		BroadCast(stm);
	}
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
GameServerSlot::GameServerSlot(GameServer* parent, IServerSlot* slot)
    : m_pISSlot(slot)
    , m_pParent(parent)
{
	slot->Advise(this);
}
GameServerSlot::~GameServerSlot()
{
}
void GameServerSlot::SpawnPlayer()
{
}

void GameServerSlot::NotifyConnect(bool sendOtherSlots)
{
	auto msg            = network::msg::Server::CONNECT;
	auto PrepareConnect = [&msg](CStream& wr, size_t id, std::string_view name)
	{
		wr.Write(msg_type(msg));
		wr.Write(id);
		auto n = std::string(name);
		wr.Write(n);
		// Write position - by default (0,0)
	};
	{
		auto    msg = network::msg::Server::CONNECTED;
		CStream stm;
		stm.Write(msg_type(msg));
		stm.Write(id);
		Send(stm);
	}
	{
		CStream wr;
		PrepareConnect(wr, id, name);
		wr.Write(0.f);
		wr.Write(0.f);
		m_pParent->BroadCast(wr, nullptr);
	}
	if (sendOtherSlots)
	{
		for (auto [first, s] : m_pParent->m_mapXSlots)
		{
			if (s == this) continue;
			auto    id   = s->id;
			auto&   name = s->name;
			CStream wr;
			PrepareConnect(wr, id, name);
			auto p = minecraft->GetPlayer(id).snake->GetHead()->GetPos();
			wr.Write(p.x);
			wr.Write(p.z);
			Send(wr);
		}
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

	if (auto it = std::find_if(m_pParent->m_Users.begin(), m_pParent->m_Users.end(),
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
	    it != m_pParent->m_Users.end())
	{
		CryLog("Client auth:\n\tmail: %s\n\tpassword: %s\n", mail.data(), password.data());
		if (isTestUser || it->password == password)
		{
			CryLog("\tSuccess\n");
			name    = it->name;
			isReady = true;
			NotifyConnect(true);
		}
		else
		{
			CryLog("Wrong password or email\n");
			m_pISSlot->Disconnect("Wrong password or email\n");
		}
	}
	else
	{
		CryLog("Wrong password or email\n");
		m_pISSlot->Disconnect("Wrong password or email\n");
	}
}
void GameServerSlot::Send(CStream& stm)
{
	m_pISSlot->SendReliable(stm);
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
		case network::msg::Client::LOOSE:
		{
			Loose(stm);
			break;
		}
		case network::msg::Client::CHANGE_DIR:
		{
			ChangeDir(stm);
			break;
		}
		break;
		default:
			break;
		}
	} while (!stm.EOS());
#endif
}
void GameServerSlot::ChangeDir(CStream& stm)
{
	uint8 d;
	stm.Read(d);

	CStream nstm;

	auto    msg = network::msg::Server::NEW_DIR;
	nstm.Write(msg_type(msg));
	nstm.Write(id);
	nstm.Write(d);

	m_pParent->BroadCast(nstm, this);
}
void GameServerSlot::Loose(CStream& stm)
{
	size_t id;
	stm.Read(id);

	CStream nstm;

	auto    msg = network::msg::Server::DISCONNECT;
	nstm.Write(msg_type(msg));
	nstm.Write(id);

	m_pParent->BroadCast(nstm, this);

	NotifyConnect(false);
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
		m_pParent->BroadCast(wr, this);
		CryLog("client message: [%s]\n", text.data());
	}
}
void GameServer::BroadCast(CStream& w, GameServerSlot* exclude)
{
	for (auto [first, s] : m_mapXSlots)
	{
		if (s != exclude && s->isReady)
			s->Send(w);
	}
}

GameServerSlot* GameServer::SlotById(size_t id)
{
	if (auto it = m_mapXSlots.find(id); it != m_mapXSlots.end())
	{
		return it->second;
	}
	return nullptr;
}

bool GameServer::CreateServerSlot(IServerSlot* pIServerSlot)
{
	auto gs             = new GameServerSlot(this, pIServerSlot);
	gs->id              = m_mapXSlots.size();
	m_mapXSlots[gs->id] = (gs);

	return true;
}
bool GameServer::GetServerInfoStatus(std::string& szServerStatus) { return false; }
bool GameServer::GetServerInfoStatus(std::string& szName, std::string& szGameType, std::string& szMap, std::string& szVersion, bool* pbPassword, int* piPlayers, int* piMaxPlayers) { return false; }
bool GameServer::GetServerInfoRules(std::string& szServerRules) { return false; }
bool GameServer::GetServerInfoPlayers(std::string* vszStrings[4], int& nStrings) { return false; }
bool GameServer::ProcessXMLInfoRequest(const char* sRequest, const char* sRespone, int nResponseMaxLength) { return false; }

void GameServerSlot::OnXServerSlotConnect(const uint8_t* pbAuthorizationID, unsigned int uiAuthorizationSize) {}
void GameServerSlot::OnXServerSlotDisconnect(const char* szCause)
{
// if the player is not fully connected,
// no action should be taken
#if 0
	if (isReady)
	{
		m_pParent->GetRules()->OnClientDisconnect(m_ScriptObjectServerSlot.GetScriptObject());
	}
#endif

#if 0
	m_wPlayerId = INVALID_WID;
#endif
	// Unregister this slot
	m_pParent->UnregisterSlot(this);

	// set as a garbage
}
void GameServerSlot::OnContextReady(CStream& stm) {} //<<FIXME>> add some level validation code
void GameServerSlot::OnData(CStream& stm)
{
	ProcessIncomming(stm);
}
void GameServerSlot::OnXPlayerAuthorization(bool bAllow, const char* szError, const uint8_t* pGlobalID,
                                            unsigned int uiGlobalIDSize) {}
void GameServerSlot::Disconnect(const char* szCause)
{
	if (m_pISSlot)
		m_pISSlot->Disconnect(szCause);
}
//////////////////////////////////////////////////////////////////////
BYTE GameServerSlot::GetID()
{
	return m_pISSlot->GetID();
}

void GameServer::UnregisterSlot(GameServerSlot* slot)
{
	m_SlotsToDisconnect.push_back(slot);
}
void GameServer::DisconnectSlot(GameServerSlot* slot)
{
	if (auto it = m_mapXSlots.find(slot->GetID()); it != m_mapXSlots.end())
	{
		auto    msg = network::msg::Server::DISCONNECT;
		auto    id  = it->second->id;
		CStream stm;
		stm.Write(0);
		stm.Write(uint8(msg));
		stm.Write(id);
		BroadCast(stm, (it->second));

		delete it->second;
		m_mapXSlots.erase(it);
	}
}
inline void GameServer::ClearSlots()
{
	//Disconnect all slots
	auto itor = m_mapXSlots.begin();
	while (itor != m_mapXSlots.end())
	{
		CXServerSlot* pSlot = itor->second;

		//if (m_pGame->IsMultiplayer() && !pSlot->IsLocalHost())
		pSlot->Disconnect("@ServerShutdown");

		++itor;
	}

	//Update The network to send the disconnection
	UpdateXServerNetwork();

	itor = m_mapXSlots.begin();
	while (itor != m_mapXSlots.end())
	{
		delete itor->second;
		++itor;
	}
	m_mapXSlots.clear();
}
inline void GameServer::UpdateXServerNetwork()
{
	FUNCTION_PROFILER(PROFILE_GAME);
	if (m_pIServer)
		m_pIServer->Update(GetCurrentTime());
}
void GameServer::RegisterSlot(GameServerSlot* pSlot)
{
	NET_TRACE("<<NET>>CXServer::RegisterSlot %d", pSlot->GetID());
	m_mapXSlots.insert(XSlotMap::iterator::value_type(pSlot->GetID(), pSlot));
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
