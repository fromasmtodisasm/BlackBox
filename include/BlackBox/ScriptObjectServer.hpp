#pragma once
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>
//#include <BlackBox/Game/Game.hpp>

struct IServer;

class CScriptObjectServer : public _ScriptableEx<CScriptObjectServer>
{
public:
	CScriptObjectServer();
	~CScriptObjectServer();

	void InitializeTemplate(IScriptSystem* pSS);

	void Init(IScriptSystem* pScriptSystem, IServer* pServer);

	int Create(IFunctionHandler *pH);
	int Start(IFunctionHandler *pH);
	int Stop(IFunctionHandler *pH);

private:
	IServer* m_pServer;
};
