#pragma once

struct IGame;
struct IInitializeUIInfo;

class CGameEngine
{
public:
	CGameEngine();
	~CGameEngine();

	//! Initialize System.
	//! @return true if initialization succeeded, false otherwise
	bool Init(
		bool			   bTestMode,
		bool			   bShaderCacheGen,
		const char*		   sCmdLine,
		IInitializeUIInfo* logo);
	//! Configures editor related engine components
	void InitAdditionalEngineComponents();

	//! Load new terrain level into 3d engine.
	//! Also load AI triangulation for this level.
	bool LoadLevel(
		const string& levelPath,
		const string& mission,
		bool		  bDeleteAIGraph,
		bool		  bReleaseResources);

	//!* Reload level if it was already loaded.
	bool ReloadLevel();
	//! Switch In/Out of AI and Physics simulation mode.
	//! @param enabled When true editor switch to simulation mode.
	void SetSimulationMode(bool enabled, bool bOnlyPhysics = false);
	//! Get current simulation mode.
	bool GetSimulationMode() const { return m_bSimulationMode; }
	//! Returns true if level is loaded.
	bool IsLevelLoaded() const { return m_bLevelLoaded; }
	//! Assign new level path name.
	void SetLevelPath(const string& path);
	//! Assign new current mission name.
	void SetMissionName(const string& mission);
	//! Return name of currently loaded level.
	const string& GetLevelName() const { return m_levelName; }
	//! Return name of currently active mission.
	const string& GetMissionName() const { return m_missionName; }
	//! Get fully specified level path.
	const string& GetLevelPath() const { return m_levelPath; }
	//! Query if engine is in game mode.
	bool IsInGameMode() const { return m_bInGameMode; }
	//! Force level loaded variable to true.
	void SetLevelLoaded(bool bLoaded) { m_bLevelLoaded = bLoaded; }
	//! Force level just created variable to true.
	void SetLevelCreated(bool bJustCreated) { m_bJustCreated = bJustCreated; }

	//! Called every frame.
	void Update();

public:
	IGame* m_pGame{nullptr};

	string						  m_levelName;
	string						  m_missionName;
	string						  m_levelPath;
	string						  m_MOD;
	bool						  m_bLevelLoaded;
	bool						  m_bInGameMode;
	bool						  m_bGameModeSuspended;
	bool						  m_bSimulationMode;
	bool						  m_bSimulationModeAI;
	bool						  m_bSyncPlayerPosition;
	bool						  m_bUpdateFlowSystem;
	bool						  m_bJustCreated;
	bool						  m_bIgnoreUpdates;
	int64						  m_lastViewportRenderTime;
	ISystem*					  m_pISystem;
	#if 0
	CNavigation*				  m_pNavigation;
	#endif
	Matrix34					  m_playerViewTM;
	struct SSystemUserCallback*	  m_pSystemUserCallback;
	#if 0
	IEditorGame*				  m_pEditorGame;
	#endif
	class CGameToEditorInterface* m_pGameToEditorInterface;
	enum EPendingGameMode
	{
		ePGM_NotPending,
		ePGM_SwitchToInGame,
		ePGM_SwitchToInEditor,
	};
	EPendingGameMode				   m_ePendingGameMode;
	#if 0
	CListenerSet<IGameEngineListener*> m_listeners;
	#endif

	//! Keeps the editor active even if no focus is set
	int m_keepEditorActive;
};
