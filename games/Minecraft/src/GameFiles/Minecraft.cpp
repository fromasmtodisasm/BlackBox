#include "Minecraft.h"

void Minecraft::init()
{
	building.entities	= &data.entities;
	building.blocks		= &data.blocks;
	assets.blocks		= &data.blocks;
	generation.building = building;
	picking.entities = &data.entities;
	picking.blocks = &data.blocks;
	building.picking = picking;

	assets.load();
	generation.generateTestScene();

	if (gEnv->pRenderer)
		m_CrossHair = gEnv->pRenderer->LoadTexture("Data/images/crosshair.bmp", 0, false);
}

void Minecraft::update()
{
	size_t ch_w = 20;
	size_t ch_h = 20;
	if (gEnv->pRenderer)
		gEnv->pRenderer->Draw2dImage(static_cast<float>(gEnv->pRenderer->GetWidth()) / 2 - 0.5f * ch_h, static_cast<float>(gEnv->pRenderer->GetHeight()) / 2 - 0.5f * ch_h, 20,20, m_CrossHair, 0, 0, 1, 1, 0, 0, 1, 0, 0.5);

}

void Assets::load()
{
	Block block{};
	block.obj	  = gEnv->p3DEngine->MakeObject("Data/minecraft/Grass_Block.obj");
	block.is_hard = true;
	block.light	  = 0;
	block.texture = -1;
	blocks->push_back(block);
}

void Building::remove(glm::ivec3 pos)
{
	if (auto e = entities->find(pos); e != entities->end())
	{
		e->second.entity->Remove();
		entities->erase(pos);
	}
	else
	{
		assert(0 && "sfasdf");
		CryError("there is no block on the position %d, %d, %d: ", pos.x, pos.y, pos.z);
	}
}

void Building::set(glm::ivec3 pos, Block::Type type)
{
	static unsigned entityCnt = 0;

	if (auto e = entities->find(pos); e != entities->end())
	{
		remove(pos);
		entities->erase(pos);
	}

	auto entity = gEnv->p3DEngine->MakeEntity(entityCnt++, 0);
	entity->SetPos(pos);
	CEntityObject obj;
	obj.scale  = glm::vec3(0.5f);
	obj.object = (*blocks)[type].obj;
	entity->SetScale(0.5f);
	entity->SetEntityObject(0, obj);

	auto transform = glm::scale(glm::mat4(1), obj.scale);
	transform	   = glm::rotate(transform, obj.angles.x, glm::vec3(1, 0, 0));
	transform	   = glm::rotate(transform, obj.angles.y, glm::vec3(0, 1, 0));
	transform	   = glm::rotate(transform, obj.angles.z, glm::vec3(0, 0, 1));
	transform	   = glm::translate(transform, obj.pos);

	entities->emplace(pos, BlockEntity{type, entity, transform});
}

void Building::destroyBlock()
{
	glm::ivec3 pos;
	if(picking.blockOnCursor(pos))
	{
		auto curTime = gEnv->pTimer->GetAsyncTime().GetMilliSeconds();
		auto delta = curTime - m_removeTime;
		if ((delta) > 500.f)
		{
			CryLog("remove delta time: %f", delta);
			remove(pos);
			m_removeTime = curTime;
		}
	}
}

void Generation::generateTestScene()
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
				building.set(glm::vec3(x, y, z), Block::Grass);
			}
		}
	}
	int width = 5;
	for (int y = 0; y < width; y++)
	{
		building.set(glm::vec3(0, y, 0), Block::Grass);
	}
	for (int i = 0; i < width; i++)
	{
		building.set(glm::vec3(i, 0, 0), Block::Grass);
		building.set(glm::vec3(i, width, 0), Block::Grass);
	}
	for (int i = 1; i < 4; i++)
	{
		building.set(glm::vec3((width - 1), i, 0), Block::Grass);
	}
	for (int i = width - 1; i > 4; i--)
	{
		building.set(glm::vec3(i, 3, 0), Block::Grass);
	}

	gEnv->pConsole->ExecuteString("load_level minecraft");
	gEnv->pConsole->ShowConsole(false);
}

bool Picking::blockOnCursor(glm::ivec3& outPos)
{
	auto game = static_cast<CXGame*>(gEnv->pSystem->GetIGame());
	auto client = &game->GetClient()->m_DummyClient;
	auto intersectionState = client->m_IntersectionState;

	auto& start = intersectionState.ray.start;
	gEnv->pRenderer->UnProjectFromScreen(
		intersectionState.mx, intersectionState.my, 0, &start.x, &start.y, &start.z);
	auto& end = intersectionState.ray.end;
	gEnv->pRenderer->UnProjectFromScreen(
		intersectionState.mx, intersectionState.my, 1, &end.x, &end.y, &end.z);


	Ray eyeRay;

	eyeRay.origin = client->m_CameraController.RenderCamera()->GetPos();
	eyeRay.direction = glm::normalize(end-start);

	float	  tMin	 = HUGE_VALF;
	float minDistance = HUGE_VALF;

	for (auto it : *entities)
	{
		auto entity = it.second;
		auto pos	= glm::vec3(it.first);
		auto camPos = client->m_CameraController.RenderCamera()->GetPos();
		auto distance = glm::distance(pos, camPos);
		//if (distance <= pickDistance)
		{
			auto obj  = entity.entity->GetIStatObj(0);
			AABB aabb = {obj->GetBoxMin(), obj->GetBoxMax()};
			aabb.Translate(pos);

			glm::vec2 tMinMax = aabb.IntersectBox(eyeRay);

			if (tMinMax.x < 0 || tMinMax.y < 0)
				continue;

			if (tMinMax.x < tMinMax.y && tMinMax.x < tMin)
			{
				tMin = tMinMax.x;
				glm::vec3 pickedPos = eyeRay.origin + eyeRay.direction * tMin;
				float blockDistance = glm::distance(eyeRay.origin, pickedPos);
				if(blockDistance < minDistance)
				{
					minDistance = blockDistance;
					outPos = it.first;
				}
			}
		}
	}

	return minDistance != HUGE_VALF;
}
