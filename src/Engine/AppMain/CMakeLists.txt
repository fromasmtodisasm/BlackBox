cmake_minimum_required(VERSION 3.10)

start_sources()
sources_platform(ALL)
add_sources("NoUberFile"
	PROJECTS AppMain
	SOURCE_GROUP "Source Files"
		"Main.cpp"
 )
EngineModule(AppMain FORCE_STATIC PCH "pch.hpp" SOLUTION_FOLDER "Engine")

target_link_libraries(${THIS_PROJECT} PUBLIC  BlackBox)
target_link_libraries(${THIS_PROJECT} PUBLIC  System)
target_link_libraries(${THIS_PROJECT} PUBLIC GUI)

 end_sources()
