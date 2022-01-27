start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	EntitySystem
	SOURCE_GROUP
	"Source Files"
	"EntitySystem.cpp"
	"Entity.cpp"
	"Entity.hpp"
	)
end_sources()

EngineModule(EntitySystem PCH "pch.hpp" SOLUTION_FOLDER
						 "Engine")
target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)
