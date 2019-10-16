#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
#include <BlackBox/ScriptSystem/ScriptObject.hpp>
#include <BlackBox/ISystem.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/ScriptSystem/FunctionHandler.hpp>
#include <BlackBox/ScriptSystem/StackGuard.hpp>

#include <string>
#include <iostream>
#include <functional>

#include <cstdarg>
#include <cstring>

extern "C" {

#define DumpCallStack(L) printf("DumpCallStack() not implemented")
}

CFunctionHandler* CScriptSystem::m_pH = nullptr;

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
	m_pSystem = pSystem;
	L = luaL_newstate();
	luaL_openlibs(L);
	CScriptObject::L = L; // Set lua state for script table class.
	CScriptObject::m_pSS = this;
	m_pH = new CFunctionHandler(this, L);

	m_stdScriptBinds.Init(pSystem, this);

	//////////////////////////////////////////////////////////////////////////
	// Execute common lua file.
	//////////////////////////////////////////////////////////////////////////
	//ExecuteFile("scripts/common.lua", true, false);

	// Make the error handler available to LUA
	RegisterErrorHandler();

	m_cvar_script_debugger = pSystem->GetIConsole()->CreateVariable(
		"lua_debugger", 0, VF_CHEAT,
		"Enables the script debugger.\n"
		"1 to trigger on breakpoints and errors\n"
		"2 to only trigger on errors\n"
		"Usage: lua_debugger [0/1/2]");

	return L ? true : false;
}

void CScriptSystem::RegisterErrorHandler(void)
{
	// Legacy approach
		/*
			 if(bDebugger)
			 {
			 //lua_register(L, LUA_ERRORMESSAGE, CScriptSystem::ErrorHandler );
			 //lua_setglobal(L, LUA_ERRORMESSAGE);
			 }
			 else
			 {
			 //lua_register(L, LUA_ERRORMESSAGE, CScriptSystem::ErrorHandler );

			 //lua_newuserdatabox(L, this);
			 //lua_pushcclosure(L, CScriptSystem::ErrorHandler, 1);
			 //lua_setglobal(L, LUA_ALERT);
			 //lua_pushcclosure(L, errorfb, 0);
			 //lua_setglobal(L, LUA_ERRORMESSAGE);
			 }
		 */

		 // Register global error handler.
		 // This just makes it available - when we call LUA, we insert it so it will be called
	if (!m_pErrorHandlerFunc)
	{
		lua_pushcfunction(L, CScriptSystem::ErrorHandler);
		m_pErrorHandlerFunc = (HSCRIPTFUNCTION)(INT_PTR)lua_ref(L, 1);
	}
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
		m_pSystem->GetIConsole()->PrintLine("LUA ERROR: %s", errormsg.c_str());
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
	return new CScriptObject;
}

IScriptObject* CScriptSystem::CreateObject()
{
	CScriptObject* result = new CScriptObject;
	result->CreateNew();
	return result;
}

IScriptObject* CScriptSystem::CreateGlobalObject(const char* sName)
{
	IScriptObject *pObj = CreateObject();
	SetGlobalValue(sName, pObj);
	return pObj;
}

int CScriptSystem::BeginCall(HSCRIPTFUNCTION hFunc)
{
	assert(hFunc != 0);
	if (!hFunc)
		return 0;

	lua_getref(L, (int)(INT_PTR)hFunc);
	if (!lua_isfunction(L, -1))
	{
#if defined(__GNUC__)
		ScriptWarning("[CScriptSystem::BeginCall] Function Ptr:%d not found", (int)(INT_PTR)hFunc);
#else
		ScriptWarning("[CScriptSystem::BeginCall] Function Ptr:%d not found", hFunc);
#endif
		m_nTempArg = -1;
		lua_pop(L, 1);
		return 0;
	}
	m_nTempArg = 0;

	return 1;
}

