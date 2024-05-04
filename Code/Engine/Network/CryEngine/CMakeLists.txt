start_sources()
sources_platform(ALL)
add_sources(
	"NoUberFile"
	PROJECTS
	Network
	SOURCE_GROUP "Source Files"
		ASEQuerySDK.h
		ASEQuerySDK.lib
		CCPEndpoint.cpp
		CCPEndpoint.h
		CMakeLists.txt
		CNP.h
		CTPEndpoint.cpp
		CTPEndpoint.h
		CTPEndpointGNB.cpp
		CTPEndpointGNB.h
		Client.cpp
		Client.h
		ClientLocal.cpp
		ClientLocal.h
		ClientStateMachine.cpp
		ClientStateMachine.h
		CommonDefines.h
		CompressionHelper.cpp
		CompressionHelper.h
		CryNetwork.cpp
		DatagramSocket.cpp
		DatagramSocket.h
		DefenceWall.cpp
		DefenceWall.h
		Interfaces.h
		MSSCCPRJ.SCC
		NETServerSnooper.cpp
		NETServerSnooper.h
		Network.cpp
		Network.h
		NewUbisoftCDKey.cpp
		NewUbisoftClient.cpp
		NewUbisoftClient.h
		NewUbisoftMSClient.cpp
		NewUbisoftRegServer.cpp
		PingCalculator.h
		PunkBusterInterface.cpp
		PunkBusterInterface.h
		RConSystem.cpp
		RConSystem.h
		ScriptObjectNewUbisoftClient.cpp
		ScriptObjectNewUbisoftClient.h
		Server.cpp
		Server.h
		ServerSlot.cpp
		ServerSlot.h
		ServerSnooper.cpp
		ServerSnooper.h
		ServerStateMachine.cpp
		ServerStateMachine.h
		StateMachine.h
		StaticCharCompressor.cpp
		StaticCharCompressor.h
		StdAfx.cpp
		StdAfx.h
		UbisoftMemory.cpp
		UbisoftMemory.h
		#todo.txt

)
end_sources()
EngineModule(Network SOLUTION_FOLDER "Engine")
#add_sdl(${THIS_PROJECT})
set(PROJECT_NAME ${THIS_PROJECT})
#find_package(SDL2_net CONFIG REQUIRED)
#target_link_libraries(${THIS_PROJECT} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_net::SDL2_net>,SDL2_net::SDL2_net,SDL2_net::SDL2_net-static>)
target_link_libraries(${THIS_PROJECT} PRIVATE Math)
target_link_libraries(${THIS_PROJECT} PRIVATE Utils)
