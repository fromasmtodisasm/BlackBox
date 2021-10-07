#pragma once
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>

#include "Entity.hpp"

struct ICrySizer;

class CEntitySystem : public IEntitySystem
{
public:
	CEntitySystem(ISystem* pSystem)
	{
		m_Entities.resize(100);
	}

	virtual void Update() override;
	virtual IScriptSystem* GetScriptSystem() override;
	virtual void Reset() override;
	virtual IEntity* SpawnEntity(CEntityDesc& ed, bool bAutoInit = true) override;
	virtual bool InitEntity(IEntity* pEntity, CEntityDesc& ed) override;
	virtual IEntity* GetEntity(EntityId id) override;
	virtual IEntity* GetEntity(const char* sEntityName) override;
	virtual EntityId FindEntity(const char* name) const override;
	virtual void RemoveEntity(EntityId entity, bool bRemoveNow = false) override;
	virtual int GetNumEntities() const override;
	virtual IEntityIt* GetEntityIterator() override;
	virtual IEntityIt* GetEntityInFrustrumIterator(bool bFromPrevFrame = false) override;
	virtual void GetEntitiesInRadius(const Legacy::Vec3& origin, float radius, std::vector<IEntity*>& entities, int physFlags = PHYS_ENTITY_ALL) const override;
	virtual void SetSink(IEntitySystemSink* sink) override;
	virtual void PauseTimers(bool bPause, bool bResume = false) override;
	virtual void RemoveSink(IEntitySystemSink* sink) override;
	virtual IEntityCamera* CreateEntityCamera() override;
	virtual void EnableClient(bool bEnable) override;
	virtual void EnableServer(bool bEnable) override;
	virtual void Release() override;
	virtual bool IsIDUsed(EntityId nID) override;
	virtual void ResetEntities() override;
	virtual void GetMemoryUsage(ICrySizer* pSizer) const override;
	virtual void SetDynamicEntityIdMode(const bool bActivate) override;
	virtual void SetDefaultEntityUpdateLevel(EEntityUpdateVisLevel eDefault) override;
	virtual void SetPrecacheResourcesMode(bool bPrecaching) override;
	virtual bool IsDynamicEntityId(EntityId id) override;
	virtual void MarkId(EntityId id) override;
	virtual void ClearId(EntityId id) override;

	std::vector<CEntity> m_Entities;

};
