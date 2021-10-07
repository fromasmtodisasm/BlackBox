#include "EntitySystem.hpp"

#include <BlackBox/Core/Platform/platform_impl.inl>


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
	return m_Entities.size();
}

IEntityIt* CEntitySystem::GetEntityIterator()
{
	return nullptr;
}

IEntityIt* CEntitySystem::GetEntityInFrustrumIterator(bool e)
{
	return nullptr;
}

void CEntitySystem::GetEntitiesInRadius(const Legacy::Vec3& origin, float radius, std::vector<IEntity*>& entities, int s) const
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
	delete this;
}

bool CEntitySystem::IsIDUsed(EntityId nID)
{
	return false;
}

void CEntitySystem::ResetEntities()
{
	m_Entities.clear();
}

void CEntitySystem::GetMemoryUsage(ICrySizer* pSizer) const
{
	pSizer->AddObject(this, sizeof(*this));
	pSizer->AddObject(m_Entities);

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
