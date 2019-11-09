#pragma once
#include <BlackBox/ScriptSystem/_ScriptableEx.hpp>
//#include <BlackBox/Game/Game.hpp>

struct IClient;

class CScriptObjectClient : public _ScriptableEx<CScriptObjectClient>
{
public:
	CScriptObjectClient();
	~CScriptObjectClient();

	void InitializeTemplate(IScriptSystem* pSS);

	void Init(IScriptSystem* pScriptSystem, IClient* pClient);

	int Connect(IFunctionHandler *pH);
	int Disconnect(IFunctionHandler *pH);
	int Send(IFunctionHandler *pH);
	int Response(IFunctionHandler *pH);

private:
	IClient* m_pClient;
};

