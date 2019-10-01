#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
#include <BlackBox/ScriptSystem/ScriptObject.hpp>
#include <BlackBox/IEngine.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/ScriptSystem/FunctionHandler.hpp>

#include <string>
#include <iostream>
#include <functional>

CFunctionHandler* CScriptSystem::m_pH = nullptr;

static int print(lua_State* L) {
	auto system = GetISystem();
	auto str = lua_tostring(L, 1);  /* get argument */
	system->getIConsole()->PrintLine(str);
	return 0;  /* number of results */
}

CScriptSystem::CScriptSystem()
	:
	m_pSystem(nullptr)
{

}

CScriptSystem::~CScriptSystem()
{
	lua_close(L);
}

bool CScriptSystem::Init(ISystem* pSystem)
{
	L = luaL_newstate();
	luaL_openlibs(L);
	lua_pushcfunction(L, print);
	lua_setglobal(L, "console_printline");

	CScriptObject::L = L; // Set lua state for script table class.
	CScriptObject::m_pSS = this;
	m_pH = new CFunctionHandler(this, L);

	//////////////////////////////////////////////////////////////////////////
	// Execute common lua file.
	//////////////////////////////////////////////////////////////////////////
	//ExecuteFile("scripts/common.lua", true, false);

	return L ? true : false;
}

bool CScriptSystem::ExecuteFile(const char* sFileName, bool bRaiseError/* = true*/, bool bForceReload/* = false*/)
{
	std::string path("res/" + std::string(sFileName));
	return luaL_dofile(L, path.c_str()) == LUA_OK;
}

bool CScriptSystem::ExecuteBuffer(const char* sBuffer, size_t nSize)
{
	int result = 0;

	if ((result = luaL_dostring(L, sBuffer)) == LUA_OK)
	{

	}
	else
	{
		std::string errormsg = lua_tostring(L, -1);
		m_pSystem->getIConsole()->PrintLine("LUA ERROR: %s", errormsg.c_str());
	}

	return result == LUA_OK;
}

bool CScriptSystem::GetGlobalValue(const char* sKey, int& nVal)
{
	bool result = false;
	lua_getglobal(L, sKey);
	if (lua_isinteger(L, -1))
	{
		result = true;
		nVal = lua_tointeger(L, -1);
	}
	return result;
}

bool CScriptSystem::GetGlobalValue(const char* sKey, float& fVal)
{
	bool result = false;
	lua_getglobal(L, sKey);
	if (lua_isnumber(L, -1))
	{
		result = true;
		fVal = lua_tonumber(L, -1);
	}
	return result;
}

bool CScriptSystem::GetGlobalValue(const char* sKey, const char*& sVal)
{
	bool result = false;
	lua_getglobal(L, sKey);
	if (lua_isstring(L, -1))
	{
		result = true;
		sVal = lua_tostring(L, -1);
	}
	return result;
}

IFunctionHandler* CScriptSystem::GetFunctionHandler()
{
	return m_pH;
}

HSCRIPT CScriptSystem::GetScriptHandle()
{
	//TODO: Rewrite it
	return L;
}

void CScriptSystem::UnloadScript(const char* sFileName)
{
}

void CScriptSystem::UnloadScripts()
{
}

bool CScriptSystem::ReloadScript(const char* sFileName, bool bRaiseError/* = true*/)
{
	return false;
}

bool CScriptSystem::ReloadScripts()
{
	return false;
}

void CScriptSystem::DumpLoadedScripts()
{
}

IScriptObject* CScriptSystem::GetGlobalObject()
{
	return nullptr;
}

IScriptObject* CScriptSystem::CreateEmptyObject()
{
	return nullptr;
}

IScriptObject* CScriptSystem::CreateObject()
{
	return new CScriptObject;
}

IScriptObject* CScriptSystem::CreateGlobalObject(const char* sName)
{
	return new CScriptObject;
}

int CScriptSystem::BeginCall(HSCRIPTFUNCTION hFunc)
{
	return 0;
}

int CScriptSystem::BeginCall(const char* sFuncName)
{
	return 0;
}

