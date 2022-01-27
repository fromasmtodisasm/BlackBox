#set(CMAKE_SYSTEM_VERSION 10.0.16299.0)
#[[ profiling configuration causes an error: 
Missing variable is:
CMAKE_EXE_LINKER_FLAGS_PROFILE
]]

#set(CMAKE_CONFIGURATION_TYPES Debug Profile Release)
set(CMAKE_CONFIGURATION_TYPES Debug Release Profile)
set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Reset the configurations to what we need" FORCE)

set(BUILD_CPU_ARCHITECTURE x64)
set(BUILD_PLATFORM Win64)
set(WINDOWS TRUE)
set(OUTPUT_DIRECTORY_NAME "win_x64")

include ("${CMAKE_CURRENT_LIST_DIR}/../../BLACKBOX-MSVC.cmake")

add_definitions(-D_WINDOWS -DWIN32 -D_WIN32 -DWIN64)
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
if (EXISTS "${SDK_DIR}/Microsoft Windows SDK/10")
	set(WINDOWS_SDK "${SDK_DIR}/Microsoft Windows SDK/10")
else()
	get_filename_component(WINDOWS_SDK  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" ABSOLUTE CACHE)
endif()
