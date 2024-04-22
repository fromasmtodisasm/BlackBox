start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	SOURCE_GROUP "Source Files"
		"GamePlatformSystem.cpp"
		"GamePlatformSystem.hpp"
)
end_sources()

EngineModule(GamePlatform PCH "pch.hpp" SOLUTION_FOLDER  "Engine")

set(USE_STEAM ON)
add_subdirectory(thirdparty 3rdparty)

message(STATUS "Using Steam")
target_link_libraries(${THIS_PROJECT} PRIVATE Steam)
target_compile_definitions(${THIS_PROJECT} PRIVATE USE_STEAM)
