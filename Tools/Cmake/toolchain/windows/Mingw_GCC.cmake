set(CMAKE_CONFIGURATION_TYPES Debug Profile Release)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}"
        CACHE STRING "Reset the configurations to what we need"
        FORCE)

set(BUILD_CPU_ARCHITECTURE x64)
set(BUILD_PLATFORM Win64)
set(WINDOWS TRUE)
set(OUTPUT_DIRECTORY_NAME "mingw_x64_gcc")

# Define the environment for cross compiling from Linux to Win32
SET(CMAKE_SYSTEM_NAME    Windows) # Target system name
#[[
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_C_COMPILER     "x86_64-w64-mingw32-gcc")
SET(CMAKE_CXX_COMPILER   "x86_64-w64-mingw32-g++")
SET(CMAKE_RC_COMPILER    "x86_64-w64-mingw32-windres")
SET(CMAKE_RANLIB         "x86_64-w64-mingw32-ranlib")

# Configure the behaviour of the find commands
SET(CMAKE_FIND_ROOT_PATH "/usr/x86_64-w64-mingw32")
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
]]

add_definitions(-D_WINDOWS -DWIN32 -D_WIN32 -DWIN64)

message(STATUS "BUILD_CPU_ARCHITECTURE = ${BUILD_CPU_ARCHITECTURE}")

#include ("${CMAKE_CURRENT_LIST_DIR}/../../CRYENGINE-GCC.cmake")
