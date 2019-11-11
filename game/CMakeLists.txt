cmake_minimum_required(VERSION 3.0)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/include)

project(TestGame CXX)

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} BlackBox)
include_directories(
	"include/"
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_BINARY_DIR}/include
)

target_sources(
  ${PROJECT_NAME}
  #Game folder
  PRIVATE ${SOURCE_DIR}/Commands.cpp
  PRIVATE ${SOURCE_DIR}/Game.cpp
  PRIVATE ${SOURCE_DIR}/GameActionInput.cpp
  PRIVATE ${SOURCE_DIR}/GameClientServer.cpp
  PRIVATE ${SOURCE_DIR}/GameDevMode.cpp
  PRIVATE ${SOURCE_DIR}/GameGUI.cpp
  PRIVATE ${SOURCE_DIR}/GameObject.cpp
  PRIVATE ${SOURCE_DIR}/GameTagPoints.cpp
  PRIVATE ${SOURCE_DIR}/Player.cpp
  PRIVATE ${SOURCE_DIR}/PlayerCommands.cpp
  PRIVATE ${SOURCE_DIR}/Client/XClient.cpp
  PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectClient.cpp
  PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectServer.cpp
  PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectGame.cpp
  PRIVATE ${SOURCE_DIR}/Server/XServer.cpp
  PRIVATE ${SOURCE_DIR}/Server/XServerRules.cpp
  PRIVATE ${SOURCE_DIR}/Network/XNetwork.cpp

  )

  file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_ROOT}/*.hpp ${INCLUDE_ROOT}/*.h)

#--------------------------------------------------
get_target_property(SOURCE_FILES ${PROJECT_NAME}  SOURCES)
source_group(
  TREE ${SOURCE_DIR}
  PREFIX TestGame 
  FILES
  ${SOURCE_FILES}
)

target_sources(${PROJECT_NAME}
  PRIVATE
  ${INCLUDE_ROOT}
  )

source_group(
  TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
  PREFIX TestGame\\Inc 
  FILES
  ${INCLUDE_FILES}
)

  add_definitions(-DUSE_GLM)
  add_definitions(-DGLM_ENABLE_EXPERIMENTAL)
  add_definitions(-DGLAD_LOADER)
