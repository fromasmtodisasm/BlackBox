#include "GameEngine.hpp"

#include <BlackBox/Core/Platform/platform_impl.inl>
#include <EditorImpl.h>

#include "EditorApp.h"
#include <Controls/QuestionDialog.h>
#include "SplashScreen.h"
#include "ProjectManagement/Utils.h"
#include <QApplication>

HWND main_hwnd();

// Implementation of System Callback structure.
struct SSystemUserCallback : public ISystemUserCallback
{
	SSystemUserCallback(IInitializeUIInfo* logo) { m_pLogo = logo; }
	virtual void OnSystemConnect(ISystem* pSystem)
	{
		ModuleInitISystem(pSystem, "Editor");
	}

	virtual bool OnSaveDocument()
	{
		return GetIEditorImpl()->SaveDocumentBackup();
	}

	virtual void OnProcessSwitch()
	{
		if (GetIEditorImpl()->IsInGameMode())
			GetIEditorImpl()->SetInGameMode(false);
	}

	virtual void OnInitProgress(const char* sProgressMsg)
	{
		if (m_pLogo)
			m_pLogo->SetInfoText(sProgressMsg);
	}

	struct SEditorSuspendUpdateScopeHelper
	{
		#if 0
		SEditorSuspendUpdateScopeHelper()
		{
			CEditApp::GetInstance()->SuspendUpdate();
		}

		~SEditorSuspendUpdateScopeHelper()
		{
			CEditApp::GetInstance()->ResumeUpdate();
		}
		#endif
	};

	virtual EQuestionResult ShowMessage(const char* text, const char* caption, EMessageBox uType)
	{
		SEditorSuspendUpdateScopeHelper suspendUpdateScope;
		(void)suspendUpdateScope;

		switch (uType)
		{
		case eMB_Error:
			CQuestionDialog::SCritical(caption, text);
			return eQR_None;

		case eMB_YesCancel:
			if (QDialogButtonBox::Yes == CQuestionDialog::SQuestion(caption, text))
				return eQR_Yes;
			return eQR_Cancel;

		case eMB_YesNoCancel:
			QDialogButtonBox::StandardButton res = CQuestionDialog::SQuestion(caption, text, QDialogButtonBox::Yes | QDialogButtonBox::No | QDialogButtonBox::Cancel);
			if (QDialogButtonBox::Yes == res)
				return eQR_Yes;
			return QDialogButtonBox::No == res ? eQR_No : eQR_Cancel;
		}

		CQuestionDialog::SWarning(caption, text);

		return eQR_None;
	}

	virtual void GetMemoryUsage(ICrySizer* pSizer)
	{
		#if 0
		GetIEditorImpl()->GetMemoryUsage(pSizer);
		#endif
	}

  private:
	IInitializeUIInfo* m_pLogo;
};

class CGameToEditorInterface
{
};

CGameEngine::CGameEngine()
{

}
CGameEngine::~CGameEngine()
{	
	if (CEditorImpl* pEditorImpl = GetIEditorImpl())
	{
		#if 0
		pEditorImpl->UnregisterNotifyListener(this);
		#endif
	}

	if (m_pISystem != nullptr)
	{
		#if 0
		m_pISystem->GetIMovieSystem()->SetCallback(NULL);
		#endif
	}

	#if 0
	CEntityScriptRegistry::Release();
	CEdMesh::ReleaseAll();

	delete m_pNavigation;

	if (m_pEditorGame)
	{
		m_pEditorGame->Shutdown();
	}
	#endif

	m_pISystem = nullptr;

	/*
	   if (m_hSystemHandle)
	   FreeLibrary(m_hSystemHandle);
	 */
	delete m_pGameToEditorInterface;
	delete m_pSystemUserCallback;
}
// Timur.
// This is FarCry.exe authentication function, this code is not for public release!!
static void GameSystemAuthCheckFunction(void* data)
{
	// src and trg can be the same pointer (in place encryption)
	// len must be in bytes and must be multiple of 8 byts (64bits).
	// key is 128bit:  int key[4] = {n1,n2,n3,n4};
	// void encipher(unsigned int *const v,unsigned int *const w,const unsigned int *const k )
#define TEA_ENCODE(src, trg, len, key)                                            \
	{                                                                             \
		unsigned int *v = (src), *w = (trg), *k = (key), nlen = (len) >> 3;       \
		unsigned int  delta = 0x9E3779B9, a = k[0], b = k[1], c = k[2], d = k[3]; \
		while (nlen--)                                                            \
		{                                                                         \
			unsigned int y = v[0], z = v[1], n = 32, sum = 0;                     \
			while (n-- > 0)                                                       \
			{                                                                     \
				sum += delta;                                                     \
				y += (z << 4) + a ^ z + sum ^ (z >> 5) + b;                       \
				z += (y << 4) + c ^ y + sum ^ (y >> 5) + d;                       \
			}                                                                     \
			w[0] = y;                                                             \
			w[1] = z;                                                             \
			v += 2, w += 2;                                                       \
		}                                                                         \
	}

	// src and trg can be the same pointer (in place decryption)
	// len must be in bytes and must be multiple of 8 bytes (64bits).
	// key is 128bit: int key[4] = {n1,n2,n3,n4};
	// void decipher(unsigned int *const v,unsigned int *const w,const unsigned int *const k)
#define TEA_DECODE(src, trg, len, key)                                            \
	{                                                                             \
		unsigned int *v = (src), *w = (trg), *k = (key), nlen = (len) >> 3;       \
		unsigned int  delta = 0x9E3779B9, a = k[0], b = k[1], c = k[2], d = k[3]; \
		while (nlen--)                                                            \
		{                                                                         \
			unsigned int y = v[0], z = v[1], sum = 0xC6EF3720, n = 32;            \
			while (n-- > 0)                                                       \
			{                                                                     \
				z -= (y << 4) + c ^ y + sum ^ (y >> 5) + d;                       \
				y -= (z << 4) + a ^ z + sum ^ (z >> 5) + b;                       \
				sum -= delta;                                                     \
			}                                                                     \
			w[0] = y;                                                             \
			w[1] = z;                                                             \
			v += 2, w += 2;                                                       \
		}                                                                         \
	}

	// Data assumed to be 32 bytes.
	int key1[4] = {1178362782, 223786232, 371615531, 90884141};
	TEA_DECODE((unsigned int*)data, (unsigned int*)data, 32, (unsigned int*)key1);
	int key2[4] = {89158165, 1389745433, 971685123, 785741042};
	TEA_ENCODE((unsigned int*)data, (unsigned int*)data, 32, (unsigned int*)key2);
}

