#pragma once
#include <BlackBox/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/ScriptBinding.hpp>
#include <BlackBox/ScriptSystem/LuaCommon.hpp> 
#include <BlackBox/Utils.hpp>
#include <BlackBox/IConsole.hpp>

#ifdef _WIN32
#include <windows.h>
#endif // Win32

#include <vector>
#include <set>
#include <string>
#include <stack>
#include <cassert>

typedef std::set<std::string, stl::less_stricmp<std::string>> ScriptFileList;
typedef ScriptFileList::iterator															ScriptFileListItor;

class CFunctionHandler;


class CScriptSystem : public IScriptSystem
{
public:
	CScriptSystem();
	~CScriptSystem();

	bool Init(ISystem* pSystem);

	void RegisterErrorHandler(void);


	virtual bool ExecuteFile(const char* sFileName, bool bRaiseError = true, bool bForceReload = false) override;
	virtual bool ExecuteBuffer(const char* sBuffer, size_t nSize) override;
	virtual bool GetGlobalValue(const char* sKey, int& nVal) override;
	virtual bool GetGlobalValue(const char* sKey, float& fVal) override;
	virtual bool GetGlobalValue(const char* sKey, const char*& sVal) override;
	virtual IFunctionHandler* GetFunctionHandler() override;
	virtual HSCRIPT GetScriptHandle() override;
	virtual void UnloadScript(const char* sFileName) override;
	virtual void UnloadScripts() override;
	virtual bool ReloadScript(const char* sFileName, bool bRaiseError = true) override;
	virtual bool ReloadScripts() override;
	virtual void DumpLoadedScripts() override;
	//
	virtual IScriptObject* GetGlobalObject() override;
	virtual IScriptObject* CreateEmptyObject() override;
	virtual IScriptObject* CreateObject() override;
	virtual IScriptObject* CreateGlobalObject(const char* sName) override;
	//////////////////////////////////////////////////////////////////////////
	// Begin Call.
	//////////////////////////////////////////////////////////////////////////
	virtual int BeginCall(HSCRIPTFUNCTION hFunc) override;
	virtual int BeginCall(const char* sFuncName) override;
	virtual int BeginCall(const char* sTableName, const char* sFuncName) override;
	virtual int BeginCall(IScriptObject* pTable, const char* sFuncName) override;



	//////////////////////////////////////////////////////////////////////////
	// End Call.
	//////////////////////////////////////////////////////////////////////////
	virtual bool EndCall() override;
	virtual bool EndCall(int& nRet) override;
	virtual bool EndCall(float& fRet) override;
	virtual bool EndCall(const char*& sRet) override;
	virtual bool EndCall(bool& bRet) override;
	virtual bool EndCall(IScriptObject*& pScriptObject) override;

	//////////////////////////////////////////////////////////////////////////
	// Get function pointer.
	//////////////////////////////////////////////////////////////////////////
	virtual HSCRIPTFUNCTION GetFunctionPtr(const char* sFuncName) override;
	virtual HSCRIPTFUNCTION GetFunctionPtr(const char* sTableName, const char* sFuncName) override;
	virtual void ReleaseFunc(HSCRIPTFUNCTION f) override;
	virtual void PushFuncParam(int nVal) override;
	virtual void PushFuncParam(float fVal) override;
	virtual void PushFuncParam(const char* sVal) override;
	virtual void PushFuncParam(bool bVal) override;
	virtual void PushFuncParam(IScriptObject* pVal) override;
	//
	virtual void SetGlobalValue(const char* sKey, int nVal) override;
	virtual void SetGlobalValue(const char* sKey, float fVal) override;
	virtual void SetGlobalValue(const char* sKey, const char* sVal) override;
	virtual void SetGlobalValue(const char* sKey, IScriptObject* pObj) override;
	virtual bool GetGlobalValue(const char* sKey, IScriptObject* pObj) override;
	virtual void SetGlobalToNull(const char* sKey) override;
	//
	virtual HTAG CreateTaggedValue(const char* sKey, int* pVal) override;
	virtual HTAG CreateTaggedValue(const char* sKey, float* pVal) override;
	virtual HTAG CreateTaggedValue(const char* sKey, char* pVal) override;
	virtual USER_DATA CreateUserData(void *ptr, int size) override;
	virtual void RemoveTaggedValue(HTAG tag) override;
	//
	virtual void RaiseError(const char* sErr, ...) override;
	virtual void ForceGarbageCollection() override;
	virtual int GetCGCount() override;
	virtual void SetGCThreshhold(int nKb) override;
	virtual void UnbindUserdata() override;
	virtual void Release() override;
	virtual void EnableDebugger(IScriptDebugSink* pDebugSink) override;
	virtual IScriptObject* GetBreakPoints() override;
	virtual HBREAKPOINT AddBreakPoint(const char* sFile, int nLineNumber) override;
	virtual IScriptObject* GetLocalVariables(int nLevel = 0) override;
	virtual IScriptObject* GetCallsStack() override;
	virtual void DebugContinue() override;
	virtual void DebugStepNext() override;
	virtual void DebugStepInto() override;
	virtual void DebugDisable() override;
	virtual BreakState GetBreakState() override;
	virtual void GetMemoryStatistics(ICrySizer* pSizer) override;
	virtual void GetScriptHash(const char* sPath, const char* szKey, unsigned int& dwHash) override;
	virtual void PostInit() override;

