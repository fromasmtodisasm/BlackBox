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
}

void Minecraft::update()
{
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
		CryError("there is no block on the position %d, %d, %d: ", pos.x, pos.y, pos.z);
	}
}

void Building::set(glm::ivec3 pos, Block::Type type)
{
	static unsigned entityCnt = 0;

	if (auto e = entities->find(pos); e != entities->end())
	{
		remove(pos)
		e->second.entity->Remove();
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
		remove(pos);
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

	//gEnv->pConsole->ExecuteString("load_level minecraft");
	gEnv->pConsole->ShowConsole(false);
}

bool Picking::blockOnCursor(glm::ivec3& outPos)
{
	float pickDistance = 7.0;

	/*
	float mouseX = 0, mouseY = 0;
	gEnv->pHardwareMouse->GetHardwareMousePosition(&mouseX, &mouseY);
	float screenWidth  = (float)gEnv->pRenderer->GetWidth();
	float screenHeight = (float)gEnv->pRenderer->GetHeight();

	float pointX = ((2.0f * mouseX) / screenWidth) - 1.0f;
	float pointY = (((2.0f * mouseY) / screenHeight) - 1.0f) * -1.0f;
	pointX = 0;
	pointY = 0;

	auto camera		= gEnv->pRenderer->GetCamera();
	auto projMatrix = camera.getProjectionMatrix();
	//pointX /= projMatrix[1][1];
	//pointY /= projMatrix[2][2];

	auto	  inverseViewMatrix = glm::inverse(camera.GetViewMatrix());
	glm::vec3 direction;

	direction.x = (pointX * inverseViewMatrix[1][1]) +
				  (pointY * inverseViewMatrix[2][1]) + inverseViewMatrix[3][1];
	direction.y = (pointX * inverseViewMatrix[1][2]) +
				  (pointY * inverseViewMatrix[2][2]) + inverseViewMatrix[3][2];
	direction.z = (pointX * inverseViewMatrix[1][3]) +
				  (pointY * inverseViewMatrix[2][3]) + inverseViewMatrix[3][3];
	*/

	auto game = static_cast<CXGame*>(gEnv->pSystem->GetIGame());
	auto client = &game->GetClient()->m_DummyClient;
	auto intersectionState = client->m_IntersectionState;
	auto centerX = (float)gEnv->pRenderer->GetWidth() / 2;
	auto centerY = (float)gEnv->pRenderer->GetHeight() / 2;

	auto& start = intersectionState.ray.start;

	gEnv->pRenderer->UnProjectFromScreen(
		centerX, centerY, 0, &start.x, &start.y, &start.z);
	auto& end = intersectionState.ray.end;
	gEnv->pRenderer->UnProjectFromScreen(
		centerX, centerY, 1, &end.x, &end.y, &end.z);

	Ray eyeRay;

	eyeRay.origin = client->m_CameraController.RenderCamera()->GetPos();
	eyeRay.direction = glm::normalize(end-start);

	//glm::vec3 origin = camera.transform.position;
	float	  tMin	 = HUGE_VALF;
	float minDistance = HUGE_VALF;

	for (auto it : *entities)
	{
		auto entity = it.second;
		auto pos	= glm::vec3(it.first);
		auto distance = glm::distance(pos, eyeRay.origin);
		//if (distance <= pickDistance)
		{
			auto inverseTransformMatrix = glm::inverse(entity.transformMatrix);
			//auto rayOrigin =
				//glm::vec4(eyeRay.origin, 1.0) * inverseTransformMatrix;
			//auto rayDirection =
				//glm::normalize(glm::vec4(eyeRay.direction, 0.0) * inverseTransformMatrix);

			auto obj  = entity.entity->GetIStatObj(0);
			AABB aabb = {obj->GetBoxMin(), obj->GetBoxMax()};

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
