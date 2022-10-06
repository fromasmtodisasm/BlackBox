#include "EntitySystem.hpp"
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/System/ITimer.hpp>

#include <BlackBox/Core/Platform/platform_impl.inl>

#include "PhysicalEntity.hpp"

#define LOG_FUNCTION() CryComment("[EntitySystemLogging]: %s", __FUNCTION__)

const int MAX_ENTITYES = 64 * 1024;

inline CEntitySystem::CEntitySystem(ISystem* pSystem)
    : m_EntityIt(m_Entities)
{
#if 1
	//m_Entities.resize(16 * 16 * 256 + 1024);
	m_Entities.resize(MAX_ENTITYES);
#endif

	{
#define SET_SCRIPTEVENT(event) Env::ScriptSystem()->SetGlobalValue("ScriptEvent_" #event, ScriptEvent_##event)
		SET_SCRIPTEVENT(Activate);
		SET_SCRIPTEVENT(Deactivate);
		SET_SCRIPTEVENT(FireModeChange);
		SET_SCRIPTEVENT(DropItem);
		SET_SCRIPTEVENT(Reset);
		SET_SCRIPTEVENT(Contact);
		SET_SCRIPTEVENT(Enter);
		SET_SCRIPTEVENT(Leave);
		SET_SCRIPTEVENT(Timer);
		SET_SCRIPTEVENT(StartAnimation);
		SET_SCRIPTEVENT(AnimationKey);
		SET_SCRIPTEVENT(EndAnimation);
		SET_SCRIPTEVENT(Respawn);
		SET_SCRIPTEVENT(ItemActivated);
		SET_SCRIPTEVENT(Hit);
		SET_SCRIPTEVENT(Fire);
		SET_SCRIPTEVENT(WeaponReady);
		SET_SCRIPTEVENT(StopFiring);
		SET_SCRIPTEVENT(Reload);
		SET_SCRIPTEVENT(Command);
		SET_SCRIPTEVENT(FireGrenade);
		SET_SCRIPTEVENT(Die);
		SET_SCRIPTEVENT(ZoomToggle);
		SET_SCRIPTEVENT(ZoomIn);
		SET_SCRIPTEVENT(ZoomOut);
		SET_SCRIPTEVENT(Land);
		SET_SCRIPTEVENT(FireCancel);
		SET_SCRIPTEVENT(GameDefinedEvent);
		SET_SCRIPTEVENT(ViewModeChange);
		SET_SCRIPTEVENT(SelectWeapon);
		SET_SCRIPTEVENT(Deafened);
		SET_SCRIPTEVENT(StanceChange);
		SET_SCRIPTEVENT(CycleGrenade);
		SET_SCRIPTEVENT(Use);
		SET_SCRIPTEVENT(MeleeAttack);
		SET_SCRIPTEVENT(PhysicalizeOnDemand);
		SET_SCRIPTEVENT(PhysCollision);
		SET_SCRIPTEVENT(FlashLightSwitch);
		SET_SCRIPTEVENT(EnterWater);
		SET_SCRIPTEVENT(CycleVehiclePos);
		SET_SCRIPTEVENT(AllClear);
		SET_SCRIPTEVENT(Expression);
		SET_SCRIPTEVENT(InVehicleAnimation);
		SET_SCRIPTEVENT(InVehicleAmmo);
		SET_SCRIPTEVENT(ProcessCharacterEffects);
		SET_SCRIPTEVENT(Jump);
#undef SET_SCRIPTEVENT
	}

	m_PhysicsInitParams.Create();
	auto [a, b, c, d] = m_PhysicsInitParams;
	m_pPhysicalWorld  = new btDiscreteDynamicsWorld(a, b, c, d);
	m_pPhysicalWorld->setGravity(btVector3(0, -9.8, 0));

	btStaticPlaneShape* floorShape = new btStaticPlaneShape(btVector3(0, 1, 0), -1);
	auto                rigidFloor = new btRigidBody(0.f, new btDefaultMotionState, floorShape);
	m_pPhysicalWorld->addRigidBody(rigidFloor);

	auto debugger = new CPhysicsDebugger;
	m_pPhysicalWorld->setDebugDrawer(debugger);
}

CEntitySystem::~CEntitySystem()
{
	if (auto debugger = m_pPhysicalWorld->getDebugDrawer(); debugger)
	{
		SAFE_DELETE(debugger);
	}
}

void CEntitySystem::Update()
{
	auto time = Env::Timer()->GetFrameTime();
	m_pPhysicalWorld->stepSimulation(time);
	//m_pPhysicalWorld->debugDrawWorld();
}

IScriptSystem* CEntitySystem::GetScriptSystem()
{
	LOG_FUNCTION();
	return nullptr;
}

void CEntitySystem::Reset()
{
	LOG_FUNCTION();
}

