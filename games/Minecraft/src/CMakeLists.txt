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
	"GameClient"
	Client/Client.cpp
	Client/XClient.cpp
	Client/XClientSnapshot.cpp
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
	GameFiles/GameTagPoints.cpp
	GameFiles/GameMemStats.cpp
	GameFiles/GameMisc.cpp
	GameFiles/GameEvents.cpp
	GameFiles/SteamHelper.cpp
	GameFiles/XVehicleSystem.cpp
	GameFiles/XArea.cpp
	#GameFiles/GameObject.h
	GameFiles/GameCallbacks.cpp
	GameFiles/XArea.h
	SOURCE_GROUP
	"Network"
	Network/XEntityProcessingCmd.cpp
	Network/XNetwork.cpp
	Network/XNetworkStats.cpp
	Network/SynchedRandomSeed.cpp
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
	"Localization"
	Localization/StringTableMgr.cpp
	Localization/StringTableMgr.h
	Localization/ScriptObjectLanguage.cpp
	Localization/ScriptObjectLanguage.h
	SOURCE_GROUP
	"Materials"
	Materials/XSurfaceMgr.cpp
	Materials/XSurfaceMgr.h
	SOURCE_GROUP
	"TimeDemoRecorder"
	TimeDemoRecorder/TimeDemoRecorder.cpp
	TimeDemoRecorder/TimeDemoRecorder.h

	TimeDemoRecorder/XDemoMgr.cpp
	TimeDemoRecorder/XDemoMgr.h
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
	#UI/UIDivX_Video.cpp
	#UI/UIDivX_Video.h
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
	#UI/UIVideoPanel.cpp
	#UI/UIVideoPanel.h
	UI/UIWidget.cpp
	UI/UIWidget.h
	SOURCE_GROUP
	"Legacy"
	Legacy/IInput.h
	Legacy/Input.cpp
	Legacy/IFont.h
	Legacy/ScriptObjectVector.h
	Legacy/ScriptObjectStream.h
	Legacy/IEntitySystem.h
	SOURCE_GROUP
	"Weapons"
	Weapons/FireType.h
	Weapons/WeaponClass.cpp
	Weapons/WeaponClass.h
	Weapons/WeaponSystemEx.cpp
	Weapons/WeaponSystemEx.h
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
	PRIVATE "${INCLUDE_ROOT}/GUI")

file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_ROOT}/*.hpp ${INCLUDE_ROOT}/*.h)

target_sources(${THIS_PROJECT} PRIVATE ${INCLUDE_FILES})
target_include_directories(${THIS_PROJECT} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/UI)
target_include_directories(${THIS_PROJECT} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Legacy)
target_include_directories(${THIS_PROJECT} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/TimeDemoRecorder)
target_include_directories(${THIS_PROJECT} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Weapons)
target_include_directories(${THIS_PROJECT} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/Network)

source_group(
	TREE ${INCLUDE_ROOT}
	PREFIX "Header Files"
	FILES ${INCLUDE_FILES})
