#include <BlackBox/ScriptObjectConsole.hpp>

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
	REG_FUNC(CScriptObjectConsole, CreateVariable);
}

void CScriptObjectConsole::Init(IScriptSystem* pScriptSystem, IConsole* pConsole)
{
	m_ScriptSystem = pScriptSystem;
	m_Console = pConsole;
}

int CScriptObjectConsole::CreateVariable(IFunctionHandler* pH)
{
	return 0;
}
