
#pragma once

#include "IEditor.h"
#include "SandboxAPI.h"
//#include "Version.h"
#include "MainMenu.hpp"
#include "MainWindow.hpp"

#include "ImGuiManager.hpp"

struct SInputEvent;

enum EEditorPathName
{
	EDITOR_PATH_OBJECTS,
	EDITOR_PATH_TEXTURES,
	EDITOR_PATH_SOUNDS,
	EDITOR_PATH_MATERIALS,
	EDITOR_PATH_UI_ICONS,
	EDITOR_PATH_LAST
};

namespace BackgroundScheduleManager
{
	class CScheduleManager;
}

namespace BackgroundTaskManager
{
	class CTaskManager;
	class CBackgroundTasksListener;
} // namespace BackgroundTaskManager

class SANDBOX_API CEditorImpl : public IEditor
	, public ISystemEventListener
	, public IRendererCallbackClient
	, public IRenderCallback
{
  public:
	CEditorImpl(CGameEngine* ge);
	~CEditorImpl();

	// Inherited via IEditor
	virtual bool Init() override;
	virtual void Draw() override;

	bool Update();

	// Inherited via ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// Inherited via IRenderCallback
	virtual void CallBack(Type type) override;
	virtual void OnRenderer_BeforeEndFrame() override;

	bool InitGUI()
	{
		m_GuiManager = CreateGUI(gEnv->pSystem);
		m_GuiManager->Init();
		return true;
	}

private:
	MainMenu   m_MainMenu;
	MainWindow m_MainWindow;
	IGame*	   m_pGame = nullptr;
	bool	   m_bQuit = false;
	IImGuiManager* m_GuiManager = nullptr;
};
