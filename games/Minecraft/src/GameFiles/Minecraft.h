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

	bool blockOnCursor(glm::ivec3& outPos, float pickDistance);

  private:
	bool tryDestroy(glm::ivec3 pos);

	std::unordered_map<glm::ivec3, IEntity*> entities;
	std::vector<IStatObj*>						types;
	unsigned entityCnt = 0;
};

class MinePlayer
{
  public:
	void init(MineWorld* mineWorld);

	void destroyBlockOnCursor();

  private:
	MineWorld* world = nullptr;
	float prevTime = 0.0;
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

	MineWorld	  world;
	MineUI ui;
	MinePlayer player;
};