int CScriptSystem::BeginCall(const char* sTableName, const char* sFuncName)
{
	lua_getglobal(L, sTableName);

	if (!lua_istable(L, -1))
	{
		ScriptWarning("[CScriptSystem::BeginCall] Tried to call %s:%s(), Table %s not found (check for syntax errors or if the file wasn't loaded)", sTableName, sFuncName, sTableName);
		m_nTempArg = -1;
		lua_pop(L, 1);
		return 0;
	}

	lua_pushstring(L, sFuncName);
	lua_gettable(L, -2);
	lua_remove(L, -2);  // Remove table global.
	m_nTempArg = 0;

	if (!lua_isfunction(L, -1))
	{
		ScriptWarning("[CScriptSystem::BeginCall] Function %s:%s not found(check for syntax errors or if the file wasn't loaded)", sTableName, sFuncName);
		m_nTempArg = -1;
		lua_pop(L, 1);
		return 0;
	}

	return 1;
}

void CScriptSystem::EndCall()
{
}

void CScriptSystem::EndCall(int& nRet)
{
}

void CScriptSystem::EndCall(float& fRet)
{
}

void CScriptSystem::EndCall(const char*& sRet)
{
}

void CScriptSystem::EndCall(bool& bRet)
{
}

void CScriptSystem::EndCall(IScriptObject* pScriptObject)
{
}

HSCRIPTFUNCTION CScriptSystem::GetFunctionPtr(const char* sFuncName)
{
	return HSCRIPTFUNCTION();
}

HSCRIPTFUNCTION CScriptSystem::GetFunctionPtr(const char* sTableName, const char* sFuncName)
{
	return HSCRIPTFUNCTION();
}

void CScriptSystem::ReleaseFunc(HSCRIPTFUNCTION f)
{
}

void CScriptSystem::PushFuncParam(int nVal)
{
}

void CScriptSystem::PushFuncParam(float fVal)
{
}

void CScriptSystem::PushFuncParam(const char* sVal)
{
}

void CScriptSystem::PushFuncParam(bool bVal)
{
}

void CScriptSystem::PushFuncParam(IScriptObject* pVal)
{
}

void CScriptSystem::SetGlobalValue(const char* sKey, int nVal)
{
}

void CScriptSystem::SetGlobalValue(const char* sKey, float fVal)
{
}

void CScriptSystem::SetGlobalValue(const char* sKey, const char* sVal)
{
}

void CScriptSystem::SetGlobalValue(const char* sKey, IScriptObject* pObj)
{
}

bool CScriptSystem::GetGlobalValue(const char* sKey, IScriptObject* pObj)
{
	return false;
}

void CScriptSystem::SetGlobalToNull(const char* sKey)
{
}

HTAG CScriptSystem::CreateTaggedValue(const char* sKey, int* pVal)
{
	return HTAG();
}

HTAG CScriptSystem::CreateTaggedValue(const char* sKey, float* pVal)
{
	return HTAG();
}

HTAG CScriptSystem::CreateTaggedValue(const char* sKey, char* pVal)
{
	return HTAG();
}

USER_DATA CScriptSystem::CreateUserData(INT_PTR nVal, int nCookie)
{
	return USER_DATA();
}

void CScriptSystem::RemoveTaggedValue(HTAG tag)
{
}

void CScriptSystem::RaiseError(const char* sErr, ...)
{
}

void CScriptSystem::ForceGarbageCollection()
{
}

int CScriptSystem::GetCGCount()
{
	return 0;
}

void CScriptSystem::SetGCThreshhold(int nKb)
{
}

void CScriptSystem::UnbindUserdata()
{
}

void CScriptSystem::Release()
{
}

void CScriptSystem::EnableDebugger(IScriptDebugSink* pDebugSink)
{
}

IScriptObject* CScriptSystem::GetBreakPoints()
{
	return nullptr;
}

HBREAKPOINT CScriptSystem::AddBreakPoint(const char* sFile, int nLineNumber)
{
	return HBREAKPOINT();
}

IScriptObject* CScriptSystem::GetLocalVariables(int nLevel/* = 0*/)
{
	return nullptr;
}

IScriptObject* CScriptSystem::GetCallsStack()
{
	return nullptr;
}

void CScriptSystem::DebugContinue()
{
}

void CScriptSystem::DebugStepNext()
{
}

void CScriptSystem::DebugStepInto()
{
}

void CScriptSystem::DebugDisable()
{
}

BreakState CScriptSystem::GetBreakState()
{
	return BreakState();
}

void CScriptSystem::GetMemoryStatistics(ICrySizer* pSizer)
{
}

void CScriptSystem::GetScriptHash(const char* sPath, const char* szKey, unsigned int& dwHash)
{
}

void CScriptSystem::PostInit()
{
}
