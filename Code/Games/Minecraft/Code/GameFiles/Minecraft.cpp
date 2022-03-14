#undef min
#undef max

#include "Minecraft.h"
#include <glm/gtc/noise.hpp>
#include <glm/glm.hpp>
#include <array>
#include <BlackBox/System/File/CryFile.h>

Minecraft* minecraft;

AABB       entityWorldAABB(IEntity* entity)
{
	AABB aabb = {glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)};
	entity->GetBBox(aabb.min, aabb.max);
	auto pos = entity->GetPos();

	// перемещаем ограничивающую рамку в соответствии с его расположением
	aabb.Translate(glm::vec3(pos.x, pos.y, pos.z));

	return aabb;
}

void MineWorld::init()
{
	auto loadAssets = [this]()
	{
		auto grass = gEnv->p3DEngine->MakeObject("Data/minecraft/Grass_Block.obj");
		types.push_back(grass);
	};

	auto generateLevel = [this]()
	{
		g_World.size_x = 20;
		g_World.size_z = 20;
		g_World.height = -1;

		for (int y = g_World.height; y < 0; y++)
		{
			for (int z = -g_World.size_x / 2; z < g_World.size_x / 2; z++)
			{
				for (int x = -g_World.size_x / 2; x < g_World.size_x / 2; x++)
				{
					//if (-10 * glm::perlin(glm::vec3(float(z) * .05f, float(z) * 0.005f, 0.f)) <= 2)
					//	continue;
					set(glm::ivec3(x, y, z), Grass);
				}
			}
		}
		int width = 5;
		for (int y = 0; y < width; y++)
		{
			set(glm::ivec3(0, y, 0), Grass);
		}
		for (int i = 0; i < width; i++)
		{
			set(glm::ivec3(i, 0, 0), Grass);
			set(glm::ivec3(i, width, 0), Grass);
		}
		for (int i = 1; i < 4; i++)
		{
			set(glm::vec3((width - 1), i, 0), Grass);
		}
		for (int i = width - 1; i > 4; i--)
		{
			set(glm::ivec3(i, 3, 0), Grass);
		}
	};

	loadAssets();
	generateLevel();

	gEnv->pConsole->ExecuteString("load_level minecraft");
	gEnv->pConsole->ShowConsole(false);
}

void MineUI::init()
{
	crossHairTexture = gEnv->pRenderer->LoadTexture(
	    "Textures/crosshair.png", nullptr, false);
}

void MineUI::draw() const
{
	auto        centerX = (float)gEnv->pRenderer->GetWidth() / 2;
	auto        centerY = (float)gEnv->pRenderer->GetHeight() / 2;

	const float width   = 20;
	const float height  = 20;

	gEnv->pRenderer->Draw2dImage(
	    centerX - 0.5f * width,
	    centerY - 0.5f * height, 20, 20, (int)crossHairTexture,
	    0, 0, 1, 1, 0, 0, 1, 0, 0.5);
}

void Minecraft::init()
{
	minecraft = this;
	//if (!gEnv->pCryPak->OpenPack("Data.pak"))
	//	CryFatalError("Cannot open Data.pak");

	world.init();
	ui.init();
	player.init();
	debug.init();
}

void Minecraft::update()
{
	debug.update();
	ui.draw();
	player.update();
}

bool MineWorld::tryDestroy(glm::ivec3 pos)
{
	if (auto e = blocks.find(pos); e != blocks.end())
	{
		gEnv->pEntitySystem->RemoveEntity(e->second->GetId(), true);
		blocks.erase(pos);
		return true;
	}
	return false;
}

void MineWorld::destroy(glm::ivec3 pos)
{
	if (!tryDestroy(pos))
	{
		CryError("there is no block on the position %d, %d, %d: ", pos.x, pos.y, pos.z);
	}
}

