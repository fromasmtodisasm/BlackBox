#pragma once

#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>

class CGame;


class CScriptObjectTest :
	public _ScriptableEx<CScriptObjectTest>
{
public:
	CScriptObjectTest();
	~CScriptObjectTest();
	void InitializeTemplate(IScriptSystem* pSS);
	int TestMethod(int i)
	{
		gEnv->pConsole->PrintLine("i = %d;", i);	
		return 0;
	}
	const char* tm2(int i, int c)
	{
		//gEnv->pConsole->PrintLine("i = %d; c = %c", i, c);	
		return "Test return value";
	}

	int tm3(int i, int c, const char* str)
	{
		gEnv->pConsole->PrintLine("i = %d; c = %c; s = %s", i, c, str);	
		return 0;
	}
	int tm4(int i)
	{
		gEnv->pConsole->PrintLine("i = %d;", i);	
		return 0;
	}

	void Init(IScriptSystem* pScriptSystem, CGame* pGame);

private:
	CGame *							m_pGame;
	ISystem *						m_pSystem;
};
