#pragma once

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
