#include <BlackBox/ScriptObjectServer.hpp>
#include <BlackBox/Game/Game.hpp>

_DECLARE_SCRIPTABLEEX(CScriptObjectServer)

CScriptObjectServer::CScriptObjectServer()
{
}

CScriptObjectServer::~CScriptObjectServer()
{
}

void CScriptObjectServer::InitializeTemplate(IScriptSystem* pSS)
{
	_ScriptableEx<CScriptObjectServer>::InitializeTemplate(pSS);
#define SCRIPT_REG_CLASSNAME CScriptObjectServer
	SCRIPT_REG_FUNC(Create);
	SCRIPT_REG_FUNC(Start);
	SCRIPT_REG_FUNC(Stop);
}

void CScriptObjectServer::Init(IScriptSystem* pScriptSystem, IServer* pServer)
{
	m_pScriptSystem = m_pScriptSystem;
	m_pServer = pServer;
	InitGlobal(pScriptSystem, "Server", this);
}

int CScriptObjectServer::Create(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(1);

	int port;
	pH->GetParam(1, port);

	if (m_pServer->Create(port))
		GetISystem()->GetIConsole()->PrintLine("Server created");
	pH->EndFunction();
}

int CScriptObjectServer::Start(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(0);
	if (m_pServer->Start())
	{
		GetISystem()->GetIConsole()->PrintLine("Server started");
	}
	pH->EndFunction();
}

int CScriptObjectServer::Stop(IFunctionHandler* pH)
{
	SCRIPT_CHECK_PARAMETERS(0);
	m_pServer->Stop();
	pH->EndFunction();
}

