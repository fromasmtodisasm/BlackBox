foreach(PROJ ${SUBPROJECTS})
	set(
		MY_BUILD_${PROJ} TRUE
		CACHE BOOL "Build the ${PROJ} subproject")
	if(MY_BUILD_${PROJ} AND
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