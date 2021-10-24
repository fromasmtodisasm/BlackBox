#include <glm/gtx/hash.hpp>

struct Block
{
	enum Type
	{
		Grass,
	};

	IStatObj* obj = nullptr;
	int		  texture = -1;
	bool	  is_hard = false;
	float	  light	  = 0;
};

struct BlockEntity
{
	Block::Type type;
	IEntity* entity = nullptr;
	glm::mat4 transformMatrix{};
};

using BlockData = std::vector<Block>;

using EntityData = std::unordered_map<glm::ivec3, BlockEntity>;

struct Data
{
	BlockData						blocks;
	EntityData entities;
};

struct Picking
{
	bool blockOnCursor(glm::ivec3& outPos);

	glm::ivec3 facePosition(glm::ivec3 block);

	void buildBlock(glm::ivec3 pos);

	EntityData* entities;
	BlockData* blocks;
};

struct Building
{
	void set(glm::ivec3 pos, Block::Type type);

	void remove(glm::ivec3 pos);

	void destroyBlock();

	EntityData* entities;
	BlockData* blocks;
	Picking picking;

	float m_removeTime = 0;
};

struct Assets
{
	void load();

	BlockData* blocks;
};

struct Generation
{
	void generateTestScene();

	Building building;
};


struct Minecraft
{
	void init();

	void update();

	Data data;
	Building building{};
	Assets assets{};
	Generation generation{};
	Picking picking{};

	int m_CrossHair = -1;
};