// FIXME: work incorrect(aabb has wrong position in world before first place block)
void MineWorld::highliteCubeTmp(glm::ivec3 pos)
{
	if (auto const e = blocks.find(pos); e != blocks.end())
	{
		auto const aabb = entityWorldAABB(e->second);
		gEnv->pAuxGeomRenderer->DrawAABB(aabb.min - 0.1f, aabb.max + 0.1f, {1, 1, 1, 1});
	}
}

bool MineWorld::has(glm::ivec3 pos)
{
	return blocks.count(pos);
}

void MineWorld::set(glm::ivec3 pos, Type type)
{
	tryDestroy(pos);

	CEntityDesc desc(0, 0);
	auto        entity = gEnv->pEntitySystem->SpawnEntity(desc);
	gEnv->p3DEngine->RegisterEntity(entity);

	entity->SetIStatObj(types[type]);
	entity->SetPos(pos);
	entity->SetScale(glm::vec3(1.0f));

	blocks.emplace(pos, entity);
}

CCamera* getCamera()
{
	auto game   = dynamic_cast<CXGame*>(gEnv->pSystem->GetIGame());
	auto client = &game->GetClient()->m_DummyClient;
	return client->m_CameraController.RenderCamera();
}

bool MineWorld::pickPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, Ray eyeRay, float& pickDistance) const
{
	float tMin        = HUGE_VALF;
	float minDistance = HUGE_VALF;

	for (auto it : blocks)
	{
		auto const pos      = glm::vec3(it.first);
		auto const camPos   = getCamera()->GetPos();
		auto const distance = glm::distance(pos, camPos);
		auto       entity   = it.second;

		if (distance <= pickDistance)
		{
			auto const      obj     = entity->GetIStatObj(0);

			auto const      aabb    = entityWorldAABB(entity);

			// расстояния до ближайшей и дальнейшей точек пересечения
			glm::vec2 const tMinMax = aabb.IntersectBox(eyeRay);

			// если объект был за спиной
			if (tMinMax.x < 0 || tMinMax.y < 0)
				continue;

			// если мы не внутри коробки и пересечение ближайшее
			if (tMinMax.x < tMinMax.y && tMinMax.x < tMin)
			{
				tMin                      = tMinMax.x;
				glm::vec3   pickedPos     = eyeRay.origin + eyeRay.direction * tMin;
				float const blockDistance = glm::distance(eyeRay.origin, pickedPos);
				if (blockDistance < minDistance)
				{
					minDistance = blockDistance;
					outBlockPos = it.first;
					outPickPos  = pickedPos;
				}
			}
		}
	}
	pickDistance = minDistance;
	return minDistance != HUGE_VALF;
}

bool MinePlayer::selectedPos(glm::ivec3& outBlockPos, glm::vec3& outPickPos, float& pickDistance) const
{
	auto  game              = dynamic_cast<CXGame*>(gEnv->pSystem->GetIGame());
	auto  intersectionState = game->GetClient()->m_DummyClient.m_IntersectionState;

	// начальная и конечная точки запуска луча
	auto& start             = intersectionState.ray.start;
	gEnv->pRenderer->UnProjectFromScreen(
	    intersectionState.mx, intersectionState.my, 0, &start.x, &start.y, &start.z);
	auto& end = intersectionState.ray.end;
	gEnv->pRenderer->UnProjectFromScreen(
	    intersectionState.mx, intersectionState.my, 1, &end.x, &end.y, &end.z);

	Ray eyeRay{};
	eyeRay.origin    = getCamera()->GetPos();
	eyeRay.direction = glm::normalize(end - start);

	return minecraft->world.pickPos(outBlockPos, outPickPos, eyeRay, pickDistance);
}

