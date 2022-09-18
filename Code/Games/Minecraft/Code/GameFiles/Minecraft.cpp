#undef min
#undef max

#include "Minecraft.h"
#include <glm/gtc/noise.hpp>
#include <glm/glm.hpp>
#include <array>
#include <BlackBox/System/File/CryFile.h>
#include <queue>
#include <deque>

Minecraft* minecraft;

CCamera* getCamera()
{
	auto game   = dynamic_cast<CXGame*>(Env::System()->GetIGame());
	auto client = &game->GetClient()->m_DummyClient;
	return client->m_CameraController.RenderCamera();
}


void       DrawField(int size)
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
	//////////////
	void Pause()
	{
		m_IsPause = !m_IsPause;
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
	}

	void Eat()
	{
	}

	void FakeEat()
	{
		m_FakeEat = true;
	}

	void MakeFood()
	{
		auto x = rand() % 20;
		auto y = rand() % 20;
	}

	IEntity* CreateCell(glm::vec3 pos)
	{
		auto        object = Env::I3DEngine()->MakeObject("minecraft/Snake.obj");

		extern int  nextEntity();
		CEntityDesc desc(nextEntity(), 0);
		desc.name = "Snake";
		auto Head = Env::EntitySystem()->SpawnEntity(desc);

		Head->SetIStatObj(object);
		Head->SetPos(pos);
		Head->SetScale(glm::vec3(1.f));
		//Head->SetAngles({-90, 0, 0});
		//Head->Physicalize();
		Env::I3DEngine()->RegisterEntity(Head);

		return Head;
	}

	void Init()
	{
		auto cell = CreateCell({0, 0, 0});
		m_Body.push_back(cell);
	}

	IEntity* GetHead()
	{
		return m_Body.front();
	}
	IEntity* GetTail()
	{
		return m_Body.back();
	}
	void Update()
	{
		if (m_IsPause)
			return;
		m_Ticked += Env::Timer()->GetRealFrameTime();
		if (m_Ticked >= m_TickTime)
		{
			if (m_FakeEat)
			{
				m_FakeEat = false;

				auto cell = CreateCell(GetHead()->GetPos() + m_Dir);
				m_Body.push_front(cell);
			}
			else
			{
				Move();
			}
			m_Ticked = 0.f;
		}
	}

	Movement             m_PrevDir = Movement::FORWARD;
	Legacy::Vec3         m_Dir     = Legacy::Vec3(0, 0, -1);
	Legacy::Vec3         m_BlockSize;
	glm::ivec2           m_HeadPos;
	std::deque<IEntity*> m_Body;
	bool                 m_NeedMove = false;
	float                m_TickTime = 0.5f;
	float                m_Ticked   = 0.f;
	bool                 m_FakeEat  = false;
	bool                 m_IsPause  = false;
};

Snake* g_Snake;

void   Minecraft::Pause()
{
	g_Snake->Pause();
}

void Minecraft::MoveSnake(Movement dir)
{
	g_Snake->ChangeDir(dir);
}

void Minecraft::FakeEat()
{
	g_Snake->FakeEat();
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

	// ���������� �������������� ����� � ������������ � ��� �������������
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

	Env::Console()->ExecuteString("load_level minecraft");
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

void Minecraft::init()
{
	Env::CryPak()->OpenPack("% fcdata%/objects.pak");
	minecraft = this;

	world.init();
	ui.init();
	player.init();
	debug.init();

	g_Snake = new Snake;
	g_Snake->Init();

	auto game   = dynamic_cast<CXGame*>(Env::System()->GetIGame());
	auto client = &game->GetClient()->m_DummyClient;
	auto& cc     = client->m_CameraController;

	//-55,-111
	getCamera()->SetAngles({-55, -111,0});
	getCamera()->updateCameraVectors();
	//cc.Freeze(true);
}

void Minecraft::update()
{
	auto         time          = Env::Timer()->GetRealFrameTime();
	static float jack_rotation = 0;
	//Jack->SetAngles({-90, 0, jack_rotation});
	//Jack->SetAngles({45, 90, 0});
	//debug.update();
	//ui.draw();
	player.update();

	jack_rotation += 16 * time;

	DrawField(20);
	g_Snake->Update();
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

	CEntityDesc desc(minecraft->world.blocks.size() + 1, (int)EEntityClass::Character);
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

			// ���������� �� ��������� � ���������� ����� �����������
			glm::vec2 const tMinMax = aabb.IntersectBox(eyeRay);

			// ���� ������ ��� �� ������
			if (tMinMax.x < 0 || tMinMax.y < 0)
				continue;

			// ���� �� �� ������ ������� � ����������� ���������
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

	// ��������� � �������� ����� ������� ����
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
	// ���� ����� ����������� � �������� ����� ���� � ������������ �������� �� ������� ��� �������������
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

	// ���� ������ �������� �� ������ � ������������ �� ������ ������ �� ��������������� �����,
	// �� ������ ��������� ����������� � ���� ����� ���
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

	// ��������� ��������� ��������, ����� �������� ��������� �� ����� ����
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

	// ���� �� �������������, �� ��������� �������� ����������� �����
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

	auto        steve = Env::I3DEngine()->MakeObject("minecraft/minecraft_steve.obj");

	CEntityDesc desc(nextEntity(), PLAYER_CLASS_ID);
	entity = Env::EntitySystem()->SpawnEntity(desc);
	Env::I3DEngine()->RegisterEntity(entity);

	entity->SetPos(getCamera()->GetPos());
	myPos = entity->GetPos();

	glm::vec3 min{-0.4, -2.3, -0.4}, max{0.4, 0.4, 0.4};
	entity->SetBBox(min, max);

	m_pSetBlockSound     = Env::SoundSystem()->LoadSound("sounds/doors/open.wav", 0);
	m_pDestroyBlockSound = Env::SoundSystem()->LoadSound("sounds/doors/close.wav", 0);
}

void MinePlayer::update()
{
	auto const aabb    = entityWorldAABB(entity);

	//Env::AuxGeomRenderer()->DrawAABB(aabb.min, aabb.max, {1, 1, 1, 1});

	auto const gravity = 0.f;
	auto       ft      = Env::Timer()->GetRealFrameTime();
	move(glm::vec3(0.0f, -1.0f, 0.0f), gravity * ft);

	applyMovement();
	//getCamera()->SetPos(entity->GetPos());
	getCamera()->SetPos(myPos);
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