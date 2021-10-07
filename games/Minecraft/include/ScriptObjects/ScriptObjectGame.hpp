#pragma once

#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>

class CXGame;


class CScriptObjectGame :
	public _ScriptableEx<CScriptObjectGame>
{
public:
	CScriptObjectGame();
	~CScriptObjectGame();
	void InitializeTemplate(IScriptSystem* pSS);

	void Init(IScriptSystem* pScriptSystem, CXGame* pGame);
	void OnNETServerFound(CIPAddress& ip, SXServerInfos& pServerInfo);
	void OnNETServerTimeout(CIPAddress& ip);

  public:
	int GetUserName(IFunctionHandler* pH);

	int SendMessage(IFunctionHandler* pH);

	int gotoMenu(IFunctionHandler *pH);
	int gotoGame(IFunctionHandler *pH);

	///////////////////////////////////////////////////////////////	
	int CreateVariable(IFunctionHandler* pH);//str
	int SetVariable(IFunctionHandler* pH);
	int RemoveVariable(IFunctionHandler* pH);//str
	int GetVariable(IFunctionHandler* pH);
	///////////////////////////////////////////////////////////////	

	/////////////////////////////////////////////////////////////
	int Connect(IFunctionHandler* pH);
	int Reconnect(IFunctionHandler* pH);
	int Disconnect(IFunctionHandler* pH);
	int GetLevelList(IFunctionHandler* pH);
	int LoadLevel(IFunctionHandler* pH);
	int LoadLevelListen(IFunctionHandler* pH);
	int LoadLevelMPServer(IFunctionHandler* pH);

	int GetVersion(IFunctionHandler* pH);
	int GetVersionString(IFunctionHandler* pH);

	int ReloadScripts(IFunctionHandler* pH);
	int Load(IFunctionHandler* pH);
	int Save(IFunctionHandler* pH);
	int LoadLatestCheckPoint(IFunctionHandler* pH);
	int ShowSaveGameMenu(IFunctionHandler* pH);
	int Quit(IFunctionHandler* pH);
	int IsPointInWater(IFunctionHandler* pH);
	int GetWaterHeight(IFunctionHandler* pH);
	int GetTagPoint(IFunctionHandler* pH);
	/////////////////////////////////////////////////////////////
	int IsServer(IFunctionHandler* pH);
	int IsClient(IFunctionHandler* pH);
	int IsMultiplayer(IFunctionHandler* pH);
	int GetMaterialIDByName(IFunctionHandler* pH);
	int ReloadMaterialPhysics(IFunctionHandler* pH);
	int StartRecord(IFunctionHandler* pH);
	int StopRecord(IFunctionHandler* pH);
	/////////////////////////////////////////////////////////////

	int GetSaveGameList(IFunctionHandler* pH);
	int ToggleMenu(IFunctionHandler* pH);
	int ShowMenu(IFunctionHandler* pH);
	int HideMenu(IFunctionHandler* pH);
	int IsInMenu(IFunctionHandler* pH);

	int SaveConfiguration(IFunctionHandler* pH);
	int LoadConfiguration(IFunctionHandler* pH);
	int LoadConfigurationEx(IFunctionHandler* pH);
	int RemoveConfiguration(IFunctionHandler* pH);
	int DrawHealthBar(IFunctionHandler* pH);
	int LoadScript(IFunctionHandler* pH);
	int GetLevelName(IFunctionHandler* pH);
	int AddCommand(IFunctionHandler* pH);

	int GetModsList(IFunctionHandler* pH);
	int LoadMOD(IFunctionHandler* pH);
	int GetCurrentModName(IFunctionHandler* pH);

	/////////////////////////////////////////////////////////////

	int SavePlayerPos(IFunctionHandler* pH);
	int LoadPlayerPos(IFunctionHandler* pH);
	/////////////////////////////////////////////////////////////
	int LoadObject(IFunctionHandler* pH);

private:
	CXGame *							m_pGame;
	ISystem *						m_pSystem;
	IConsole *					m_pConsole;
	CScriptObjectVector	m_pGetTagPoint;

	bool _GetProfileFileNames( IFunctionHandler *pH, string &outSystem, string &outGame, const char *insCallerName );
};