#pragma once

#include <BlackBox/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>


class CScriptObjectSystem :
public _ScriptableEx<CScriptObjectSystem>
{
public:
	CScriptObjectSystem(ISystem* pSystem, IScriptSystem* pSS);
	~CScriptObjectSystem();
	static void InitializeTemplate(IScriptSystem* pSS);

	void Init(IScriptSystem *pScriptSystem, ISystem* pSystem);
	int EnumDisplayFormats(IFunctionHandler* pH);
private:
	ISystem* m_pSystem;
	ILog* m_pLog;
	IRender* m_pRenderer;
	IConsole* m_pConsole;
};
