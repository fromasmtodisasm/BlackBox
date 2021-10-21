#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Utils/smartptr.hpp>

#include <BlackBox/Core/Platform/CryLibrary.h>

#include <BlackBox/Core/Platform/platform_impl.inl>
#include <crtdbg.h>
#include "resource.h"

// Advise notebook graphics drivers to prefer discrete GPU when no explicit application profile exists
extern "C"
{
	// nVidia
	DLL_EXPORT DWORD NvOptimusEnablement = 0x00000001;
	// AMD
	DLL_EXPORT int AmdPowerXpressRequestHighPerformance = 1;
}

static INT_PTR SelectDeviceProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_INITDIALOG:
	{
		//hCombo = GetDlgItem(hwnd, IDC_DEVICE_LIST);
		//SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)("TestDevice"));
		return true;
	}
	case WM_COMMAND:
	{
		break;
	}
	case WM_CLOSE:
		EndDialog(hwnd, 0);
		return false;
	}

	return false;
}

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
		startupParams.pSystem->Start();
		startupParams.pSystem->Release();
		result = EXIT_SUCCESS;
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

