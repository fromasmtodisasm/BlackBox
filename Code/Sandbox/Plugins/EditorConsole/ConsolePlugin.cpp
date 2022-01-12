#if o
#include <IEditorClassFactory.h>

#include "ConsolePlugin.h"
#include "ConsoleWindow.h"

#include "ViewPane.h"

#include <BlackBox/Core/Platform/platform_impl.inl>

//single instance
CConsolePlugin* CConsolePlugin::s_pInstance;

using CConsoleWindow = gui::Console;

CConsoleWindow* g_console;

REGISTER_VIEWPANE_FACTORY_AND_MENU(CConsoleWindow, "Console", "Tools", false, "Advanced")

REGISTER_PLUGIN(CConsolePlugin);

//create plugin instance
CConsolePlugin::CConsolePlugin() : CEngineListener(GetIEditor() ? GetIEditor()->GetSystem() : NULL)
{
	s_pInstance = this;
	if (GetIEditor())
	{
		Init(CONSOLE_MAX_HISTORY);
	}
	static CConsoleWindow console;
	g_console = &console;
    GetIEditor()->RegisterNotifyListener(g_console);

}

//release plugin instance
CConsolePlugin::~CConsolePlugin()
{
    GetIEditor()->UnregisterNotifyListener(g_console);

	s_pInstance = NULL;
}

void CConsolePlugin::OnEditorNotifyEvent(EEditorNotifyEvent event)
{
	switch (event)
	{
	case eNotify_OnInit:
		break;
	case eNotify_OnQuit:
		break;
	case eNotify_OnIdleUpdate:
		static bool p_open;
		g_console->Draw("Console", &p_open);
		break;
	default:
		break;
	}
}

#endif
