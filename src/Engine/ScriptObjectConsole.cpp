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
	REG_FUNC(CScriptObjectConsole, PrintLine, this);
}

void CScriptObjectConsole::Init(IScriptSystem *pScriptSystem, IConsole* pConsole)
{
	m_Console = pConsole;
	InitGlobal(pScriptSystem, "Console", this);
}

int CScriptObjectConsole::PrintLine(IFunctionHandler* pH)
{
	m_Console->PrintLine(__FUNCTION__);
	return 0;
}
