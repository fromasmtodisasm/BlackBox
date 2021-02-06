#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <BlackBox/Core/Platform/CryLibrary.h>

#include <BlackBox/Core/Platform/platform_impl.inl>
#include <crtdbg.h>

// Advise notebook graphics drivers to prefer discrete GPU when no explicit application profile exists
#if 1
extern "C"
{
	// nVidia
	DLL_EXPORT DWORD NvOptimusEnablement = 0x00000001;
	// AMD
	DLL_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}
#endif


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{
	SSystemInitParams startupParams;
	startupParams.sLogFileName = "Game.log";
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Note: lpCmdLine does not contain the filename.
	const string cmdLine = GetCommandLineA();
	strcpy(startupParams.szSystemCmdLine, cmdLine.c_str());

	auto result = EXIT_FAILURE;
	//_CrtMemState s1, s2, s3;
	//_CrtMemCheckpoint(&s1);
	for (int i = 0; i < 1; i++)
	{
		if (InitializeEngine(startupParams))
		{
			startupParams.pSystem->Start();
			startupParams.pSystem->Release();
			result = EXIT_SUCCESS;
		}
		
	}
#if 0
	_CrtMemCheckpoint(&s2);
	if (_CrtMemDifference(&s3, &s1, &s2))
	{
		printf("leak detected\n");
		_CrtMemDumpStatistics(&s3);
		_CrtDumpMemoryLeaks();
	}
#endif
	return result;
}

