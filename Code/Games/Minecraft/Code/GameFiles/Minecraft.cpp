#undef min
#undef max

#include "Minecraft.h"
#include <glm/gtc/noise.hpp>
#include <glm/glm.hpp>
#include <array>
#include <BlackBox/System/File/CryFile.h>
#include <queue>
#include <deque>

#include <BlackBox/Network/Socks.hpp>

enum class GameState
{
	InMenu,
	InGame,
	Started,
	Loose,
	Won,
};

Minecraft* minecraft;
GameState  g_CurrentGameState = GameState::InMenu;
CRYSOCKET  g_ClientSocket     = INVALID_SOCKET;

CCamera*   getCamera()
{
	auto game   = dynamic_cast<CXGame*>(Env::System()->GetIGame());
	auto client = &game->GetClient()->m_DummyClient;
	return client->m_CameraController.RenderCamera();
}

void DrawField(int size)
{
	auto color     = UCol(Legacy::Vec3(0, 1, 1));
	//for (int i = -size / 2; size /2; i++)
	auto step      = size / 2;
	auto half_size = size / 2;
	for (int i = 0; i <= size; i++)
	{
		Env::AuxGeomRenderer()->DrawLine({-half_size, 0, i - step}, color, {half_size, 0, i - step}, color);
	}

	for (int i = 0; i <= size; i++)
	{
		Env::AuxGeomRenderer()->DrawLine({i - step, 0, -half_size}, color, {i - step, 0, half_size}, color);
	}

	//for (int i = -size / 2; size /2; i++)
	//{
	//	Env::AuxGeomRenderer()->DrawLine(p + width * v, color, p - width * v, color);
	//}
}

struct Snake
{
	enum class CellType
	{
		Head,
		Body,
		Food,
		Last
	};

	std::array<IStatObj*, size_t(CellType::Last)> m_CellObjects;

	//////////////
	void                                          Pause()
	{
		m_IsPause = !m_IsPause;
	}
	~Snake()
	{
		for (auto& body : m_Body)
		{
			Env::EntitySystem()->RemoveEntity(body->GetId(), true);
		}
		m_Body.clear();
		Env::EntitySystem()->RemoveEntity(m_Food->GetId(), true);
	}
	void GameOver()
	{
	}
	void ChangeDir(Movement dir)
	{
		switch (dir)
		{
		case Movement::FORWARD:
			if (m_PrevDir == Movement::BACKWARD) return;
			m_Dir = Legacy::Vec3{0, 0, 1};
			break;
		case Movement::BACKWARD:
			if (m_PrevDir == Movement::FORWARD) return;
			m_Dir = Legacy::Vec3{0, 0, -1};
			break;
		case Movement::LEFT:
			if (m_PrevDir == Movement::RIGHT) return;
			m_Dir = Legacy::Vec3{1, 0, 0};
			break;
		case Movement::RIGHT:
			if (m_PrevDir == Movement::LEFT) return;
			m_Dir = Legacy::Vec3{-1, 0, 0};
			break;
		default:
			break;
		}

		m_PrevDir = dir;
	}
	void Move()
	{
		auto Head = m_Body.front();
		auto Tail = m_Body.back();
		m_Body.pop_back();
		m_Body.push_front(Tail);
		auto Pos = Head->GetPos();
		Pos += m_Dir;

		Tail->SetPos(Pos);
		//Tail->SetIStatObj(m_CellObjects[size_t(CellType::Head)]);
	}

	void Eat()
	{
		auto HeadPos = GetHead()->GetPos();
		MoveHead();
		auto body = CreateCell(HeadPos, CellType::Body);
		auto it   = m_Body.begin() + 1;
		m_Body.insert(it, body);

		MakeFood();
		m_pEatSound->Play();
	}

	void FakeEat()
	{
		m_FakeEat = true;
	}

	void MakeFood()
	{
		auto x    = rand() % 20 - 10;
		auto z    = rand() % 20 - 10;
		m_FoodPos = glm::ivec2(x, z);
		m_Food->SetPos({x, 0, z});
	}

	IEntity* CreateCell(glm::vec3 pos, CellType celType)
	{
		auto        object = m_CellObjects[size_t(celType)];

		extern int  nextEntity();
		CEntityDesc desc(nextEntity(), 0);
		desc.name   = "Snake";
		auto Result = Env::EntitySystem()->SpawnEntity(desc);

		Result->SetIStatObj(object);
		Result->SetPos(pos);
		Result->SetScale(glm::vec3(1.f));
		Env::I3DEngine()->RegisterEntity(Result);

		return Result;
	}

