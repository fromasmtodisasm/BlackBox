#pragma once
#include "SandboxAPI.h"

class CCryEditDoc;
class CEditCommandLineInfo;
class CEditorImpl;
class CGameEngine;
class CMannequinChangeMonitor;

class SANDBOX_API CEditApp
{
  public:
	enum ECreateLevelResult
	{
		ECLR_OK = 0,
		ECLR_DIR_CREATION_FAILED
	};

	CEditApp();
	~CEditApp();

	static CEditApp* GetInstance();

public:
	bool InitInstance();
	bool PostInit();
	int	 ExitInstance();

	void Run();

private:
	CEditorImpl* m_pEditor{nullptr};
};