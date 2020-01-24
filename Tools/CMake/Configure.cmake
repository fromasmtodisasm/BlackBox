if(ANDROID OR LINUX)
	# WIN32 Should be unset after project() line to work correctly
	unset(WIN32)
	unset(WINDOWS)
endif()

message(STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMAKE_SYSTEM_VERSION = ${CMAKE_SYSTEM_VERSION}")
message(STATUS "BUILD_PLATFORM = ${BUILD_PLATFORM}")
message(STATUS "BUILD_CPU_ARCHITECTURE = ${BUILD_CPU_ARCHITECTURE}")
message(STATUS "OUTPUT_DIRECTORY_NAME = ${OUTPUT_DIRECTORY_NAME}")

if(NOT ${CMAKE_GENERATOR} MATCHES "Visual Studio")
	set(valid_configs Debug Profile Release)
	list(FIND valid_configs "${CMAKE_BUILD_TYPE}" config_index)
	if(${config_index} EQUAL -1)
		message(SEND_ERROR "Build type \"${CMAKE_BUILD_TYPE}\" is not supported, set CMAKE_BUILD_TYPE to one of ${valid_configs}")
	endif()
endif()

# Prefix all Visual Studio solution folder names with this string
set(VS_FOLDER_PREFIX "BLACKBOX/")

set(MODULES CACHE INTERNAL "List of engine modules being built" FORCE)
set(GAME_MODULES CACHE INTERNAL "List of game modules being built" FORCE)

set(game_folder CACHE INTERNAL "Game folder used for resource files on Windows" FORCE)

# Define Options
get_property(global_defines DIRECTORY "${BLACKBOX_DIR}" PROPERTY COMPILE_DEFINITIONS)
get_property(global_includes DIRECTORY "${BLACKBOX_DIR}" PROPERTY INCLUDE_DIRECTORIES)
get_property(global_links DIRECTORY "${BLACKBOX_DIR}" PROPERTY LINK_DIRECTORIES)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/ProjectCVarOverrides.h")
	file(WRITE "${CMAKE_BINARY_DIR}/ProjectCVarOverrides.h" "")
endif ()
list(APPEND global_defines "BB_CVAR_OVERRIDE_FILE=\"${CMAKE_BINARY_DIR}/ProjectCVarOverrides.h\"")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/ProjectCVarWhitelist.h")
	file(WRITE "${CMAKE_BINARY_DIR}/ProjectCVarWhitelist.h" "")
endif ()
list(APPEND global_defines "BB_CVAR_WHITELIST_FILE=\"${CMAKE_BINARY_DIR}/ProjectCVarWhitelist.h\"")

if(NOT EXISTS "${CMAKE_BINARY_DIR}/ProjectEngineDefineOverrides.h")
	file(WRITE "${CMAKE_BINARY_DIR}/ProjectEngineDefineOverrides.h" "")
endif ()
list(APPEND global_defines "BB_ENGINE_DEFINE_OVERRIDE_FILE=\"${CMAKE_BINARY_DIR}/ProjectEngineDefineOverrides.h\"")

if (OPTION_RUNTIME_CVAR_OVERRIDES)
	list(APPEND global_defines "USE_RUNTIME_CVAR_OVERRIDES")
endif()
# custom defines
list(APPEND global_defines "BLACKBOX_DEFINE")

include("${TOOLS_CMAKE_DIR}/CommonOptions.cmake")
include("${TOOLS_CMAKE_DIR}/CommonMacros.cmake")

# Must be included after SDK_DIR definition
include("${TOOLS_CMAKE_DIR}/CopyFilesToBin.cmake")

if(OPTION_STATIC_LINKING)
	# Enable static libraries
	MESSAGE(STATUS "Use Static Linking (.lib/.a)")
	set(BUILD_SHARED_LIBS FALSE)
else()
	# Enable dynamic libraries
	MESSAGE(STATUS "Use Dynamic Linking (.dll/.so)")
	set(BUILD_SHARED_LIBS TRUE)
endif()

if (OPTION_USE_GUI)
	add_definitions(GUI)
endif()

include("${TOOLS_CMAKE_DIR}/Build.cmake")

