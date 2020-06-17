cmake_minimum_required(VERSION 3.10)
project(Game CXX)

#include(FetchContent)
add_definitions("/ZI")

###################################################
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_ROOT ${PROJECT_SOURCE_DIR}/include)
###################################################

add_library(${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} PRIVATE BlackBox)
#target_link_libraries(${PROJECT_NAME} PRIVATE GUI)
if (WINDOWS)
	target_link_libraries(${PROJECT_NAME} PRIVATE Ws2_32)
endif()
target_include_directories(${PROJECT_NAME}
	PRIVATE "${INCLUDE_ROOT}"
	PRIVATE "${INCLUDE_ROOT}/GameFiles"
)

target_sources(
	${PROJECT_NAME}
	PRIVATE ${SOURCE_DIR}/Client/XClient.cpp
	PRIVATE ${SOURCE_DIR}/Client/Client.cpp
	PRIVATE ${SOURCE_DIR}/Commands.cpp
	PRIVATE ${SOURCE_DIR}/GameFiles/EntityClassRegistry.cpp
	PRIVATE ${SOURCE_DIR}/GameFiles/Game.cpp
	PRIVATE ${SOURCE_DIR}/GameFiles/GameActionInput.cpp
	PRIVATE ${SOURCE_DIR}/GameFiles/GameClientServer.cpp
	PRIVATE ${SOURCE_DIR}/GameFiles/GameDevMode.cpp
	PRIVATE ${SOURCE_DIR}/GameFiles/GameLoading.cpp
	PRIVATE ${SOURCE_DIR}/GameFiles/GameMods.cpp
	PRIVATE ${SOURCE_DIR}/GameFiles/XVehicleSystem.cpp
	#PRIVATE ${SOURCE_DIR}/GameFiles/GameGUI.cpp
	PRIVATE ${SOURCE_DIR}/GameFiles/GameObject.cpp
	PRIVATE ${SOURCE_DIR}/GameFiles/GameTagPoints.cpp
	PRIVATE ${SOURCE_DIR}/Network/XNetwork.cpp
	PRIVATE ${SOURCE_DIR}/Network/XEntityProcessingCmd.cpp
	PRIVATE ${SOURCE_DIR}/Player.cpp
	PRIVATE ${SOURCE_DIR}/Player/XPlayer.cpp
	PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectClient.cpp
	PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectGame.cpp
	PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectInput.cpp
	PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectServer.cpp
	PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectServerSlot.cpp
	PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectStream.cpp
	PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptObjectTest.cpp
	PRIVATE ${SOURCE_DIR}/ScriptObjects/ScriptTimerMgr.cpp
	PRIVATE ${SOURCE_DIR}/Server/XServer.cpp
	PRIVATE ${SOURCE_DIR}/Server/XServerRules.cpp
	PRIVATE ${SOURCE_DIR}/Server/XServerSlot.cpp
	PRIVATE ${SOURCE_DIR}/Main.cpp

	#[[
	PRIVATE ${SOURCE_DIR}/UI/IngameDialog.cpp
	PRIVATE ${SOURCE_DIR}/UI/IngameDialog.h
	PRIVATE ${SOURCE_DIR}/UI/ScriptObjectUI.cpp
	PRIVATE ${SOURCE_DIR}/UI/ScriptObjectUI.h
	PRIVATE ${SOURCE_DIR}/UI/UIButton.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIButton.h
	PRIVATE ${SOURCE_DIR}/UI/UICVars.cpp
	PRIVATE ${SOURCE_DIR}/UI/UICVars.h
	PRIVATE ${SOURCE_DIR}/UI/UICheckBox.cpp
	PRIVATE ${SOURCE_DIR}/UI/UICheckBox.h
	PRIVATE ${SOURCE_DIR}/UI/UIComboBox.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIComboBox.h
	PRIVATE ${SOURCE_DIR}/UI/UIDivX_Video.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIDivX_Video.h
	PRIVATE ${SOURCE_DIR}/UI/UIEditBox.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIEditBox.h
	PRIVATE ${SOURCE_DIR}/UI/UIHud.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIHud.h
	PRIVATE ${SOURCE_DIR}/UI/UIListView.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIListView.h
	PRIVATE ${SOURCE_DIR}/UI/UIMessages.h
	PRIVATE ${SOURCE_DIR}/UI/UIScreen.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIScreen.h
	PRIVATE ${SOURCE_DIR}/UI/UIScrollBar.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIScrollBar.h
	PRIVATE ${SOURCE_DIR}/UI/UIStatic.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIStatic.h
	PRIVATE ${SOURCE_DIR}/UI/UISystem.cpp
	PRIVATE ${SOURCE_DIR}/UI/UISystem.h
	PRIVATE ${SOURCE_DIR}/UI/UIVideoPanel.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIVideoPanel.h
	PRIVATE ${SOURCE_DIR}/UI/UIWidget.cpp
	PRIVATE ${SOURCE_DIR}/UI/UIWidget.h
	]]

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

target_precompile_headers(${PROJECT_NAME}
  PRIVATE 
  "src/pch.hpp"
)
