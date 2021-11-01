#include "Minecraft.h"
#undef min
#undef max

Minecraft* minecraft;

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
		auto grass = gEnv->p3DEngine->MakeObject("Data/minecraft/Grass_Block.obj");
		types.push_back(grass);
	};

	auto generateLevel = [this]()
	{
		g_World.size_x = 40;
		g_World.size_z = 40;
		g_World.height = -1;

		for (int y = g_World.height; y < 0; y++)
		{
			for (int z = -g_World.size_x / 2; z < g_World.size_x / 2; z++)
			{
				for (int x = -g_World.size_x / 2; x < g_World.size_x / 2; x++)
				{
					set(glm::ivec3(x, y, z), Grass);
				}
			}
		}
		int width = 5;
		for (int y = 0; y < width; y++)
		{
			set(glm::ivec3(0, y, 0), Grass);
		}
		for (int i = 0; i < width; i++)
		{
			set(glm::ivec3(i, 0, 0), Grass);
			set(glm::ivec3(i, width, 0), Grass);
		}
		for (int i = 1; i < 4; i++)
		{
			set(glm::vec3((width - 1), i, 0), Grass);
		}
		for (int i = width - 1; i > 4; i--)
		{
			set(glm::ivec3(i, 3, 0), Grass);
		}
	};

	loadAssets();
	generateLevel();

	gEnv->pConsole->ExecuteString("load_level minecraft");
	gEnv->pConsole->ShowConsole(false);
}

void MineUI::init()
{
	crossHairTexture = gEnv->pRenderer->LoadTexture(
		"Data/images/crosshair.png", nullptr, false);
}

void MineUI::draw() const
{
	auto centerX = (float)gEnv->pRenderer->GetWidth() / 2;
	auto centerY = (float)gEnv->pRenderer->GetHeight() / 2;

	const float width  = 20;
	const float height = 20;

	gEnv->pRenderer->Draw2dImage(
		centerX - 0.5f * width,
		centerY - 0.5f * height, 20, 20, (int)crossHairTexture,
		0, 0, 1, 1, 0, 0, 1, 0, 0.5);
}

void Minecraft::init()
{
	minecraft = this;

	world.init();
	ui.init();
	player.init();
	debug.init();
}

void Minecraft::update()
{
	debug.update();
	ui.draw();
	player.update();
}

bool MineWorld::tryDestroy(glm::ivec3 pos)
{
	if (auto e = blocks.find(pos); e != blocks.end())
	{
		e->second->Remove();
		blocks.erase(pos);
		return true;
	}
	return false;
}

void MineWorld::destroy(glm::ivec3 pos)
{
	if (!tryDestroy(pos))
	{
		CryError("there is no block on the position %d, %d, %d: ", pos.x, pos.y, pos.z);
	}
}

void MineWorld::highliteCubeTmp(glm::ivec3 pos)
{
	if (auto const e = blocks.find(pos); e != blocks.end())
	{
		auto const aabb = entityWorldAABB(e->second);
		minecraft->debug.drawBox(aabb.min - 0.1f, aabb.max + 0.1f);
	}
}

void MineWorld::set(glm::ivec3 pos, Type type)
{
	tryDestroy(pos);

	auto entity = gEnv->p3DEngine->MakeEntity();

	entity->SetIStatObj(types[type]);
	entity->SetPos(pos);
	entity->SetScale(glm::vec3(1.0f));

	blocks.emplace(pos, entity);
}

CCamera* getCamera()
{
	auto game	= dynamic_cast<CXGame*>(gEnv->pSystem->GetIGame());
	auto client = &game->GetClient()->m_DummyClient;
	return client->m_CameraController.RenderCamera();
}

bool MineWorld::pickPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, Ray eyeRay, float pickDistance) const
{
	float tMin		  = HUGE_VALF;
	float minDistance = HUGE_VALF;

	for (auto it : blocks)
	{
		auto const pos		= glm::vec3(it.first);
		auto const camPos	= getCamera()->GetPos();
		auto const distance = glm::distance(pos, camPos);
		auto	   entity	= it.second;

		if (distance <= pickDistance)
		{
			auto const obj = entity->GetIStatObj(0);

			auto const aabb = entityWorldAABB(entity);

			// ���������� �� ��������� � ���������� ����� �����������
			glm::vec2 const tMinMax = aabb.IntersectBox(eyeRay);

			// ���� ������ ��� �� ������
			if (tMinMax.x < 0 || tMinMax.y < 0)
				continue;

			// ���� �� �� ������ ������� � ����������� ���������
			if (tMinMax.x < tMinMax.y && tMinMax.x < tMin)
			{
				tMin					  = tMinMax.x;
				glm::vec3	pickedPos	  = eyeRay.origin + eyeRay.direction * tMin;
				float const blockDistance = glm::distance(eyeRay.origin, pickedPos);
				if (blockDistance < minDistance)
				{
					minDistance = blockDistance;
					outBlockPos = it.first;
					outPickPos	= pickedPos;
				}
			}
		}
	}

	return minDistance != HUGE_VALF;
}

