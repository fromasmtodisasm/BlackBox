#include <BlackBox/Core/Platform/platform_impl.inl>
#include <BlackBox/Core/Platform/Windows.hpp>
#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
#include <BlackBox/ScriptSystem/FunctionHandler.hpp>
#include <BlackBox/ScriptSystem/ScriptObject.hpp>
#include <BlackBox/ScriptSystem/StackGuard.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <functional>

#include <cstdarg>
#include <cstring>

#undef CryLog
#define CryLog(format, ...) ScriptWarning(format, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////
// Pointer to Global ISystem.
static ISystem* gISystem = nullptr;
/*
ISystem* GetISystem()
{
  return gISystem;
}
*/

extern "C" {
	//////////////////////////////////////////////////////////////////////////
	void DumpCallStack(lua_State* L)
	{
		lua_Debug ar;

		memset(&ar, 0, sizeof(lua_Debug));

		//////////////////////////////////////////////////////////////////////////
		// Print callstack.
		//////////////////////////////////////////////////////////////////////////
		int level = 0;
		while (lua_getstack(L, level++, &ar))
		{
			const char* slevel = "";
			if (level == 1)
				slevel = "  ";
			int nRes = lua_getinfo(L, "lnS", &ar);
			if (ar.name)
				CryLog("$6%s    > %s, (%s: %d)", slevel, ar.name, ar.short_src, ar.currentline);
			else
				CryLog("$6%s    > (null) (%s: %d)", slevel, ar.short_src, ar.currentline);
		}
	}

}

CFunctionHandler* CScriptSystem::m_pH = nullptr;

//////////////////////////////////////////////////////////////////////////
const char* FormatPath(char* const sLowerName, const char* sPath)
{
  strcpy(sLowerName, sPath);
  int i = 0;
  while (sLowerName[i] != 0)
  {
    if (sLowerName[i] == '\\')
      sLowerName[i] = '/';
    i++;
  }
  return sLowerName;
}

CScriptSystem::CScriptSystem()
  :
  m_pSystem(nullptr)
{
}

CScriptSystem::~CScriptSystem()
{
  m_stdScriptBinds.Done();

  if (L)
  {
    lua_close(L);

    L = NULL;
  }
}

bool CScriptSystem::Init(ISystem* pSystem)
{
  gISystem = pSystem;
  m_pSystem = pSystem;
  L = luaL_newstate();
  luaL_openlibs(L);
  CScriptObject::L = L; // Set lua state for script table class.
  CScriptObject::m_pSS = this;
  m_pH = new CFunctionHandler(this, L);

  m_stdScriptBinds.Init(pSystem, this);

  // Ensure the debugger is in the correct mode
  //EnableDebugger((ELuaDebugMode)m_cvar_script_debugger->GetIVal());

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
  std::string path(sFileName);

  auto it = m_dqLoadedFiles.find(path);
  std::string src;
  std::string buffer;
  if (it == m_dqLoadedFiles.end() || bForceReload)
  {
    m_dqLoadedFiles.insert(path);
    std::ifstream fin("res/" + path);
    if (!fin.is_open()) return false;
    while (std::getline(fin, buffer)) {
      src += buffer;
      src += '\n';
    }
    fin.close();
  }
  //return luaL_dofile(L, path.c_str()) == LUA_OK;
  return ExecuteBuffer(src.c_str(), src.length());
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
    nVal = static_cast<int>(lua_tointeger(L, -1));
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
    fVal = static_cast<float>(lua_tonumber(L, -1));
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
  if (strlen(sFileName) <= 0)
    return;

  char lowerName[_MAX_PATH];
  const char* sTemp = FormatPath(lowerName, sFileName);
  //ScriptFileListItor itor = std::find(m_dqLoadedFiles.begin(), m_dqLoadedFiles.end(), sTemp.c_str());
  ScriptFileListItor itor = m_dqLoadedFiles.find(/*CONST_TEMP_STRING(*/sTemp/*)*/);
  if (itor != m_dqLoadedFiles.end())
  {
    RemoveFileFromList(itor);
  }
}

void CScriptSystem::UnloadScripts()
{
  m_dqLoadedFiles.clear();
}

bool CScriptSystem::ReloadScript(const char* sFileName, bool bRaiseError/* = true*/)
{
  return ExecuteFile(sFileName, bRaiseError, GetISystem()->IsDevMode());
}

bool CScriptSystem::ReloadScripts()
{
  ScriptFileListItor itor;
  itor = m_dqLoadedFiles.begin();
  while (itor != m_dqLoadedFiles.end())
  {
    ReloadScript(itor->c_str(), true);
    ++itor;
  }
  return true;
}

