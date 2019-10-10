#include <BlackBox/ScriptObjectConsole.hpp>
#include <BlackBox/IConsole.hpp>

#include <iostream>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_DECLARE_SCRIPTABLEEX(CScriptObjectConsole)


CScriptObjectConsole::CScriptObjectConsole() 
	:
	m_Console(nullptr)
{
}

CScriptObjectConsole::~CScriptObjectConsole()
{
}

void CScriptObjectConsole::InitializeTemplate(IScriptSystem* pSS)
{
	_ScriptableEx<CScriptObjectConsole>::InitializeTemplate(pSS);

#undef SCRIPT_REG_CLASSNAME
#define SCRIPT_REG_CLASSNAME CScriptObjectConsole

	SCRIPT_REG_FUNC(PrintLine);
	SCRIPT_REG_FUNC(Clear);
	SCRIPT_REG_FUNC(Show);
}

void CScriptObjectConsole::Init(IScriptSystem *pScriptSystem, IConsole* pConsole)
{
	m_Console = pConsole;
	InitGlobal(pScriptSystem, "Console", this);
}

int CScriptObjectConsole::PrintLine(IFunctionHandler* pH)
{
	const char* str;
	pH->GetParam(1, str);
	m_Console->PrintLine("-- %s;", str);
	return pH->EndFunction(m_pScriptThis);
}

int CScriptObjectConsole::Clear(IFunctionHandler* pH)
{
	m_Console->Clear();
	return pH->EndFunction();
}

int CScriptObjectConsole::Show(IFunctionHandler* pH)
{
	bool show = false;
	pH->GetParam(1, show);
	m_Console->ShowConsole(show);
	return pH->EndFunction();
}
