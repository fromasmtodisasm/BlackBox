start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	Sound
	SOURCE_GROUP
	"Source Files"
	"Sound.cpp"
	"Sound.hpp"
	)
end_sources()

enginemodule(Sound FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER  "Engine")
target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)
