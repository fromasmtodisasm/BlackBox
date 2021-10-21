#if 0
#include <BlackBox/ScriptObjectClient.hpp>
#include <BlackBox/Network/INetwork.hpp>
#include <Game.hpp>

_DECLARE_SCRIPTABLEEX(CScriptObjectClient)

CScriptObjectClient::CScriptObjectClient()
{
}

CScriptObjectClient::~CScriptObjectClient()
{
}

void CScriptObjectClient::InitializeTemplate(IScriptSystem* pSS)
{
	_ScriptableEx<CScriptObjectClient>::InitializeTemplate(pSS);
#define SCRIPT_REG_CLASSNAME CScriptObjectClient
	SCRIPT_REG_FUNC(Connect);
	SCRIPT_REG_FUNC(Disconnect);
	SCRIPT_REG_FUNC(Send);
	SCRIPT_REG_FUNC(Response);
}

void CScriptObjectClient::Init(IScriptSystem* pScriptSystem, IClient* pClient)
{
	m_pScriptSystem = pScriptSystem;
	m_pClient = pClient;
	InitGlobal(pScriptSystem, "Client", this);
}

int CScriptObjectClient::Connect(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(2);
	const char* pszAddr;
	int Port;
	pH->GetParam(1, pszAddr);
	pH->GetParam(2, Port);

	int result = false;
	if (pszAddr)
		result = m_pClient->Connect(pszAddr, Port);
	else
		m_pScriptSystem->RaiseError("Connect() parameter is nil");

	return pH->EndFunction(result);
}

int CScriptObjectClient::Disconnect(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(0);
	m_pClient->Disconnect();
	return pH->EndFunction();
}

int CScriptObjectClient::Send(IFunctionHandler* pH)
{
	CHECK_PARAMETERS(1);
	const char* pszMsg;
	int Port;
	pH->GetParam(1, pszMsg);

	int result = false;
	if (pszMsg)
		result = m_pClient->Send(pszMsg);
	else
		m_pScriptSystem->RaiseError("Send() parameter is nil");

	return pH->EndFunction(result);
}

int CScriptObjectClient::Response(IFunctionHandler* pH)
{
	auto resp = m_pClient->Response();
	return pH->EndFunction(resp.c_str());
}
#endif
