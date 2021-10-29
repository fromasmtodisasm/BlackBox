#include "Minecraft.h"

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
	world.init();
	ui.init();
	player.init(&world);
}

void Minecraft::update() const
{
	ui.draw();
}

bool MineWorld::tryDestroy(glm::ivec3 pos)
{
	if (auto e = entities.find(pos); e != entities.end())
	{
		e->second->Remove();
		entities.erase(pos);
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

	auto entity = gEnv->p3DEngine->MakeEntity(entityCnt++, 0);
	entity->SetPos(pos);
	CEntityObject obj;
	obj.scale  = glm::vec3(0.5f);
	obj.object = types[type];
	entity->SetScale(0.5f);
	entity->SetEntityObject(0, obj);

	entities.emplace(pos, entity);
}

bool MineWorld::blockOnCursor(glm::ivec3& outPos, float pickDistance)
{
	auto game			   = dynamic_cast<CXGame*>(gEnv->pSystem->GetIGame());
	auto client			   = &game->GetClient()->m_DummyClient;
	auto intersectionState = client->m_IntersectionState;

	auto& start = intersectionState.ray.start;
	gEnv->pRenderer->UnProjectFromScreen(
		intersectionState.mx, intersectionState.my, 0, &start.x, &start.y, &start.z);
	auto& end = intersectionState.ray.end;
	gEnv->pRenderer->UnProjectFromScreen(
		intersectionState.mx, intersectionState.my, 1, &end.x, &end.y, &end.z);

	Ray eyeRay{};
	eyeRay.origin	 = client->m_CameraController.RenderCamera()->GetPos();
	eyeRay.direction = glm::normalize(end - start);

	float tMin		  = HUGE_VALF;
	float minDistance = HUGE_VALF;

	for (auto it : entities)
	{
		auto const pos		= glm::vec3(it.first);
		auto const camPos	= client->m_CameraController.RenderCamera()->GetPos();
		auto const distance = glm::distance(pos, camPos);
		if (distance <= pickDistance)
		{
			auto const obj = it.second->GetIStatObj(0);

			AABB aabb = {obj->GetBoxMin(), obj->GetBoxMax()};
			aabb.min *= it.second->GetScale();
			aabb.max *= it.second->GetScale();

			auto tPos = pos;
			tPos.y += 0.5;
			aabb.Translate(tPos);

			glm::vec2 const tMinMax = aabb.IntersectBox(eyeRay);

			if (tMinMax.x < 0 || tMinMax.y < 0)
				continue;

			if (tMinMax.x < tMinMax.y && tMinMax.x < tMin)
			{
				tMin					  = tMinMax.x;
				glm::vec3	pickedPos	  = eyeRay.origin + eyeRay.direction * tMin;
				float const blockDistance = glm::distance(eyeRay.origin, pickedPos);
				if (blockDistance < minDistance)
				{
					minDistance = blockDistance;
					outPos		= it.first;
				}
			}
		}
	}

	return minDistance != HUGE_VALF;
}
void MinePlayer::init(MineWorld* mineWorld)
{
	world = mineWorld;
}

void MinePlayer::destroyBlockOnCursor()
{
	const float pickDistance = 7.0f;
	const float removeTime	 = 300;

	glm::ivec3 pos;
	if (world->blockOnCursor(pos, pickDistance))
	{
		auto const curTime = gEnv->pTimer->GetAsyncTime().GetMilliSeconds();
		auto const delta   = curTime - prevTime;
		if (delta > removeTime)
		{
			CryLog("remove delta time: %f", delta);
			world->destroy(pos);
			prevTime = curTime;
		}
	}
}
