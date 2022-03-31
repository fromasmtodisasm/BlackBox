#include "Physics.hpp"

#include <BlackBox/Renderer/IRenderAuxGeom.hpp>


Legacy::Vec3 btCast(const btVector3& from)
{
	return {from.x(), from.y(), from.z()};
}

void CPhysicsDebugger::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	Env::AuxGeomRenderer()->DrawLine(btCast(from), UCol(btCast(color)), btCast(to), UCol(btCast(color)));
}

void CPhysicsDebugger::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
}

void CPhysicsDebugger::reportErrorWarning(const char* warningString)
{
}

void CPhysicsDebugger::draw3dText(const btVector3& location, const char* textString)
{
}

void CPhysicsDebugger::setDebugMode(int debugMode)
{
	m_Mode = (DebugDrawModes)debugMode;
}

int CPhysicsDebugger::getDebugMode() const
{
	return m_Mode;
}
