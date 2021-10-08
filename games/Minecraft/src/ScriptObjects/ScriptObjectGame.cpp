#include <ScriptObjects/ScriptObjectGame.hpp>
#include <Server/XServer.hpp>

#include <ICryPak.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_DECLARE_SCRIPTABLEEX(CScriptObjectGame)

CScriptObjectGame::CScriptObjectGame()
{
}

CScriptObjectGame::~CScriptObjectGame()
{
}

void CScriptObjectGame::InitializeTemplate(IScriptSystem* pSS)
{
	_ScriptableEx<CScriptObjectGame>::InitializeTemplate(pSS);
#undef SCRIPT_REG_CLASSNAME
#define SCRIPT_REG_CLASSNAME CScriptObjectGame
	SCRIPT_REG_TEMPLFUNC_U(GetUserName);
	SCRIPT_REG_TEMPLFUNC_U(SendMessage);
	SCRIPT_REG_TEMPLFUNC_U(gotoMenu);
	SCRIPT_REG_TEMPLFUNC_U(gotoGame);

	SCRIPT_REG_TEMPLFUNC_U(GetTagPoint);
	SCRIPT_REG_TEMPLFUNC_U(CreateVariable);
	SCRIPT_REG_TEMPLFUNC_U(SetVariable);
	SCRIPT_REG_TEMPLFUNC_U(RemoveVariable);
	SCRIPT_REG_TEMPLFUNC_U(GetVariable);
	SCRIPT_REG_TEMPLFUNC_U(LoadLevel);

	SCRIPT_REG_TEMPLFUNC_U(AddCommand);

	SCRIPT_REG_TEMPLFUNC_U(Quit);

	SCRIPT_REG_TEMPLFUNC_U(SavePlayerPos);
	SCRIPT_REG_TEMPLFUNC_U(LoadPlayerPos);

	SCRIPT_REG_TEMPLFUNC_U(SaveConfiguration);

	SCRIPT_REG_TEMPLFUNC_U(Save);
	SCRIPT_REG_TEMPLFUNC_U(Load);

	SCRIPT_REG_TEMPLFUNC_U(LoadObject);

	SCRIPT_REG_TEMPLFUNC_U(Connect);
	SCRIPT_REG_TEMPLFUNC_U(Reconnect);
	SCRIPT_REG_TEMPLFUNC_U(Disconnect);
	SCRIPT_REG_TEMPLFUNC_U(GetLevelList);
	SCRIPT_REG_TEMPLFUNC_U(LoadLevel);

#if 0
	AllowPropertiesMapping(pSS);
	RegisterProperty( "test_string",PROPERTY_TYPE_STRING,offsetof(CXGame,test_string));
#endif
}

void CScriptObjectGame::Init(IScriptSystem* pScriptSystem, CXGame* pGame)
{
	m_pGame	   = pGame;
	m_pSystem  = pGame->GetSystem();
	m_pConsole = m_pSystem->GetIConsole();
	InitGlobal(pScriptSystem, "Game", this);

	// entity classes
	m_pScriptSystem->SetGlobalValue("SPECTATOR_CLASS_ID", SPECTATOR_CLASS_ID);
	m_pScriptSystem->SetGlobalValue("ADVCAMSYSTEM_CLASS_ID", ADVCAMSYSTEM_CLASS_ID);
	m_pScriptSystem->SetGlobalValue("PLAYER_CLASS_ID", PLAYER_CLASS_ID);
	m_pScriptSystem->SetGlobalValue("SYNCHED2DTABLE_CLASS_ID", SYNCHED2DTABLE_CLASS_ID);

	if (!EnablePropertiesMapping(m_pGame))
	{
		CryError("<CryGame> (CScriptObjectGame::Init) failed");
		return;
	}
}

void CScriptObjectGame::Reset()
{
	#if 0
	if (m_vRenderersObjs.size())
	{
		while (!m_vRenderersObjs.empty())
		{
			CScriptObjectRenderer* p = m_vRenderersObjs.back();
			m_vRenderersObjs.pop_back();
			delete p;
		}
	}
	#endif
}


