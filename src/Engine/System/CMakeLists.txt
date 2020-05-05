cmake_minimum_required(VERSION 3.10)
project(System CXX)
###################################################
add_library(System)

##################################################
## Define the two required variables before including
# the source code for watching a git repository.

if (NEED_VC)
	set(PRE_CONFIGURE_FILE "VersionControl.cpp.in")
	set(POST_CONFIGURE_FILE ${CMAKE_BINARY_DIR}/VersionControl.cpp)
	include(${CMAKE_SOURCE_DIR}/Tools/CMake/GitWatcher.cmake)
endif()
##################################################

target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE World)
target_link_libraries(${PROJECT_NAME} PRIVATE Utils)
target_link_libraries(${PROJECT_NAME} PRIVATE GUI)

target_sources(${PROJECT_NAME}
  PRIVATE ${ENGINE_DIR}/System/CmdLine.cpp
  PRIVATE ${ENGINE_DIR}/System/CmdLineArg.cpp
  PRIVATE ${ENGINE_DIR}/System/Commands.cpp
  PRIVATE ${ENGINE_DIR}/System/Console.cpp
  PRIVATE ${ENGINE_DIR}/System/File/CryPak.cpp
  PRIVATE ${ENGINE_DIR}/System/HardwareMouse.cpp
  PRIVATE ${ENGINE_DIR}/System/Linux/Runtime.cpp
  PRIVATE ${ENGINE_DIR}/System/Log.cpp
  PRIVATE ${ENGINE_DIR}/System/System.cpp
  PRIVATE ${ENGINE_DIR}/System/SystemEventDispatcher.cpp
  PRIVATE ${ENGINE_DIR}/System/Timer.cpp

  PRIVATE ${ENGINE_DIR}/System/ScriptBinds/ScriptObjectConsole.cpp
  PRIVATE ${ENGINE_DIR}/System/ScriptBinds/ScriptObjectRenderer.cpp
  PRIVATE ${ENGINE_DIR}/System/ScriptBinds/ScriptObjectScript.cpp
)

if (OPTION_USE_LINUXINPUT)
	target_compile_definitions(${PROJECT_NAME} PUBLIC -DUSE_LINUXINPUT)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${BLACKBOX_ROOT}/include/System)
if (NEED_VC)
	set_target_properties(check_git_repository PROPERTIES FOLDER Tools)
endif()

get_target_property(SOURCE_FILES System SOURCES )

source_group(
	TREE ${CMAKE_CURRENT_SOURCE_DIR}
	PREFIX "Source Files"
	FILES
	${SOURCE_FILES}
)

target_sources(${PROJECT_NAME}
  PRIVATE ${POST_CONFIGURE_FILE}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_private_headers()
