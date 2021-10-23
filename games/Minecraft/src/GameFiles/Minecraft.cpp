#include "Minecraft.h"

void Minecraft::init()
{
	blocks.loadAssets();

	g_World.size_x = 40;
	g_World.size_z = 40;
	g_World.height = -1;

	for (int y = g_World.height; y < 0; y++)
	{
		for (int z = -g_World.size_x / 2; z < g_World.size_x / 2; z++)
		{
			for (int x = -g_World.size_x / 2; x < g_World.size_x / 2; x++)
			{
				blocks.set(glm::vec3(x, y, z), Block::Grass);
			}
		}
	}
	int width = 5;
	for (int y = 0; y < width; y++)
	{
		blocks.set(glm::vec3(0, y, 0), Block::Grass);
	}
	for (int i = 0; i < width; i++)
	{
		blocks.set(glm::vec3(i, 0, 0), Block::Grass);
		blocks.set(glm::vec3(i, width, 0), Block::Grass);
	}
	for (int i = 1; i < 4; i++)
	{
		blocks.set(glm::vec3((width - 1), i, 0), Block::Grass);
	}
	for (int i = width - 1; i > 4; i--)
	{
		blocks.set(glm::vec3(i, 3, 0), Block::Grass);
	}

	gEnv->pConsole->ExecuteString("load_level minecraft");
	gEnv->pConsole->ShowConsole(false);
}

void Minecraft::update()
{
}

void BlockManager::loadAssets()
{
	// init block types
	Block block;
	block.obj	  = gEnv->p3DEngine->MakeObject("Data/minecraft/Grass_Block.obj");
	block.is_hard = true;
	block.light	  = 0;
	block.texture = -1;
	blocks.push_back(block);
}

void BlockManager::remove(glm::ivec3 pos)
{
	if (auto e = entities.find(pos); e != entities.end())
	{
		e->second.entity->Remove();
		entities.erase(pos);
	}
	else
	{
		CryError("there is no block on the position %d, %d, %d: ", pos.x, pos.y, pos.z);
	}
}

void BlockManager::set(glm::ivec3 pos, Block::Type type)
{
	if (auto e = entities.find(pos); e != entities.end())
	{
		e->second.entity->Remove();
		entities.erase(pos);
	}

	auto entity = gEnv->p3DEngine->MakeEntity(entityCnt++, 0);
	entity->SetPos(pos);
	CEntityObject obj;
	obj.scale  = glm::vec3(0.5f);
	obj.object = blocks[type].obj;
	entity->SetScale(0.5f);
	entity->SetEntityObject(0, obj);
}
