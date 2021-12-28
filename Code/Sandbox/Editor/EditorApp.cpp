#include "EditorApp.h"
#include "MainWindow.hpp"
#include "GameEngine.hpp"
#include "EditorImpl.h"

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
	startupParams.sLogFileName		= "Editor.log";
	startupParams.bManualEngineLoop = true;
	//startupParams.bEditor			= true;
	startupParams.bEditor			= false;
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

		m_pEditor = new CEditorImpl(new CGameEngine(gEnv->pSystem->GetIGame()));

		result = m_pEditor->Init();
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
	while(true)
	{
		if (!m_pEditor->Update())
			return;
	}
}

CEditApp::CEditApp()
{
	s_pCCryEditApp = this;
}

CEditApp::~CEditApp()
{
}
