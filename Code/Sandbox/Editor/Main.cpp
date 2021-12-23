#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/MathHelper.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include "MainWindow.hpp"

#include <iostream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <filesystem>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{
	//DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOGBAR), 0, SelectDeviceProc, 0);

	SSystemInitParams startupParams;
	startupParams.sLogFileName = "Game.log";
	startupParams.bManualEngineLoop = true;
	// Enable run-time memory check for debug builds.

	// Note: lpCmdLine does not contain the filename.
	const string cmdLine = GetCommandLineA();
	strcpy(startupParams.szSystemCmdLine, cmdLine.c_str());

	auto result = EXIT_FAILURE;
#if defined(DEBUG) | defined(_DEBUG) && 0
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtMemState s1, s2, s3;
	_CrtMemCheckpoint(&s1);
#endif
	if (InitializeEngine(startupParams, true))
	{
		auto pSystem = startupParams.pSystem;
        pSystem->GetILog()->Log("ISystem created");
        //pSystem->GetILog()->Log("Current working directory: %s", path.c_str());
        gEnv = pSystem->GetGlobalEnvironment();
        MainWindow mainWindow;

        while (mainWindow.Update())
                ;
        result = EXIT_SUCCESS;
		//startupParams.pSystem->Start();
		//startupParams.pSystem->Release();
		//result = EXIT_SUCCESS;
	}

#if defined(DEBUG) | defined(_DEBUG) && 0
	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		OutputDebugString("Memory Leak detected\n");
		_CrtMemDumpStatistics(&s3);
		_CrtDumpMemoryLeaks();
	}
#endif
	return result;
}

