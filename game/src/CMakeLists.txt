# ##############################################################################
set(INCLUDE_ROOT ${CMAKE_CURRENT_LIST_DIR}/../include)
# ##############################################################################

start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	Game
	SOURCE_GROUP
	"Source Files"
	Main.cpp
	Commands.cpp
	GUI/TextEditorDemo.cpp
	# GameFiles/GameGUI.cpp
	SOURCE_GROUP
	"Client"
	Client/Client.cpp
	Client/XClient.cpp
	SOURCE_GROUP
	"GameFiles"
	GameFiles/EntityClassRegistry.cpp
	GameFiles/DevMode.cpp
	GameFiles/Game.cpp
	GameFiles/GameActionInput.cpp
	GameFiles/GameClientServer.cpp
	GameFiles/GameDevMode.cpp
	GameFiles/GameLoading.cpp
	GameFiles/GameMods.cpp
	GameFiles/GameObject.cpp
	GameFiles/GameTagPoints.cpp
	GameFiles/SteamHelper.cpp
	GameFiles/XVehicleSystem.cpp
	SOURCE_GROUP
	"Network"
	Network/XEntityProcessingCmd.cpp
	Network/XNetwork.cpp
	Network/XNetworkStats.cpp
	SOURCE_GROUP
	"Player"
	Player/XPlayer.cpp
	SOURCE_GROUP
	"ScriptObjects"
	ScriptObjects/ScriptObjectClient.cpp
	ScriptObjects/ScriptObjectGame.cpp
	ScriptObjects/ScriptObjectInput.cpp
	ScriptObjects/ScriptObjectServer.cpp
	ScriptObjects/ScriptObjectServerSlot.cpp
	ScriptObjects/ScriptObjectStream.cpp
	ScriptObjects/ScriptObjectTest.cpp
	ScriptObjects/ScriptTimerMgr.cpp
	SOURCE_GROUP
	"Server"
	Server/Server.cpp
	Server/XServer.cpp
	Server/XServerRules.cpp
	Server/XServerSlot.cpp
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
end_sources()
gamemodule(Game FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER "Projects/Game")

add_subdirectory(../thirdparty 3rdparty)

target_link_libraries(${THIS_PROJECT} PRIVATE BlackBox)
if(OPTION_USE_GUI)
	# target_link_libraries(${THIS_PROJECT} PRIVATE GUI)
	# target_link_libraries(${THIS_PROJECT} PRIVATE ImGuiColorTextEdit)
	# target_link_libraries(${THIS_PROJECT} PRIVATE ImGuiFileBrowser)
endif()
if(WINDOWS)
	target_link_libraries(${THIS_PROJECT} PRIVATE Ws2_32)
	target_link_libraries(${THIS_PROJECT} PRIVATE dbghelp)
endif()
if(USE_STEAM)
	target_link_libraries(${THIS_PROJECT} PRIVATE Steam)
	target_compile_definitions(${THIS_PROJECT} PRIVATE USE_STEAM)
endif()
target_include_directories(
	${THIS_PROJECT}
	PRIVATE "${INCLUDE_ROOT}"
	PRIVATE "${INCLUDE_ROOT}/GameFiles"
	PRIVATE "${INCLUDE_ROOT}/GUI")

file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_ROOT}/*.hpp ${INCLUDE_ROOT}/*.h)

target_sources(${THIS_PROJECT} PRIVATE ${INCLUDE_FILES})

source_group(
	TREE ${INCLUDE_ROOT}
	PREFIX "Header Files"
	FILES ${INCLUDE_FILES})
