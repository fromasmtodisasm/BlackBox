#pragma once
#include <BlackBox/IScriptSystem.hpp>
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

typedef std::set<std::string, stl::less_stricmp<std::string>> ScriptFileList;
typedef ScriptFileList::iterator															ScriptFileListItor;

class CFunctionHandler;


class CScriptSystem : public IScriptSystem
{
public:
	CScriptSystem();
	~CScriptSystem();

	bool Init(ISystem* pSystem);

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
	//
	virtual int BeginCall(HSCRIPTFUNCTION hFunc) override;
	virtual int BeginCall(const char* sFuncName) override;
	virtual int BeginCall(const char* sTableName, const char* sFuncName) override;
	virtual void EndCall() override;
	virtual void EndCall(int& nRet) override;
	virtual void EndCall(float& fRet) override;
	virtual void EndCall(const char*& sRet) override;
	virtual void EndCall(bool& bRet) override;
	virtual void EndCall(IScriptObject* pScriptObject) override;
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
	virtual USER_DATA CreateUserData(INT_PTR nVal, int nCookie) override;
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

	void PushObject(IScriptObject* pObj);
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

	HSCRIPTFUNCTION       m_pErrorHandlerFunc;

	ScriptFileList        m_dqLoadedFiles;

	//CScriptBindings       m_stdScriptBinds;
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

};