bool MinePlayer::selectedPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, float pickDistance) const
{
	auto game			   = dynamic_cast<CXGame*>(gEnv->pSystem->GetIGame());
	auto intersectionState = game->GetClient()->m_DummyClient.m_IntersectionState;

	// ��������� � �������� ����� ������� ����
	auto& start = intersectionState.ray.start;
	gEnv->pRenderer->UnProjectFromScreen(
		intersectionState.mx, intersectionState.my, 0, &start.x, &start.y, &start.z);
	auto& end = intersectionState.ray.end;
	gEnv->pRenderer->UnProjectFromScreen(
		intersectionState.mx, intersectionState.my, 1, &end.x, &end.y, &end.z);

	Ray eyeRay{};
	eyeRay.origin	 = getCamera()->GetPos();
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

bool isFloatInteger(float a)
{
	// ���� ����� ����������� � �������� ����� ���� � ������������ �������� �� ������� ��� �������������
	float ceiled = roundFloat(a);
	return abs(ceiled - a) <= 0.00003;
}

bool MinePlayer::blockSideOnCursor(glm::ivec3& outBlockPos, glm::ivec3& outSidePos, float pickDistance) const
{
	glm::vec3 pickPos;
	if (selectedPos(outBlockPos, pickPos, pickDistance))
	{
		auto const origin = getCamera()->GetPos();
		outSidePos		  = outBlockPos;

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

void MinePlayer::applyMovement()
{
	auto newPos = entity->GetPos() + movement;

	glm::vec3 min, max;
	entity->GetBBox(min, max);
	auto const center	 = (max - min) * 0.5f;
	auto const entityPos = entity->GetPos();

	std::vector const boundingPoints = {
		min, min + max.x, min + max.y, min + max.z,
		min + max.x + max.y, min + max.x + max.z,
		min + min.y + min.z, min + min.x + min.y + min.z};

	for (auto const point : boundingPoints)
	{
		auto const pointDir = point - center;
		auto const v		= pointDir * movement;
		// ���� ������ �������� �� ������ � ������������ �� ������ ������ �� ��������������� �����,
		// �� ������ ��������� ����������� � ���� ����� ���
		if (!(v.x < 0 && v.y < 0 && v.z < 0))
		{
			auto const checkCube = glm::ivec3(entityPos + point);
			if (minecraft->world.isIntersect(checkCube, entityWorldAABB(entity)))
			{
				glm::ivec3 const intersectSide = point + 0.5f;
				glm::vec3 const	 dif		   = point - glm::vec3(intersectSide);
				newPos -= dif;
			}
		}
	}

	entity->SetPos(newPos);
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
	getCamera()->mode = CCamera::Mode::FPS;

	auto steve = gEnv->p3DEngine->MakeObject("Data/minecraft/minecraft_steve.obj");
	// auto steveTexture = gEnv->pRenderer->LoadTexture("Data/minecraft/Minecraft_steve_skin.jpg");
	// steve->SetTexture(steveTexture);

	entity = gEnv->p3DEngine->MakeEntity();

	entity->SetPos(glm::vec3(5, 10, 5));
	glm::vec3 min{-0.5, -2.5, -0.5}, max{0.5, 0.5, 0.5};
	entity->SetBBox(min, max);
}

void MinePlayer::update()
{
	auto const aabb	   = entityWorldAABB(entity);

	minecraft->debug.drawTmpBox(aabb.min, aabb.max);

	auto const gravity = 2.f;
	move(glm::vec3(0.0f, -1.0f, 0.0f), gravity * gEnv->pTimer->GetRealFrameTime());

	applyMovement();
	getCamera()->SetPos(entity->GetPos());
	movement = glm::vec3(0.0f);
}

bool timingAction(float& prevTime, float interval)
{
	auto const curTime = gEnv->pTimer->GetAsyncTime().GetMilliSeconds();
	auto const delta   = curTime - prevTime;
	if (delta > interval)
	{
		prevTime = curTime;
		return true;
	}
	return false;
}

void MinePlayer::destroyBlockOnCursor()
{
	const float pickDistance = 14.0f;
	const float interval	 = 300;

	glm::ivec3 pos;
	glm::vec3  outPickPos;

	if (timingAction(destroyTime, interval) &&
		selectedPos(pos, outPickPos, pickDistance))
	{
		minecraft->world.destroy(pos);
	}
}

void MinePlayer::placeBlockOnCursor()
{
	const float pickDistance = 14.0f;
	const float interval	 = 300;

	glm::ivec3 pos, side;
	if (timingAction(placeTime, interval) &&
		blockSideOnCursor(pos, side, pickDistance))
	{
		minecraft->world.set(side, MineWorld::Grass);
	}
}

void MinePlayer::move(glm::vec3 direction, float value)
{
	movement += direction * value;
}

void MineDebug::init()
{
	model = gEnv->p3DEngine->MakeObject("Data/minecraft/bbox.obj");
	drawBox({0, 0, 0}, {5, 5, 5});
}

void MineDebug::update()
{
	for (auto box : tmpBoxes)
	{
		box->Remove();
	}
	tmpBoxes.clear();
}

IEntity* makeBox(IStatObj* model, glm::vec3 pos1, glm::vec3 pos2)
{
	auto bbox = gEnv->p3DEngine->MakeEntity();
	bbox->SetIStatObj(model);

	bbox->SetPos(pos1);
	auto dif = pos2 - pos1;

	bbox->SetScale(dif);
	return bbox;
}

void MineDebug::drawBox(glm::vec3 pos1, glm::vec3 pos2)
{
	makeBox(model, pos1, pos2);
}

void MineDebug::drawTmpBox(glm::vec3 pos1, glm::vec3 pos2)
{
	tmpBoxes.push_back(makeBox(model, pos1, pos2));
}
