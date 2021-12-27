#include "EditorApp.h"
#include "MainWindow.hpp"

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/platform_impl.inl>

//////////////////////////////////////////////////////////////////////////
// The one and only CCryEditApp object
//////////////////////////////////////////////////////////////////////////
CEditApp theApp;

namespace
{
	CEditApp* s_pCCryEditApp = 0;
}

CEditApp* CEditApp::GetInstance()
{
	return s_pCCryEditApp;
}

bool CEditApp::InitInstance()
{
	SSystemInitParams startupParams;
	startupParams.sLogFileName		= "Game.log";
	startupParams.bManualEngineLoop = true;
	// Enable run-time memory check for debug builds.

	// Note: lpCmdLine does not contain the filename.
	const string cmdLine = GetCommandLineA();
	strcpy(startupParams.szSystemCmdLine, cmdLine.c_str());

	auto result = false;

	if (InitializeEngine(startupParams, true))
	{
		auto pSystem = startupParams.pSystem;
		pSystem->GetILog()->Log("ISystem created");
		//pSystem->GetILog()->Log("Current working directory: %s", path.c_str());
		gEnv = pSystem->GetGlobalEnvironment();

		result = true;
	}
	return result;
}

bool CEditApp::PostInit()
{
	return false;
}

int CEditApp::ExitInstance()
{
	gEnv->pSystem->Release();
	return 0;
}

void CEditApp::Run()
{
    MainWindow mainWindow;
	while (mainWindow.Update())
		;
}

CEditApp::CEditApp()
{
	s_pCCryEditApp = this;
}

CEditApp::~CEditApp()
{
}
