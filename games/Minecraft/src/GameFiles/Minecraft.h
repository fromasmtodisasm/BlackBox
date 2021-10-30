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

	bool blockOnCursor(glm::ivec3& outBlockPos, float pickDistance) const;

	bool blockSideOnCursor(glm::ivec3& outBlockPos, glm::ivec3& outSidePos, float pickDistance) const;

	bool isIntersect(glm::ivec3 pos, AABB aabb) const;

  private:
	bool selectedPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, float pickDistance) const;

	bool tryDestroy(glm::ivec3 pos);

	std::unordered_map<glm::ivec3, IEntity*> blocks;
	std::vector<IStatObj*>					 types;
};

class MinePlayer
{
  public:
	void init(MineWorld* mineWorld);

	void update();

	void destroyBlockOnCursor();

	void placeBlockOnCursor();

  private:
	MineWorld* world	   = nullptr;
	IEntity*   entity		   = nullptr;
	float	   destroyTime = 0.0;
	float	   placeTime   = 0.0;
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

	void update();

	MineWorld  world;
	MineUI	   ui;
	MinePlayer player;
};