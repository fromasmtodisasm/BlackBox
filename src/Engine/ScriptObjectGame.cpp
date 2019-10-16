#include <BlackBox/ScriptObjectGame.hpp>
#include <BlackBox/Game/Game.hpp>

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
}

void CScriptObjectGame::Init(IScriptSystem* pScriptSystem, CGame* pGame)
{
	m_pGame = pGame;
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
