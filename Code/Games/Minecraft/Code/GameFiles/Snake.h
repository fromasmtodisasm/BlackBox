#pragma once

struct Snake
{
	//////////////
	void Pause()
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
#if 0
		Env::EntitySystem()->RemoveEntity(m_Food->GetId(), true);
#endif
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
		auto body = minecraft->CreateCell(HeadPos, Minecraft::CellType::Body);
		auto it   = m_Body.begin() + 1;
		m_Body.insert(it, body);

		if (minecraft->IsServer())
		{
			minecraft->Eat(m_Owner);
			minecraft->MakeFood();
		}
		//m_pEatSound->Play();
	}

	void Init()
	{
		m_Body.push_back(minecraft->CreateCell({0, 0, 0}, Minecraft::CellType::Body));

		//m_pEatSound           = Env::SoundSystem()->LoadSound("minecraft/eat.mp3", 0);
		//m_pSelfIntersectSound = Env::SoundSystem()->LoadSound("minecraft/self_intersect.mp3", 0);

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
		auto cell = minecraft->CreateCell(pos, Minecraft::CellType::Head);
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
		return glm::ivec3(HeadPos + m_Dir) == glm::ivec3(minecraft->m_FoodPos.x, 0, minecraft->m_FoodPos.y);
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
//m_pSelfIntersectSound->Play();
#if 0
			minecraft->RestartSnake(this);
#endif
		}
		else if (OutOfBounds())
		{
//m_pSelfIntersectSound->Play();
#if 0
			minecraft->RestartSnake(this);
#else
			m_Dir  = -m_Dir;
			auto p = GetHead()->GetPos();
			p += m_Dir;
			GetHead()->SetPos(p);
#endif
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

	size_t               m_Owner   = 0;

	Movement             m_PrevDir = Movement::FORWARD;
	Legacy::Vec3         m_Dir     = Legacy::Vec3(0, 0, -1);
	Legacy::Vec3         m_BlockSize;
	glm::ivec2           m_HeadPos;
	std::deque<IEntity*> m_Body;
	bool                 m_NeedMove = false;
	float                m_TickTime = 0.4f;
	float                m_Ticked   = 0.f;
	bool                 m_FakeEat  = false;
	bool                 m_IsPause  = false;

	ISound*              m_pEatSound;           //     = Env::SoundSystem()->LoadSound("sounds/doors/open.wav", 0);
	ISound*              m_pSelfIntersectSound; //     = Env::SoundSystem()->LoadSound("sounds/doors/open.wav", 0);
};