bool CGameEngine::Init(bool bTestMode, bool bShaderCacheGen, const char* sInCmdLine, IInitializeUIInfo* logo)
{
	m_pSystemUserCallback = new SSystemUserCallback(logo);
	m_pGameToEditorInterface = new CGameToEditorInterface();

	SSystemInitParams startupParams;

	startupParams.bEditor = true;

	startupParams.sLogFileName	= "Editor.log";
	startupParams.pUserCallback = m_pSystemUserCallback;

	startupParams.hWnd = main_hwnd();

	if (sInCmdLine)
	{
		cry_strcpy(startupParams.szSystemCmdLine, sInCmdLine);
		#if 0
		if (strstr(sInCmdLine, "-export"))
			startupParams.bUnattendedMode = true;
		#endif
	}

	startupParams.pCheckFunc = &GameSystemAuthCheckFunction;

	#if 0
	if (bShaderCacheGen)
		startupParams.bSkipFont = true;
	#endif

	// We do this manually in the Editor
	startupParams.bExecuteCommandLine = false;

	#if 0
	if (strstr(sInCmdLine, "-create_project"))
	{
		const string projectPath = AskUserToSpecifyProject(SplashScreen::GetSplashScreen(), true, CSelectProjectDialog::Tab::Create);
		if (projectPath.empty())
		{
			// Exit Sandbox
			return false;
		}
		AppendProjectPathToCommandLine(projectPath, startupParams);
	}
	else if (strstr(sInCmdLine, "-project") == 0)
	{
		const string engineFolder = FindCryEngineRootFolder();
		if (IsProjectSpecifiedInSystemConfig(engineFolder))
		{
			//1. it is responsibility of a user to check correctness of system.cfg file
			//2. It is engine responsibility to run itself with this information (it will parse and use system.cfg as game_launcher)
		}
		else
		{
			string projPath = FindProjectInFolder(engineFolder);
			if (projPath.empty())
			{
				projPath = AskUserToSpecifyProject(SplashScreen::GetSplashScreen(), true, CSelectProjectDialog::Tab::Open);
			}

			if (projPath.empty())
			{
				// Exit Sandbox
				return false;
			}

			AppendProjectPathToCommandLine(projPath, startupParams);
		}
	}
	#endif

	auto wnd_proc_before = (WNDPROC)GetWindowLongPtr((HWND)startupParams.hWnd, GWLP_WNDPROC);
	if (!InitializeEngine(startupParams, true))
	{
		return false;
	}
	auto wnd_proc_after = (WNDPROC)GetWindowLongPtr((HWND)startupParams.hWnd, GWLP_WNDPROC);
	m_SDL_EventFilter	= std::make_unique<SDL_EventFilter>((HWND)startupParams.hWnd);
	qApp->installNativeEventFilter(m_SDL_EventFilter.get());
	
	#if 0
	assert(wnd_proc_before == wnd_proc_after && "Wnd procedures missmatch");
	#endif
	m_pGame = gEnv->pSystem->GetIGame();
	return true;
}

void CGameEngine::Update()
{

}

void CGameEngine::SetSimulationMode(bool enabled, bool bOnlyPhysics)
{
}