	void TraceScriptError(const char* file, int line, const char* errorStr);

	void PushObject(IScriptObject* pObj);

	virtual void LogStackTrace();

public:
	inline void ToAny(bool& val, int nIdx)
	{
		val = lua_toboolean(L, nIdx);
	}

	inline void ToAny(int& val, int nIdx)
	{
		val = static_cast<int>(lua_tointeger(L, nIdx));
	}

	inline void ToAny(float& val, int nIdx)
	{
		val = static_cast<float>(lua_tonumber(L, nIdx));
	}

	inline void ToAny(const char*& val, int nIdx)
	{
		val = lua_tostring(L, nIdx);
	}

	inline void ToAny(HSCRIPTFUNCTION& val, int nIdx)
	{
		lua_pushvalue(L, nIdx);
		val = (HSCRIPTFUNCTION)(INT_PTR)lua_ref(L, 1);
	}

	inline void ToAny(IScriptObject*& pObj, int nIdx)
	{
		if (lua_istable(L, -1))
		{
			pObj = CreateEmptyObject();
			//pObj->AddRef();
		}
		lua_pushvalue(L, -1);
		pObj->Attach();
	}
	template<typename T> bool PopAny(T & val)
	{
		/*bool res = */ToAny(val, -1);
		lua_pop(L, 1);
		return true;
	}
	///////////////////////////////////////////////////////////////////

	inline void PushAny(bool& val)
	{
		lua_pushboolean(L, val);
	}

	inline void PushAny(int& val)
	{
		lua_pushinteger(L, val);
	}

	inline void PushAny(float& val)
	{
		lua_pushnumber(L, val);
	}

	inline void PushAny(const char*& val)
	{
		lua_pushstring(L, val);
	}

	inline void PushAny(HSCRIPTFUNCTION & val)
	{
		lua_getref(L, val);
		assert(lua_type(L, -1) == LUA_TFUNCTION);
	}

	inline void PushAny(IScriptObject * &val)
	{
		PushObject(val);
	}




private:
	bool       EndCallN(int nReturns);
	template<typename T>
	bool       EndCallAny(T& any);
	template<typename T>
	bool       EndCallAnyN(T*& any);
	template<typename T>
	void PushFuncParamAny(T val);

	static int ErrorHandler(lua_State* L);


private:
	static CScriptSystem* s_mpScriptSystem;
	lua_State* L;
	ICVar* m_cvar_script_debugger; // Stores current debugging mode
	ICVar* m_cvar_script_coverage;
	int                   m_nTempArg;
	int                   m_nTempTop;

	IScriptObject* m_pUserDataMetatable;
	IScriptObject* m_pPreCacheBufferTable;
	std::vector<std::string>   m_vecPreCached;

	HSCRIPTFUNCTION       m_pErrorHandlerFunc = 0;

	ScriptFileList        m_dqLoadedFiles;

	CScriptBindings       m_stdScriptBinds;
	ISystem* m_pSystem;

	float                 m_fGCFreq;      //!< relative time in seconds
	float                 m_lastGCTime;   //!< absolute time in seconds
	int                   m_nLastGCCount; //!<
	int                   m_forceReloadCount;

	//CScriptTimerMgr* m_pScriptTimerMgr;

	// Store a simple callstack that can be inspected in C++ debugger
	const static int			MAX_CALLDEPTH = 32;
	int										m_nCallDepth;
	std::stack<std::string>     m_sCallDescriptions[MAX_CALLDEPTH];
private:
	static CFunctionHandler *m_pH;


	// Inherited via IScriptSystem
	virtual void ShowDebugger(const char* pszSourceFile, int iLine, const char* pszReason) override;

};

template<typename T>
inline bool CScriptSystem::EndCallAny(T& any)
{
	//CHECK_STACK(L);
	if (!EndCallN(1))
		return false;
	return PopAny(any);
}

template<typename T>
inline bool CScriptSystem::EndCallAnyN(T*& anys)
{
	if (!EndCallN(n))
		return false;
	for (int i = 0; i < n; i++)
	{
		if (!PopAny(anys[i]))
			return false;
	}
	return true;
}

template<typename T>
inline void CScriptSystem::PushFuncParamAny(T val)
{
	if (m_nTempArg == -1)
		return;
	PushAny(val);
	m_nTempArg++;
}
