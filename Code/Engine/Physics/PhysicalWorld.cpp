#include "PhysicalWorld.hpp"
void CPhysicalWorld::Init()
{
}

IGeomManager* CPhysicalWorld::GetGeomManager()
{
	NOT_IMPLEMENTED_V;
}
IPhysUtils* CPhysicalWorld::GetPhysUtils()
{
	NOT_IMPLEMENTED_V;
}
void CPhysicalWorld::Shutdown(int bDeleteGeometries)
{
}
void CPhysicalWorld::Release()
{
}

void CPhysicalWorld::SetupEntityGrid(int axisz, vectorf org, int nx, int ny, float stepx, float stepy)
{
}
void CPhysicalWorld::SetHeightfieldData(const primitives::heightfield* phf)
{
}
int CPhysicalWorld::GetHeightfieldData(primitives::heightfield* phf)
{
	NOT_IMPLEMENTED_V;
}
PhysicsVars* CPhysicalWorld::GetPhysVars()
{
	return &m_PhysVars;
}
IPhysicalEntity* CPhysicalWorld::CreatePhysicalEntity(pe_type type, pe_params* params, void* pforeigndata, int iforeigndata, int id)
{
	NOT_IMPLEMENTED_V;
}
IPhysicalEntity* CPhysicalWorld::CreatePhysicalEntity(pe_type type, float lifeTime, pe_params* params, void* pForeignData, int iForeignData,
                                                      int id, IPhysicalEntity* pHostPlaceholder)
{
	NOT_IMPLEMENTED_V;
}
IPhysicalEntity* CPhysicalWorld::CreatePhysicalPlaceholder(pe_type type, pe_params* params, void* pForeignData, int iForeignData, int id)
{
	NOT_IMPLEMENTED_V;
}
int CPhysicalWorld::DestroyPhysicalEntity(IPhysicalEntity* pent, int mode)
{
	NOT_IMPLEMENTED_V;
}

int CPhysicalWorld::SetPhysicalEntityId(IPhysicalEntity* pent, int id, int bReplace)
{
	NOT_IMPLEMENTED_V;
}
int CPhysicalWorld::GetPhysicalEntityId(IPhysicalEntity* pent)
{
	NOT_IMPLEMENTED_V;
}
IPhysicalEntity* CPhysicalWorld::GetPhysicalEntityById(int id)
{
	NOT_IMPLEMENTED_V;
}

int CPhysicalWorld::SetSurfaceParameters(int surface_idx, float bounciness, float friction, unsigned int flags)
{
	NOT_IMPLEMENTED_V;
}
int CPhysicalWorld::GetSurfaceParameters(int surface_idx, float& bounciness, float& friction, unsigned int& flags)
{
	NOT_IMPLEMENTED_V;
}

void CPhysicalWorld::TimeStep(float time_interval, int flags)
{

}
float CPhysicalWorld::GetPhysicsTime()
{
	NOT_IMPLEMENTED_V;
}
int CPhysicalWorld::GetiPhysicsTime()
{
	NOT_IMPLEMENTED_V;
}
void CPhysicalWorld::SetPhysicsTime(float time)
{
}
void CPhysicalWorld::SetiPhysicsTime(int itime)
{
}
void CPhysicalWorld::SetSnapshotTime(float time_snapshot, int iType)
{
}
void CPhysicalWorld::SetiSnapshotTime(int itime_snapshot, int iType)
{
}

int CPhysicalWorld::GetEntitiesInBox(vectorf ptmin, vectorf ptmax, IPhysicalEntity**& pList, int objtypes)
{
	NOT_IMPLEMENTED_V;
}

int CPhysicalWorld::RayWorldIntersection(vectorf org, vectorf dir, int objtypes, unsigned int flags, ray_hit* hits, int nMaxHits,
                                         IPhysicalEntity* pSkipEnt, IPhysicalEntity* pSkipEntAux)
{
	NOT_IMPLEMENTED_V;
}

void CPhysicalWorld::ResetDynamicEntities()
{
}
void CPhysicalWorld::DestroyDynamicEntities()
{
}
void CPhysicalWorld::PurgeDeletedEntities()
{
}

void CPhysicalWorld::SimulateExplosion(vectorf epicenter, vectorf epicenterImp, float rmin, float rmax, float r, float impulsive_pressure_at_r,
                                       int nOccRes, int nGrow, float rmin_occ, IPhysicalEntity** pSkipEnts, int nSkipEnts,
                                       int iTypes)
{
}

float CPhysicalWorld::IsAffectedByExplosion(IPhysicalEntity* pent)
{
	NOT_IMPLEMENTED_V;
}

void CPhysicalWorld::DrawPhysicsHelperInformation(void (*DrawLineFunc)(float*, float*))
{
}

int CPhysicalWorld::CollideEntityWithBeam(IPhysicalEntity* _pent, vectorf org, vectorf dir, float r, ray_hit* phit)
{
	NOT_IMPLEMENTED_V;
}
int CPhysicalWorld::RayTraceEntity(IPhysicalEntity* pient, vectorf origin, vectorf dir, ray_hit* pHit, pe_params_pos* pp)
{
	NOT_IMPLEMENTED_V;
}

void CPhysicalWorld::GetMemoryStatistics(ICrySizer* pSizer)
{
}

void CPhysicalWorld::SetPhysicsStreamer(IPhysicsStreamer* pStreamer)
{
}
void CPhysicalWorld::SetPhysicsEventClient(IPhysicsEventClient* pEventClient)
{
}
float CPhysicalWorld::GetLastEntityUpdateTime(IPhysicalEntity* pent)
{
	NOT_IMPLEMENTED_V;
}
int CPhysicalWorld::GetEntityProfileInfo(phys_profile_info*& pList)
{
	NOT_IMPLEMENTED_V;
}

int CPhysicalWorld::SerializeWorld(const char* fname, int bSave)
{
	NOT_IMPLEMENTED_V;
}
int CPhysicalWorld::SerializeGeometries(const char* fname, int bSave)
{
	NOT_IMPLEMENTED_V;
}
