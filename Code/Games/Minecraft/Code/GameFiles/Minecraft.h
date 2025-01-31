#include <glm/gtx/hash.hpp>

class MineWorld
{
public:
	enum Type
	{
		Grass,
	};

	void                                     init();

	void                                     set(glm::ivec3 pos, Type type);

	void                                     destroy(glm::ivec3 pos);

	void                                     highliteCubeTmp(glm::ivec3 pos);

	bool                                     has(glm::ivec3 pos);

	bool                                     isIntersect(glm::ivec3 pos, AABB aabb) const;

	bool                                     pickPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, Ray eyeRay, float& pickDistance) const;

	//private:
	bool                                     tryDestroy(glm::ivec3 pos);

	int                                      nextEntity() { return entityCnt++; }

	IEntity*                                 SpawnBox(const Legacy::Vec3& pos, const Legacy::Vec3& velocity);

	int                                      entityCnt = 1;

	std::unordered_map<glm::ivec3, IEntity*> blocks;
	std::vector<IStatObj*>                   types;

	IStatObj*                                m_pMtlBox;
};

class MineDebug
{
public:
	void init();

	void update();

	void drawBox(glm::vec3 pos1, glm::vec3 pos2);

	void drawTmpBox(glm::vec3 pos1, glm::vec3 pos2);

private:
	IStatObj*             model = nullptr;
	std::vector<IEntity*> tmpBoxes;
};

class MinePlayer
{
public:
	void         init();

	void         update();

	void         destroyBlockOnCursor();

	void         placeBlockOnCursor();

	void         move(glm::vec3 direction, float value);

	bool         selectedPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, float& pickDistance) const;

	bool         blockSideOnCursor(glm::ivec3& outBlockPos, glm::ivec3& outSidePos, float& pickDistance) const;

	void         applyMovement();

	bool         moveOutsideCollisionByPoint(glm::vec3& newPos, glm::vec3 point);

	bool         moveOutsideCollision(glm::vec3& newPos);

	IEntity*     entity = nullptr;
	glm::vec3    movement{};

	ISound*      m_pSetBlockSound;
	ISound*      m_pDestroyBlockSound;

	float        cameraDistance = -4.0;
	float        destroyTime    = 0.0;
	float        placeTime      = 0.0;

	Legacy::Vec3 myPos;
	int          m_ClickFrame = 0;
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
	void       init();

	void       update();

	MineWorld  world;
	MineUI     ui;
	MinePlayer player;
	MineDebug  debug;

	IEntity*   Jack;
};

extern Minecraft* minecraft;