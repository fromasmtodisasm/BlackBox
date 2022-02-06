start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	Utils
	SOURCE_GROUP
	"Source Files"
	"Utils.cpp")
end_sources()

EngineModule(Utils PCH "pch.hpp" SOLUTION_FOLDER "Engine")
