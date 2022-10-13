#undef min
#undef max

#include "Minecraft.h"
#include <glm/gtc/noise.hpp>
#include <glm/glm.hpp>
#include <array>
#include <BlackBox/System/File/CryFile.h>
#include <queue>
#include <deque>

#include <Network/Minecraft/Server/Server.h>
#include "Snake.h"

Minecraft* minecraft;

enum class GameState
{
	InMenu,
	InGame,
	Started,
	Loose,
	Won,
};
namespace
{
	GameState     g_CurrentGameState = GameState::InMenu;

	constexpr int DEFAULT_PORT       = 9999;
	constexpr int DEFAULT_BUFFER     = 4096;
	constexpr int MAX_BIND_ATTEMPTS  = 8; // it will try to connect using ports from DEFAULT_PORT to DEFAULT_PORT + MAX_BIND_ATTEMPTS - 1

} // namespace
namespace
{
	CCamera* getCamera()
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
		for (int i = 0; i <= size + 1; i++)
		{
			Env::AuxGeomRenderer()->DrawLine({-half_size, 0, i - step}, color, {half_size, 0, i - step}, color);
		}

		for (int i = 0; i <= size + 1; i++)
		{
			Env::AuxGeomRenderer()->DrawLine({i - step, 0, -half_size}, color, {i - step, 0, half_size}, color);
		}

		//for (int i = -size / 2; size /2; i++)
		//{
		//	Env::AuxGeomRenderer()->DrawLine(p + width * v, color, p - width * v, color);
		//}
	}
} // namespace

int nextEntity() { return minecraft->world.nextEntity(); }

struct GameClient : public IClientSink
{
	GameClient()
	{
	}
	void OnXConnect() override
	{
		Auth("test@mail.com", "123456");
	}
	void OnXClientDisconnect(const char* szCause) override
	{
		CryLog("Disconnection occur\n");
	}
	void OnXContextSetup(CStream& stmContext) override {}
	void OnXData(CStream& stm) override
	{
		ProcessIncomming(stm);
	}
	void         OnXServerTimeout() override {}
	void         OnXServerRessurect() override {}
	unsigned int GetTimeoutCompensation() override { return 0; }
	void         MarkForDestruct() override
	{
		m_bSelfDestruct = true;
	}
	bool DestructIfMarked() override
	{
		if (m_bSelfDestruct)
		{
			delete this;
			return true; // was deleted
		}
		return false; // was not deleted
	}

	bool Init()
	{
		m_pClient = minecraft->m_pGame->CreateClient(this);

		return m_pClient != nullptr;
	}

