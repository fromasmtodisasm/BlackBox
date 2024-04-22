start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	SOURCE_GROUP "Source Files"
		"CListenerArea.cpp"
		"CListenerArea.hpp"
		"Sound.cpp"
		"Sound.hpp"
		"SoundBase.hpp"
		"SoundDLL.cpp"
		"SoundSystem.cpp"
		"SoundSystem.hpp"
)
end_sources()

EngineModule(Sound PCH "pch.hpp" SOLUTION_FOLDER  "Engine")
target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)

add_sdl(${THIS_PROJECT})
find_package(SDL2_mixer_ext CONFIG REQUIRED)
target_link_libraries(${THIS_PROJECT} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer_ext::SDL2_mixer_ext>,SDL2_mixer_ext::SDL2_mixer_ext,SDL2_mixer_ext::SDL2_mixer_ext_Static>)
