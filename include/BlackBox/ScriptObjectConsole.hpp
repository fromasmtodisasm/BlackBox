#pragma once

#include <BlackBox/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>


class CScriptObjectConsole :
public _ScriptableEx<CScriptObjectConsole>
{
public:
	CScriptObjectConsole();
	~CScriptObjectConsole();
	static void InitializeTemplate(IScriptSystem* pSS);

	void Init(IScriptSystem* pScriptSystem, IConsole* pConsole);

	int CreateVariable(IFunctionHandler* pH);

private:
	IScriptSystem* m_ScriptSystem;
	IConsole* m_Console;
};