	bool Connect(const char* szAddr)
	{
		m_pClient->SetServerIP(szAddr);
		//bDoSwitch = _bDoLateSwitch;

		m_pClient->InitiateCDKeyAuthorization(false);
		return true;
	}
	void Disconnect(const char* szCause)
	{
		m_pClient->Disconnect(szCause);
	}
	void Auth(std::string mail, std::string password)
	{
#if 0
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
#endif
#if 1
		auto    message = network::msg::Client::AUTH;

		CStream stm;
		stm.Write(msg_type(message));
		stm.Write(mail);
		stm.Write(password);

		m_pClient->SendReliable(stm);
#endif
	}
	void Update()
	{
		uint time = 0;
		m_pClient->Update(time);

		static IFont* pFont{};
		if (!pFont)
		{
			pFont = Env::Renderer()->GetIFont();
			pFont->Init("arial.ttf", 14, 14);
		}

		auto drawPlayerStats = [](auto& player, int row)
		{
			auto snake = player.snake;
			if (snake)
			{
				auto pos = snake->GetHead()->GetPos();

				char buffer[256];
				sprintf(buffer, "<[%s] snake> pos: (%.0f,%.0f,%.0f); Foods: %d", player.name.c_str(), pos.x, pos.y, pos.z, snake->m_FoodsEaten);
				float color[] = {1.f, 1.f, 1.f, 1.f};
				pFont->RenderText(buffer, 0, float(row * 14), 1.f, color);
			}
		};

		int i = 0;
		for (auto [id, player] : minecraft->GetPlayers())
		{
			drawPlayerStats(player, i);
			i++;
		}
	}
	void SendInputToServer(Movement Dir)
	{
		CStream stm;
		auto    msg = network::msg::Client::CHANGE_DIR;
		auto    d   = uint8(Dir);

		stm.Write(msg_type(msg));
		stm.Write(d);

		m_pClient->SendReliable(stm);
	}
	void OnConnect(CStream& stm)
	{
		size_t id = 0;
		string name;
		float  x, y;
		stm.Read(id);
		stm.Read(name);
		stm.Read(x);
		stm.Read(y);

		minecraft->AddPlayer(id, name, {x, y});
	}
	void OnConnected(CStream& stm)
	{
		stm.Read(playerId);

		g_CurrentGameState = GameState::InGame;
	}
	void OnDisconnect(CStream& stm)
	{
		size_t id;

		stm.Read(id);

		minecraft->RemovePlayer(id, true);
	}
	void OnMessage(CStream& stm)
	{
		size_t      id;
		std::string text;
		stm.Read(id);
		stm.Read(text);

#if 0
		if (auto it = std::find_if(m_online.begin(), m_online.end(), [id](const Friend& f)
		                           { return f.id == id; });
		    it != m_online.end())
#endif
		{
			CryLog("msg: %s", text.data());
		}
	}
	void OnNewDir(CStream& stm)
	{
		size_t id;
		uint8  dir;
		stm.Read(id);
		stm.Read(dir);

		auto p = minecraft->GetPlayer(id);
		if (p.snake)
		{
			p.snake->ChangeDir(Movement(dir));
		}
	}
	void OnUpdateSnake(CStream& stm)
	{
		//id,x,z
		size_t id;
		float  x, z;
		size_t    food;
		stm.Read(id);
		stm.Read(x);
		stm.Read(z);
		stm.Read(food);

		auto p = minecraft->GetPlayer(id);
		p.snake->m_FoodsEaten = food;

		p.snake->GetHead()->SetPos({x, 0, z});
	}
	void ProcessIncomming(CStream& stm)
	{
#define HANDLE_MESSAGE(m, fn)     \
	case network::msg::Server::m: \
		(fn(stm));                \
		break;
		auto EmtpyHandle = [](CStream& stm)
		{ assert(0); };

		do
		{
			msg_type message;
			if (!stm.Read(message))
				return;
			switch (network::msg::Server(message))
			{
				HANDLE_MESSAGE(MESSAGE, OnMessage);
				HANDLE_MESSAGE(NAME, EmtpyHandle);
				HANDLE_MESSAGE(JOIN, EmtpyHandle);
				HANDLE_MESSAGE(CONNECT, OnConnect);
				HANDLE_MESSAGE(CONNECTED, OnConnected);
				HANDLE_MESSAGE(DISCONNECT, OnDisconnect);
				HANDLE_MESSAGE(NEW_DIR, OnNewDir);
				HANDLE_MESSAGE(UPDATE_SNAKE, OnUpdateSnake);
				HANDLE_MESSAGE(NEW_FOOD, OnNewFood);
				HANDLE_MESSAGE(EAT, OnEat);
			default:
				break;
			}
		} while (!stm.EOS());
	}
	void OnNewFood(CStream& stm)
	{
		float x, z;
		stm.Read(x);
		stm.Read(z);

		minecraft->m_FoodPos = glm::ivec2(x, z);
		minecraft->m_Food->SetPos({x, 0, z});
	}
	void OnEat(CStream& stm)
	{
		size_t id;
		int x; int y;
		stm.Read(id);
		stm.Read(x);
		stm.Read(y);

		auto p = minecraft->GetPlayer(id);
		p.snake->Eat(x, y);
	}

	void Loose(size_t id)
	{
		CStream stm;
		auto    msg = network::msg::Client::LOOSE;
		stm.Write(msg_type(msg));
		stm.Write(id);
		m_pClient->SendReliable(stm);
	}

	IClient*    m_pClient       = nullptr;
	bool        m_bSelfDestruct = false; //!< usually false, to make sure the client is only released in one place

