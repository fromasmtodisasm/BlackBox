cmake_minimum_required(VERSION 3.10)
#project(BlackBox)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(
		FATAL_ERROR 
		"Use build directory different from source direcory!"	
	)
endif()

if(NOT DEFINED BLACKBOX_DIR)
	set(BLACKBOX_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
endif()

set(ALL_PROJECT_SOURCES)
# Part of the setup is split into the InitialSetup file so projects created by cryrun can include it.
# This needs to be re-visited after the toolchain files are cleaned up.
include(Tools/CMake/InitialSetup.cmake)

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
		LANGUAGES C CXX)

###################################################
include(Tools/CMake/Configure.cmake)
###################################################
include(CPack)
# Packaging support
set(CPACK_PACKAGE_VENDOR "fromasmtodisasm")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Some summary")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENCE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

set(CPACK_NSIS_EXECUTABLES_DIRECTORY ".")
set(CPACK_NSIS_CREATE_ICONS "CreateShortCut '\$SMPROGRAMS\\\\${PROJECT_NAME}.lnk' '\$INSTDIR\\\\bin\\\\Launcher.exe'")
