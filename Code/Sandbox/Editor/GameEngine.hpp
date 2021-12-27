#pragma once

struct IGame;

class CGameEngine
{
public:
	CGameEngine(IGame* pGame);
public:
	IGame* m_pGame;
};
