#pragma once
#include <BlackBox/IScriptSystem.hpp>

#ifdef _WIN32
#include <windows.h>
#endif // Win32


#include <lua.hpp>
#include <lualib.h>

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
	virtual IScriptObject* GetGlobalObject() override;
	virtual IScriptObject* CreateEmptyObject() override;
	virtual IScriptObject* CreateObject() override;
	virtual IScriptObject* CreateGlobalObject(const char* sName) override;
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
	virtual void SetGlobalValue(const char* sKey, int nVal) override;
	virtual void SetGlobalValue(const char* sKey, float fVal) override;
	virtual void SetGlobalValue(const char* sKey, const char* sVal) override;
	virtual void SetGlobalValue(const char* sKey, IScriptObject* pObj) override;
	virtual bool GetGlobalValue(const char* sKey, IScriptObject* pObj) override;
	virtual void SetGlobalToNull(const char* sKey) override;
	virtual HTAG CreateTaggedValue(const char* sKey, int* pVal) override;
	virtual HTAG CreateTaggedValue(const char* sKey, float* pVal) override;
	virtual HTAG CreateTaggedValue(const char* sKey, char* pVal) override;
	virtual USER_DATA CreateUserData(INT_PTR nVal, int nCookie) override;
	virtual void RemoveTaggedValue(HTAG tag) override;
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
private:
	ISystem* m_System;
	lua_State* L;
	static CFunctionHandler *m_pH;

};

