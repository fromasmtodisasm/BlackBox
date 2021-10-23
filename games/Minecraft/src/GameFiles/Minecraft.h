#include <glm/gtx/hash.hpp>

struct BlockDesc
{
	std::string name;
	std::string mesh_path;
	std::string texture_path;
	bool		is_hard = false;
	float		light	= 0;
};

struct Block
{
	enum Type
	{
		Grass,
	};

	IStatObj* obj;
	int		  texture;
	bool	  is_hard = false;
	float	  light	  = 0;
};

struct BlockEntity
{
	IEntity* entity;
};

class BlockManager
{
  public:
	void loadAssets();

	void remove(glm::ivec3 pos);

	void set(glm::ivec3 pos, Block::Type type);

  private:
	std::vector<Block>							blocks;
	std::unordered_map<glm::ivec3, BlockEntity> entities;
	unsigned									entityCnt = 0;
};

class Minecraft
{
  public:
	void init();

	void update();

	BlockManager blocks;
};