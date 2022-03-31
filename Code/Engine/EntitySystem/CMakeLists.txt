start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Source Files"
		"EntitySystem.cpp"
		"EntitySystem.hpp"
		"Entity.cpp"
		"Entity.hpp"
		"EntityCharacter.cpp"
		"EntityCharacter.hpp"
	SOURCE_GROUP "Physics"
		"Physics.cpp"
		"Physics.hpp"
		"PhysicalEntity.cpp"
		"PhysicalEntity.hpp"
	)
end_sources()

EngineModule(EntitySystem PCH "pch.hpp" SOLUTION_FOLDER "Engine")
target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)

find_package(Bullet CONFIG REQUIRED)
# Note: 2 target(s) were omitted.
target_link_libraries(${THIS_PROJECT} PRIVATE LinearMath Bullet3Common BulletDynamics BulletSoftBody BulletCollision)
