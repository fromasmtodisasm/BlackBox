start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	Physics
	SOURCE_GROUP
	"Source Files"
	"Physics.cpp"
	"Physics.hpp"
	"PhysicalWorld.cpp"
	"PhysicalWorld.hpp"
	)
end_sources()

EngineModule(Physics FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER

						 "Engine")
target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)