	std::string server          = "127.0.0.1";

	size_t      playerId        = 0;
};

void Minecraft::Pause()
{
	//g_Snake->Pause();
}

void Minecraft::AddPlayer(size_t id, const string& name, glm::ivec2 pos)
{
	Player newPlayer;
	newPlayer.id   = id;
	newPlayer.name = name;

	auto snake     = std::make_shared<Snake>();

	snake->m_Owner = id;
	snake->Init();
	snake->GetHead()->SetPos({pos.x, 0, pos.y});

	newPlayer.snake = snake;
	Players[id]     = newPlayer;

	auto p          = Players[id];
}

void Minecraft::RemovePlayer(size_t id, bool now)
{
	if (auto it = Players.find(id); it != Players.end())
	{
		if (!now)
		{
			PlayersToDestruct.push_back(id);
		}
		else
		{
			Players.erase(it);
		}
	}
}

Player Minecraft::GetPlayer(size_t id)
{
	if (auto it = Players.find(id); it != Players.end())
	{
		return it->second;
	}
	return Player();
}

Player Minecraft::GetLocalPlayer()
{
	return GetPlayer(localPlayerId);
}

Minecraft::PlayersMap& Minecraft::GetPlayers()
{
	return Players;
}

bool Minecraft::IsServer()
{
	return m_pServer != nullptr;
}

void Minecraft::MakeFood()
{
	auto x    = rand() % 20 - 10;
	auto z    = rand() % 20 - 10;
	m_FoodPos = glm::ivec2(x, z);
	m_Food->SetPos({x, 0, z});

	CStream stm;
	auto    msg = network::msg::Server::NEW_FOOD;
	stm.Write(msg_type(msg));
	stm.Write(float(x));
	stm.Write(float(z));

	m_pServer->BroadCast(stm);
}

void Minecraft::Eat(size_t id, int x, int y)
{
	CStream stm;
	auto    msg = network::msg::Server::EAT;

	stm.Write(msg_type(msg));
	stm.Write(id);
	stm.Write(x);
	stm.Write(y);

	auto s = m_pServer->SlotById(id);

	m_pServer->BroadCast(stm, nullptr);
}

void Minecraft::MoveSnake(Movement dir, int id)
{
	auto p = GetPlayer(id);
	p.snake->ChangeDir(dir);

	m_GameClient->SendInputToServer(dir);
}
void Minecraft::MoveLocalSnake(Movement dir)
{
	MoveSnake(dir, localPlayerId);
}
void Minecraft::RestartSnake(Snake* snake)
{
	RemovePlayer(snake->m_Owner, false);
	m_GameClient->Loose(snake->m_Owner);
}

