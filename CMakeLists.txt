cmake_minimum_required(VERSION 3.21)
if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(
		FATAL_ERROR 
		"Use build directory different from source direcory!"	
	)
endif()

if(NOT DEFINED BLACKBOX_DIR)
	set(BLACKBOX_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

# Part of the setup is split into the InitialSetup file so projects created by cryrun can include it.
# This needs to be re-visited after the toolchain files are cleaned up.
include(Tools/CMake/InitialSetup.cmake)
#add_compile_definitions(-DUSE_DEBUG_NEW)

# Setting the project version
if (NOT VERSION)
	if (METADATA_VERSION)
		set(VERSION ${METADATA_VERSION})
	else()
		set(VERSION "1.0.0.0")
	endif()
endif()
set(METADATA_VERSION ${VERSION} CACHE STRING "Version number for executable metadata" FORCE)

# Declare the project.
set(METADATA_PROJECT_NAME "BLACKBOX" CACHE STRING "Name of the solution project")
project("${METADATA_PROJECT_NAME}_${BUILD_PLATFORM}"
		VERSION "${METADATA_VERSION}"
		# Strange Behaviour! D in tail not worked!
		LANGUAGES C CXX)

###################################################
include(Tools/CMake/Configure.cmake)
###################################################