void CScriptSystem::DumpLoadedScripts()
{
  ScriptFileListItor itor;
  itor = m_dqLoadedFiles.begin();
  while (itor != m_dqLoadedFiles.end())
  {
    //LogAlways("%s", itor->c_str());
    m_pSystem->GetIConsole()->PrintLine("%s", itor->c_str());
    ++itor;
  }
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
  IScriptObject* pObj = CreateObject();
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
    return 0;
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

void CScriptSystem::SetGlobalValue(const char* sKey, int Val)
{
	SetGlobalAny(sKey, Val);
}

void CScriptSystem::SetGlobalValue(const char* sKey, float Val)
{
	SetGlobalAny(sKey, Val);
}

void CScriptSystem::SetGlobalValue(const char* sKey, const char* Val)
{
	SetGlobalAny(sKey, Val);
}

void CScriptSystem::SetGlobalValue(const char* sKey, IScriptObject* pObj)
{
	SetGlobalAny(sKey, pObj);
}

void CScriptSystem::SetGlobalToNull(const char* sKey)
{
	SetGlobalAny(sKey, nullptr);
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

USER_DATA CScriptSystem::CreateUserData(INT_PTR nVal,int nCookie)
{
  CHECK_STACK(L);

	auto size = sizeof(UserDataInfo);
  UserDataInfo* ud = (UserDataInfo*)lua_newuserdata(L, size);
	ud->ptr = nVal;
	ud->cookie = nCookie;
	lua_pop(L, 1);

  return ud;
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
#if !defined(EXCLUDE_NORMAL_LOG)
	int beforeUsage = lua_gc(L, LUA_GCCOUNT, 0) * 1024 + lua_gc(L, LUA_GCCOUNTB, 0);
#endif

	// Do a full garbage collection cycle.
	lua_gc(L, LUA_GCCOLLECT, 0);

#if !defined(EXCLUDE_NORMAL_LOG)
	int fracUsage = lua_gc(L, LUA_GCCOUNTB, 0);
	int totalUsage = lua_gc(L, LUA_GCCOUNT, 0) * 1024 + fracUsage;
#endif

#if USE_RAW_LUA_ALLOCS
	// Nothing to do.
	#ifdef USE_GLOBAL_BUCKET_ALLOCATOR
	gLuaAlloc.cleanup();
	#endif
#endif

	CryComment("Lua garbage collection %i -> %i", beforeUsage, totalUsage);

	/*char sTemp[200];
	   lua_StateStats lss;
	   lua_getstatestats(L,&lss);
	   cry_sprintf(sTemp,"protos=%d closures=%d tables=%d udata=%d strings=%d\n",lss.nProto,lss.nClosure,lss.nHash,lss.nUdata,lss.nString);
	   OutputDebugString("BEFORE GC STATS :");
	   OutputDebugString(sTemp);*/

	//lua_setgcthreshold(L, 0);

	/*lua_getstatestats(L,&lss);
	   cry_sprintf(sTemp,"protos=%d closures=%d tables=%d udata=%d strings=%d\n",lss.nProto,lss.nClosure,lss.nHash,lss.nUdata,lss.nString);
	   OutputDebugString("AFTER GC STATS :");
	   OutputDebugString(sTemp);*/
}

int CScriptSystem::GetCGCount()
{
	//return lua_getgccount(L);
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
  delete this;
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
#if BB_PLATFORM_WINDOWS && 0
	if (m_pLuaDebugger)
		m_pLuaDebugger->AddBreakPoint(sFile, nLineNumber);
#endif
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// For debugger.
//////////////////////////////////////////////////////////////////////////
IScriptObject* CScriptSystem::GetLocalVariables(int nLevel/* = 0*/)
{
	lua_Debug ar;
	const char* name;
	IScriptObject* pObj = CreateEmptyObject();
	//pObj->AddRef();

	// Attach a new table
#if !defined(_RELEASE)
	const int checkStack = lua_gettop(L);
#endif
	lua_newtable(L);
	lua_pushvalue(L, -1);
	pObj->Attach();
	//AttachTable(pObj);

	// Get the stack frame
	while (lua_getstack(L, nLevel, &ar) != 0)
	{
		// Push a sub-table for this frame (recursive only)
    #if 0
		if (bRecursive)
		{
			assert(lua_istable(L, -1) && "The result table should be on the top of the stack");
			lua_pushinteger(L, nLevel);
			lua_newtable(L);
			lua_pushvalue(L, -1);
			lua_insert(L, -3);
			lua_rawset(L, -4);
			assert(lua_istable(L, -1) && lua_istable(L, -2) && lua_gettop(L) == checkStack + 2 && "There should now be two tables on the top of the stack");
		}
    #endif

		// Assign variable names and values for the current frame to the table on top of the stack
		int i = 1;
#if !defined(_RELEASE)
		const int checkInner = lua_gettop(L);
#endif
		//assert(checkInner == checkStack + 1 + bRecursive && "Too much stack space used");
		assert(lua_istable(L, -1) && "The target table must be on the top of the stack");
		while ((name = lua_getlocal(L, &ar, i++)) != NULL)
		{
			if (strcmp(name, "(*temporary)") == 0)
			{
				// Not interested in temporaries
				lua_pop(L, 1);
			}
			else
			{
				// Push the name, swap the top two items, and set in the table
				lua_pushstring(L, name);
				lua_insert(L, -2);
				assert(lua_gettop(L) == checkInner + 2 && "There should be a key-value pair on top of the stack");
				lua_rawset(L, -3);
			}
			assert(lua_gettop(L) == checkInner && "Unbalanced algorithm problem");
			assert(lua_istable(L, -1) && "The target table should be on the top of the stack");
		}

		// Pop the sub-table (recursive only)
    #if 0
		if (bRecursive)
		{
			assert(lua_istable(L, -1) && lua_istable(L, -2) && "There should now be two tables on the top of the stack");
			lua_pop(L, 1);
		}
		else break;
    #endif

		nLevel++;
	}

	// Pop the result table from the stack
	lua_pop(L, 1);
	assert(lua_gettop(L) == checkStack && "Unbalanced algorithm problem");
	return pObj;
}

const int LEVELS1 = 12; /* size of the first part of the stack */
const int LEVELS2 = 10; /* size of the second part of the stack */

void CScriptSystem::GetCallStack(std::vector<SLuaStackEntry>& callstack)
{
	callstack.clear();

	int level = 0;
	int firstpart = 1;  /* still before eventual `...' */
	lua_Debug ar;
	while (lua_getstack(L, level++, &ar))
	{

		char buff[512];  /* enough to fit following `cry_sprintf's */
		if (level == 2)
		{
			//luaL_addstring(&b, ("stack traceback:\n"));
		}
		else if (level > LEVELS1 && firstpart)
		{
			/* no more than `LEVELS2' more levels? */
			if (!lua_getstack(L, level + LEVELS2, &ar))
				level--;  /* keep going */
			else
			{
				//		luaL_addstring(&b, ("       ...\n"));  /* too many levels */
				while (lua_getstack(L, level + LEVELS2, &ar))  /* find last levels */
					level++;
			}
			firstpart = 0;
			continue;
		}

		sprintf(buff, ("%4d:  "), level - 1);

		lua_getinfo(L, ("Snl"), &ar);
		switch (*ar.namewhat)
		{
		case 'l':
			sprintf(buff, "function[local] `%.50s'", ar.name);
			break;
		case 'g':
			sprintf(buff, "function[global] `%.50s'", ar.name);
			break;
		case 'f':  /* field */
			sprintf(buff, "field `%.50s'", ar.name);
			break;
		case 'm':  /* field */
			sprintf(buff, "method `%.50s'", ar.name);
			break;
		case 't':  /* tag method */
			sprintf(buff, "`%.50s' tag method", ar.name);
			break;
		default:
			strcpy(buff, "");
			break;
		}

		SLuaStackEntry se;
		se.description = buff;
		se.line = ar.currentline;
		se.source = ar.source;

		callstack.push_back(se);
	}
}

IScriptObject* CScriptSystem::GetCallsStack()
{
	std::vector<SLuaStackEntry> stack;

	IScriptObject* pCallsStack = CreateObject();
	assert(pCallsStack);

	//pCallsStack->AddRef();
	GetCallStack(stack);

	for (size_t i = 0; i < stack.size(); ++i)
	{
		SmartScriptObject pEntry(this);

		pEntry->SetValue("description", stack[i].description.c_str());
		pEntry->SetValue("line", stack[i].line);
		pEntry->SetValue("sourcefile", stack[i].source.c_str());

		pCallsStack->PushBack(pEntry);
	}
	return pCallsStack;
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

void CScriptSystem::PushObject(const IScriptObject* pObj)
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

void CScriptSystem::AddFileToList(const char* sName)
{
  m_dqLoadedFiles.insert(sName);
}

void CScriptSystem::RemoveFileFromList(const ScriptFileListItor& itor)
{
  m_dqLoadedFiles.erase(itor);
}

void CScriptSystem::ShowDebugger(const char* pszSourceFile, int iLine, const char* pszReason)
{
}

SCRIPTSYSTEM_API IScriptSystem * CreateScriptSystem(ISystem* pSystem, bool bStdLibs)
{
  auto ss = new CScriptSystem();
  if (ss->Init(pSystem))
    return ss;
  else
    return nullptr;
}
