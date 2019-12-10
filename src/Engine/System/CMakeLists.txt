cmake_minimum_required(VERSION 3.15)
project(System CXX)
###################################################
add_library(System)

##################################################
## Define the two required variables before including
# the source code for watching a git repository.
set(PRE_CONFIGURE_FILE "VersionControl.cpp.in")
set(POST_CONFIGURE_FILE ${CMAKE_BINARY_DIR}/VersionControl.cpp)
include(${CMAKE_SOURCE_DIR}/Tools/CMake/GitWatcher.cmake)
##################################################

target_link_libraries(${PROJECT_NAME} PRIVATE BB_Private)
target_link_libraries(${PROJECT_NAME} PRIVATE Common)
target_link_libraries(${PROJECT_NAME} PRIVATE Math)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGlBackend)

target_link_libraries(${PROJECT_NAME} PRIVATE World)

target_sources(${PROJECT_NAME}
  PRIVATE ${ENGINE_DIR}/System/CmdLine.cpp
  PRIVATE ${ENGINE_DIR}/System/CmdLineArg.cpp
  PRIVATE ${ENGINE_DIR}/System/Commands.cpp
  PRIVATE ${ENGINE_DIR}/System/Console.cpp
  PRIVATE ${ENGINE_DIR}/System/Linux/Runtime.cpp
  PRIVATE ${ENGINE_DIR}/System/Log.cpp
  PRIVATE ${ENGINE_DIR}/System/System.cpp
  PRIVATE ${ENGINE_DIR}/System/SystemEventDispatcher.cpp
  PRIVATE ${ENGINE_DIR}/System/Timer.cpp
  PRIVATE ${POST_CONFIGURE_FILE}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${BLACKBOX_ROOT}/include/System)
set_target_properties(check_git_repository PROPERTIES FOLDER Tools)

add_private_headers()