void CScriptObjectGame::OnNETServerFound(CIPAddress& ip, SXServerInfos& pServerInfo)
{
	SmartScriptObject pServer(m_pScriptSystem);

	pServer->SetValue("Name", pServerInfo.strName.c_str());
	pServer->SetValue("Map", pServerInfo.strMap.c_str());
	pServer->SetValue("Players", (int)pServerInfo.nPlayers);
	pServer->SetValue("MaxPlayers", (int)pServerInfo.nMaxPlayers);
	pServer->SetValue("GameType", pServerInfo.strGameType.c_str());
	pServer->SetValue("Mod", pServerInfo.strMod.c_str());
	pServer->SetValue("Ping", (int)pServerInfo.nPing);
	pServer->SetValue("IP", pServerInfo.IP.GetAsString(true));
	pServer->SetValue("Password", (int)((pServerInfo.nServerFlags & SXServerInfos::FLAG_PASSWORD) ? 1 : 0));
	pServer->SetValue("CheatsEnabled", (int)((pServerInfo.nServerFlags & SXServerInfos::FLAG_CHEATS) ? 1 : 0));
	char str[80];
	pServerInfo.VersionInfo.ToString(str);
	pServer->SetValue("GameVersion", str);
	pServer->SetValue("InternetServer", (int)((pServerInfo.nServerFlags & SXServerInfos::FLAG_NET) ? 1 : 0));
	pServer->SetValue("ComputerType", (int)pServerInfo.nComputerType);
	pServer->SetValue("PunkBuster", (int)((pServerInfo.nServerFlags & SXServerInfos::FLAG_PUNKBUSTER) ? 1 : 0));

	HSCRIPTFUNCTION pfOnNETServerFound = 0;

	if (m_pScriptThis->GetValue("OnNETServerFound", pfOnNETServerFound))
	{
		m_pScriptSystem->BeginCall(pfOnNETServerFound);
		m_pScriptSystem->PushFuncParam(GetScriptObject());
		m_pScriptSystem->PushFuncParam(pServer);
		m_pScriptSystem->EndCall();
	}
	m_pScriptSystem->ReleaseFunc(pfOnNETServerFound);
}

void CScriptObjectGame::OnNETServerTimeout(CIPAddress& ip)
{
	SmartScriptObject pServer(m_pScriptSystem);

	pServer->SetValue("Name", "");
	pServer->SetValue("Map", "");
	pServer->SetValue("Players", 0);
	pServer->SetValue("MaxPlayers", 0);
	pServer->SetValue("GameType", "");
	pServer->SetValue("Mod", "");
	pServer->SetValue("Ping", 0);
	pServer->SetValue("IP", ip.GetAsString(true));
	pServer->SetValue("Password", 0);
	pServer->SetValue("CheatsEnabled", 0);
	pServer->SetValue("GameVersion", "");
	pServer->SetValue("InternetServer", 0);
	pServer->SetValue("ComputerType", 0);
	pServer->SetValue("PunkBuster", 0);

	HSCRIPTFUNCTION pfOnNETServerTimeout = 0;

	if (m_pScriptThis->GetValue("OnNETServerTimeout", pfOnNETServerTimeout))
	{
		m_pScriptSystem->BeginCall(pfOnNETServerTimeout);
		m_pScriptSystem->PushFuncParam(GetScriptObject());
		m_pScriptSystem->PushFuncParam(pServer);
		m_pScriptSystem->EndCall();
	}
	m_pScriptSystem->ReleaseFunc(pfOnNETServerTimeout);
}

int CScriptObjectGame::GetUserName(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(0);

	return pH->EndFunction(m_pSystem->GetUserName());
}

int CScriptObjectGame::SendMessage(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(1);
	const char* pszMsg;
	pH->GetParam(1, pszMsg);

	if (pszMsg)
		m_pGame->SendMessage(pszMsg);
	else
		m_pScriptSystem->RaiseError("SendMessage() parameter is nil");

	return pH->EndFunction();
}

