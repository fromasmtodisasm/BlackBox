#pragma once

#include <BlackBox/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>

class CGame;


class CScriptObjectGame :
	public _ScriptableEx<CScriptObjectGame>
{
public:
	CScriptObjectGame();
	~CScriptObjectGame();
	void InitializeTemplate(IScriptSystem* pSS);

	void Init(IScriptSystem* pScriptSystem, CGame* pGame);

	int Stop(IFunctionHandler *pH);
	int gotoMenu(IFunctionHandler *pH);
	int gotoFullscreen(IFunctionHandler *pH);
	int gotoGame(IFunctionHandler *pH);
	int gotoFly(IFunctionHandler *pH);
	int gotoEdit(IFunctionHandler *pH);
	int showMenu(IFunctionHandler *pH);
	int getFPS(IFunctionHandler *pH);

private:
	IConsole* m_Console;
	CGame *m_pGame;
};