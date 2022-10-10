# ##############################################################################
set(INCLUDE_ROOT ${CMAKE_CURRENT_LIST_DIR}/../include)
# ##############################################################################

start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	SOURCE_GROUP "Source Files"
		Main.cpp
		Commands.cpp
	SOURCE_GROUP "GameClient"
		Client/Client.cpp
		Client/XClient.cpp
		Client/XClientSnapshot.cpp
	SOURCE_GROUP "GameSystem"
		GameSystem/XSystemBase.cpp
		GameSystem/XSystemBase.h
		GameSystem/XSystemClient.cpp
		GameSystem/XSystemClient.h
		GameSystem/XSystemDummy.cpp
		GameSystem/XSystemDummy.h
		GameSystem/XSystemEntities.cpp
		GameSystem/XSystemEntities.h
		GameSystem/XSystemServer.cpp
		GameSystem/XSystemServer.h
	SOURCE_GROUP "GameFiles"
		GameFiles/GameObject.h
		# GameFiles/GameGUI.cpp
		GameFiles/DevMode.cpp
		GameFiles/EntityClassRegistry.cpp
		GameFiles/Game.cpp
		GameFiles/Minecraft.cpp
		GameFiles/GameActionInput.cpp
		GameFiles/GameCallbacks.cpp
		GameFiles/GameClientServer.cpp
		GameFiles/GameDevMode.cpp
		GameFiles/GameEvents.cpp
		GameFiles/GameLoading.cpp
		GameFiles/GameMemStats.cpp
		GameFiles/GameMisc.cpp
		GameFiles/GameMods.cpp
		GameFiles/GameMovieUser.cpp
		GameFiles/GameTagPoints.cpp
		GameFiles/SteamHelper.cpp
		GameFiles/XArea.cpp
		GameFiles/XArea.h
		GameFiles/XVehicleSystem.cpp
		GameFiles/CameraController.cpp

		GameFiles/Snake.cpp
		GameFiles/Snake.h
	SOURCE_GROUP "Network"
		Network/NetEntityInfo.cpp
		Network/SynchedRandomSeed.cpp
		Network/XEntityProcessingCmd.cpp
		Network/XNetwork.cpp
		Network/XNetworkStats.cpp
	SOURCE_GROUP "Network\\\\Minecraft\\\\Common"
       Network/Minecraft/Common/ClientServer.h
	SOURCE_GROUP "Network\\\\Minecraft\\\\Server"
        Network/Minecraft/Server/Server.cpp
        Network/Minecraft/Server/Server.h
        Network/Minecraft/Server/users.txt
	SOURCE_GROUP "Player"
		Player/XPlayer.cpp
		Player/XPlayerCamera.cpp
		Player/Spectator.cpp
		Player/Spectator.h
		Player/AdvCamSystem.cpp
		Player/AdvCamSystem.h
	SOURCE_GROUP "ScriptObjects"
		ScriptObjects/ScriptObjectClient.cpp
		ScriptObjects/ScriptObjectGame.cpp
		ScriptObjects/ScriptObjectPlayer.cpp
		ScriptObjects/ScriptObjectInput.cpp
		ScriptObjects/ScriptObjectServer.cpp
		ScriptObjects/ScriptObjectServerSlot.cpp
		ScriptObjects/ScriptObjectStream.cpp
		ScriptObjects/ScriptObjectTest.cpp
		ScriptObjects/ScriptTimerMgr.cpp
	SOURCE_GROUP "Server"
		Server/Server.cpp
		Server/XServer.cpp
		Server/XServerRules.cpp
		Server/XServerSlot.cpp
		Server/XSnapShot.h
		Server/XSnapShot.cpp
	SOURCE_GROUP "Localization"
		Localization/StringTableMgr.cpp
		Localization/StringTableMgr.h
		Localization/ScriptObjectLanguage.cpp
		Localization/ScriptObjectLanguage.h
	SOURCE_GROUP "Materials"
		Materials/XSurfaceMgr.cpp
		Materials/XSurfaceMgr.h
	SOURCE_GROUP "TimeDemoRecorder"
		TimeDemoRecorder/TimeDemoRecorder.cpp
		TimeDemoRecorder/TimeDemoRecorder.h

		TimeDemoRecorder/XDemoMgr.cpp
		TimeDemoRecorder/XDemoMgr.h
	SOURCE_GROUP "UI"
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
		UI/UIVideoPanel.cpp
		UI/UIVideoPanel.h
		UI/UIWidget.cpp
		UI/UIWidget.h
	#UI/Proxy.cpp
	SOURCE_GROUP "Legacy"
		Legacy/CryCharAnimationParams.h
		Legacy/I3DEngine.h
		Legacy/IBindable.h
		Legacy/ICryPak.h
		Legacy/IEntitySystem.h
		Legacy/IInput.h
		Legacy/IScriptSystem.h
		Legacy/ITimer.h
		Legacy/Input.cpp
		Legacy/Input.hpp
		Legacy/ScriptObjectStream.h
		Legacy/ScriptObjectVector.h
		Legacy/Stream.h
		Legacy/TArray.h
		Legacy/TimeValue.h
	SOURCE_GROUP "Weapons"
		Weapons/FireType.h
		Weapons/WeaponClass.cpp
		Weapons/WeaponClass.h
		Weapons/WeaponSystemEx.cpp
		Weapons/WeaponSystemEx.h
	SOURCE_GROUP "Vehicle"
		#Vehicle/XVehicle.cpp
		Vehicle/XVehicle.h
		#Vehicle/XVehicleProxy.cpp
		Vehicle/XVehicleProxy.h
		#Vehicle/XVehicleProxyHeli.cpp
		#Vehicle/XVehicleSystem.cpp
		Vehicle/XVehicleSystem.h
)
end_sources()
GameModule(MineCRY PCH "pch.hpp" SOLUTION_FOLDER "Projects/Games")

