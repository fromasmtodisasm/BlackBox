#include <glm/gtx/hash.hpp>

class MineWorld
{
  public:
	enum Type
	{
		Grass,
	};

	void init();

	void set(glm::ivec3 pos, Type type);

	void destroy(glm::ivec3 pos);

	bool blockOnCursor(glm::ivec3& outBlockPos, float pickDistance);

	bool blockSideOnCursor(glm::ivec3& outBlockPos, glm::ivec3& outSidePos, float pickDistance);

  private:
	bool selectedPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, float pickDistance);

	bool tryDestroy(glm::ivec3 pos);

	std::unordered_map<glm::ivec3, IEntity*> entities;
	std::vector<IStatObj*>					 types;
	unsigned								 entityCnt = 0;
};

class MinePlayer
{
  public:
	void init(MineWorld* mineWorld);

	void destroyBlockOnCursor();

	void placeBlockOnCursor();

  private:
	MineWorld* world	= nullptr;
	float	   destroyTime = 0.0;
	float	   placeTime = 0.0;
};

class MineUI
{
  public:
	void init();

	void draw() const;

  private:
	unsigned crossHairTexture = 0;
};

struct Minecraft
{
	void init();

	void update() const;

	MineWorld  world;
	MineUI	   ui;
	MinePlayer player;
};