int signDirection(float val)
{
	if (val > 0)
	{
		return 1;
	}
	else if (val == 0)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

float roundFloat(float d)
{
	return floorf(d + 0.5f);
}

glm::ivec3 floorVec(glm::vec3 v)
{
	return {floorf(v.x), floorf(v.y), floorf(v.z)};
}

bool isFloatInteger(float a, float s = 0.00003)
{
	// если число округлилось и осталось равно себе с определенным допуском мы счиатем его целочисленным
	float ceiled = roundFloat(a);
	return abs(ceiled - a) <= s;
}

bool MinePlayer::blockSideOnCursor(glm::ivec3& outBlockPos, glm::ivec3& outSidePos, float& pickDistance) const
{
	glm::vec3 pickPos;
	if (selectedPos(outBlockPos, pickPos, pickDistance))
	{
		auto const origin = getCamera()->GetPos();
		outSidePos        = outBlockPos;

		if (isFloatInteger(pickPos.x))
		{
			outSidePos.x -= signDirection(outSidePos.x - origin.x);
		}
		else if (isFloatInteger(pickPos.y))
		{
			outSidePos.y -= signDirection(outSidePos.y - origin.y);
		}
		else if (isFloatInteger(pickPos.z))
		{
			outSidePos.z -= signDirection(outSidePos.z - origin.z);
		}
		return true;
	}
	return false;
}

bool isBoundigPointCoDirectedWithMovement(IEntity* entity, glm::vec3 point, glm::vec3 movement)
{
	glm::vec3 min, max;
	entity->GetBBox(min, max);

	auto const center   = min + (max - min) * 0.5f;

	auto const pointDir = point - center;
	auto const v        = pointDir * movement;

	// если вектор движения не совпал с направлением от центра игрока до рассматриваемой точки,
	// то смысла проверять пересечение в этой точке нет
	return !(v.x <= 0 && v.y <= 0 && v.z <= 0);
}

std::array<glm::vec3, 8> getBoundingPoints(IEntity* entity)
{
	glm::vec3 min, max;
	entity->GetBBox(min, max);

	return {
	    min,
	    {min.x, max.y, max.z},
	    {min.x, max.y, min.z},
	    {min.x, min.y, max.z},
	    max,
	    {max.x, min.y, min.z},
	    {max.x, min.y, max.z},
	    {max.x, max.y, min.z}};
}

bool MinePlayer::moveOutsideCollisionByPoint(glm::vec3& newPos, glm::vec3 point)
{
	auto checkCube = floorVec(newPos + point);

	if (!minecraft->world.isIntersect(checkCube, entityWorldAABB(entity)))
	{
		return false;
	}

	glm::ivec3 const intersectSide = floorVec(newPos + point + 0.5f);

	// добавляем небольшое смещение, чтобы избежать попадания на грань куба
	auto const       dif           = newPos + point - glm::vec3(intersectSide) + point * 0.00001f;

	float            minDif        = HUGE_VALF;
	int              minI          = -1;

	for (int i = 0; i != 3; ++i)
	{
		glm::vec3 newBlockPos = newPos + point;
		newBlockPos[i] -= dif[i];

		if (dif[i] != 0.0f && abs(dif[i]) < minDif && !minecraft->world.has(floorVec(newBlockPos)))
		{
			minDif = abs(dif[i]);
			minI   = i;
		}
	}

	if (minI != -1)
	{
		newPos[minI] -= dif[minI];
		return true;
	}

	return false;
}

bool MinePlayer::moveOutsideCollision(glm::vec3& newPos)
{
	for (auto const point : getBoundingPoints(entity))
	{
		if (isBoundigPointCoDirectedWithMovement(entity, point, movement))
		{
			if (moveOutsideCollisionByPoint(newPos, point))
			{
				return true;
			}
		}
	}
	return false;
}

void MinePlayer::applyMovement()
{
	auto& world  = minecraft->world;
	auto  newPos = entity->GetPos() + movement;

	// если мы передвинулись, то запускаем проверку пересечений опять
	while (moveOutsideCollision(newPos))
	{
	}

	for (int i = 0; i < 3; ++i)
	{
		newPos[i] = round(newPos[i] * 10000) / 10000;
	}

	entity->SetPos(newPos);
}

bool MineWorld::isIntersect(glm::ivec3 pos, AABB otherAABB) const
{
	auto block = blocks.find(pos);
	if (block != blocks.end())
	{
		auto const aabb = entityWorldAABB(block->second);
		return aabb.IsIntersectBox(otherAABB);
	}
	return false;
}

void MinePlayer::init()
{
	getCamera()->mode = CCamera::Mode::FPS;

	auto        steve = gEnv->p3DEngine->MakeObject("Data/minecraft/minecraft_steve.obj");

	CEntityDesc desc(0, 0);
	entity = gEnv->pEntitySystem->SpawnEntity(desc);
	gEnv->p3DEngine->RegisterEntity(entity);

	entity->SetPos(glm::vec3(5, 20, 5));
	glm::vec3 min{-0.4, -2.3, -0.4}, max{0.4, 0.4, 0.4};
	entity->SetBBox(min, max);

	m_pSetBlockSound     = gEnv->pSoundSystem->LoadSound("sounds/doors/open.wav", 0);
	m_pDestroyBlockSound = gEnv->pSoundSystem->LoadSound("sounds/doors/close.wav", 0);
}

void MinePlayer::update()
{
	auto const aabb = entityWorldAABB(entity);

	gEnv->pAuxGeomRenderer->DrawAABB(aabb.min, aabb.max, {1, 1, 1, 1});

	auto const gravity = 4.f;
	auto       ft      = gEnv->pTimer->GetRealFrameTime();
	move(glm::vec3(0.0f, -1.0f, 0.0f), gravity * ft);

	applyMovement();
	getCamera()->SetPos(entity->GetPos());
	movement = glm::vec3(0.0f);
}

bool timingAction(float& prevTime, float interval)
{
	auto const curTime = gEnv->pTimer->GetAsyncTime().GetMilliSeconds();
	auto const delta   = curTime - prevTime;
	//CryLog("delta time: %f", delta);
	if (delta > interval)
	{
		prevTime = curTime;
		return true;
	}
	return false;
}

void MinePlayer::destroyBlockOnCursor()
{
	float       pickDistance = 1000.0f;
	const float interval     = 300;

	glm::ivec3  pos;
	glm::vec3   outPickPos;

	if (timingAction(destroyTime, interval) &&
	    selectedPos(pos, outPickPos, pickDistance))
	{
		minecraft->world.destroy(pos);
		m_pDestroyBlockSound->Play();
	}
}

void MinePlayer::placeBlockOnCursor()
{
	float       pickDistance = 1000.0f;
	const float interval     = 300;

	glm::ivec3  pos, side;
	if (timingAction(placeTime, interval) &&
	    blockSideOnCursor(pos, side, pickDistance))
	{
		minecraft->world.set(side, MineWorld::Grass);
		m_pSetBlockSound->Play();
	}
}

void MinePlayer::move(glm::vec3 direction, float value)
{
	movement += direction * value;
}

void MineDebug::init()
{
	model = gEnv->p3DEngine->MakeObject("Data/minecraft/bbox.obj");
	//drawBox({0, 0, 0}, {5, 5, 5});
}

void MineDebug::update()
{
	//CryLog("MineDebug::update");
	static float prevTime = 0;
	auto const   curTime  = gEnv->pTimer->GetAsyncTime().GetMilliSeconds();
	auto const   delta    = curTime - prevTime;
	prevTime              = curTime;
	//CryLog("delta time: %f", delta);
	for (auto box : tmpBoxes)
	{
		//gEnv->p3DEngine->UnRegisterEntity(box);
		gEnv->pEntitySystem->RemoveEntity(box->GetId());
	}
	tmpBoxes.clear();
}

#if 0
void MineDebug::drawBox(glm::vec3 pos1, glm::vec3 pos2)
{
	makeBox(model, pos1, pos2);
}

void MineDebug::drawTmpBox(glm::vec3 pos1, glm::vec3 pos2)
{
	tmpBoxes.push_back(makeBox(model, pos1, pos2));
}
#endif