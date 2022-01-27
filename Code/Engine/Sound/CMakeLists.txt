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
	"SoundSystem.hpp"
	"SoundSystem.cpp"
	"CListenerArea.hpp"
	"CListenerArea.cpp"
	"SoundDLL.cpp"
	)
end_sources()

EngineModule(Sound PCH "pch.hpp" SOLUTION_FOLDER  "Engine")
target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)

add_sdl(${THIS_PROJECT})
add_package(sdl2-mixer)
target_link_libraries(${THIS_PROJECT} PRIVATE SDL2::SDL2_mixer)
