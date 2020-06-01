set(CMAKE_CONFIGURATION_TYPES Debug Profile Release)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)

set(BUILD_CPU_ARCHITECTURE x64)
set(BUILD_PLATFORM Linux64)
set(LINUX 1)
set(LINUX64 1)

set(OUTPUT_DIRECTORY_NAME "linux_x64_clang")

set(CMAKE_C_COMPILER cc)
set(CMAKE_CXX_COMPILER c++)

add_definitions(-DLINUX64)

message(STATUS "BUILD_CPU_ARCHITECTURE = ${BUILD_CPU_ARCHITECTURE}")

#include ("${CMAKE_CURRENT_LIST_DIR}/../../CRYENGINE-CLANG.cmake")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DLINUX -D__linux__ -DOPENGL -fPIC -Wno-reorder-ctor -Wno-unused-private-field -Wno-unused-variable -Wno-unknown-pragmas" CACHE STRING "C Common Flags" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DLINUX -D__linux__ -DOPENGL -fPIC -Wno-reorder-ctor -Wno-unused-private-field -Wno-unused-variable -Wno-unknown-pragmas" CACHE STRING "C++ Common Flags" FORCE)