int CScriptSystem::BeginCall(const char* sFuncName)
{
	if (L)
	{
		lua_getglobal(L, sFuncName);
		m_nTempArg = 0;

		if (!lua_isfunction(L, -1))
		{
			ScriptWarning("[CScriptSystem::BeginCall] Function %s not found(check for syntax errors or if the file wasn't loaded)", sFuncName);
			m_nTempArg = -1;
			lua_pop(L, 1);
			return 0;
		}

		return 1;
	}
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

int CScriptSystem::BeginCall(IScriptObject* pTable, const char* sFuncName)
{
	PushObject(pTable);

	lua_pushstring(L, sFuncName);
	lua_gettable(L, -2);
	lua_remove(L, -2);  // Remove table global.
	m_nTempArg = 0;

	if (!lua_isfunction(L, -1))
	{
		ScriptWarning("[CScriptSystem::BeginCall] Function %s not found in the table", sFuncName);
		m_nTempArg = -1;
		lua_pop(L, 1);
		return 0;
	}

	return 1;
}

bool CScriptSystem::EndCall()
{
	return EndCallN(0);
}

bool CScriptSystem::EndCall(int& nRet)
{
	return EndCallAny(nRet);
}

bool CScriptSystem::EndCall(float& fRet)
{
	return EndCallAny(fRet);
}

bool CScriptSystem::EndCall(const char*& sRet)
{
	return EndCallAny(sRet);
}

bool CScriptSystem::EndCall(bool& bRet)
{
	return EndCallAny(bRet);
}

bool CScriptSystem::EndCall(IScriptObject*& pScriptObject)
{
	return EndCallAny(pScriptObject);
}

HSCRIPTFUNCTION CScriptSystem::GetFunctionPtr(const char* sFuncName)
{
	CHECK_STACK(L);
	HSCRIPTFUNCTION func;
	lua_getglobal(L, sFuncName);
	if (lua_isnil(L, -1) || (!lua_isfunction(L, -1)))
	{
		lua_pop(L, 1);
		return NULL;
	}
	func = (HSCRIPTFUNCTION)(INT_PTR)lua_ref(L, 1);

	return func;
}

HSCRIPTFUNCTION CScriptSystem::GetFunctionPtr(const char* sTableName, const char* sFuncName)
{
	CHECK_STACK(L);
	HSCRIPTFUNCTION func;
	lua_getglobal(L, sTableName);
	if (!lua_istable(L, -1))
	{
		lua_pop(L, 1);
		return 0;
	}
	lua_pushstring(L, sFuncName);
	lua_gettable(L, -2);
	lua_remove(L, -2);  // Remove table global.
	if (lua_isnil(L, -1) || (!lua_isfunction(L, -1)))
	{
		lua_pop(L, 1);
		return FALSE;
	}
	func = (HSCRIPTFUNCTION)(INT_PTR)lua_ref(L, 1);
	return func;
}

void CScriptSystem::ReleaseFunc(HSCRIPTFUNCTION f)
{
	CHECK_STACK(L);

	if (f)
	{
#ifdef _DEBUG
		lua_getref(L, (int)(INT_PTR)f);
		assert(lua_type(L, -1) == LUA_TFUNCTION);
		lua_pop(L, 1);
#endif
		lua_unref(L, (int)(INT_PTR)f);
	}
}

void CScriptSystem::PushFuncParam(int nVal)
{
	PushFuncParamAny(nVal);
}

void CScriptSystem::PushFuncParam(float fVal)
{
	PushFuncParamAny(fVal);
}

void CScriptSystem::PushFuncParam(const char* sVal)
{
	PushFuncParamAny(sVal);
}

void CScriptSystem::PushFuncParam(bool bVal)
{
	PushFuncParamAny(bVal);
}

void CScriptSystem::PushFuncParam(IScriptObject* pVal)
{
	PushFuncParamAny(pVal);
}

void CScriptSystem::SetGlobalValue(const char* sKey, int nVal)
{
	CHECK_STACK(L);
	lua_pushinteger(L, nVal);
	lua_setglobal(L, sKey);
}

void CScriptSystem::SetGlobalValue(const char* sKey, float fVal)
{
	CHECK_STACK(L);
	lua_pushnumber(L, fVal);
	lua_setglobal(L, sKey);
}

void CScriptSystem::SetGlobalValue(const char* sKey, const char* sVal)
{
	CHECK_STACK(L);
	lua_pushstring(L, sVal);
	lua_setglobal(L, sKey);
}

void CScriptSystem::SetGlobalValue(const char* sKey, IScriptObject* pObj)
{
	CHECK_STACK(L);
	PushObject(pObj);
	lua_setglobal(L, sKey);
}

bool CScriptSystem::GetGlobalValue(const char* sKey, IScriptObject* pObj)
{
	lua_getglobal(L, sKey);

	if (!lua_istable(L, -1))
	{
		return false;
		//pObj = CreateEmptyObject();
		//pObj->AddRef();
	}
	lua_pushvalue(L, -1);
	pObj->Attach();
	//AttachTable(pObj);

	return true;
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

USER_DATA CScriptSystem::CreateUserData(void* ptr, int size)
{
	CHECK_STACK(L);

	void* nptr = lua_newuserdata(L, size);
	memcpy(nptr, ptr, size);
	CScriptObject* pNewTbl = new CScriptObject();
	pNewTbl->Attach();

	return pNewTbl;
}

void CScriptSystem::RemoveTaggedValue(HTAG tag)
{
}

void CScriptSystem::RaiseError(const char* format, ...)
{
	va_list arglist;
	char sBuf[2048];
	int nCurrentLine = 0;
	const char* sSourceFile = "undefined";

	va_start(arglist, format);
	vsprintf(sBuf, format, arglist);
	va_end(arglist);

	ScriptWarning("[Lua Error] %s", sBuf);

	TraceScriptError(sSourceFile, nCurrentLine, sBuf);
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

void CScriptSystem::TraceScriptError(const char* file, int line, const char* errorStr)
{
	lua_Debug ar;

	// If in debug mode, try to enable debugger.
	if ((ELuaDebugMode)m_cvar_script_debugger->GetIVal() != eLDM_NoDebug)
	{
		if (lua_getstack(L, 1, &ar))
		{
			if (lua_getinfo(L, "lnS", &ar))
			{
#if LUA_REMOTE_DEBUG_ENABLED
				if (g_pLuaRemoteDebug && g_pLuaRemoteDebug->IsClientConnected())
				{
					g_pLuaRemoteDebug->OnScriptError(L, &ar, errorStr);
				}
				else
#endif
				{
					ShowDebugger(file, line, errorStr);
				}
			}
		}
	}
	else
	{
		LogStackTrace();
	}
}

void CScriptSystem::PushObject(IScriptObject* pObj)
{
	((CScriptObject*)pObj)->PushRef();
}

void CScriptSystem::LogStackTrace()
{
	::DumpCallStack(L);
}

bool CScriptSystem::EndCallN(int nReturns)
{
	if (m_nTempArg < 0 || !L)
		return false;

	int base = lua_gettop(L) - m_nTempArg;  // function index.
	lua_getref(L, (int)(INT_PTR)m_pErrorHandlerFunc);
	lua_insert(L, base);  // put it under chunk and args

	//signal(SIGINT, cry_laction);
	int status = lua_pcall(L, m_nTempArg, nReturns, base);
	//signal(SIGINT, SIG_DFL);
	lua_remove(L, base);  // remove error handler function.

	return status == 0;
}

int CScriptSystem::ErrorHandler(lua_State* L)
{
	//if (!lua_isstoredebuginfo(L))
	//	return 0; // ignore script errors if engine is running without game

	// Handle error
	lua_Debug ar;
	CScriptSystem* pThis = (CScriptSystem*)GetISystem()->GetIScriptSystem();

	memset(&ar, 0, sizeof(lua_Debug));

	const char* sErr = lua_tostring(L, 1);

	if (sErr)
	{
		ScriptWarning("[Lua Error] %s", sErr);
	}

	//////////////////////////////////////////////////////////////////////////
	// Print error callstack.
	//////////////////////////////////////////////////////////////////////////
	int level = 1;
	while (lua_getstack(L, level++, &ar))
	{
		lua_getinfo(L, "lnS", &ar);
		if (ar.name)
			CryLog("$6    > %s, (%s: %d)", ar.name, ar.short_src, ar.currentline);
		else
			CryLog("$6    > (null) (%s: %d)", ar.short_src, ar.currentline);
	}

	if (sErr)
	{
		ICVar* lua_StopOnError = GetISystem()->GetIConsole()->GetCVar("lua_StopOnError");
		if (lua_StopOnError && lua_StopOnError->GetIVal() != 0)
		{
			ScriptWarning("![Lua Error] %s", sErr);
		}
	}

	pThis->TraceScriptError(ar.source, ar.currentline, sErr);

	return 0;
}

void CScriptSystem::ShowDebugger(const char* pszSourceFile, int iLine, const char* pszReason)
{
}
