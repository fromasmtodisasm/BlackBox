set_property(GLOBAL PROPERTY DEBUG_CONFIGURATIONS Debug Profile)

# Fix slashes on paths
file(TO_CMAKE_PATH "${BLACKBOX_DIR}" BLACKBOX_DIR)
file(TO_CMAKE_PATH "${PROJECT_DIR}" PROJECT_DIR)
file(TO_CMAKE_PATH "${TOOLS_CMAKE_DIR}" TOOLS_CMAKE_DIR)

set(TOOLS_CMAKE_DIR "${BLACKBOX_DIR}/Tools/CMake")
set(CMAKE_MODULE_PATH "${TOOLS_CMAKE_DIR}/modules")

###################################################
message(STATUS "CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_CONFIGURATION_TYPES = ${CMAKE_CONFIGURATION_TYPES}")
message(STATUS "OPTION_PROFILE = ${OPTION_PROFILE}")
message(STATUS "OPTION_PCH = ${OPTION_PCH}")
message(STATUS "BLACKBOX_DIR = ${BLACKBOX_DIR}")
message(STATUS "SDK_DIR = ${SDK_DIR}")
message(STATUS "PROJECT_DIR = ${PROJECT_DIR}")
message(STATUS "TOOLS_CMAKE_DIR = ${TOOLS_CMAKE_DIR}")
###################################################

set(ENGINE_DIR ${CMAKE_SOURCE_DIR}/src/Engine)
set(SUBMODULES_DIR ${CMAKE_SOURCE_DIR}/submodules)
set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/include)
set(BLACKBOX_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)
set(BLACKBOX_PUBLIC_INCLUDE_DIRS ${BLACKBOX_INCLUDE_DIRS}/public/BlackBox)
set(BLACKBOX_PRIVATE_INCLUDE_DIRS ${BLACKBOX_INCLUDE_DIRS}/private/BlackBox)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

###################################################

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BLACKBOX_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

###################################################

if (UNIX OR CYGWIN)
	set(UNIX_OR_CYGWIN ON)
	message(STATUS "UNIX_OR_CYGWIN")
	find_package(PkgConfig REQUIRED)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DLINUX64)
  target_compile_definitions(${PROJECT_NAME} PUBLIC -DLINUX)
else()
  add_compile_definitions(BB_PLATFORM_WINDOWS)
endif()
# Prefix all Visual Studio solution folder names with this string
set(VS_FOLDER_PREFIX "BLACKBOX/")

###################################################

set(
	SUBPROJECTS
	src
	game
	#Tools/CMake
	res
)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
