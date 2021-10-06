# ##############################################################################
set(INCLUDE_ROOT ${CMAKE_CURRENT_LIST_DIR}/../include)
# ##############################################################################

start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	Minecraft
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
	GameFiles/GameMemStats.cpp
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
	SOURCE_GROUP
	"UI"
	UI/IngameDialog.cpp
	UI/IngameDialog.h
	UI/ScriptObjectUI.cpp
	UI/ScriptObjectUI.h
	UI/UIButton.cpp
	UI/UIButton.h
	UI/UICVars.cpp
	UI/UICVars.h
	UI/UICheckBox.cpp
	UI/UICheckBox.h
	UI/UIComboBox.cpp
	UI/UIComboBox.h
	UI/UIDivX_Video.cpp
	UI/UIDivX_Video.h
	UI/UIEditBox.cpp
	UI/UIEditBox.h
	UI/UIHud.cpp
	UI/UIHud.h
	UI/UIListView.cpp
	UI/UIListView.h
	UI/UIMessages.h
	UI/UIScreen.cpp
	UI/UIScreen.h
	UI/UIScrollBar.cpp
	UI/UIScrollBar.h
	UI/UIStatic.cpp
	UI/UIStatic.h
	UI/UISystem.cpp
	UI/UISystem.h
	UI/UIVideoPanel.cpp
	UI/UIVideoPanel.h
	UI/UIWidget.cpp
	UI/UIWidget.h
)
end_sources()
gamemodule(Minecraft FORCE_SHARED_WIN PCH "pch.hpp" SOLUTION_FOLDER "Projects/Games")

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
	PRIVATE "${INCLUDE_ROOT}/GUI"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Redirect"
	)

file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_ROOT}/*.hpp ${INCLUDE_ROOT}/*.h)

target_sources(${THIS_PROJECT} PRIVATE ${INCLUDE_FILES})

source_group(
	TREE ${INCLUDE_ROOT}
	PREFIX "Header Files"
	FILES ${INCLUDE_FILES})
