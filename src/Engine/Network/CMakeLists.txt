start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	PROJECTS Network
	SOURCE_GROUP "SourceFiles"
		"Client.cpp"
		"Network.cpp"
		"Server.cpp"
 )
end_sources()
EngineModule(Network FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER "Engine")
add_SDL(${THIS_PROJECT})
set(PROJECT_NAME ${THIS_PROJECT})
add_SDL_net()
target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)
