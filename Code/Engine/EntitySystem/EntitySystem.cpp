#include "EntitySystem.hpp"
#include <BlackBox\ScriptSystem\IScriptSystem.hpp>

#include <BlackBox/Core/Platform/platform_impl.inl>

#define LOG_FUNCTION() CryLog("[EntitySystemLogging]: %s", __FUNCTION__)

inline CEntitySystem::CEntitySystem(ISystem* pSystem) : m_EntityIt(m_Entities)
{
	m_Entities.resize(100);

#define SET_SCRIPTEVENT(event) gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_"#event, ScriptEvent_##event)
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



void CEntitySystem::Update()
{
	//LOG_FUNCTION();
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
	LOG_FUNCTION();
	return nullptr;
}

bool CEntitySystem::InitEntity(IEntity* pEntity, CEntityDesc& ed)
{
	LOG_FUNCTION();
	return false;
}

IEntity* CEntitySystem::GetEntity(EntityId id)
{
	LOG_FUNCTION();
	return nullptr;
}

IEntity* CEntitySystem::GetEntity(const char* sEntityName)
{
	LOG_FUNCTION();
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
	LOG_FUNCTION();
}

void CEntitySystem::SetSink(IEntitySystemSink* sink)
{
	LOG_FUNCTION();
}

void CEntitySystem::PauseTimers(bool bPause, bool e)
{
	LOG_FUNCTION();
}

void CEntitySystem::RemoveSink(IEntitySystemSink* sink)
{
	LOG_FUNCTION();
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


extern "C"
{
	ENTITYDLL_API struct IEntitySystem * CreateEntitySystem(ISystem *pISystem)
	{
		return new CEntitySystem(pISystem);	
	}
}

#undef LOG_FUNCTION
