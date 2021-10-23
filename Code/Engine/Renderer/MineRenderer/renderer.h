//
// Created by smirn on 22.10.2021.
//

#ifndef BLACKBOX_WIN64_RENDERER_H
#define BLACKBOX_WIN64_RENDERER_H

#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/3DEngine/IStatObj.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <glm/gtx/hash.hpp>

class BlockType
{
	friend class BlockEntity;

  public:
	void loadMesh(std::string const& path)
	{
		obj = gEnv->p3DEngine->MakeObject(path.data());
	}

	void loadTexture(std::string const& path)
	{
		texture = gEnv->pRenderer->LoadTexture(path.data());
		obj->SetTexture(texture);
	}

	void setLighting(float value)
	{
		if (value > 0)
		{
		}
	}

  private:
	IStatObj* obj;
	unsigned  texture;
	float	  lighting;
};

class BlockEntity
{
  public:
	void setType(BlockType const* type)
	{
		entity = gEnv->p3DEngine->MakeEntity(cnt++, 0);
		CEntityObject obj;
		obj.scale  = glm::vec3(1);
		obj.object = type->obj;
		entity->SetEntityObject(0, obj);
	}

	void calcWorldSpace(glm::vec3 position)
	{
		entity->SetPos(position);
	}

  private:
	int		 cnt	= 0;
	IEntity* entity = nullptr;
};

class Minecraft
{
  public:
	void setBlock(std::string const& block, glm::vec<3, int> position)
	{
		if (world.find(position) != world.end())
		{
			throw std::runtime_error("the position already has a block");
		}

		BlockEntity entity;
		entity.calcWorldSpace(position);
		entity.setType(&blocks.at(block));
		world.emplace(position, entity);
	}

	void init()
	{
		BlockType grass{};
		grass.loadMesh("Data/minecraft/Grass_Block.obj");
		blocks.emplace("grass", grass);

		setBlock("grass", glm::vec3(0, 0, 0));
	}

	void update()
	{
	}

	std::unordered_map<glm::vec<3, int>, BlockEntity> world;
	std::unordered_map<std::string, BlockType>		blocks;
};

#endif // BLACKBOX_WIN64_RENDERER_H
