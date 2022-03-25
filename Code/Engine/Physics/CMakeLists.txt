start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	SOURCE_GROUP "Source Files"
		"Physics.cpp"
		"Physics.hpp"
		"PhysicalWorld.cpp"
		"PhysicalWorld.hpp"
)
end_sources()

EngineModule(Physics PCH "pch.hpp" SOLUTION_FOLDER "Engine")

target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)

find_package(Bullet CONFIG REQUIRED)
# Note: 2 target(s) were omitted.
target_link_libraries(${THIS_PROJECT} PRIVATE LinearMath Bullet3Common BulletDynamics BulletSoftBody)
