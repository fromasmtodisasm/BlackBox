# BLACKBOX_DIR is expected to be set by the starting CMakeLists.txt
if (NOT DEFINED BLACKBOX_DIR)
    message(FATAL_ERROR "BLACKBOX_DIR is not set. Please set it before including InitialSetup.cmake")
endif()

#set_property(GLOBAL PROPERTY DEBUG_CONFIGURATIONS Debug Profile)

# Turn on the ability to create folders to organize projects (.vcxproj)
# It creates "CMakePredefinedTargets" folder by default and adds CMake
# defined projects like INSTALL.vcxproj and ZERO_CHECK.vcxproj
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Set the PROJECT_DIR to the source path used to start CMake
if(NOT DEFINED PROJECT_DIR)
  set(PROJECT_DIR "${CMAKE_SOURCE_DIR}")
endif()

# Fix slashes on paths
file(TO_CMAKE_PATH "${BLACKBOX_DIR}" BLACKBOX_DIR)
file(TO_CMAKE_PATH "${PROJECT_DIR}" PROJECT_DIR)
file(TO_CMAKE_PATH "${TOOLS_CMAKE_DIR}" TOOLS_CMAKE_DIR)

set(TOOLS_CMAKE_DIR "${BLACKBOX_DIR}/Tools/CMake")
set(TOOLS_DIR "${BLACKBOX_DIR}/Tools")
set(CMAKE_MODULE_PATH "${TOOLS_CMAKE_DIR}/modules")

###################################################
message(STATUS "CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_GENERATOR_PLATFORM = ${CMAKE_GENERATOR_PLATFORM}")
message(STATUS "CMAKE_CONFIGURATION_TYPES = ${CMAKE_CONFIGURATION_TYPES}")
message(STATUS "OPTION_PROFILE = ${OPTION_PROFILE}")
message(STATUS "OPTION_PCH = ${OPTION_PCH}")
message(STATUS "BLACKBOX_DIR = ${BLACKBOX_DIR}")
message(STATUS "SDK_DIR = ${SDK_DIR}")
message(STATUS "PROJECT_DIR = ${PROJECT_DIR}")
message(STATUS "TOOLS_CMAKE_DIR = ${TOOLS_CMAKE_DIR}")
###################################################

#if (DEFINED VCPKG_INSTALLER AND (NOT DEFINED VCPKG OR NOT CMAKE_TOOLCHAIN_FILE))
  include(${BLACKBOX_DIR}/Tools/CMake/PrepareVcpkg.cmake)
  PrepareVcpkg()
#endif()

# Including the Toolchain file, as it sets important variables.
if(DEFINED TOOLCHAIN_FILE)
	message(STATUS "USED TOOLCHAIN FILE ${TOOLS_CMAKE_DIR}/${TOOLCHAIN_FILE}")
	include(${TOOLS_CMAKE_DIR}/${TOOLCHAIN_FILE})
elseif(WIN32)
	include("${TOOLS_CMAKE_DIR}/toolchain/windows/WindowsPC-MSVC.cmake")
endif()

if (NOT DEFINED BUILD_PLATFORM)
	# For now, we expect BUILD_PLATFORM to have been set via a Toolchain file.
	message(FATAL_ERROR "BUILD_PLATFORM not defined. Please always supply one of the CRYENGINE toolchain files.")
endif()

set(ENGINE_DIR ${BLACKBOX_DIR}/Code/Engine)
set(SUBMODULES_DIR ${BLACKBOX_DIR}/submodules)
set(INCLUDE_DIRECTORIES ${BLACKBOX_DIR}/include)
set(BLACKBOX_INCLUDE_DIRS ${BLACKBOX_DIR}/include)
set(BLACKBOX_PUBLIC_INCLUDE_DIRS ${BLACKBOX_INCLUDE_DIRS}/public/BlackBox)
set(BLACKBOX_PRIVATE_INCLUDE_DIRS ${BLACKBOX_INCLUDE_DIRS}/private/BlackBox)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BLACKBOX_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

if (WIN32)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()
# Prefix all Visual Studio solution folder names with this string
set(VS_FOLDER_PREFIX "BLACKBOX/")

