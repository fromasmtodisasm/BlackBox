
#pragma once

#include "SandboxAPI.h"
#include "IEditor.h"
//#include "Version.h"

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
}

class SANDBOX_API CEditorImpl : public IEditor, public ISystemEventListener
{
  public:
	CEditorImpl(CGameEngine* ge);
	~CEditorImpl();
};
