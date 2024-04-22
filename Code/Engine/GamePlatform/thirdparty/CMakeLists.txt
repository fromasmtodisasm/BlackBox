message(STATUS "Thirdparty libs adding...")
if (USE_STEAM AND NOT TARGET Steam)
	add_subdirectory(Steam)
endif()
