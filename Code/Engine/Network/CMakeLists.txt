start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	Network
	SOURCE_GROUP "Source Files"
		"Client.cpp"
		"Network.cpp"
		"Server.cpp"
		"NetServerSnooper.cpp"
		"NetServerSnooper.h"
		"ServerSnooper.cpp"
		"ServerSnooper.h"
)
end_sources()
EngineModule(Network PCH "pch.hpp" SOLUTION_FOLDER "Engine")
add_sdl(${THIS_PROJECT})
set(PROJECT_NAME ${THIS_PROJECT})
find_package(SDL2_net CONFIG REQUIRED)
target_link_libraries(${THIS_PROJECT} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,SDL2_net::SDL2_net,SDL2_net::SDL2_net-static>)
target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)
