
foreach(PROJ ${SUBPROJECTS})
	set(
		BLACKBOX_BUILD_${PROJ} TRUE
		CACHE BOOL "Build the ${PROJ} subproject")
	if(BLACKBOX_BUILD_${PROJ} AND
		EXISTS
		"${CMAKE_SOURCE_DIR}/${PROJ}/CMakeLists.txt")
		message(
			STATUS
			"The project ${PROJ} will be included")
		add_subdirectory("${BLACKBOX_ROOT}/${PROJ}")
	else()
		message(
			STATUS
			"The project ${PROJ} will NOT be included")
	endif()
endforeach()

set_property(DIRECTORY tests PROPERTY VS_STARTUP_PROJECT Launcher)
set_property(TARGET Launcher PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
