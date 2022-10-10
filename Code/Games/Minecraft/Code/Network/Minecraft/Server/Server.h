// ClientServer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <Network/Minecraft/Common/ClientServer.h>

// TODO: Reference additional headers your program requires here.

#if 0
struct ServerSlot;

struct Server
{
	Server(int numPlayers);
	~Server();

	bool                                  Start();
	void                                  Stop();

	void                                  OnNewConnection(network::Socket& socket);

	void                                  OnDisconnect(ServerSlot* slot, const char* why);

	void                                  DisconnectPending();

	void                                  CheckConnections();

	void                                  ThreadFunc();

	void                                  Update();

	std::function<bool(ServerSlot* slot)> OnCreateServerSlot;
	std::function<bool(ServerSlot* slot)> OnDisconnectServerSlot;

	network::Socket                       m_Socket;

	int                                   NumPlayers;

	std::vector<ServerSlot*>              clients;

	//bool m_bRunning = true;
	std::thread                           m_thread;

	std::mutex                            m_ClientsLock;
	std::vector<ServerSlot*>              m_toDisconnect;
};

struct ServerSlot
{
	ServerSlot() = default;
	~ServerSlot();

	ServerSlot(network::Socket& socket, Server* server);

	void                              Start();
	void                              Stop();
	void                              Disconnect();

	void                              Update();

	///////////////////////
	void                              ThreadFunc();
	///////////////////////
	void                              Send(CStream& stm);
	///////////////////////
	std::function<void(CStream& stm)> OnData;
	///////////////////////

	network::Socket                   m_Socket;
	Server*                           m_server;
	bool                              m_bRunning = true;
	std::string                       name;
	size_t                            id;
	std::thread                       m_thread;
};
#endif
struct GameServer;
struct GameServerSlot : public IServerSlotSink
{
	GameServerSlot(GameServer* parent, IServerSlot* slot);

	~GameServerSlot();

	virtual void OnXServerSlotConnect(const uint8_t* pbAuthorizationID, unsigned int uiAuthorizationSize);
	virtual void OnXServerSlotDisconnect(const char* szCause);
	virtual void OnContextReady(CStream& stm); //<<FIXME>> add some level validation code
	virtual void OnData(CStream& stm);
	virtual void OnXPlayerAuthorization(bool bAllow, const char* szError, const uint8_t* pGlobalID,
	                                    unsigned int uiGlobalIDSize);

	void         Disconnect(const char* szCause);

	void         SpawnPlayer();

	void         OnMessage(CStream& stm);
	void         NotifyConnect(bool sendOtherSlots);
	void         OnAuth(CStream& stm);

	void         Send(CStream& w);
	void         ProcessIncomming(CStream& stm);

	void         ChangeDir(CStream& stm);

	void         Loose(CStream& stm);
	BYTE         GetID();

	std::string  name;
	size_t       id        = 0;
	IServerSlot* m_pISSlot = nullptr;
	GameServer*  m_pParent = nullptr;

	bool         isReady   = false;
};

struct GameServer : public IServerSlotFactory
{
	typedef std::map<BYTE, GameServerSlot*> SlotsMap;
	using XSlotMap     = SlotsMap;
	using CXServerSlot = GameServerSlot;
	//////////////////////////////////////////
	GameServer(class Minecraft* pGame, WORD nPort, const char* szName, bool listen);
	~GameServer();

	bool                         CreateServerSlot(IServerSlot* pIServerSlot) override;
	bool                         GetServerInfoStatus(std::string& szServerStatus) override;
	bool                         GetServerInfoStatus(std::string& szName, std::string& szGameType, std::string& szMap, std::string& szVersion, bool* pbPassword, int* piPlayers, int* piMaxPlayers) override;
	bool                         GetServerInfoRules(std::string& szServerRules) override;
	bool                         GetServerInfoPlayers(std::string* vszStrings[4], int& nStrings) override;
	bool                         ProcessXMLInfoRequest(const char* sRequest, const char* sRespone, int nResponseMaxLength) override;

	void                         Update();
	void                         LoadUsers();
	void                         BroadCast(CStream& w, GameServerSlot* exclude = nullptr);
	GameServerSlot*              SlotById(size_t id);
	bool                         IsOK() { return m_bOK; }

	void                         UnregisterSlot(GameServerSlot* slot);

	void                         DisconnectSlot(GameServerSlot* slot);
	void                         ClearSlots();
	void                         UpdateXServerNetwork();
	//////////////////////////////////////////
	void                         RegisterSlot(GameServerSlot* pSlot);
	//////////////////////////////////////////
	IServer*                     m_pIServer = nullptr;

	bool                         m_bOK;     //!< true=server creation was successful, false otherwise
	bool                         m_bListen; //!< server accepts non-local connections

	Minecraft*                   m_pGame;

	XSlotMap                     m_mapXSlots;
	std::vector<GameServerSlot*> m_SlotsToDisconnect;

	std::vector<User>            m_Users;
};
