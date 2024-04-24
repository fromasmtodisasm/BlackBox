#include "pch.hpp"
#include <BlackBox\Core\Platform\platform_impl.inl>

#include "PhysicalWorld.hpp"

IPhysicalWorld* CreatePhysicalWorld(struct ISystem* pLog)
{
	return new CPhysicalWorld(pLog);
}
