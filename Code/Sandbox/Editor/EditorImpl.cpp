#include "EditorImpl.h"
#include "GameEngine.hpp"

#include <BlackBox\Core\IGame.hpp>

CEditorImpl::CEditorImpl(CGameEngine* ge)
{
	m_pGame = ge->m_pGame;
	gEnv->pRenderer->RegisterCallbackClient(this);
}

CEditorImpl::~CEditorImpl()
{
	SAFE_RELEASE(m_GuiManager);
}

void CEditorImpl::Draw()
{
	//m_MainMenu.Draw();
	m_MainWindow.Draw();
}

bool CEditorImpl::Update()
{
	gEnv->pSystem->GetIInput()->Update(true);
	m_GuiManager->NewFrame();
	m_GuiManager->ShowDemoWindow();
	gEnv->pSystem->Update(ESYSUPDATE_EDITOR);

	return m_pGame->Update();
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

bool CEditorImpl::Init()
{
    if (!InitGUI())
	{
		return false;	
	}
    gEnv->pInput->AddEventListener(m_GuiManager);
	return true;
}
