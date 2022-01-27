
class CTestRenderer : public IGame
{
public:
	// Inherited via IGame
	virtual bool Init(ISystem* pSystem, bool bDedicatedSrv, bool bInEditor, const char* szGameMod) override
	{
		return false;
	}
	virtual bool Update() override
	{
		return true;
	}
	virtual bool Run(bool& bRelaunch) override
	{
		while (m_IsRunning)
		{
			Update();
		}
		return false;
	}
	virtual const char* IsMODLoaded() override
	{
		return nullptr;
	}
	virtual IGameMods* GetModsInterface() override
	{
		return nullptr;
	}
	virtual void Release() override
	{
	}
	virtual void ExecuteScheduledEvents() override
	{
	}
	virtual bool UseFixedStep() override
	{
		return false;
	}
	virtual int SnapTime(float fTime, float fOffset = 0.5f) override
	{
		return 0;
	}
	virtual int SnapTime(int iTime, float fOffset = 0.5f) override
	{
		return 0;
	}
	virtual int GetiFixedStep() override
	{
		return 0;
	}
	virtual float GetFixedStep() override
	{
		return 0.0f;
	}
	virtual bool LoadLevelForEditor(const char* pszLevelDirectory, const char* pszMissionName = 0) override
	{
		return false;
	}
	virtual IEntityClassRegistry* GetClassRegistry() override
	{
		return nullptr;
	}
	virtual void OnSetVar(ICVar* pVar) override
	{
	}
	virtual void ResetState() override
	{
	}
	virtual void GetMemoryStatistics(ICrySizer* pSizer) override
	{
	}
	virtual void SaveConfiguration(const char* sSystemCfg, const char* sGameCfg, const char* sProfile) override
	{
	}
	virtual void ReloadScripts() override
	{
	}
	virtual bool GetModuleState(EGameCapability eCap) override
	{
		return false;
	}
	virtual IPhysicsStreamer* GetPhysicsStreamer() override
	{
		return nullptr;
	}
	virtual IPhysicsEventClient* GetPhysicsEventClient() override
	{
		return nullptr;
	}
	virtual void UpdateDuringLoading() override
	{
	}
	virtual IXAreaMgr* GetAreaManager() override
	{
		return nullptr;
	}
	virtual ITagPointManager* GetTagPointManager() override
	{
		return nullptr;
	}
	virtual void SendMessageA(const char* s)
	{
	}

	bool m_IsRunning = true;
};

#if 0
IGAME_API IGame* CreateGameInstance()
{
	CTestRenderer* game = new CTestRenderer();
	return (game);
}
#endif