#pragma once
#include "IPhysics.h"

class CEntitySystem;
class CEntity;

class CEntityMotionState : public btMotionState
{
public:
	CEntityMotionState(CEntity& entity)
	    : m_Entity(entity)
	{
	}

	// Inherited via btMotionState
	virtual void getWorldTransform(btTransform& worldTrans) const override;
	virtual void setWorldTransform(const btTransform& worldTrans) override;

	CEntity&     m_Entity;
};


class CPhysicalEntity : public IPhysicalEntity
{
public:
	CPhysicalEntity(IEntity* pEntity);
	// Inherited via IPhysicalEntity
	virtual pe_type         GetType() override;
	virtual int             SetParams(pe_params* params) override;
	virtual int             GetParams(pe_params* params) override;
	virtual int             GetStatus(pe_status* status) override;
	virtual int             Action(pe_action*) override;
	virtual int             AddGeometry(phys_geometry* pgeom, pe_geomparams* params, int id = -1) override;
	virtual void            RemoveGeometry(int id) override;
	virtual void*           GetForeignData(int itype = 0) override;
	virtual int             GetiForeignData() override;
	virtual int             GetStateSnapshot(CStream& stm, float time_back = 0, int flags = 0) override;
	virtual int             SetStateFromSnapshot(CStream& stm, int flags = 0) override;
	virtual int             PostSetStateFromSnapshot() override;
	virtual int             GetStateSnapshotTxt(char* txtbuf, int szbuf, float time_back = 0) override;
	virtual void            SetStateFromSnapshotTxt(const char* txtbuf, int szbuf) override;
	virtual unsigned int    GetStateChecksum() override;
	virtual void            StartStep(float time_interval) override;
	virtual int             Step(float time_interval) override;
	virtual void            StepBack(float time_interval) override;
	virtual IPhysicalWorld* GetWorld() override;
	virtual void            GetMemoryStatistics(ICrySizer* pSizer) override;

	btRigidBody*                  m_pRigidBody;
	CEntityMotionState            m_MotionState;
	CEntitySystem*                m_pEntitySystem;

};