int CScriptObjectGame::gotoMenu(IFunctionHandler* pH)
{
	m_pGame->GotoMenu();
	return pH->EndFunction();
}

int CScriptObjectGame::gotoGame(IFunctionHandler* pH)
{
	m_pGame->GotoGame();
	return pH->EndFunction();
}

int CScriptObjectGame::GetVersion(IFunctionHandler* pH)
{
	return 0;
}

int CScriptObjectGame::GetVersionString(IFunctionHandler* pH)
{
	return 0;
}

int CScriptObjectGame::ReloadScripts(IFunctionHandler* pH)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
/*!load the game from a file
	@param sFileName the name of the target file[optional] the default "is farcry_save.sav"
*/
int CScriptObjectGame::Load(IFunctionHandler* pH)
{
	const char* sFileName = "";
	if (pH->GetParamCount()) pH->GetParam(1, sFileName);
	m_pGame->Load(sFileName);
	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*!save the game on a file
	@param sFileName the name of the target file[optional] the default "is farcry_save.sav"
*/
int CScriptObjectGame::Save(IFunctionHandler* pH)
{
	const char* sFileName = "";
	if (pH->GetParamCount()) pH->GetParam(1, sFileName);
	m_pGame->Save(sFileName, NULL, NULL);
	return pH->EndFunction();
}

int CScriptObjectGame::LoadLatestCheckPoint(IFunctionHandler* pH)
{
	return 0;
}

int CScriptObjectGame::ShowSaveGameMenu(IFunctionHandler* pH)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
/*!quit the game
*/
int CScriptObjectGame::Quit(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(0);
	//m_pConsole->Exit(NULL);
	m_pSystem->Quit();
	return pH->EndFunction();
}

int CScriptObjectGame::IsPointInWater(IFunctionHandler* pH)
{
	return 0;
}

int CScriptObjectGame::GetWaterHeight(IFunctionHandler* pH)
{
	return 0;
}

int CScriptObjectGame::GetTagPoint(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(1);

	const char* sTPName;
	ITagPoint*	pTP = NULL;
	Legacy::Vec3		vec(0, 0, 0);

	if (pH->GetParam(1, sTPName))
	{
		pTP = m_pGame->GetTagPoint(sTPName);
		if (!pTP) return pH->EndFunctionNull();

		pTP->GetPos(vec);
	}
	m_pGetTagPoint.Set(vec);
	return pH->EndFunction(m_pGetTagPoint);
}
int CScriptObjectGame::IsServer(IFunctionHandler* pH)
{
	return 0;
}
int CScriptObjectGame::IsClient(IFunctionHandler* pH)
{
	return 0;
}
int CScriptObjectGame::IsMultiplayer(IFunctionHandler* pH)
{
	return 0;
}
int CScriptObjectGame::GetMaterialIDByName(IFunctionHandler* pH)
{
	return 0;
}
int CScriptObjectGame::ReloadMaterialPhysics(IFunctionHandler* pH)
{
	return 0;
}
int CScriptObjectGame::StartRecord(IFunctionHandler* pH)
{
	return 0;
}
int CScriptObjectGame::StopRecord(IFunctionHandler* pH)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
bool CScriptObjectGame::_GetProfileFileNames(IFunctionHandler* pH, string& outSystem, string& outGame, const char* insCallerName)
{
	outSystem = "system.cfg";
	outGame	  = "game.cfg";

	if (pH->GetParamCount() > 0) // use given profile name or don't use profiles
	{
		CHECK_PARAMETERS(1);

		const char* sProfileName;
		pH->GetParam(1, sProfileName);

		if (!sProfileName || sProfileName == "")
		{
			m_pScriptSystem->RaiseError("%s profilename is nil or empty", insCallerName);
			return false;
		}

		string sName;

		outSystem = string("Profiles/Player/") + sProfileName + "_" + outSystem;
		outGame	  = string("Profiles/Player/") + sProfileName + "_" + outGame;
	}
	return true;
}

int CScriptObjectGame::SaveConfiguration(IFunctionHandler* pH)
{
	string sSystemCfg, sGameCfg;

	if (!_GetProfileFileNames(pH, sSystemCfg, sGameCfg, __FUNCTION__))
		return pH->EndFunction();

	if (m_pGame->m_bDedicatedServer)
		return pH->EndFunction();

	if (m_pGame->m_bEditor)
		return pH->EndFunction();

	const char* szProfileName = 0;

	if (pH->GetParamCount() > 0)
	{
		pH->GetParam(1, szProfileName);
	}

	// profile is already specified in the string
	m_pGame->SaveConfiguration(sSystemCfg.c_str(), sGameCfg.c_str(), NULL);

	if (szProfileName)
	{
		string path = "profiles/player/";
		path += szProfileName;
#if defined(LINUX)
		mkdir(path.c_str(), 0xFFFF);
#else
		if (auto result = _mkdir(path.c_str()); result != 0)
		{
			if (result == EEXIST)
			{
				m_pSystem->GetILog()->LogWarning("Path already exist");
			}
			else if (ENOENT)
			{
				m_pSystem->GetILog()->LogWarning("Path was not found");
			}
		}
#endif
		path += "savegames/";
#if defined(LINUX)
		mkdir(path.c_str(), 0xFFFF);
#else
		_mkdir(path.c_str());
#endif
	}

	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
int CScriptObjectGame::LoadConfiguration(IFunctionHandler* pH)
{
	string sSystemCfg, sGameCfg;

	if (!_GetProfileFileNames(pH, sSystemCfg, sGameCfg, __FUNCTION__))
		return pH->EndFunction();

	//m_pScriptSystem->ExecuteFile(sSystemCfg.c_str(),true,true);
	//m_pScriptSystem->ExecuteFile(sGameCfg.c_str(),true,true);

	m_pGame->LoadConfiguration(sSystemCfg, sGameCfg);

	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*!create a console variable
  @param sName name of the console variable
  @param sDefault the default value[optional]
*/
int CScriptObjectGame::CreateVariable(IFunctionHandler* pH)
{
	int			nPCount = pH->GetParamCount();
	const char* sName;
	const char* sDefault;
	const char* sflags;
	int			iflags = VF_SAVEGAME;
	pH->GetParam(1, sName);
	if (nPCount > 1)
	{
		pH->GetParam(2, sDefault);
		if (nPCount > 2)
		{
			switch (pH->GetParamType(3))
			{
			case ScriptVarType::String:
				if (pH->GetParam(3, sflags))
				{
					if (strcmp(sflags, "NetSynch") == 0)
						iflags |= VF_NET_SYNCED;
				}
				else
					m_pSystem->GetILog()->LogWarning("Game:CreateVariable can't get the 3rd parameter (string)");
				break;
			case ScriptVarType::Number:
				if (pH->GetParam(3, iflags))
				{
					// do nothing, the flags must be the VF_* flags
				}
				else
					m_pSystem->GetILog()->LogWarning("Game:CreateVariable can't get the 3rd parameter (number)");
				break;
			default:
				m_pSystem->GetILog()->LogWarning("Game:CreateVariable unexpected 3rd (flags) parameter type (%s)", ScriptVarTypeAsCStr(pH->GetParamType(3)));
				break;
			}
		}
	}
	else
	{
		sDefault = "0";
	}

	m_pConsole->CreateVariable(sName, sDefault, iflags);
	return pH->EndFunction();
}

int CScriptObjectGame::SetVariable(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(2);
	const char* sName;
	if (!pH->GetParam(1, sName))
	{
		m_pScriptSystem->RaiseError("SetVariable invalid parameter type %s, string expected", ScriptVarTypeAsCStr(pH->GetParamType(1)));
		return pH->EndFunctionNull();
	}

	ICVar* pVar = m_pSystem->GetIConsole()->GetCVar(sName);
	if (!pVar)
	{
		m_pScriptSystem->RaiseError("SetVariable invalid variable name \"%s\": no such variable found", sName);
		return pH->EndFunctionNull();
	}

	if ((pVar->GetFlags() & VF_CHEAT) && (!m_pGame->IsDevModeEnable()))
	{
		// [martin] hid this message because we don't want to help hacking
		// m_pSystem->GetILog()->LogWarning ("\001 Variable %s is cheat protected.", sName);
		return pH->EndFunctionNull();
	}

	switch (pH->GetParamType(2))
	{
	case ScriptVarType::Null:
		pVar->Set("");
		break;
	case ScriptVarType::String:
	{
		const char* pVal;
		if (!pH->GetParam(2, pVal))
		{
			m_pScriptSystem->RaiseError("SetVariable cannot retrieve the variable %s string value", sName);
			return pH->EndFunctionNull();
		}
		pVar->Set(pVal);
	}
	break;
	case ScriptVarType::Number:
	{
		int	  nVal;
		float fVal;
		if (pH->GetParam(2, fVal))
			pVar->Set(fVal);
		else if (pH->GetParam(2, nVal))
			pVar->Set(nVal);
		else
		{
			m_pScriptSystem->RaiseError("SetVariable cannot retrieve the variable %s numeric value", sName);
			return pH->EndFunctionNull();
		}
	}
	break;

	default:
		m_pScriptSystem->RaiseError("SetVariable cannot set the variable %s value: unsupported type %s", sName, ScriptVarTypeAsCStr(pH->GetParamType(2)));
		break;
	}
	return pH->EndFunction();
}

int CScriptObjectGame::RemoveVariable(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(1);
	const char* sName;
	pH->GetParam(1, sName);
	m_pConsole->UnregisterVariable(sName, true);
	return pH->EndFunction();
}

int CScriptObjectGame::GetVariable(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(1);

	const char* sName;
	if (!pH->GetParam(1, sName))
	{
		m_pScriptSystem->RaiseError("GetVariable invalid parameter type %s, string expected", ScriptVarTypeAsCStr(pH->GetParamType(1)));
		return pH->EndFunctionNull();
	}

	ICVar* pVar = m_pSystem->GetIConsole()->GetCVar(sName);
	if (!pVar)
	{
		m_pScriptSystem->RaiseError("GetVariable invalid variable name \"%s\": no such variable found", sName);
		return pH->EndFunctionNull();
	}

	switch (pVar->GetType())
	{
	case CVAR_INT:
		return pH->EndFunction(pVar->GetIVal());
	case CVAR_FLOAT:
		return pH->EndFunction(pVar->GetFVal());
	case CVAR_STRING:
		return pH->EndFunction(pVar->GetString());
	default:
		return pH->EndFunctionNull();
	}
}

int CScriptObjectGame::AddCommand(IFunctionHandler* pH)
{
	int			nPCount = pH->GetParamCount();
	const char* sName;
	const char* sCommand;
	const char* sHelp  = NULL;
	int			iflags = 0;
	pH->GetParam(1, sName);
	if (nPCount > 1)
	{
		pH->GetParam(2, sCommand);
		if (nPCount > 2)
		{
			if (!pH->GetParam(3, sHelp))
				sHelp = NULL;
		}
		if (sHelp)
			static_cast<IBaseConsole*>(m_pConsole)->AddCommand(sName, sCommand, VF_DUMPTODISK | VF_SAVEGAME, sHelp);
		else
			static_cast<IBaseConsole*>(m_pConsole)->AddCommand(sName, sCommand, VF_DUMPTODISK, "");
	}

	return pH->EndFunction();
}

int CScriptObjectGame::SavePlayerPos(IFunctionHandler* pH)
{
	const char* sName = NULL;
	const char* sDesc = NULL;
	pH->GetParam(1, sName);
	pH->GetParam(2, sDesc);

	if (sName)
		m_pGame->DevMode_SavePlayerPos(0, sName, sDesc);

	return pH->EndFunction();
}

int CScriptObjectGame::LoadPlayerPos(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(2);
	const char* sName;
	int			index;
	if (pH->GetParam(1, index))
	{
		if (pH->GetParam(2, sName))
		{
			m_pGame->DevMode_LoadPlayerPos(index, sName);
		}
	}
	return pH->EndFunction();
}

int CScriptObjectGame::LoadObject(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(1);
	const char* sName;
	IStatObj*	obj{};
	if (pH->GetParam(1, sName))
	{
		obj = gEnv->p3DEngine->MakeObject(sName);
	}
	return pH->EndFunction(obj);
}

//////////////////////////////////////////////////////////////////////
/*!create a local client and connect it to a server
	@param sServer string containing the server name or ip number
	@param bShowConsole (optional, default true)
*/
int CScriptObjectGame::Connect(IFunctionHandler* pH)
{
	bool bDoLateSwitch = false, bDoCDAuthorization = false;
	//if a local server exist shutdown it
	//m_pGame->ShutdownServer();

	const char* sServer = NULL;

	if (pH->GetParamCount() != 0)
		pH->GetParam(1, sServer);

	if (pH->GetParamCount() > 1)
		pH->GetParam(2, bDoLateSwitch);

	if (pH->GetParamCount() > 2)
		pH->GetParam(3, bDoCDAuthorization);

	m_pGame->ShutdownClient();
	m_pGame->ShutdownServer();

	if (sServer == NULL)
	{
		if (!m_pGame->m_pServer)
			return pH->EndFunction();

		sServer = "127.0.0.1";
	}
	bool bReturn = true;

	if (!m_pGame->m_bEditor)
	{
		HSCRIPTFUNCTION pfnOnConnectBegin = m_pScriptSystem->GetFunctionPtr("Game", "OnConnectBegin");

		if (pfnOnConnectBegin)
		{
			m_pScriptSystem->BeginCall(pfnOnConnectBegin);
			m_pScriptSystem->PushFuncParam(m_pGame->GetScriptObject());
			m_pScriptSystem->PushFuncParam(sServer);
			m_pScriptSystem->EndCall(bReturn);

			m_pScriptSystem->ReleaseFunc(pfnOnConnectBegin);
		}
	}

	if (!bReturn)
	{
		return pH->EndFunctionNull();
	}

	m_pGame->StartupClient();

	// if bDoLateSwitch is true then it does the 3 lines below but after the connection is completed.
	m_pGame->m_pClient->XConnect((char*)sServer, bDoLateSwitch, bDoCDAuthorization);

	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*!create a local client and reconnect to the last server
@param sServer string containing the server name or ip number
@param bShowConsole (optional, default true)
*/
int CScriptObjectGame::Reconnect(IFunctionHandler *pH)
{
	CHECK_PARAMETERS(0);

	if (!m_pGame->m_szLastAddress.empty())
	{
		m_pScriptSystem->BeginCall("Game", "Connect");
		m_pScriptSystem->PushFuncParam(this->GetScriptObject());
		m_pScriptSystem->PushFuncParam(m_pGame->m_szLastAddress.c_str());
		m_pScriptSystem->PushFuncParam((int)(m_pGame->m_bLastDoLateSwitch ? 1 : 0));
		m_pScriptSystem->PushFuncParam((int)(m_pGame->m_bLastCDAuthentication ? 1 : 0));
		m_pScriptSystem->EndCall();
	}
	else
	{
		m_pConsole->PrintLine("No previous connect command.");
	}

	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
/*!disconnect the current connection to a remote server
@param sCause string describing the cause of the disconnection[optional]
*/
int CScriptObjectGame::Disconnect(IFunctionHandler *pH)
{
	int iCount=pH->GetParamCount();

	if(iCount>1)
	{
		m_pScriptSystem->RaiseError("Game.Disconnect too many parameters");
		return pH->EndFunction();
	}

	const char *sCause=0;

	if(iCount>0)
		pH->GetParam(1,sCause);			// sCause might get 0 when LUA passed a nil value

	if(!sCause)
		sCause="@UserDisconnected";

	m_pGame->ShutdownClient();
	m_pGame->ShutdownServer();

	return pH->EndFunction();
}

//////////////////////////////////////////////////////////////////////
// Scans the given vector of strings for presence of szString
// case-insensitive
// returns true when the string is present in the array
bool HasStringI(const std::vector<string>& arrStrings, const char* szString)
{
	for (std::vector<string>::const_iterator it = arrStrings.begin(); it != arrStrings.end(); ++it)
		if (!stricmp(it->c_str(), szString))
			return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
// Returns the table - list of levels with the given mission in them
int CScriptObjectGame::GetLevelList(IFunctionHandler* pH)
{
	// the first and only parameter is the name of the mission
	// empty name means all levels will be returned
	const char* pszMissionFilter = NULL;

	if (pH->GetParamCount() >= 1 && !pH->GetParam(1, pszMissionFilter))
	{
		m_pScriptSystem->RaiseError("CScriptObjectGame::GetLevelList : 1st (%s) of %d arguments couldn't be resolved as mission name string.", ScriptVarTypeAsCStr(pH->GetParamType(1)), pH->GetParamCount());
		return pH->EndFunctionNull();
	}

	_SmartScriptObject pObj(m_pScriptSystem);
	int				   nLevel = 1;

	string sLevelsFolder = m_pGame->GetLevelsFolder();

	struct _finddata_t c_file;
	intptr_t		   hFile;

	ICryPak* pIPak = m_pSystem->GetIPak();

	if ((hFile = pIPak->FindFirst((sLevelsFolder + string("/*.*")).c_str(), &c_file)) == -1L)
		return (pH->EndFunction(*pObj));

	// warning: this should be empty here
	std::vector<string> arrMissions;

	do {
		arrMissions.clear();

		if ((strncmp(c_file.name, ".", 1) != 0) &&
			(c_file.attrib & _A_SUBDIR) && m_pGame->GetLevelMissions((sLevelsFolder + "/" + c_file.name).c_str(), arrMissions) &&
			(!pszMissionFilter || HasStringI(arrMissions, pszMissionFilter)))
		{
			_SmartScriptObject pLevelObj(m_pScriptSystem);

			pLevelObj->SetValue("Name", c_file.name);

			_SmartScriptObject pMissionObj(m_pScriptSystem);

			for (int i = 0; i < (int)arrMissions.size(); i++)
			{
				static char szIndex[4];
				pMissionObj->SetValue(itoa(i + 1, szIndex, 10), arrMissions[i].c_str());
			}

			pLevelObj->SetValue("MissionList", pMissionObj);

			// we found a level
			pObj->SetAt(nLevel, pLevelObj);

			++nLevel;
		}

	} while (pIPak->FindNext(hFile, &c_file) == 0);
	pIPak->FindClose(hFile);

	return pH->EndFunction(*pObj);
}

//////////////////////////////////////////////////////////////////////
/*!Load a level ,start a local client and connect it to the local server, no external connections (sp game)
	@param sMapName the name of the map to load
	@param sMissionName the name of the mission[optional]
*/
int CScriptObjectGame::LoadLevel(IFunctionHandler* pH)
{
	const char* szMapName;
	const char* szMissionName = "";

	if (m_pGame->m_bDedicatedServer)
		return pH->EndFunction();

	pH->GetParam(1, szMapName);
	if (pH->GetParamCount() == 2) pH->GetParam(2, szMissionName);

	ICVar* pVar = m_pSystem->GetIConsole()->GetCVar("g_GameType");
	pVar->Set("Default");

	m_pGame->m_tPlayerPersistentData.m_bDataSaved = false;
	m_pGame->LoadLevelCS(false, szMapName, szMissionName, false);

	return pH->EndFunction();
}
