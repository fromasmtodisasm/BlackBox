#include "EditorApp.h"
#include "EditorImpl.h"
#include "GameEngine.hpp"
#include "MainWindow.hpp"
#include "SplashScreen.h"

#include <BlackBox/Core/Path.hpp>
#include <BlackBox/Core/Platform/Platform.hpp>
//#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/Core/Platform/CryLibrary.h>
#include <BlackBox/System/File/ICryPak.hpp>

//////////////////////////////////////////////////////////////////////////
// The one and only CCryEditApp object
//////////////////////////////////////////////////////////////////////////
CEditApp theApp;
extern QWidget* game_window;

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
	auto pGameEngine = InitGameSystem();
	if (!pGameEngine)
	{
		return false;
	}

	auto result = false;


    m_pEditor = new CEditorImpl(pGameEngine.release());

    result = m_pEditor->Init();

	SplashScreen::SetVersion(m_pEditor->GetFileVersion());

    // Create Sandbox user folder if necessary
    GetISystem()->GetIPak()->MakeDir("%USER%/Sandbox");


    InitPlugins();

    m_pEditor->InitFinished();
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

	#if 0
	gEnv->pSystem->Release();
	#endif

	auto h = GetModuleHandleA("System.dll");
	if (h)
		CryFreeLibrary(h);

	return 0;
}

void CEditApp::InitPlugins()
{
	CRY_PROFILE_FUNCTION(PROFILE_LOADING_ONLY);
	SplashScreen::SetText("Loading Plugins...");
	// Load the plugins
	{
		Command_LoadPlugins();
	}
}

void CEditApp::Run()
{
	#if 0
	while (true)
	{
		if (!IdleProcessing(false))
			return;
	}
	#endif
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

namespace
{

// Callback class for initialization messages from system.
struct SInitializeUIInfo : IInitializeUIInfo
{
	virtual void SetInfoText(const char* text) override
	{
		SplashScreen::SetText(text);
	}

	static SInitializeUIInfo& GetInstance()
	{
		static SInitializeUIInfo theInstance;
		return theInstance;
	}
};

} // namespace

std::unique_ptr<CGameEngine> CEditApp::InitGameSystem()
{
	bool bShaderCacheGen = false;
	//m_bPrecacheShaderList | m_bPrecacheShaders | m_bPrecacheShadersLevels;
	//std::unique_ptr<CGameEngine> pGameEngine	 = stl::make_unique<CGameEngine>();
	std::unique_ptr<CGameEngine> pGameEngine	 = std::make_unique<CGameEngine>();
	auto						 uiinfo		 = &SInitializeUIInfo::GetInstance();
	if (!pGameEngine->Init(m_bTestMode, bShaderCacheGen, GetCommandLineA(), uiinfo))
	{
		return nullptr;
	}

	return pGameEngine;
}

CEditApp::CEditApp()
{
	s_pCCryEditApp = this;
}

CEditApp::~CEditApp()
{
}