add_subdirectory(../thirdparty 3rdparty)

target_link_libraries(${THIS_PROJECT} PRIVATE BlackBox)
if(WINDOWS)
	target_link_libraries(${THIS_PROJECT} PRIVATE Ws2_32)
	target_link_libraries(${THIS_PROJECT} PRIVATE dbghelp)
endif()
if(USE_STEAM)
	target_link_libraries(${THIS_PROJECT} PRIVATE Steam)
	target_compile_definitions(${THIS_PROJECT} PRIVATE USE_STEAM)
endif()

set(OPTION_NOT_USE_DIVX_SDK ON)
if(OPTION_NOT_USE_DIVX_SDK)
	target_compile_definitions(${THIS_PROJECT} PRIVATE NOT_USE_DIVX_SDK)
	target_compile_definitions(${THIS_PROJECT} PRIVATE NOT_USE_BINK_SDK)
endif()

file(GLOB_RECURSE INCLUDE_FILES ${INCLUDE_ROOT}/*.hpp ${INCLUDE_ROOT}/*.h)

target_sources(${THIS_PROJECT} PRIVATE ${INCLUDE_FILES})

target_include_directories(${THIS_PROJECT} 
	PRIVATE 
		"${INCLUDE_ROOT}"
		"${INCLUDE_ROOT}/GameFiles"
		"${INCLUDE_ROOT}/Network"
		"${INCLUDE_ROOT}/Bink"
		"${INCLUDE_ROOT}/GUI"
	
		${CMAKE_CURRENT_LIST_DIR}/UI
		${CMAKE_CURRENT_LIST_DIR}/Legacy
		${CMAKE_CURRENT_LIST_DIR}/TimeDemoRecorder
		${CMAKE_CURRENT_LIST_DIR}/Weapons
		${CMAKE_CURRENT_LIST_DIR}/Network
		${CMAKE_CURRENT_LIST_DIR}/Vehicle
		${CMAKE_CURRENT_LIST_DIR}/GameSystem
		${CMAKE_CURRENT_LIST_DIR}/Player
)

source_group(
	TREE ${INCLUDE_ROOT}
	PREFIX "Header Files"
	FILES ${INCLUDE_FILES})