	void Init()
	{
		const char* cellObjectPaths[] = {
		    "minecraft/Snake.obj", //head
		    "minecraft/Grass_Block.obj",
		    "minecraft/Food.obj",
		    //"minecraft/Food.obj"
		};
		for (size_t i = 0; i < size_t(CellType::Last); i++)
		{
			auto object      = Env::I3DEngine()->MakeObject(cellObjectPaths[i]);
			m_CellObjects[i] = object;
		}

		m_Body.push_back(CreateCell({0, 0, 0}, CellType::Body));

		m_Food = CreateCell({0, 0, 0}, CellType::Food);
		MakeFood();
		m_pEatSound           = Env::SoundSystem()->LoadSound("minecraft/eat.mp3", 0);
		m_pSelfIntersectSound = Env::SoundSystem()->LoadSound("minecraft/self_intersect.mp3", 0);

		//m_pSetBlockSound = Env::SoundSystem()->LoadSound("sounds/doors/open.wav", 0);
	}

	IEntity* GetHead()
	{
		return m_Body.front();
	}
	IEntity* GetTail()
	{
		return m_Body.back();
	}
	void CreateHead(Legacy::Vec3 pos)
	{
		auto cell = CreateCell(pos, CellType::Head);
		m_Body.push_front(cell);
	}
	void MoveHead()
	{
		auto pos = GetHead()->GetPos();
		GetHead()->SetPos(pos + m_Dir);
	}
	bool SelfIntersect()
	{
		auto HeadPos = GetHead()->GetPos();
		for (auto& it = m_Body.begin() + 1; it != m_Body.end(); it++)
		{
			if (HeadPos == (*it)->GetPos())
			{
				return true;
			}
		}
		return false;
	}
	bool OutOfBounds()
	{
		auto p3  = GetHead()->GetPos();
		auto p2  = glm::ivec2(p3.x, p3.z);
		auto h_x = g_World.size_x / 2;
		auto h_z = g_World.size_z / 2;
		if (p2.x > h_x || p2.x < -h_x)
			return true;
		if (p2.y > h_z || p2.y < -h_z)
			return true;
		return false;
	}
	bool FoodUnderHead()
	{
		auto HeadPos = GetHead()->GetPos();
		return glm::ivec3(HeadPos + m_Dir) == glm::ivec3(m_FoodPos.x, 0, m_FoodPos.y);
	}
	void Update()
	{
		if (m_IsPause)
			return;
		m_Ticked += Env::Timer()->GetRealFrameTime();
		if (m_Ticked >= m_TickTime)
		{
			Tick();
			m_Ticked = 0.f;
		}
	}
	void Tick()
	{
		if (SelfIntersect())
		{
			m_pSelfIntersectSound->Play();
			minecraft->RestartSnake(this);
		}
		else if (OutOfBounds())
		{
			m_pSelfIntersectSound->Play();
			minecraft->RestartSnake(this);
		}
		else if (FoodUnderHead())
		{
			Eat();
		}
		else
		{
			Move();
		}
	}

	Movement             m_PrevDir = Movement::FORWARD;
	Legacy::Vec3         m_Dir     = Legacy::Vec3(0, 0, -1);
	Legacy::Vec3         m_BlockSize;
	glm::ivec2           m_HeadPos;
	std::deque<IEntity*> m_Body;
	bool                 m_NeedMove = false;
	float                m_TickTime = 0.1f;
	float                m_Ticked   = 0.f;
	bool                 m_FakeEat  = false;
	bool                 m_IsPause  = false;

	IEntity*             m_Food;
	glm::ivec2           m_FoodPos;

	ISound*              m_pEatSound;           //     = Env::SoundSystem()->LoadSound("sounds/doors/open.wav", 0);
	ISound*              m_pSelfIntersectSound; //     = Env::SoundSystem()->LoadSound("sounds/doors/open.wav", 0);
};

std::vector<Snake*> Snakes;

namespace network
{
	enum Commands : uchar
	{
		Connect,
		NewClient,
		Disconnect,
		Move,
		MakeFood,
		Eat,
	};
#define DEFAULT_PORT      9999
#define DEFAULT_BUFFER    4096
#define MAX_BIND_ATTEMPTS 8 // it will try to connect using ports from DEFAULT_PORT to DEFAULT_PORT + MAX_BIND_ATTEMPTS - 1
} // namespace network
struct SnakeServer;

struct SnakeServerSlot : IServerSlotSink
{
	SnakeServerSlot(struct SnakeServer* server, CRYSOCKET client)
	    : m_SnakeServer(server)
	    , m_Client(client)
	{
	}
	// Inherited via IServerSlotSink
	virtual void OnXServerSlotConnect(const uint8_t* pbAuthorizationID, unsigned int uiAuthorizationSize) override
	{
	}
	virtual void OnXServerSlotDisconnect(const char* szCause) override
	{
	}
	// When client connected, create his on server
	virtual void OnContextReady(CStream& stm) override
	{
	}
	virtual void OnData(CStream& stm) override
	{
		Sock::send(m_Client, (const char*)stm.GetPtr(), stm.GetSize(), 0);
	}
	virtual void OnXPlayerAuthorization(bool bAllow, const char* szError, const uint8_t* pGlobalID, unsigned int uiGlobalIDSize) override
	{
	}
	bool Update()
	{
#define DEFAULT_BUFLEN 512
		int  iResult;
		char recvbuf[DEFAULT_BUFLEN];
		int  recvbuflen = DEFAULT_BUFLEN;
		// Receive until the peer shuts down the connection
		do {
			iResult = recv(m_Client, recvbuf, recvbuflen, 0);
			if (iResult > 0)
			{
				printf("Bytes received: %d\n", iResult);

				// Echo the buffer back to the sender
				int iSendResult = send(m_Client, recvbuf, iResult, 0);
				if (iSendResult == SOCKET_ERROR)
				{
					printf("send failed with error: %d\n", WSAGetLastError());
					closesocket(m_Client);
					WSACleanup();
					return 1;
				}
				printf("Bytes sent: %d\n", iSendResult);
				break;
			}
			else if (iResult == 0)
				printf("Connection closing...\n");
			else
			{
				printf("recv failed with error: %d\n", WSAGetLastError());
				closesocket(m_Client);
				WSACleanup();
				return 1;
			}

		} while (iResult > 0);
		return true;
	}

