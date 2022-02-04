cmake_minimum_required(VERSION 3.10)
###################################################
include(GenerateExportHeader)
###################################################
set(BLACKBOX_PROJECT BlackBox)
###################################################
add_library(${BLACKBOX_PROJECT} INTERFACE)
###################################################
#set_target_properties(${BLACKBOX_PROJECT} PROPERTIES FOLDER "Engine")
#--------------------------------------------------

macro(setup_subsystems)
	add_subsystem(Math)
	target_link_libraries(${BLACKBOX_PROJECT} INTERFACE Math)

	add_subsystem(Utils)
	add_subsystem(3DEngine)
	add_subsystem(Input)
	add_subsystem(Renderer)
	add_subsystem(ScriptSystem)
	add_subsystem(Physics)
	add_subsystem(Sound)
	#if (OPTION_USE_GUI)
	#	add_compile_definitions("ENABLE_DEBUG_GUI")
	#	add_subsystem(GUI)
	#endif()
	add_subsystem(System)
	add_subsystem(EntitySystem)
	add_subsystem(Window)
	#add_subsystem(Profiler)
	add_subsystem(Network)

	add_subsystem(AppMain)
	add_subsystem(Entry)
	add_subsystem(Common)

	#foreach(system IN LISTS ALL_PROJECT_SYSTEMS)
	#	add_dependencies(${BLACKBOX_PROJECT} ${system})
	#	message(STATUS "sub: ${system}")
	#endforeach()
endmacro()

macro(setup_dependencies)

endmacro()

macro(main)
	set_property(
		DIRECTORY APPEND PROPERTY INCLUDE_DIRECTORIES 
		$<INSTALL_INTERFACE:${BLACKBOX_INCLUDE_DIRS}>

		$<BUILD_INTERFACE:${BLACKBOX_INCLUDE_DIRS}>
		$<BUILD_INTERFACE:${BLACKBOX_INCLUDE_DIRS}>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
	)

	target_include_directories(${BLACKBOX_PROJECT}
		INTERFACE
		$<BUILD_INTERFACE:${BLACKBOX_INCLUDE_DIRS}/public>
		$<INSTALL_INTERFACE:include>
		$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
	)

	set(CMAKE_CXX_STANDARD 17)
	target_compile_features(BlackBox INTERFACE cxx_std_17)

	set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
	###################################################
	if (BB_TESET_ENABLE)
	  target_compile_definitions(${BLACKBOX_PROJECT} INTERFACE -DTEST)
	  target_compile_definitions(${BLACKBOX_PROJECT} INTERFACE -DSINGLE_PASS)
	endif()
	add_package(tinyxml2)
	add_package(freetype)
	###################################################


	#The remote console is useful in development, but it is a potential security vulnerability, therefore opt-in
	option(OPTION_DEVELOPER_CONSOLE_IN_RELEASE "Enables the developer console in Release builds" ON)
        option(OPTION_REMOTE_CONSOLE "Allows remote console connection" OFF)
	setup_subsystems()
	if (LINUX OR UNIX OR APPLE)
		target_link_libraries(BlackBox INTERFACE ${CMAKE_DL_LIBS})
	endif()
endmacro()

main()
