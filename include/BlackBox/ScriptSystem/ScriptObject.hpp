#pragma once
#include <BlackBox/IScriptSystem.hpp>
#include <BlackBox/ScriptSystem/LuaCommon.hpp> 

class CScriptSystem;

enum
{
	DELETED_REF = -1,
	NULL_REF = 0,
};

class CScriptObject : public IScriptObject 
{
public:
	CScriptObject() { m_nRef = NULL_REF; m_nRefCount = 0; }
	~CScriptObject(); 
	// Унаследовано через IScriptObject
	virtual int GetRef() override; 
	virtual void Attach() override; 
	virtual void Attach(IScriptObject* so) override; 
	virtual void Delegate(IScriptObject* pObj) override; 
	// PushBack
	virtual void PushBack(int nVal) override; 
	virtual void PushBack(float fVal) override; 
	virtual void PushBack(const char* sVal) override; 
	virtual void PushBack(bool bVal) override; 
	virtual void PushBack(IScriptObject* pObj) override; 
	// SetValue
	virtual void SetValue(const char* sKey, int nVal) override; 
	virtual void SetValue(const char* sKey, float fVal) override; 
	virtual void SetValue(const char* sKey, const char* sVal) override; 
	virtual void SetValue(const char* sKey, bool bVal) override; 
	virtual void SetValue(const char* sKey, IScriptObject* pObj) override; 
	virtual void SetValue(const char* sKey, USER_DATA ud) override; 
	virtual void SetToNull(const char* sKey) override; 
	// GetValue
	virtual bool GetValue(const char* sKey, int& nVal) override; 
	virtual bool GetValue(const char* sKey, float& fVal) override; 
	virtual bool GetValue(const char* sKey, bool& bVal) override; 
	virtual bool GetValue(const char* sKey, const char*& sVal) override; 
	virtual bool GetValue(const char* sKey, IScriptObject* pObj) override; 
	virtual bool GetValue(const char* sKey, HSCRIPTFUNCTION& funcVal) override; 
	virtual bool GetUDValue(const char* sKey, USER_DATA& nValue, int& nCookie) override; 
	virtual bool GetFuncData(const char* sKey, unsigned int*& pCode, int& iSize) override; 
	// Chain
	virtual bool BeginSetGetChain() override; 
	virtual bool GetValueChain(const char* sKey, int& nVal) override; 
	virtual bool GetValueChain(const char* sKey, float& fVal) override; 
	virtual bool GetValueChain(const char* sKey, bool& bVal) override; 
	virtual bool GetValueChain(const char* sKey, const char*& sVal) override; 
	virtual bool GetValueChain(const char* sKey, IScriptObject* pObj) override; 
	virtual bool GetValueChain(const char* sKey, HSCRIPTFUNCTION& funcVal) override; 
	virtual bool GetUDValueChain(const char* sKey, USER_DATA& nValue, int& nCookie) override; 
	virtual void SetValueChain(const char* sKey, int nVal) override; 
	virtual void SetValueChain(const char* sKey, float fVal) override; 
	virtual void SetValueChain(const char* sKey, const char* sVal) override; 
	virtual void SetValueChain(const char* sKey, bool bVal) override; 
	virtual void SetValueChain(const char* sKey, IScriptObject* pObj) override; 
	virtual void SetValueChain(const char* sKey, USER_DATA ud) override; 
	virtual void SetToNullChain(const char* sKey) override; 
	virtual void EndSetGetChain() override; 

	virtual ScriptVarType GetValueType(const char* sKey) override; 
	virtual ScriptVarType GetAtType(int nIdx) override; 
	// SetAt
	virtual void SetAt(int nIdx, int nVal) override; 
	virtual void SetAt(int nIdx, float fVal) override; 
	virtual void SetAt(int nIdx, bool bVal) override; 
	virtual void SetAt(int nIdx, const char* sVal) override; 
	virtual void SetAt(int nIdx, IScriptObject* pObj) override; 
	virtual void SetAtUD(int nIdx, USER_DATA nValue) override; 
	virtual void SetNullAt(int nIdx) override; 
	// GetAt
	virtual bool GetAt(int nIdx, int& nVal) override; 
	virtual bool GetAt(int nIdx, float& fVal) override; 
	virtual bool GetAt(int nIdx, bool& bVal) override; 
	virtual bool GetAt(int nIdx, const char*& sVal) override; 
	virtual bool GetAt(int nIdx, IScriptObject* pObj) override; 
	virtual bool GetAtUD(int nIdx, USER_DATA& nValue, int& nCookie) override; 
	virtual bool BeginIteration() override; 
	virtual bool MoveNext() override; 
	// GetCurrent
	virtual bool GetCurrent(int& nVal) override; 
	virtual bool GetCurrent(float& fVal) override; 
	virtual bool GetCurrent(bool& bVal) override; 
	virtual bool GetCurrent(const char*& sVal) override; 
	virtual bool GetCurrent(IScriptObject* pObj) override; 
	virtual bool GetCurrentPtr(const void*& pObj) override; 
	virtual bool GetCurrentFuncData(unsigned int*& pCode, int& iSize) override; 
	virtual bool GetCurrentKey(const char*& sVal) override; 
	virtual bool GetCurrentKey(int& nKey) override; 
	virtual ScriptVarType GetCurrentType() override; 

	virtual void EndIteration() override; 
	virtual void SetNativeData(void*) override; 
	virtual void* GetNativeData() override; 
	virtual void Clear() override; 
	virtual int Count() override; 
	virtual bool Clone(IScriptObject* pObj) override; 
	virtual void Dump(IScriptObjectDumpSink* p) override; 
	virtual bool AddFunction(const char* sName, SCRIPT_FUNCTION pThunk, int nFuncID) override; 
	virtual bool AddSetGetHandlers(SCRIPT_FUNCTION pSetThunk, SCRIPT_FUNCTION pGetThunk) override; 
	virtual void RegisterParent(IScriptObjectSink* pSink) override; 
	virtual void Detach() override; 
	virtual void Release() override; 
	virtual bool GetValueRecursive(const char* szPath, IScriptObject* pObj) override; 
public:
	// --------------------------------------------------------------------------
	void CreateNew();
	// Create object from pool.
	void Recreate() { m_nRef = NULL_REF; m_nRefCount = 1; };
	// Assign a metatable to a table.
	void SetMetatable(IScriptObject* pMetatable);
	// Push reference of this object to the stack.
	void PushRef();
	// Push reference to specified script table to the stack.
	void PushRef(IScriptObject* pObj);
private:
	static void CloneTable(int srcTable, int trgTable);
	static void CloneTable_r(int srcTable, int trgTable);
	static void ReferenceTable_r(int scrTable, int trgTable);



public:
	// Lua state, set by CScriptSystem::Init
	static lua_State* L;
	// Pointer to ScriptSystem, set by CScriptSystem::Init
	static CScriptSystem* m_pSS; 
	void* m_pNativeData;
	struct member_ptr
	{
		int fID;
		int ref;
	}m_member_ptr; 

private:
	int m_nRef;
	int m_nRefCount;

};
