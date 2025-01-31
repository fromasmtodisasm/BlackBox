#include "PhysicalEntity.hpp"
#include "BlackBox/EntitySystem/IEntitySystem.hpp"
#include "Entity.hpp"
#include "EntitySystem.hpp"

void CEntityMotionState::getWorldTransform(btTransform& worldTrans) const
{
	btTransform transform;
	transform.setIdentity();
	auto        a = m_Entity.m_Angles;
	auto p = m_Entity.m_Pos;
	transform.setOrigin(btVector3(p.x, p.y, p.z));
	transform.setRotation(btQuaternion(a.x, a.y, a.z));

	worldTrans = transform;
}

void CEntityMotionState::setWorldTransform(const btTransform& worldTrans)
{
	btTransform transform = worldTrans;
	auto&       a         = m_Entity.m_Angles;
	transform.getRotation().getEulerZYX(a.x, a.y, a.z);
	auto& p = m_Entity.m_Pos;
	auto  o = transform.getOrigin();
	p.x     = o.x();
	p.y     = o.y();
	p.z     = o.z();
}

CPhysicalEntity::CPhysicalEntity(IEntity* pEntity)
    : m_MotionState(*(CEntity*)pEntity)
    , m_pEntitySystem((CEntitySystem*)Env::EntitySystem())

{
	Legacy::Vec3 min, max;
	pEntity->GetBBox(min, max);
	float                                    mass = 4.f;
	auto                                     half = (max - min);
	btVector3                                ext(half.x, half.y, half.z);
	btBoxShape*                              collsion = new btBoxShape(ext);
	btVector3                                localInertia(0, 0, 0);
		collsion->calculateLocalInertia(mass, localInertia);
	btRigidBody::btRigidBodyConstructionInfo ci(mass, &m_MotionState, collsion, localInertia);
	m_pRigidBody = new btRigidBody(ci);
	//m_pRigidBody->setCollisionFlags(btRigidBody::CollisionFlags::CF_DYNAMIC_OBJECT);
	//m_pRigidBody->set
	//m_pRigidBody->applyImpulse(btVector3(100, 10, 10), btVector3(0, 0, 0));
}

pe_type CPhysicalEntity::GetType()
{
	return pe_type();
}

int CPhysicalEntity::SetParams(pe_params* params)
{
	return 0;
}

int CPhysicalEntity::GetParams(pe_params* params)
{
	return 0;
}

int CPhysicalEntity::GetStatus(pe_status* status)
{
	return 0;
}

int CPhysicalEntity::Action(pe_action* action)
{
	if (action->type == 2) // impulse
	{
		pe_action_impulse* impulse = (pe_action_impulse*)action;

		auto               v       = impulse->impulse;
		auto               p       = impulse->point;
		m_pRigidBody->activate(true);
		m_pRigidBody->applyCentralForce(btVector3(v.x, v.y, v.z));
		m_pRigidBody->applyGravity();
		return 1;
	}
	else if (action->type == 10) // velocity
	{
		auto* velocity = (pe_action_set_velocity*)action;

		auto               v       = velocity->v;
		m_pRigidBody->activate(true);
		m_pRigidBody->setLinearVelocity(btVector3(v.x, v.y, v.z));
		m_pRigidBody->applyGravity();
		return 1;
	}
	return 0;
}

int CPhysicalEntity::AddGeometry(phys_geometry* pgeom, pe_geomparams* params, int id/* = -1*/)
{
	return 0;
}

void CPhysicalEntity::RemoveGeometry(int id)
{
}

void* CPhysicalEntity::GetForeignData(int itype/* = 0*/)
{
	return nullptr;
}

int CPhysicalEntity::GetiForeignData()
{
	return 0;
}

int CPhysicalEntity::GetStateSnapshot(CStream& stm, float time_back/* = 0*/, int flags/* = 0*/)
{
	return 0;
}

int CPhysicalEntity::SetStateFromSnapshot(CStream& stm, int flags/* = 0*/)
{
	return 0;
}

int CPhysicalEntity::PostSetStateFromSnapshot()
{
	return 0;
}

int CPhysicalEntity::GetStateSnapshotTxt(char* txtbuf, int szbuf, float time_back/* = 0*/)
{
	return 0;
}

void CPhysicalEntity::SetStateFromSnapshotTxt(const char* txtbuf, int szbuf)
{
}

unsigned int CPhysicalEntity::GetStateChecksum()
{
	return 0;
}

void CPhysicalEntity::StartStep(float time_interval)
{
}

int CPhysicalEntity::Step(float time_interval)
{
	return 0;
}

void CPhysicalEntity::StepBack(float time_interval)
{
}

IPhysicalWorld* CPhysicalEntity::GetWorld()
{
	return nullptr;
}

void CPhysicalEntity::GetMemoryStatistics(ICrySizer* pSizer)
{
}
