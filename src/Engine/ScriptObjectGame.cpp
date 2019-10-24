#include <BlackBox/ScriptObjectGame.hpp>
#include <BlackBox/Game/Game.hpp>
#include <BlackBox/IMarkers.hpp>
#include <BlackBox/IConsole.hpp>

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
	REG_FUNC(CScriptObjectGame, Stop);
	REG_FUNC(CScriptObjectGame, gotoMenu);
	REG_FUNC(CScriptObjectGame, gotoGame);
	REG_FUNC(CScriptObjectGame, gotoFly);
	REG_FUNC(CScriptObjectGame, gotoEdit);
	REG_FUNC(CScriptObjectGame, showMenu);
	REG_FUNC(CScriptObjectGame, getFPS);

	REG_FUNC(CScriptObjectGame, GetTagPoint);
	REG_FUNC(CScriptObjectGame, CreateVariable);
	REG_FUNC(CScriptObjectGame, SetVariable);
	REG_FUNC(CScriptObjectGame, RemoveVariable);
	REG_FUNC(CScriptObjectGame, GetVariable);

	REG_FUNC(CScriptObjectGame, AddCommand);

	REG_FUNC(CScriptObjectGame, SavePlayerPos);
	REG_FUNC(CScriptObjectGame, LoadPlayerPos);
}

void CScriptObjectGame::Init(IScriptSystem* pScriptSystem, CGame* pGame)
{
	m_pGame = pGame;
	m_pSystem = pGame->GetSystem();
	m_pConsole = m_pSystem->GetIConsole();
	InitGlobal(pScriptSystem, "Game", this);
}

int CScriptObjectGame::Stop(IFunctionHandler* pH)
{
	m_pGame->Stop();
	return pH->EndFunction();
}

int CScriptObjectGame::gotoMenu(IFunctionHandler* pH)
{
	m_pGame->gotoMenu();
	return pH->EndFunction();
}

int CScriptObjectGame::gotoFullscreen(IFunctionHandler* pH)
{
	m_pGame->gotoFullscreen();
	return pH->EndFunction();
}

int CScriptObjectGame::gotoGame(IFunctionHandler* pH)
{
	m_pGame->gotoGame();
	return pH->EndFunction();
}

int CScriptObjectGame::gotoFly(IFunctionHandler* pH)
{
	m_pGame->gotoFly();
	return pH->EndFunction();
}

int CScriptObjectGame::gotoEdit(IFunctionHandler* pH)
{
	m_pGame->gotoEdit();
	return pH->EndFunction();
}

int CScriptObjectGame::showMenu(IFunctionHandler* pH)
{
	m_pGame->showMenu();
	return pH->EndFunction();
}

int CScriptObjectGame::getFPS(IFunctionHandler* pH)
{
	return pH->EndFunction(m_pGame->getFPS());
}

int CScriptObjectGame::GetTagPoint(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(1);

	const char* sTPName;
	ITagPoint* pTP = NULL;
	Vec3 vec(0, 0, 0);

	if (pH->GetParam(1, sTPName))
	{
		pTP = m_pGame->GetTagPoint(sTPName);
		if (!pTP) return pH->EndFunctionNull();

		pTP->GetPos(vec);
	}
	m_pGetTagPoint.Set(vec);
	return pH->EndFunction(m_pGetTagPoint);
}
//////////////////////////////////////////////////////////////////////
/*!create a console variable
	@param sName name of the console variable
	@param sDefault the default value[optional]
*/
int CScriptObjectGame::CreateVariable(IFunctionHandler* pH)
{
	int nPCount = pH->GetParamCount();
	const char* sName;
	const char* sDefault;
	const char* sflags;
	int iflags = 0;
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
						;// iflags = VF_REQUIRE_NET_SYNC;
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
		int nVal;
		float fVal;
		if (pH->GetParam(2, fVal))
			pVar->Set(fVal);
		else
			if (pH->GetParam(2, nVal))
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
	int nPCount = pH->GetParamCount();
	const char* sName;
	const char* sCommand;
	const char* sHelp = NULL;
	int iflags = 0;
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
			m_pConsole->AddCommand(sName, sCommand, 0, sHelp);
		else
			m_pConsole->AddCommand(sName, sCommand, 0/*VF_NOHELP*/, "");
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
	int index;
	if (pH->GetParam(1, index))
	{
		if (pH->GetParam(2, sName))
		{
			m_pGame->DevMode_LoadPlayerPos(index, sName);
		}
	}
	return pH->EndFunction();
}
