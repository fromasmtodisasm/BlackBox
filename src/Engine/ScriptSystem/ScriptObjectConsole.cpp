#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/ScriptSystem/ScriptObjectConsole.hpp>
#include <BlackBox/System/IConsole.hpp>

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
  SCRIPT_REG_FUNC(ExecuteString);
  SCRIPT_REG_FUNC(CreateKeyBind);
}

void CScriptObjectConsole::Init(IScriptSystem* pScriptSystem, IConsole* pConsole)
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

int CScriptObjectConsole::ExecuteString(IFunctionHandler* pH)
{
  const char* str;
  pH->GetParam(1, str);
  m_Console->ExecuteString(str);
  return pH->EndFunction();
}

int CScriptObjectConsole::CreateKeyBind(IFunctionHandler* pH)
{
  CHECK_PARAMETERS(2);
  const char* key;
  const char* cmd;
  pH->GetParam(1, key);
  pH->GetParam(2, cmd);
  m_Console->CreateKeyBind(key, cmd);
  return pH->EndFunction();
  return 0;
}
