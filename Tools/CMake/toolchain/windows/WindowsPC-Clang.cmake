set(CMAKE_CONFIGURATION_TYPES Debug Profile Release)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)

set(BUILD_CPU_ARCHITECTURE x64)
set(BUILD_PLATFORM Win64)
set(WINDOWS TRUE)
set(OUTPUT_DIRECTORY_NAME "windows_x64_clang")

###############################################################################
find_program(CLANG_CL_PROGRAM clang-cl)
find_program(LLVM_LIB_PROGRAM llvm-lib)
find_program(LLD_LINK_PROGRAM lld-link)

set(CMAKE_C_COMPILER ${CLANG_CL_PROGRAM} CACHE FILEPATH "C compiler" FORCE)
set(CMAKE_CXX_COMPILER ${CLANG_CL_PROGRAM} CACHE FILEPATH "C++ compiler" FORCE)
set(CMAKE_AR ${LLVM_LIB_PROGRAM} CACHE FILEPATH "Archiver" FORCE)
set(CMAKE_LINKER ${LLD_LINK_PROGRAM} CACHE FILEPATH "Linker" FORCE)
###############################################################################

message(STATUS "CMAKE_C_COMPILER = ${CMAKE_C_COMPILER}")
message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")

add_definitions(-D_WINDOWS -DWIN32 -D_WIN32 -D_WIN64)

include ("${CMAKE_CURRENT_LIST_DIR}/../../BLACKBOX-CLANG.cmake")