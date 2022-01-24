
#pragma once

#include "IEditor.h"
#include "SandboxAPI.h"
#include <BlackBox/Core/MathHelper.hpp>
#include "Version.h"
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
	virtual bool	 Init() override;
	void			 ExecuteCommand(const char* sCommand, ...);
	virtual void	 Draw() override;

	bool			 SaveDocumentBackup();
	virtual ISystem* GetSystem() override;

	CPluginManager* GetPluginManager() { return m_pPluginManager; }
	CGameEngine*	GetGameEngine() { return m_pGameEngine; }

	bool	Update();
	Version GetFileVersion() { return m_fileVersion; }
	void InitFinished();

	void Notify(EEditorNotifyEvent event);
	void RegisterNotifyListener(IEditorNotifyListener* listener);
	void UnregisterNotifyListener(IEditorNotifyListener* listener);

	// Inherited via ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// Inherited via IRenderCallback
	virtual void CallBack(Type type) override;
	virtual void OnRenderer_BeforeEndFrame() override;

	virtual bool IsInGameMode() override;
	virtual void SetInGameMode(bool inGame) override;

	bool InitGUI()
	{
		#if 0
		m_GuiManager = CreateGUI(gEnv->pSystem);
		m_GuiManager->Init();
		#endif
		return true;
	}

protected:
	void DetectVersion();

private:
	//! List of all notify listeners.
	std::list<IEditorNotifyListener*> m_listeners;

	MainMenu			 m_MainMenu;
	MainWindow			 m_MainWindow;
	IGame*				 m_pGame = nullptr;
	CGameEngine*		 m_pGameEngine = nullptr;
	IImGuiManager*		 m_GuiManager  = nullptr;
	CPluginManager*		 m_pPluginManager;
	IEditorClassFactory* m_pClassFactory;
	Version				 m_fileVersion;
	Version				 m_productVersion;

	bool m_bQuit	= false;
	bool m_bExiting = false;


	// Inherited via IEditor
	virtual IEditorClassFactory* GetClassFactory() override;
};

CEditorImpl* GetIEditorImpl();