IEntity* CEntitySystem::SpawnEntity(CEntityDesc& ed, bool t)
{
	if (m_Entities.size() <= MAX_ENTITYES)
	{
		auto e = &m_Entities[ed.id];

		if (t)
		{
			InitEntity(e, ed);
			if (m_pEntitySystemSink)
				m_pEntitySystemSink->OnSpawnContainer(ed, e);
		}

		m_nSpawnedEntities++;
		return e;
	}

	return nullptr;
}

bool CEntitySystem::InitEntity(IEntity* pEntity, CEntityDesc& ed)
{
	CEntity* e = (CEntity*)pEntity;
	e->~CEntity();

	new (e) CEntity(ed);

	e->SetClassId(ed.ClassId);
	e->SetName(ed.name);

	if (m_pEntitySystemSink)
		m_pEntitySystemSink->OnSpawnContainer(ed, e);
	return true;
}

IEntity* CEntitySystem::GetEntity(EntityId id)
{
	return &m_Entities[id];
}

IEntity* CEntitySystem::GetEntity(const char* sEntityName)
{
	//LOG_FUNCTION();
	return nullptr;
}

EntityId CEntitySystem::FindEntity(const char* name) const
{
	LOG_FUNCTION();
	return EntityId();
}

void CEntitySystem::RemoveEntity(EntityId entity, bool w)
{
	LOG_FUNCTION();
	CryComment("Try remove entity with id: %d", entity);
	m_nSpawnedEntities--;
	auto pEntity = &m_Entities[entity];
	m_pEntitySystemSink->OnRemove(pEntity);
	Env::I3DEngine()->UnRegisterEntity(pEntity);
}

int CEntitySystem::GetNumEntities() const
{
	LOG_FUNCTION();
	return m_Entities.size();
}

IEntityIt* CEntitySystem::GetEntityIterator()
{
	LOG_FUNCTION();
	return &m_EntityIt;
}

IEntityIt* CEntitySystem::GetEntityInFrustrumIterator(bool e)
{
	LOG_FUNCTION();
	return nullptr;
}

void CEntitySystem::GetEntitiesInRadius(const Legacy::Vec3& origin, float radius, std::vector<IEntity*>& entities, int s) const
{
	for (size_t i = 0; i < m_nSpawnedEntities; i++)
	{
		auto* e = (CEntity*)&m_Entities[i];
		Legacy::Vec3 min, max;
		e->GetBBox(min, max);
		if (glm::length(e->GetPos() - origin) <= radius)
			entities.push_back((IEntity*)e);
	}
}

void CEntitySystem::SetSink(IEntitySystemSink* sink)
{
	m_pEntitySystemSink = sink;
}

void CEntitySystem::PauseTimers(bool bPause, bool e)
{
	LOG_FUNCTION();
}

void CEntitySystem::RemoveSink(IEntitySystemSink* sink)
{
	m_pEntitySystemSink = nullptr;
}

IEntityCamera* CEntitySystem::CreateEntityCamera()
{
	LOG_FUNCTION();
	return nullptr;
}

void CEntitySystem::EnableClient(bool bEnable)
{
	LOG_FUNCTION();
}

void CEntitySystem::EnableServer(bool bEnable)
{
	LOG_FUNCTION();
}

void CEntitySystem::Release()
{
	LOG_FUNCTION();
	delete this;
}

bool CEntitySystem::IsIDUsed(EntityId nID)
{
	LOG_FUNCTION();
	return false;
}

void CEntitySystem::ResetEntities()
{
	LOG_FUNCTION();
	m_Entities.clear();
}

void CEntitySystem::GetMemoryUsage(ICrySizer* pSizer) const
{
	LOG_FUNCTION();
	pSizer->AddObject(this, sizeof(*this));
	pSizer->AddObject(m_Entities);
}

void CEntitySystem::SetDynamicEntityIdMode(const bool bActivate)
{
	LOG_FUNCTION();
}

void CEntitySystem::SetDefaultEntityUpdateLevel(EEntityUpdateVisLevel eDefault)
{
	LOG_FUNCTION();
}

void CEntitySystem::SetPrecacheResourcesMode(bool bPrecaching)
{
	LOG_FUNCTION();
}

bool CEntitySystem::IsDynamicEntityId(EntityId id)
{
	LOG_FUNCTION();
	return false;
}

void CEntitySystem::MarkId(EntityId id)
{
	LOG_FUNCTION();
}

void CEntitySystem::ClearId(EntityId id)
{
	LOG_FUNCTION();
}

void CEntitySystem::AddToPhysicalWorld(CPhysicalEntity* pEntity)
{
	m_pPhysicalWorld->addRigidBody(pEntity->m_pRigidBody);
}

extern "C"
{
	ENTITYDLL_API struct IEntitySystem* CreateEntitySystem(ISystem* pISystem)
	{
		return new CEntitySystem(pISystem);
	}
}

#undef LOG_FUNCTION
