cmake_minimum_required(VERSION 3.15)
project(BlackBox)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(
		FATAL_ERROR 
		"Use build directory different from source direcory!"	
	)
endif()
# Part of the setup is split into the InitialSetup file so projects created by cryrun can include it.
# This needs to be re-visited after the toolchain files are cleaned up.
include(Tools/CMake/InitialSetup.cmake)

###################################################
include(Tools/CMake/CommonMacros.cmake)
###################################################
include(Tools/CMake/Build.cmake)
###################################################

set_property(DIRECTORY tests PROPERTY VS_STARTUP_PROJECT Launcher)
set_property(TARGET Launcher PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
