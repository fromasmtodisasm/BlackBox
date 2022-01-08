#include "EditorImpl.h"
#include "GameEngine.hpp"

#include "PluginManagerWindow.h"
#include <BlackBox\Core\IGame.hpp>

#include "EditorCommonInit.h"

#include "ClassFactory.h"

static CEditorImpl* s_pEditor = nullptr;

IEditor* GetIEditor() { return s_pEditor; }

CEditorImpl* GetIEditorImpl()
{
	return s_pEditor;
}

CEditorImpl::CEditorImpl(CGameEngine* ge)
{
	m_pGame = ge->m_pGame;
	gEnv->pRenderer->RegisterCallbackClient(this);

	s_pEditor = this;

	EditorCommon::SetIEditor(this);

	m_pPluginManager = new CPluginManager;
	m_pClassFactory	 = CClassFactory::Instance();
}

CEditorImpl::~CEditorImpl()
{
	UnregisterNotifyListener(&m_MainWindow);
	SAFE_RELEASE(m_GuiManager);

	SAFE_DELETE(m_pPluginManager)
	EditorCommon::Deinitialize();

	s_pEditor = nullptr;
}

void CEditorImpl::Draw()
{
	//m_MainMenu.Draw();
	//m_MainWindow.Draw();
	PluginManagerWindow::Draw();
}

bool CEditorImpl::SaveDocumentBackup()
{
	if (m_bExiting)
		return false;
	return false;
}

inline ISystem* CEditorImpl::GetSystem() { return gEnv->pSystem; }

bool CEditorImpl::Update()
{
	gEnv->pSystem->GetIInput()->Update(true);
	m_GuiManager->NewFrame();
	m_GuiManager->ShowDemoWindow();
	gEnv->pSystem->Update(ESYSUPDATE_EDITOR);
	#if 0
	GetIEditorImpl()->Notify(eNotify_OnIdleUpdate);
	#endif

	return m_pGame->Update();
}

void CEditorImpl::InitFinished()
{
	Notify(eNotify_OnInit);

	// Let system wide listeners know about this as well.
	GetISystem()->GetISystemEventDispatcher()->OnSystemEvent(ESYSTEM_EVENT_EDITOR_ON_INIT, 0, 0);
}

void CEditorImpl::Notify(EEditorNotifyEvent event)
{
	#if 0
	if (m_bExiting)
		return;
	#endif

	std::list<IEditorNotifyListener*>::iterator it = m_listeners.begin();
	for (auto it : m_listeners)
	{
		(it++)->OnEditorNotifyEvent(event);
	
	}
}

void CEditorImpl::RegisterNotifyListener(IEditorNotifyListener* listener)
{
	listener->m_bIsRegistered = true;
	stl::push_back_unique(m_listeners, listener);
}

void CEditorImpl::UnregisterNotifyListener(IEditorNotifyListener* listener)
{
	m_listeners.remove(listener);
	listener->m_bIsRegistered = false;
}

void CEditorImpl::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
}

void CEditorImpl::CallBack(Type type)
{
	switch (type)
	{
	case IRenderCallback::eOnRender:
	{
		//auto s = m_NextFrameViewPortSize;
		//gEnv->pRenderer->SetViewport(0, 0, static_cast<int>(s.x), static_cast<int>(s.y));
		//break;
	}
	case IRenderCallback::eBeforeSwapBuffers:
	{
	}
	default:
		break;
	}
}

void CEditorImpl::OnRenderer_BeforeEndFrame()
{
	Draw();
	m_GuiManager->Render();
}

IEditorClassFactory* CEditorImpl::GetClassFactory()
{
	return m_pClassFactory;
}

bool CEditorImpl::IsInGameMode()
{
	if (m_pGameEngine)
		return m_pGameEngine->IsInGameMode();
	return false;
}

void CEditorImpl::SetInGameMode(bool inGame)
{	
	static bool bWasInSimulationMode(false);

	if (inGame)
	{
		bWasInSimulationMode = GetIEditorImpl()->GetGameEngine()->GetSimulationMode();
		GetIEditorImpl()->GetGameEngine()->SetSimulationMode(false);
		#if 0
		GetIEditorImpl()->GetCommandManager()->Execute("game.enter");
		#endif
	}
	else
	{
		#if 0
		GetIEditorImpl()->GetCommandManager()->Execute("game.exit");
		#endif
		GetIEditorImpl()->GetGameEngine()->SetSimulationMode(bWasInSimulationMode);
	}
}

bool CEditorImpl::Init()
{
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CEditorImpl");
	if (!InitGUI())
	{
		return false;
	}
	gEnv->pInput->AddEventListener(m_GuiManager);
	EditorCommon::Initialize();

    GetIEditor()->RegisterNotifyListener(&m_MainWindow);

	return true;
}

void CEditorImpl::ExecuteCommand(const char* sCommand, ...)
{
	char	buffer[1024];
	va_list args;
	va_start(args, sCommand);
	vsprintf(buffer, sCommand, args);
	va_end(args);
	//m_pCommandManager->Execute(buffer);
	GetISystem()->GetIConsole()->ExecuteString(buffer);
}
