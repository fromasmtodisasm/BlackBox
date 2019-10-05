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

	void Init(IScriptSystem *pScriptSystem, IConsole* pConsole);

	int PrintLine(IFunctionHandler* pH);
	int Clear(IFunctionHandler* pH);
	int Show(IFunctionHandler* pH);

private:
	IConsole* m_Console;
};
