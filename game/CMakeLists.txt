cmake_minimum_required(VERSION 3.10)
project(FPS_Game CXX)

include(FetchContent)

###################################################
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_ROOT ${PROJECT_SOURCE_DIR}/include)
###################################################

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE BlackBox::Common)
target_include_directories(${PROJECT_NAME}
	PRIVATE "${INCLUDE_ROOT}"
	PRIVATE "${INCLUDE_ROOT}/GameFiles"
)

target_sources(
  ${PROJECT_NAME}
  PRIVATE ${SOURCE_DIR}/Client/XClient.cpp
  PRIVATE ${SOURCE_DIR}/Commands.cpp
  PRIVATE ${SOURCE_DIR}/GameFiles/Game.cpp
  PRIVATE ${SOURCE_DIR}/GameFiles/GameActionInput.cpp
  PRIVATE ${SOURCE_DIR}/GameFiles/GameClientServer.cpp
  PRIVATE ${SOURCE_DIR}/GameFiles/GameDevMode.cpp
  PRIVATE ${SOURCE_DIR}/GameFiles/GameMods.cpp
  PRIVATE ${SOURCE_DIR}/GameFiles/GameGUI.cpp
  PRIVATE ${SOURCE_DIR}/GameFiles/GameObject.cpp
  PRIVATE ${SOURCE_DIR}/GameFiles/GameTagPoints.cpp
  PRIVATE ${SOURCE_DIR}/Network/XNetwork.cpp
  PRIVATE ${SOURCE_DIR}/Player.cpp
  PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectClient.cpp
  PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectGame.cpp
  PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectInput.cpp
  PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectServer.cpp
  PRIVATE ${SOURCE_DIR}/Server/XServer.cpp
  PRIVATE ${SOURCE_DIR}/Server/XServerRules.cpp

  )

  file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_ROOT}/*.hpp ${INCLUDE_ROOT}/*.h)

#--------------------------------------------------
get_target_property(SOURCE_FILES ${PROJECT_NAME}  SOURCES)
source_group(
  TREE ${SOURCE_DIR}
  PREFIX ${PROJECT_NAME} 
  FILES
  ${SOURCE_FILES}
)

target_sources(${PROJECT_NAME}
  PRIVATE
  ${INCLUDE_FILES}
  )

source_group(
  TREE ${INCLUDE_ROOT}
  PREFIX "Header Files" 
  FILES
  ${INCLUDE_FILES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "Projects/Game")
