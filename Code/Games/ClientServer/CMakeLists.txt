#start_sources()
#sources_platform(ALL)
#add_sources(
#	"NoUberFile"
#	PROJECTS
#	Client
#	SOURCE_GROUP "Root"
#		Client/Client.cpp
#		Client/Client.h
#	PROJECTS
#	Server
#	SOURCE_GROUP "Root"
#		Server/Server.cpp
#		Server/Server.h
#	PROJECTS
#	CS_Common
#	SOURCE_GROUP "Root"
#		Common/imgui_impl_dx11.cpp
#		Common/imgui_impl_sdl.cpp
#		Common/ClientServer.h
#		Common/main.cpp
#)
#end_sources()
#
#EngineStaticModule(CS_Common SOLUTION_FOLDER "Projects/Tools")
#ConsoleApplication(Client SOLUTION_FOLDER "Projects/Tools")
#ConsoleApplication(Server SOLUTION_FOLDER "Projects/Tools")
#
#
#if(WINDOWS)
#	target_include_directories(CS_Common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Common)
#	find_package(imgui CONFIG REQUIRED)
#	target_link_libraries(CS_Common PUBLIC imgui::imgui)
#
#	find_package(SDL2 CONFIG REQUIRED)
#	target_link_libraries(CS_Common PUBLIC SDL2::SDL2 SDL2::SDL2main)
#
#	target_link_libraries(CS_Common PUBLIC d3d11 Ws2_32)
#	target_link_libraries(CS_Common PUBLIC dbghelp)
##############################################################
#	target_link_libraries(Client CS_Common)
#	target_link_libraries(Server CS_Common)
#endif()