	SnakeServer* m_SnakeServer;
	CRYSOCKET    m_Client;
};

struct SnakeServer
{
	bool Start()
	{
		m_bAcceptClients = true;
		CRYSOCKADDR_IN local;

#ifdef BB_PLATFORM_WINAPI
		WSADATA wsd;
		if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
		{
			Env::Log()->LogError("[RemoteKeyboard] Failed to load Winsock!\n");
			return false;
		}
#endif

		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_socket == CRY_SOCKET_ERROR)
		{
			CryLog("Remote console FAILED. socket() => CRY_SOCKET_ERROR");
			return false;
		}

		//auto nRet = ioctlsocket(m_socket, FIONBIO, (unsigned long*)&ul);
		//if (nRet == CRY_SOCKET_ERROR)
		//{
		//	CryLog("Failed to put the socket into non-blocking mode. ioctlsocket() => CRY_SOCKET_ERROR");
		//	return false;
		//}

		local.sin_addr.s_addr                    = htonl(INADDR_ANY);
		local.sin_family                         = AF_INET;

		int                port                  = DEFAULT_PORT;
		int                maxAttempts           = MAX_BIND_ATTEMPTS;

		// Get the port from the commandline if provided.
		const ICmdLineArg* pRemoteConsolePortArg = Env::System()->GetICmdLine()->FindArg(ECmdLineArgType::eCLAT_Pre, "remoteConsolePort");
		if (pRemoteConsolePortArg != nullptr)
		{
			// If a valid port is retrieved, only one bind attempt will be made.
			// Otherwise, use the default values.
			port = pRemoteConsolePortArg->GetIValue();
			if (port <= 0)
			{
				port = DEFAULT_PORT;
			}
			else
			{
				maxAttempts = 1;
			}
		}

		bool bindOk = false;
		for (int i = 0; i < maxAttempts; ++i)
		{
			local.sin_port   = htons(port + i);
			const int result = Sock::bind(m_socket, (CRYSOCKADDR*)&local, sizeof(local));
			if (Sock::TranslateSocketError(result) == Sock::eCSE_NO_ERROR)
			{
				bindOk = true;
				break;
			}
		}
		if (!bindOk)
		{
			CryLog("Remote console FAILED. bind() => CRY_SOCKET_ERROR. Faild ports from %d to %d", port, port + maxAttempts - 1);
			return false;
		}

		Sock::listen(m_socket, 8);

		CRYSOCKADDR_IN saIn;
		CRYSOCKLEN_T   slen = sizeof(saIn);
		if (Sock::getsockname(m_socket, (CRYSOCKADDR*)&saIn, &slen) == 0)
		{
			CryLog("Remote console listening on: %d\n", ntohs(saIn.sin_port));
		}
		else
		{
			CryLog("Remote console FAILED to listen on: %d\n", ntohs(saIn.sin_port));
		}

		IsStarted = true;
		return true;
	}
	void Stop()
	{
		IsStarted = false;
	}
	void Update()
	{
		CRYSOCKLEN_T   iAddrSize;
		CRYSOCKET      sClient;
		CRYSOCKADDR_IN client;
		while (m_bAcceptClients)
		{
			iAddrSize = sizeof(client);
			sClient   = Sock::accept(m_socket, (CRYSOCKADDR*)&client, &iAddrSize);
			if (!m_bAcceptClients || sClient == CRY_INVALID_SOCKET)
			{
				break;
			}

			auto pClient = new SnakeServerSlot(this, sClient);
			m_clients.push_back(pClient);
		}
		for (auto& client : m_clients)
		{
			client->Update();
		}
	}

	typedef std::vector<SnakeServerSlot*> TClients;
	TClients                              m_clients;
	CRYSOCKET                             m_socket;
	CStream                               m_eventBuffer;
	volatile bool                         m_bAcceptClients;
	friend struct Snake;
	bool IsStarted = false;
};

SnakeServer g_SnakeServer;

void        Minecraft::Pause()
{
	//g_Snake->Pause();
}

void Minecraft::MoveSnake(Movement dir, int id)
{
	Snakes[id]->ChangeDir(dir);
}
void Minecraft::RestartSnake(Snake* snake)
{
	auto it = std::find(Snakes.begin(), Snakes.end(), snake);
	if (it != Snakes.end())
	{
		delete snake;
		*it = new Snake;
		(*it)->Init();
	}
}