enum class EEntityClass : int
{
	Block,
	Character

};

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
			for (int z = -g_World.size_x / 2; z <= g_World.size_x / 2; z++)
			{
				for (int x = -g_World.size_x / 2; x <= g_World.size_x / 2; x++)
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

bool ConnectToGame(const char* ip)
{
	if (GetGame()->StartupClient() && minecraft->StartupClient())
	{
		if (!minecraft->m_GameClient->Connect(ip))
		{
			CryError("Failed to connect to server");
			return false;
		}
	}
	g_CurrentGameState = GameState::InGame;
	return true;
}

void StartGame()
{
	if (!minecraft->StartupServer(true))
	{
		CryError("Failed to start server");
		return;
	}

	if (GetGame()->StartupClient() && minecraft->StartupClient())
	{
		if (!minecraft->m_GameClient->Connect("127.0.0.1"))
		{
			CryError("Failed to connect to server");
			return;
		}
	}
	g_CurrentGameState = GameState::InGame;
}
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   "9999"

void StopGame()
{
	g_CurrentGameState = GameState::InMenu;
	GetGame()->ShutdownClient();
	minecraft->ShutdownClient();
	minecraft->ShutdownServer();

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
#else
	const char* cellObjectPaths[] = {
	    "minecraft/Snake.obj", //head
	    "minecraft/Grass_Block.obj",
	    "minecraft/Food.obj",
	    //"minecraft/Food.obj"
	};
	for (size_t i = 0; i < size_t(Minecraft::CellType::Last); i++)
	{
		auto object = Env::I3DEngine()->MakeObject(cellObjectPaths[i]);
		m_CellObjects[i] = object;
	}
	m_Food = minecraft->CreateCell({0, 0, 0}, Minecraft::CellType::Food);

#endif

	auto  game = m_pGame = dynamic_cast<CXGame*>(Env::System()->GetIGame());
	auto  client         = &game->GetClient()->m_DummyClient;
	auto& cc             = client->m_CameraController;

#if 0
	//-55,-111
	getCamera()->SetAngles({-55, -111, 0});
	getCamera()->updateCameraVectors();
	cc.Freeze(true);
#endif
	Env::Console()->AddCommand(
	    "game.start",
	    [](IConsoleCmdArgs*)
	    {
		    StartGame();
	    });
	Env::Console()->AddCommand(
	    "game.stop",
	    [](IConsoleCmdArgs*)
	    { StopGame(); });
	Env::Console()->AddCommand(
	    "game.connect",
	    [](IConsoleCmdArgs* args)
	    {
#if 1
		    if (args->GetArgCount() > 1)
		    {
			    auto ip = args->GetArg(1);
			    ;
			    ConnectToGame(ip);
		    }
#else
		    assert(0);
#endif
	    });
}

void InMenuUpdate()
{
	Env::Console()->ShowConsole(true);
}

void InGameUpdate()
{
	for (auto p : minecraft->PlayersToDestruct)
	{
		minecraft->RemovePlayer(p, true);
	}
#if 1
	minecraft->m_GameClient->Update();

	auto server = minecraft->m_pServer;
	if (server)
	{
		server->Update();
	}
#endif

	getCamera()->SetPos({2, 25, 20});
	getCamera()->SetAngles({-55, -111, 0});
	getCamera()->updateCameraVectors();
	DrawField(g_World.size_x);
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

bool Minecraft::StartupServer(bool listen, const char* szName)
{
	CryLog("Creating the server");

	ShutdownServer(); // to be sure

	int nPort = m_pGame->sv_port->GetIVal();

	// set port and create server
	if (!m_pServer)
		m_pServer = new GameServer(this, nPort, szName, listen);

	if (!m_pServer || !m_pServer->IsOK()) // Check if the server has been created
	{
		// failed, lets try a different port
		CryLog("Server creation failed ! Try with another port");
		SAFE_DELETE(m_pServer);
#if 0
		m_pServer = new GameServer(this, nPort + 1, szName, listen);
		sv_port->Set(nPort + 1);
		if (!m_pServer || !m_pServer->IsOK()) // Check if the server has been created
		{
			SAFE_DELETE(m_pServer);
			m_pLog->Log("Server creation failed !");
			return false;
		}
#else
		return false;
#endif
	}

#if 0
	if (m_pRConSystem)
		m_pRConSystem->OnServerCreated(m_pServer->m_pIServer);
#endif

	CryLog("Server created");

	//m_pServer->Update(); // server is created but map wasn't set yet we don't want to allow connects before

	return true;
}
void Minecraft::ShutdownServer()
{
	if (!m_pServer)
		return;

	//if (!m_pServer->IsInDestruction())
	{
		CryLog("Shutdown CXServer");
		SAFE_DELETE(m_pServer);
		CryLog("CXServer shutdowned");
	}
}

bool Minecraft::StartupClient()
{
	CryLog("Creating the Client");

	ShutdownClient(); // to be sure

	m_GameClient = new GameClient;

	if (!m_GameClient->Init(/*this*/)) // Check if the client has been created
	{
		ShutdownClient();

		CryLog("Client creation failed !");
		return false;
	}

	CryLog("Client created");

	return true;
}
void Minecraft::ShutdownClient()
{
	if (!m_GameClient)
		return;

	CryLog("Disconnect the client");
	m_GameClient->Disconnect("@ClientHasQuit");
	CryLog("Shutdown the Client");

	m_GameClient->MarkForDestruct();
	m_GameClient->DestructIfMarked();
	m_GameClient = NULL;
}
