#include "Minecraft.h"

void MineWorld::init()
{
	auto loadAssets = [this]()
	{
		auto grass = gEnv->p3DEngine->MakeObject("Data/minecraft/Grass_Block.obj");
		glm::vec3 min, max;
		grass->GetBBox(min, max);
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
	world.init();
	ui.init();
	player.init(&world);
}

void Minecraft::update()
{
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

void MineWorld::set(glm::ivec3 pos, Type type)
{
	tryDestroy(pos);

	auto entity = gEnv->p3DEngine->MakeEntity();

	entity->SetIStatObj(types[type]);
	entity->SetPos(pos);
	entity->SetScale(0.5f);

	blocks.emplace(pos, entity);
}

CCamera* getCamera()
{
	auto game	= dynamic_cast<CXGame*>(gEnv->pSystem->GetIGame());
	auto client = &game->GetClient()->m_DummyClient;
	return client->m_CameraController.RenderCamera();
}

AABB entityWorldAABB(IEntity* entity, glm::vec3 pos)
{
	AABB aabb = {};
	entity->GetBBox(aabb.min, aabb.max);

	// перемещаем ограничивающую рамку в соответствии с его расположением
	aabb.Translate(glm::vec3(pos.x, pos.y + 0.5, pos.z));

	return aabb;
}

bool MineWorld::selectedPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, float pickDistance) const
{
	auto game			   = dynamic_cast<CXGame*>(gEnv->pSystem->GetIGame());
	auto intersectionState = game->GetClient()->m_DummyClient.m_IntersectionState;

	// начальная и конечная точки запуска луча
	auto& start = intersectionState.ray.start;
	gEnv->pRenderer->UnProjectFromScreen(
		intersectionState.mx, intersectionState.my, 0, &start.x, &start.y, &start.z);
	auto& end = intersectionState.ray.end;
	gEnv->pRenderer->UnProjectFromScreen(
		intersectionState.mx, intersectionState.my, 1, &end.x, &end.y, &end.z);

	Ray eyeRay{};
	eyeRay.origin	 = getCamera()->GetPos();
	eyeRay.direction = glm::normalize(end - start);

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

			auto const aabb = entityWorldAABB(entity, pos);

			// расстояния до ближайшей и дальнейшей точек пересечения
			glm::vec2 const tMinMax = aabb.IntersectBox(eyeRay);

			// если объект был за спиной
			if (tMinMax.x < 0 || tMinMax.y < 0)
				continue;

			// если мы не внутри коробки и пересечение ближайшее
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

bool MineWorld::blockOnCursor(glm::ivec3& outPos, float pickDistance) const
{
	glm::vec3 pickPos;
	return selectedPos(outPos, pickPos, pickDistance);
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
	// если число округлилось и осталось равно себе с определенным допуском мы счиатем его целочисленным
	float ceiled = roundFloat(a);
	return abs(ceiled - a) <= 0.00003;
}

bool MineWorld::blockSideOnCursor(glm::ivec3& outBlockPos, glm::ivec3& outSidePos, float pickDistance) const
{
	glm::vec3 pickPos;
	if (selectedPos(outBlockPos, pickPos, pickDistance))
	{
		auto const origin = getCamera()->GetPos();
		outSidePos		  = outBlockPos;

		if (isFloatInteger(pickPos.x + 0.5f))
		{
			outSidePos.x -= signDirection(outSidePos.x - origin.x);
		}
		else if (isFloatInteger(pickPos.y))
		{
			outSidePos.y -= signDirection(outSidePos.y - origin.y);
		}
		else if (isFloatInteger(pickPos.z + 0.5f))
		{
			outSidePos.z -= signDirection(outSidePos.z - origin.z);
		}
		return true;
	}
	return false;
}

bool MineWorld::isIntersect(glm::ivec3 pos, AABB otherAABB) const
{
	auto block = blocks.find(pos);
	if (block != blocks.end())
	{
		auto const aabb = entityWorldAABB(block->second, pos);
		return aabb.IsIntersectBox(otherAABB);
	}
	return false;
}

void MinePlayer::init(MineWorld* mineWorld)
{
	world = mineWorld;

	// auto steve = gEnv->p3DEngine->MakeObject("Data/minecraft/minecraft_steve.obj");
	// auto steveTexture = gEnv->pRenderer->LoadTexture("Data/minecraft/Minecraft_steve_skin.jpg");
	// steve->SetTexture(steveTexture);

	entity = gEnv->p3DEngine->MakeEntity();

	//entity->SetPos(glm::vec3(0, 1, 0));
	glm::vec3 min{-1, 0, -1}, max{1, 2, 1};
  entity->SetBBox(min, max);
}

void MinePlayer::update()
{
	auto cameraPos  = getCamera()->GetPos();
	auto const playerAABB = entityWorldAABB(entity, cameraPos);

	glm::ivec3 const cameraBlockPos = {cameraPos.x, cameraPos.y, cameraPos.z};

	glm::vec3 newEntityPos = entity->GetPos();

	auto checkIntersect = [=, &newEntityPos](
							  int checkAxis, int checkPos1, int checkPos2)
	{
		auto p1 = cameraBlockPos;
		p1[checkAxis] += checkPos1;

		auto p2 = cameraBlockPos;
		p2[checkAxis] += checkPos2;

		if (!world->isIntersect(p1, playerAABB) && !world->isIntersect(p2, playerAABB))
		{
			newEntityPos[checkAxis] = cameraPos[checkAxis];
		}
	};

	checkIntersect(0, -1, 1);
	checkIntersect(1, -2, 1);
	checkIntersect(2, -1, 1);

	entity->SetPos(newEntityPos);
	getCamera()->SetPos(newEntityPos);
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
	if (timingAction(destroyTime, interval) &&
		world->blockOnCursor(pos, pickDistance))
	{
		world->destroy(pos);
	}
}

void MinePlayer::placeBlockOnCursor()
{
	const float pickDistance = 14.0f;
	const float interval	 = 300;

	glm::ivec3 pos, side;
	if (timingAction(placeTime, interval) &&
		world->blockSideOnCursor(pos, side, pickDistance))
	{
		world->set(side, MineWorld::Grass);
	}
}
