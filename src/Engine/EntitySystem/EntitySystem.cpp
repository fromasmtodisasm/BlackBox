#include "EntitySystem.hpp"
#include <BlackBox\ScriptSystem\IScriptSystem.hpp>

#include <BlackBox/Core/Platform/platform_impl.inl>


inline CEntitySystem::CEntitySystem(ISystem* pSystem) : m_EntityIt(m_Entities)
{
	m_Entities.resize(100);

	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Activate", ScriptEvent_Activate);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Deactivate", ScriptEvent_Deactivate);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_FireModeChange", ScriptEvent_FireModeChange);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_DropItem", ScriptEvent_DropItem);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Reset", ScriptEvent_Reset);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Contact", ScriptEvent_Contact);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Enter", ScriptEvent_Enter);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Leave", ScriptEvent_Leave);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Timer", ScriptEvent_Timer);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_StartAnimation", ScriptEvent_StartAnimation);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_AnimationKey", ScriptEvent_AnimationKey);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_EndAnimation", ScriptEvent_EndAnimation);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Respawn", ScriptEvent_Respawn);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_ItemActivated", ScriptEvent_ItemActivated);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Hit", ScriptEvent_Hit);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Fire", ScriptEvent_Fire);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_WeaponReady", ScriptEvent_WeaponReady);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_StopFiring", ScriptEvent_StopFiring);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Reload", ScriptEvent_Reload);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Command", ScriptEvent_Command);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_FireGrenade", ScriptEvent_FireGrenade);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Die", ScriptEvent_Die);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_ZoomToggle", ScriptEvent_ZoomToggle);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_ZoomIn", ScriptEvent_ZoomIn);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_ZoomOut", ScriptEvent_ZoomOut);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Land", ScriptEvent_Land);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_FireCancel", ScriptEvent_FireCancel);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_GameDefinedEvent", ScriptEvent_GameDefinedEvent);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_ViewModeChange", ScriptEvent_ViewModeChange);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_SelectWeapon", ScriptEvent_SelectWeapon);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Deafened", ScriptEvent_Deafened);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_StanceChange", ScriptEvent_StanceChange);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_CycleGrenade", ScriptEvent_CycleGrenade);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Use", ScriptEvent_Use);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_MeleeAttack", ScriptEvent_MeleeAttack);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_PhysicalizeOnDemand", ScriptEvent_PhysicalizeOnDemand);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_PhysCollision", ScriptEvent_PhysCollision);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_FlashLightSwitch", ScriptEvent_FlashLightSwitch);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_EnterWater", ScriptEvent_EnterWater);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_CycleVehiclePos", ScriptEvent_CycleVehiclePos);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_AllClear", ScriptEvent_AllClear); // sent when the main player has no opposition around him
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Expression", ScriptEvent_Expression);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_InVehicleAnimation", ScriptEvent_InVehicleAnimation);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_InVehicleAmmo", ScriptEvent_InVehicleAmmo);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_ProcessCharacterEffects", ScriptEvent_ProcessCharacterEffects);
	gEnv->pScriptSystem->SetGlobalValue("ScriptEvent_Jump", ScriptEvent_Jump); //! jump event
}

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
	return &m_EntityIt;
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
