#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <BlackBox/Core/Platform/CryLibrary.h>

#include <BlackBox/Core/Platform/platform_impl.inl>

// Advise notebook graphics drivers to prefer discrete GPU when no explicit application profile exists
extern "C"
{
	// nVidia
	DLL_EXPORT DWORD NvOptimusEnablement = 0x00000001;
	// AMD
	DLL_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, [[maybe_unused]] int nShowCmd)
{
	SSystemInitParams startupParams;
	startupParams.sLogFileName = "Game.log";
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Note: lpCmdLine does not contain the filename.
	const string cmdLine = GetCommandLineA();
	strcpy(startupParams.szSystemCmdLine, cmdLine.c_str());

	if (InitializeEngine(startupParams))
	{
		startupParams.pSystem->Start();
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