enum class EEntityClass : int
{
	Block,
	Character

};

int  nextEntity() { return minecraft->world.nextEntity(); }

AABB entityWorldAABB(IEntity* entity)
{
	AABB aabb = {glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)};
	entity->GetBBox(aabb.min, aabb.max);
	auto pos = entity->GetPos();

	// перемещаем ограничивающую рамку в соответствии с его расположением
	aabb.Translate(glm::vec3(pos.x, pos.y, pos.z));

	return aabb;
}

void MineWorld::init()
{
	auto loadAssets = [this]()
	{
		auto grass = Env::I3DEngine()->MakeObject("minecraft/Grass_Block.obj");
		//auto grass = Env::I3DEngine()->MakeObject("objects/editor/mtlbox.cgf");

		types.push_back(grass);
	};

	auto generateLevel = [this]()
	{
		g_World.size_x = 20;
		g_World.size_z = 20;
		g_World.height = -1;

#if 1
		for (int y = g_World.height; y < 0; y++)
		{
			for (int z = -g_World.size_x / 2; z < g_World.size_x / 2; z++)
			{
				for (int x = -g_World.size_x / 2; x < g_World.size_x / 2; x++)
				{
					//if (-10 * glm::perlin(glm::vec3(float(z) * .05f, float(z) * 0.005f, 0.f)) <= 2)
					//	continue;
					set(glm::ivec3(x, y, z), Grass);
				}
			}
		}
#else
		/*CEntityDesc desc(nextEntity(), (int)EEntityClass::Character);
		auto        entity = Env::EntitySystem()->SpawnEntity(desc);
		Env::I3DEngine()->RegisterEntity(entity);

		entity->Physicalize();

		entity->SetIStatObj(types[Grass]);
		entity->SetPos({0, 30, 0});
		entity->SetScale(glm::vec3(20.f, 1.f, 20.f));*/

#endif
	};

	loadAssets();
	generateLevel();
	{
		char* objects[] = {
		    "objects/characters/story_characters/krieger_mutant/krieger_mutant.cgf",
		    "objects/characters/pmodels/hero/hero.cgf",
		    "objects/editor/MtlSphere.cgf",
		    "objects/editor/mtlbox.cgf",
		    "objects/editor/mtlteapot.cgf",
		    "objects/characters/animals/parrot/parrot.cgf",
		    "objects/editor/MtlSphere.cgf",
		    "objects/weapons/m4/m4.cgf",
		    "objects/vehicles/buggy/buggy.cgf",
		    "objects/editor/arrow.cgf",
		    "objects/editor/arrow.cgf",
		    "minecraft/Grass_Block.obj",
		};
		m_pMtlBox   = Env::I3DEngine()->MakeObject(objects[3]);
		auto camera = Env::System()->GetViewCamera();
#if 0
		auto box    = SpawnBox({5, 5, 5}, {0, 0, 0});
		Env::I3DEngine()->RegisterEntity(box);
#endif
#if 0
		{
			auto        object = Env::I3DEngine()->MakeObject(objects[1]);

			CEntityDesc desc(nextEntity(), 0);
			desc.name  = "Hero";
			auto* Jack = Env::EntitySystem()->SpawnEntity(desc);

			Jack->SetIStatObj(object);
			Jack->SetPos({-0, 40, -5});
			Jack->SetScale(glm::vec3(0.01f));
			Jack->SetAngles({-90, 0, 0});
			Jack->Physicalize();
			Env::I3DEngine()->RegisterEntity(Jack);
		}

		{
			auto        object = Env::I3DEngine()->MakeObject(objects[0]);

			//auto Jack = Env::I3DEngine()->MakeObject();
			//auto        Jack = types[0];

			CEntityDesc desc(nextEntity(), 0);
			desc.name       = "Hero";
			minecraft->Jack = Env::EntitySystem()->SpawnEntity(desc);
			auto* Jack      = minecraft->Jack;

			Jack->SetIStatObj(object);
			Jack->SetPos({-5, 50, -5});
			Jack->SetScale(glm::vec3(0.02f));
			Jack->SetAngles({45, 90, 0});
			Jack->Physicalize();
			Env::I3DEngine()->RegisterEntity(Jack);
		}
#endif
		//{
		//	auto object = types[0];
		//	//Env::I3DEngine()->MakeObject(objects[0]);

		//	//auto Jack = Env::I3DEngine()->MakeObject();
		//	//auto        Jack = types[0];

		//	CEntityDesc desc(nextEntity(), 0);
		//	desc.name       = "Hero";
		//	auto Jack      = Env::EntitySystem()->SpawnEntity(desc);

		//	Jack->SetIStatObj(object);
		//	Jack->SetPos({-5, 20, -5});
		//	Jack->SetScale(glm::vec3(3.f));
		//	Jack->SetAngles({45, 90, 0});
		//	Env::I3DEngine()->RegisterEntity(Jack);
		//}
	}

	//Env::Console()->ExecuteString("load_level minecraft");
	Env::Console()->ShowConsole(false);
}

