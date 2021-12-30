#pragma once

class CGameEngine;

struct ISystem;
struct IEditorClassFactory;
struct IEditorNotifyListener;

struct IGuiElement
{
	virtual void Draw() = 0;
};

// Global editor notify events.
enum EEditorNotifyEvent
{
	// Global events.
	eNotify_OnInit = 10,  // Sent after editor fully initialized.
	eNotify_OnQuit,		  // Sent before editor quits.
	eNotify_OnIdleUpdate, // Sent every frame while editor is idle.
};
//////////////////////////////////////////////////////////////////////////

void CryFatalError(const char*, ...);

struct IEditor
{
	virtual bool	 Init()		  = 0;
	virtual void	 Draw()		  = 0;
	virtual ISystem* GetSystem() = 0;

	virtual IEditorClassFactory* GetClassFactory() = 0;

	virtual void                     Notify(EEditorNotifyEvent event) = 0;
	//! Register Editor notifications listener.
	virtual void                     RegisterNotifyListener(IEditorNotifyListener* listener) = 0;
	//! Unregister Editor notifications listener.
	virtual void                     UnregisterNotifyListener(IEditorNotifyListener* listener) = 0;

};

IEditor* GetIEditor();

//! Derive from this class if you want to register for getting global editor notifications.
//! Use IAutoEditorNotifyListener for convenience
struct IEditorNotifyListener
{
	bool m_bIsRegistered;

	IEditorNotifyListener() : m_bIsRegistered(false) {}
	virtual ~IEditorNotifyListener()
	{
		if (m_bIsRegistered)
			CryFatalError("Destroying registered IEditorNotifyListener");
	}

	//! called by the editor to notify the listener about the specified event.
	virtual void OnEditorNotifyEvent(EEditorNotifyEvent event) = 0;
};

#pragma warning(disable: 4251)