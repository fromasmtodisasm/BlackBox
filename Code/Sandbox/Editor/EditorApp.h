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

	void InitPlugins();

	void Run();

	bool IdleProcessing(bool bBackground);

	std::unique_ptr<CGameEngine> InitGameSystem();

  private:
	CEditorImpl* m_pEditor{nullptr};
	bool		 m_bExiting{false};
	bool		 m_bTestMode{false};
};