void MineUI::init()
{
	crossHairTexture = Env::Renderer()->LoadTexture(
	    "Textures/crosshair.png", nullptr, false);
}

void MineUI::draw() const
{
	auto        centerX = (float)Env::Renderer()->GetWidth() / 2;
	auto        centerY = (float)Env::Renderer()->GetHeight() / 2;

	const float width   = 20;
	const float height  = 20;

	Env::Renderer()->Draw2dImage(
	    centerX - 0.5f * width,
	    centerY - 0.5f * height, 20, 20, (int)crossHairTexture,
	    0, 0, 1, 1, 0, 0, 1, 0, 0.5);
}

auto GetGame()
{
	auto game = dynamic_cast<CXGame*>(Env::System()->GetIGame());
	return game;
}
auto GetClient()
{
	auto client = &GetGame()->GetClient()->m_DummyClient;
	return client;
}

auto GetCameraController()
{
	auto& cc = GetClient()->m_CameraController;
}

bool SendStream(CStream& stm, CRYSOCKET s)
{
	//Sock::send(s, (const char*)stm.GetPtr(), stm.GetSize(), 0);
	return false;
}

bool ClientConnect(const char* ip);

void SendClientConnect()
{
	CStream stm;
}

void StartGame()
{
	if (GetGame()->StartupClient())
	{
		if (!g_SnakeServer.Start())
		{
			CryError("Failed to start server");
			return;
		}
		if (!ClientConnect("127.0.0.1"))
		{
			CryError("Failed to connect to server");
			return;
		}
		g_CurrentGameState = GameState::InGame;
	}
}
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   "9999"
bool ClientConnect(const char* ip)
{
	WSADATA          wsaData;
	struct addrinfo *result = NULL,
	                *ptr    = NULL,
	                hints;
	const char* sendbuf = "this is a test";
	//char        recvbuf[DEFAULT_BUFLEN];
	int         iResult;
	int         recvbuflen = DEFAULT_BUFLEN;

	// Initialize Winsock
	iResult                = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		CryError("WSAStartup failed with error: %d\n", iResult);
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family   = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult           = getaddrinfo(ip, DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		CryError("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return false;
	}
	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to server
		g_ClientSocket = socket(ptr->ai_family, ptr->ai_socktype,
		                        ptr->ai_protocol);
		if (g_ClientSocket == INVALID_SOCKET)
		{
			CryError("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return false;
		}

		// Connect to server.
		iResult = connect(g_ClientSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			closesocket(g_ClientSocket);
			g_ClientSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (g_ClientSocket == INVALID_SOCKET)
	{
		CryError("Unable to connect to server!\n");
		WSACleanup();
		return false;
	}
	return true;
}

void UpdateClientNetwork()
{
}

void StopGame()
{
	g_CurrentGameState = GameState::InMenu;
	GetGame()->ShutdownClient();
	//GetClient()->OnUnloadScene();
}

void Minecraft::init()
{
	Env::CryPak()->OpenPack("% fcdata%/objects.pak");
	minecraft = this;

	world.init();
	ui.init();
	player.init();
	debug.init();

#if 0
	auto player = new Snake;
	player->Init();
	Snakes.push_back(player);
#endif

	auto  game   = dynamic_cast<CXGame*>(Env::System()->GetIGame());
	auto  client = &game->GetClient()->m_DummyClient;
	auto& cc     = client->m_CameraController;

#if 0
	//-55,-111
	getCamera()->SetAngles({-55, -111, 0});
	getCamera()->updateCameraVectors();
	cc.Freeze(true);
#endif
	Env::Console()->AddCommand(
	    "game.start",
	    [](IConsoleCmdArgs*)
	    { StartGame(); });
	Env::Console()->AddCommand(
	    "game.stop",
	    [](IConsoleCmdArgs*)
	    { StopGame(); });
	Env::Console()->AddCommand(
	    "game.connect",
	    [](IConsoleCmdArgs* args)
	    {
		    if (args->GetArgCount() > 1)
		    {
			    auto ip = args->GetArg(1);
			    if (ClientConnect(ip))
				    g_CurrentGameState = GameState::InGame;
		    }
	    });
}

void InMenuUpdate()
{
	Env::Console()->ShowConsole(true);
}

network::Commands ParseCommand()
{
	return network::NewClient;
}

void ReadNetwork(CStream& stm)
{
	network::Commands command;
	stm.Read((uchar&)command);
	switch (command)
	{
	case network::Connect:
		break;
	case network::NewClient:
		break;
	case network::Disconnect:
		break;
	case network::Move:
		break;
	case network::MakeFood:
		break;
	case network::Eat:
		break;
	default:
		break;
	}
}

void InGameUpdate()
{
	CStream NetworkStream;
	ReadNetwork(NetworkStream);

	if (g_SnakeServer.IsStarted)
	{
		g_SnakeServer.Update();
	}

	getCamera()->SetPos({2, 25, 20});
	getCamera()->SetAngles({-55, -111, 0});
	getCamera()->updateCameraVectors();
	DrawField(g_World.size_x);
	for (auto& snake : Snakes)
	{
		snake->Update();
	}
}

void Minecraft::update()
{
	switch (g_CurrentGameState)
	{
	case GameState::InMenu:
		InMenuUpdate();
		break;
	case GameState::InGame:
		player.update();
		InGameUpdate();
		break;
	case GameState::Started:
		break;
	case GameState::Loose:
		break;
	case GameState::Won:
		break;
	default:
		break;
	}
}

bool MineWorld::tryDestroy(glm::ivec3 pos)
{
	if (auto e = blocks.find(pos); e != blocks.end())
	{
		Env::EntitySystem()->RemoveEntity(e->second->GetId(), true);
		blocks.erase(pos);
		return true;
	}
	return false;
}

IEntity* MineWorld::SpawnBox(const Legacy::Vec3& pos, const Legacy::Vec3& velocity)
{
	auto        object = m_pMtlBox;

	CEntityDesc desc(nextEntity(), PLAYER_CLASS_ID);
	desc.angles = {-90, 0, 0};
	desc.pos    = pos;
	desc.name   = "Box";

	auto* Box   = Env::EntitySystem()->SpawnEntity(desc);

	Box->SetIStatObj(object);
	Box->SetScale(glm::vec3(0.02f));
	Box->Physicalize();

	pe_action_set_velocity action;
	action.v = Legacy::from(velocity);

	Box->GetPhysics()->Action(&action);
	Env::I3DEngine()->RegisterEntity(Box);

	return Box;
}

void MineWorld::destroy(glm::ivec3 pos)
{
	if (!tryDestroy(pos))
	{
		CryError("there is no block on the position %d, %d, %d: ", pos.x, pos.y, pos.z);
	}
}

// FIXME: work incorrect(aabb has wrong position in world before first place block)
void MineWorld::highliteCubeTmp(glm::ivec3 pos)
{
	if (auto const e = blocks.find(pos); e != blocks.end())
	{
		auto const aabb = entityWorldAABB(e->second);
		Env::AuxGeomRenderer()->DrawAABB(aabb.min - 0.1f, aabb.max + 0.1f, {1, 1, 1, 1});
	}
}

bool MineWorld::has(glm::ivec3 pos)
{
	return blocks.count(pos);
}

void MineWorld::set(glm::ivec3 pos, Type type)
{
	tryDestroy(pos);

	CEntityDesc desc(nextEntity(), (int)EEntityClass::Character);
	auto        entity = Env::EntitySystem()->SpawnEntity(desc);
	Env::I3DEngine()->RegisterEntity(entity);

	entity->SetIStatObj(types[type]);
	entity->SetPos(pos);
	entity->SetScale(glm::vec3(1.f));

	blocks.emplace(pos, entity);
}

bool MineWorld::pickPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, Ray eyeRay, float& pickDistance) const
{
	float tMin        = HUGE_VALF;
	float minDistance = HUGE_VALF;

	for (auto it : blocks)
	{
		auto const pos      = glm::vec3(it.first);
		auto const camPos   = getCamera()->GetPos();
		auto const distance = glm::distance(pos, camPos);
		auto       entity   = it.second;

		if (distance <= pickDistance)
		{
			auto const      obj     = entity->GetIStatObj(0);

			auto const      aabb    = entityWorldAABB(entity);

			// расстояния до ближайшей и дальнейшей точек пересечения
			glm::vec2 const tMinMax = aabb.IntersectBox(eyeRay);

			// если объект был за спиной
			if (tMinMax.x < 0 || tMinMax.y < 0)
				continue;

			// если мы не внутри коробки и пересечение ближайшее
			if (tMinMax.x < tMinMax.y && tMinMax.x < tMin)
			{
				tMin                      = tMinMax.x;
				glm::vec3   pickedPos     = eyeRay.origin + eyeRay.direction * tMin;
				float const blockDistance = glm::distance(eyeRay.origin, pickedPos);
				if (blockDistance < minDistance)
				{
					minDistance = blockDistance;
					outBlockPos = it.first;
					outPickPos  = pickedPos;
				}
			}
		}
	}
	pickDistance = minDistance;
	return minDistance != HUGE_VALF;
}

bool MinePlayer::selectedPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, float& pickDistance) const
{
	auto  game              = dynamic_cast<CXGame*>(Env::System()->GetIGame());
	auto  intersectionState = game->GetClient()->m_DummyClient.m_IntersectionState;

	// начальная и конечная точки запуска луча
	auto& start             = intersectionState.ray.start;
	Env::Renderer()->UnProjectFromScreen(
	    intersectionState.mx, intersectionState.my, 0, &start.x, &start.y, &start.z);
	auto& end = intersectionState.ray.end;
	Env::Renderer()->UnProjectFromScreen(
	    intersectionState.mx, intersectionState.my, 1, &end.x, &end.y, &end.z);

	Ray eyeRay{};
	eyeRay.origin    = getCamera()->GetPos();
	eyeRay.direction = glm::normalize(end - start);

	return minecraft->world.pickPos(outBlockPos, outPickPos, eyeRay, pickDistance);
}

int signDirection(float val)
{
	if (val > 0)
	{
		return 1;
	}
	else if (val == 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

float roundFloat(float d)
{
	return floorf(d + 0.5f);
}

glm::ivec3 floorVec(glm::vec3 v)
{
	return {floorf(v.x), floorf(v.y), floorf(v.z)};
}

bool isFloatInteger(float a, float s = 0.00003)
{
	// если число округлилось и осталось равно себе с определенным допуском мы счиатем его целочисленным
	float ceiled = roundFloat(a);
	return abs(ceiled - a) <= s;
}

bool MinePlayer::blockSideOnCursor(glm::ivec3& outBlockPos, glm::ivec3& outSidePos, float& pickDistance) const
{
	glm::vec3 pickPos;
	if (selectedPos(outBlockPos, pickPos, pickDistance))
	{
		auto const origin = getCamera()->GetPos();
		outSidePos        = outBlockPos;

		if (isFloatInteger(pickPos.x))
		{
			outSidePos.x -= signDirection(outSidePos.x - origin.x);
		}
		else if (isFloatInteger(pickPos.y))
		{
			outSidePos.y -= signDirection(outSidePos.y - origin.y);
		}
		else if (isFloatInteger(pickPos.z))
		{
			outSidePos.z -= signDirection(outSidePos.z - origin.z);
		}
		return true;
	}
	return false;
}

bool isBoundigPointCoDirectedWithMovement(IEntity* entity, glm::vec3 point, glm::vec3 movement)
{
	glm::vec3 min, max;
	entity->GetBBox(min, max);

	auto const center   = min + (max - min) * 0.5f;

	auto const pointDir = point - center;
	auto const v        = pointDir * movement;

	// если вектор движения не совпал с направлением от центра игрока до рассматриваемой точки,
	// то смысла проверять пересечение в этой точке нет
	return !(v.x <= 0 && v.y <= 0 && v.z <= 0);
}

std::array<glm::vec3, 8> getBoundingPoints(IEntity* entity)
{
	glm::vec3 min, max;
	entity->GetBBox(min, max);

	return {
	    min,
	    {min.x, max.y, max.z},
	    {min.x, max.y, min.z},
	    {min.x, min.y, max.z},
	    max,
	    {max.x, min.y, min.z},
	    {max.x, min.y, max.z},
	    {max.x, max.y, min.z}};
}

bool MinePlayer::moveOutsideCollisionByPoint(glm::vec3& newPos, glm::vec3 point)
{
	auto checkCube = floorVec(newPos + point);

	if (!minecraft->world.isIntersect(checkCube, entityWorldAABB(entity)))
	{
		return false;
	}

	glm::ivec3 const intersectSide = floorVec(newPos + point + 0.5f);

	// добавляем небольшое смещение, чтобы избежать попадания на грань куба
	auto const       dif           = newPos + point - glm::vec3(intersectSide) + point * 0.00001f;

	float            minDif        = HUGE_VALF;
	int              minI          = -1;

	for (int i = 0; i != 3; ++i)
	{
		glm::vec3 newBlockPos = newPos + point;
		newBlockPos[i] -= dif[i];

		if (dif[i] != 0.0f && abs(dif[i]) < minDif && !minecraft->world.has(floorVec(newBlockPos)))
		{
			minDif = abs(dif[i]);
			minI   = i;
		}
	}

	if (minI != -1)
	{
		newPos[minI] -= dif[minI];
		return true;
	}

	return false;
}

bool MinePlayer::moveOutsideCollision(glm::vec3& newPos)
{
	for (auto const point : getBoundingPoints(entity))
	{
		if (isBoundigPointCoDirectedWithMovement(entity, point, movement))
		{
			if (moveOutsideCollisionByPoint(newPos, point))
			{
				return true;
			}
		}
	}
	return false;
}

void MinePlayer::applyMovement()
{
	auto& world  = minecraft->world;
	//auto  newPos = entity->GetPos() + movement;
	auto& newPos = myPos + movement;
	//entity->GetPos() + movement;

	// если мы передвинулись, то запускаем проверку пересечений опять
	while (moveOutsideCollision(newPos))
	{
	}

	for (int i = 0; i < 3; ++i)
	{
		newPos[i] = round(newPos[i] * 10000) / 10000;
	}

	myPos = newPos;

	//entity->SetPos(newPos);
}

bool MineWorld::isIntersect(glm::ivec3 pos, AABB otherAABB) const
{
	auto block = blocks.find(pos);
	if (block != blocks.end())
	{
		auto const aabb = entityWorldAABB(block->second);
		return aabb.IsIntersectBox(otherAABB);
	}
	return false;
}

void MinePlayer::init()
{
	Env::ScriptSystem()->ExecuteFile("scripts/common.lua");

	//getCamera()->mode = CCamera::Mode::FPS;

	auto steve           = Env::I3DEngine()->MakeObject("minecraft/minecraft_steve.obj");

	//CEntityDesc desc(nextEntity(), PLAYER_CLASS_ID);
	//entity = Env::EntitySystem()->SpawnEntity(desc);
	//Env::I3DEngine()->RegisterEntity(entity);

	//entity->SetPos(getCamera()->GetPos());
	//myPos = entity->GetPos();

	//glm::vec3 min{-0.4, -2.3, -0.4}, max{0.4, 0.4, 0.4};
	//entity->SetBBox(min, max);

	m_pSetBlockSound     = Env::SoundSystem()->LoadSound("sounds/doors/open.wav", 0);
	m_pDestroyBlockSound = Env::SoundSystem()->LoadSound("sounds/doors/close.wav", 0);
}

void MinePlayer::update()
{
#if 0
	auto const aabb    = entityWorldAABB(entity);
	Env::AuxGeomRenderer()->DrawAABB(aabb.min, aabb.max, {1, 1, 1, 1});
#endif

	auto const gravity = 0.f;
	auto       ft      = Env::Timer()->GetRealFrameTime();
	move(glm::vec3(0.0f, -1.0f, 0.0f), gravity * ft);

	//applyMovement();
	//getCamera()->SetPos(entity->GetPos());
	//getCamera()->SetPos(myPos);
	movement          = glm::vec3(0.0f);

	///////////////////////////////////////////////////////////
	auto       camera = Env::System()->GetViewCamera();
	EntityList entities;
	Env::EntitySystem()->GetEntitiesInRadius(camera.GetPos(), 25, entities);
	for each (const auto& e in entities)
	{
		auto impulse    = pe_action_impulse{};
		impulse.impulse = vectorf(0, 210, 0);
		impulse.point   = vectorf(0, 0, 0);
		if (auto p = e->GetPhysics(); p)
			p->Action(&impulse);
	}
}

bool timingAction(float& prevTime, float interval)
{
	auto const curTime = Env::Timer()->GetAsyncTime().GetMilliSeconds();
	auto const delta   = curTime - prevTime;
	//CryLog("delta time: %f", delta);
	if (delta > interval)
	{
		prevTime = curTime;
		return true;
	}
	return false;
}

void MinePlayer::destroyBlockOnCursor()
{
	float       pickDistance = 1000.0f;
	const float interval     = 300;

	glm::ivec3  pos;
	glm::vec3   outPickPos;

	if (timingAction(destroyTime, interval) &&
	    selectedPos(pos, outPickPos, pickDistance))
	{
		minecraft->world.destroy(pos);
		m_pDestroyBlockSound->Play();
	}

	auto camera      = Env::System()->GetViewCamera();
	auto testEntity3 = Env::EntitySystem()->GetEntity(2);
	auto testEntity2 = Env::EntitySystem()->GetEntity(2);
	if (testEntity3 && (m_ClickFrame < (Env::Renderer()->GetFrameID() - 5)))
	{
		//auto p          = testEntity3->GetPhysics();

		//auto impulse    = pe_action_impulse{};
		//impulse.impulse = Legacy::from(camera.Front);
		//impulse.point   = vectorf(0, 0, 0);
		//p->Action(&impulse);

		//p = testEntity2->GetPhysics();
		//impulse.impulse = vectorf(0, 0, 0);
		//p->Action(&impulse);

		auto box = minecraft->world.SpawnBox(camera.GetPos(), camera.Front * 10.f);
		Env::I3DEngine()->RegisterEntity(box);
	}

	m_ClickFrame = Env::Renderer()->GetFrameID();
}

void MinePlayer::placeBlockOnCursor()
{
	float       pickDistance = 1000.0f;
	const float interval     = 300;

	glm::ivec3  pos, side;
	if (timingAction(placeTime, interval) &&
	    blockSideOnCursor(pos, side, pickDistance))
	{
		minecraft->world.set(side, MineWorld::Grass);
		m_pSetBlockSound->Play();
	}
}

void MinePlayer::move(glm::vec3 direction, float value)
{
	movement += direction * value;
}

void MineDebug::init()
{
	model = Env::I3DEngine()->MakeObject("minecraft/bbox.obj");
	//drawBox({0, 0, 0}, {5, 5, 5});
}

void MineDebug::update()
{
	//CryLog("MineDebug::update");
	static float prevTime = 0;
	auto const   curTime  = Env::Timer()->GetAsyncTime().GetMilliSeconds();
	auto const   delta    = curTime - prevTime;
	prevTime              = curTime;
	//CryLog("delta time: %f", delta);
	for (auto box : tmpBoxes)
	{
		//Env::I3DEngine()->UnRegisterEntity(box);
		Env::EntitySystem()->RemoveEntity(box->GetId());
	}
	tmpBoxes.clear();
}

#if 0
void MineDebug::drawBox(glm::vec3 pos1, glm::vec3 pos2)
{
	makeBox(model, pos1, pos2);
}

void MineDebug::drawTmpBox(glm::vec3 pos1, glm::vec3 pos2)
{
	tmpBoxes.push_back(makeBox(model, pos1, pos2));
}
#endif