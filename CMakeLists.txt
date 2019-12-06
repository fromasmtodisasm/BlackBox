cmake_minimum_required(VERSION 3.15)
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(
		FATAL_ERROR 
		"Use build directory different from source direcory!"	
	)
endif()
project(BlackBox)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BLACKBOX_ROOT ${CMAKE_CURRENT_SOURCE_DIR})


message(STATUS "CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_CONFIGURATION_TYPES = ${CMAKE_CONFIGURATION_TYPES}")
message(STATUS "OPTION_PROFILE = ${OPTION_PROFILE}")
message(STATUS "OPTION_PCH = ${OPTION_PCH}")
message(STATUS "CRYENGINE_DIR = ${CRYENGINE_DIR}")
message(STATUS "SDK_DIR = ${SDK_DIR}")
message(STATUS "PROJECT_DIR = ${PROJECT_DIR}")
message(STATUS "TOOLS_CMAKE_DIR = ${TOOLS_CMAKE_DIR}")

# Prefix all Visual Studio solution folder names with this string
set(VS_FOLDER_PREFIX "BLACKBOX/")
#add_subdirectory(lib)

set(
	SUBPROJECTS
	src
	game
	tests
	res
)

include(Tools/CMake/Build.cmake)

#[[
add_subdirectory(src)
add_subdirectory(game)
add_subdirectory(tests)
add_subdirectory(res)
]]
