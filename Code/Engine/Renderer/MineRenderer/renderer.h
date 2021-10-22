//
// Created by smirn on 22.10.2021.
//

#ifndef BLACKBOX_WIN64_RENDERER_H
#define BLACKBOX_WIN64_RENDERER_H

#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/3DEngine/IStatObj.hpp>

class BlockType
{
	friend class BlockEntity;
  public:
	void loadMesh(std::string const& path)
	{
		obj = gEnv->p3DEngine->MakeObject(path);
	}

	void loadTexture(std::string const& path)
	{
		texture = gEnv->pRenderer->LoadTexture(path.data());
	}

	void setLighting(float value)
	{
		if(value > 0)
		{
		}
	}

  private:
	IStatObj * obj;
	int texture;
	std::string meshPath;
	std::string texturePath;
	float		lighting;
};

class BlockEntity
{
  public:
	void setType(BlockType const* type)
	{
		type->obj->P
	}

	void calcWorldSpace(glm::vec3 position)
	{

	}

  private:

};

class Minecraft
{
	void setBlock(Block const* block, glm::vec3 position)
	{
	}

	void init()
	{

	}

	void update()
	{

	}
};

#endif //BLACKBOX_WIN64_RENDERER_H
