#include "EditorApp.h"
#include "EditorImpl.h"
#include "GameEngine.hpp"
#include "MainWindow.hpp"
#include "SplashScreen.h"

#include <BlackBox/Core/Path.hpp>
#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/System/File/ICryPak.hpp>

//////////////////////////////////////////////////////////////////////////
// The one and only CCryEditApp object
//////////////////////////////////////////////////////////////////////////
CEditApp theApp;

namespace
{
	CEditApp* s_pCCryEditApp = 0;
}

void Command_UnloadPlugins()
{
	GetIEditorImpl()->GetPluginManager()->UnloadAllPlugins();
}

void Command_LoadPlugins()
{
	Command_UnloadPlugins();

	char path[MAX_PATH];
	CryGetExecutableFolder(CRY_ARRAY_COUNT(path), path);
	const string executableFilePath = PathUtil::Make(path, "EditorPlugins\\*.dll");
	GetIEditorImpl()->GetPluginManager()->LoadPlugins(executableFilePath.c_str());
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
	startupParams.bEditor = false;
	// Enable run-time memory check for debug builds.

	// Note: lpCmdLine does not contain the filename.
	const string cmdLine = GetCommandLineA();
	strcpy(startupParams.szSystemCmdLine, cmdLine.c_str());

	auto result = false;

	if (InitializeEngine(startupParams, true))
	{
		auto pSystem = startupParams.pSystem;
		pSystem->GetILog()->Log("ISystem created");
		gEnv = pSystem->GetGlobalEnvironment();

		m_pEditor = new CEditorImpl(new CGameEngine(gEnv->pSystem->GetIGame()));

		result = m_pEditor->Init();
		// Create Sandbox user folder if necessary
		GetISystem()->GetIPak()->MakeDir("%USER%/Sandbox");

		InitPlugins();

        m_pEditor->InitFinished();
	}
	return result;
}

bool CEditApp::PostInit()
{
	return false;
}

int CEditApp::ExitInstance()
{
	if (GetIEditorImpl())
		GetIEditorImpl()->Notify(eNotify_OnQuit);

	m_bExiting = true;

	//HEAP_CHECK

	//////////////////////////////////////////////////////////////////////////
	// Quick end for editor.
	if (gEnv && gEnv->pSystem)
		gEnv->pSystem->Quit();
	//////////////////////////////////////////////////////////////////////////

	if (m_pEditor)
	{
		delete m_pEditor;
		m_pEditor = 0;
	}

	// save accelerator manager configuration.
	//m_AccelManager.SaveOnExit();

#if 0
	if (m_mutexApplication)
		CloseHandle(m_mutexApplication);
#endif

	return 0;
}

void CEditApp::InitPlugins()
{
	CRY_PROFILE_FUNCTION(PROFILE_LOADING_ONLY);
#if 0
	SplashScreen::SetText("Loading Plugins...");
#endif
	// Load the plugins
	{
		Command_LoadPlugins();
	}
}

void CEditApp::Run()
{
	while (true)
	{
		if (!IdleProcessing(false))
			return;
	}
}

bool CEditApp::IdleProcessing(bool bBackground)
{
	auto res = false;
	if (m_pEditor)
	{
		res = m_pEditor->Update();
	}

	#if 0
	GetIEditorImpl()->Notify(eNotify_OnIdleUpdate);
	#endif
	return res;
}

CEditApp::CEditApp()
{
	s_pCCryEditApp = this;
}

CEditApp::~CEditApp()
{
}
