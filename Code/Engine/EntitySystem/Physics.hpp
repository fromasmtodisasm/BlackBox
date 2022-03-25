#pragma once

struct SPhysicsInitParams
{
	btDispatcher*             dispatcher;
	btBroadphaseInterface*    broadPhase;
	btConstraintSolver*       constraintSolver;
	btCollisionConfiguration* collisionConfiguration;

	void                      Create()
	{
		collisionConfiguration = new btDefaultCollisionConfiguration();
		dispatcher             = new btCollisionDispatcher(collisionConfiguration);
		broadPhase             = new btDbvtBroadphase();
		constraintSolver       = new btSequentialImpulseConstraintSolver;
	}
};