option(OPTION_RC "Include RC in the build" ${WINDOWS})
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
	if (OPTION_ENGINE)
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
		if (OPTION_PROFILER)
			add_subsystem(Profiler)
		endif()
		add_subsystem(Network)

		add_subsystem(AppMain)
		add_subsystem(Common)
		add_subsystem(GamePlatform)


		if(EXISTS "${BLACKBOX_DIR}/Engine/Scripts")
			add_subdirectory(Engine/Scripts)
		endif()
	endif()
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

	if (OPTION_ENGINE OR OPTION_SHADERCACHEGEN)
		# Shaders custom project
		if(EXISTS "${BLACKBOX_DIR}/Engine/Shaders")
			add_subdirectory(Engine/Shaders)
		endif()
	endif()

	setup_subsystems()
	if (LINUX OR UNIX OR APPLE)
		target_link_libraries(BlackBox INTERFACE ${CMAKE_DL_LIBS})
	endif()
endmacro()

main()
