#include <BlackBox/EntitySystem/EntitySystem.hpp>


void CEntitySystem::Update()
{
}

IScriptSystem* CEntitySystem::GetScriptSystem()
{
	return nullptr;
}

void CEntitySystem::Reset()
{
}

IEntity* CEntitySystem::SpawnEntity(CEntityDesc& ed, bool t)
{
	return nullptr;
}

bool CEntitySystem::InitEntity(IEntity* pEntity, CEntityDesc& ed)
{
	return false;
}

IEntity* CEntitySystem::GetEntity(EntityId id)
{
	return nullptr;
}

IEntity* CEntitySystem::GetEntity(const char* sEntityName)
{
	return nullptr;
}

EntityId CEntitySystem::FindEntity(const char* name) const
{
	return EntityId();
}

void CEntitySystem::RemoveEntity(EntityId entity, bool w)
{
}

int CEntitySystem::GetNumEntities() const
{
	return 0;
}

IEntityIt* CEntitySystem::GetEntityIterator()
{
	return nullptr;
}

IEntityIt* CEntitySystem::GetEntityInFrustrumIterator(bool e)
{
	return nullptr;
}

void CEntitySystem::GetEntitiesInRadius(const Vec3& origin, float radius, std::vector<IEntity*>& entities, int s) const
{
}

void CEntitySystem::SetSink(IEntitySystemSink* sink)
{
}

void CEntitySystem::PauseTimers(bool bPause, bool e)
{
}

void CEntitySystem::RemoveSink(IEntitySystemSink* sink)
{
}

IEntityCamera* CEntitySystem::CreateEntityCamera()
{
	return nullptr;
}

void CEntitySystem::EnableClient(bool bEnable)
{
}

void CEntitySystem::EnableServer(bool bEnable)
{
}

void CEntitySystem::Release()
{
}

bool CEntitySystem::IsIDUsed(EntityId nID)
{
	return false;
}

void CEntitySystem::ResetEntities()
{
}

void CEntitySystem::GetMemoryStatistics(ICrySizer* pSizer)
{
}

void CEntitySystem::SetDynamicEntityIdMode(const bool bActivate)
{
}

void CEntitySystem::SetDefaultEntityUpdateLevel(EEntityUpdateVisLevel eDefault)
{
}

void CEntitySystem::SetPrecacheResourcesMode(bool bPrecaching)
{
}

bool CEntitySystem::IsDynamicEntityId(EntityId id)
{
	return false;
}

void CEntitySystem::MarkId(EntityId id)
{
}

void CEntitySystem::ClearId(EntityId id)
{
}


extern "C"
{
	ENTITYDLL_API struct IEntitySystem * CreateEntitySystem(ISystem *pISystem)
	{
		return new CEntitySystem(pISystem);	
	}
}
