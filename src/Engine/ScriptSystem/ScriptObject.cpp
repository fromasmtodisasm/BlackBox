#include <BlackBox/ScriptSystem/ScriptObject.hpp>

CScriptObject::CScriptObject()
{
}

CScriptObject::~CScriptObject()
{
}

int CScriptObject::GetRef()
{
	return 0;
}

void CScriptObject::Attach()
{
}

void CScriptObject::Attach(IScriptObject* so)
{
}

void CScriptObject::Delegate(IScriptObject* pObj)
{
}

void CScriptObject::PushBack(int nVal)
{
}

void CScriptObject::PushBack(float fVal)
{
}

void CScriptObject::PushBack(const char* sVal)
{
}

void CScriptObject::PushBack(bool bVal)
{
}

void CScriptObject::PushBack(IScriptObject* pObj)
{
}

void CScriptObject::SetValue(const char* sKey, int nVal)
{
}

void CScriptObject::SetValue(const char* sKey, float fVal)
{
}

void CScriptObject::SetValue(const char* sKey, const char* sVal)
{
}

void CScriptObject::SetValue(const char* sKey, bool bVal)
{
}

void CScriptObject::SetValue(const char* sKey, IScriptObject* pObj)
{
}

void CScriptObject::SetValue(const char* sKey, USER_DATA ud)
{
}

void CScriptObject::SetToNull(const char* sKey)
{
}

bool CScriptObject::GetValue(const char* sKey, int& nVal)
{
	return false;
}

bool CScriptObject::GetValue(const char* sKey, float& fVal)
{
	return false;
}

bool CScriptObject::GetValue(const char* sKey, bool& bVal)
{
	return false;
}

bool CScriptObject::GetValue(const char* sKey, const char*& sVal)
{
	return false;
}

bool CScriptObject::GetValue(const char* sKey, IScriptObject* pObj)
{
	return false;
}

bool CScriptObject::GetValue(const char* sKey, HSCRIPTFUNCTION& funcVal)
{
	return false;
}

bool CScriptObject::GetUDValue(const char* sKey, USER_DATA& nValue, int& nCookie)
{
	return false;
}

bool CScriptObject::GetFuncData(const char* sKey, unsigned int*& pCode, int& iSize)
{
	return false;
}

bool CScriptObject::BeginSetGetChain()
{
	return false;
}

bool CScriptObject::GetValueChain(const char* sKey, int& nVal)
{
	return false;
}

bool CScriptObject::GetValueChain(const char* sKey, float& fVal)
{
	return false;
}

bool CScriptObject::GetValueChain(const char* sKey, bool& bVal)
{
	return false;
}

bool CScriptObject::GetValueChain(const char* sKey, const char*& sVal)
{
	return false;
}

bool CScriptObject::GetValueChain(const char* sKey, IScriptObject* pObj)
{
	return false;
}

bool CScriptObject::GetValueChain(const char* sKey, HSCRIPTFUNCTION& funcVal)
{
	return false;
}

bool CScriptObject::GetUDValueChain(const char* sKey, USER_DATA& nValue, int& nCookie)
{
	return false;
}

void CScriptObject::SetValueChain(const char* sKey, int nVal)
{
}

void CScriptObject::SetValueChain(const char* sKey, float fVal)
{
}

void CScriptObject::SetValueChain(const char* sKey, const char* sVal)
{
}

void CScriptObject::SetValueChain(const char* sKey, bool bVal)
{
}

void CScriptObject::SetValueChain(const char* sKey, IScriptObject* pObj)
{
}

void CScriptObject::SetValueChain(const char* sKey, USER_DATA ud)
{
}

void CScriptObject::SetToNullChain(const char* sKey)
{
}

void CScriptObject::EndSetGetChain()
{
}

ScriptVarType CScriptObject::GetValueType(const char* sKey)
{
	return ScriptVarType();
}

ScriptVarType CScriptObject::GetAtType(int nIdx)
{
	return ScriptVarType();
}

void CScriptObject::SetAt(int nIdx, int nVal)
{
}

void CScriptObject::SetAt(int nIdx, float fVal)
{
}

void CScriptObject::SetAt(int nIdx, bool bVal)
{
}

void CScriptObject::SetAt(int nIdx, const char* sVal)
{
}

void CScriptObject::SetAt(int nIdx, IScriptObject* pObj)
{
}

void CScriptObject::SetAtUD(int nIdx, USER_DATA nValue)
{
}

void CScriptObject::SetNullAt(int nIdx)
{
}

bool CScriptObject::GetAt(int nIdx, int& nVal)
{
	return false;
}

bool CScriptObject::GetAt(int nIdx, float& fVal)
{
	return false;
}

bool CScriptObject::GetAt(int nIdx, bool& bVal)
{
	return false;
}

bool CScriptObject::GetAt(int nIdx, const char*& sVal)
{
	return false;
}

bool CScriptObject::GetAt(int nIdx, IScriptObject* pObj)
{
	return false;
}

bool CScriptObject::GetAtUD(int nIdx, USER_DATA& nValue, int& nCookie)
{
	return false;
}

bool CScriptObject::BeginIteration()
{
	return false;
}

bool CScriptObject::MoveNext()
{
	return false;
}

bool CScriptObject::GetCurrent(int& nVal)
{
	return false;
}

bool CScriptObject::GetCurrent(float& fVal)
{
	return false;
}

bool CScriptObject::GetCurrent(bool& bVal)
{
	return false;
}

bool CScriptObject::GetCurrent(const char*& sVal)
{
	return false;
}

bool CScriptObject::GetCurrent(IScriptObject* pObj)
{
	return false;
}

bool CScriptObject::GetCurrentPtr(const void*& pObj)
{
	return false;
}

bool CScriptObject::GetCurrentFuncData(unsigned int*& pCode, int& iSize)
{
	return false;
}

bool CScriptObject::GetCurrentKey(const char*& sVal)
{
	return false;
}

bool CScriptObject::GetCurrentKey(int& nKey)
{
	return false;
}

ScriptVarType CScriptObject::GetCurrentType()
{
	return ScriptVarType();
}

void CScriptObject::EndIteration()
{
}

void CScriptObject::SetNativeData(void*)
{
}

void* CScriptObject::GetNativeData()
{
	return nullptr;
}

void CScriptObject::Clear()
{
}

int CScriptObject::Count()
{
	return 0;
}

bool CScriptObject::Clone(IScriptObject* pObj)
{
	return false;
}

void CScriptObject::Dump(IScriptObjectDumpSink* p)
{
}

bool CScriptObject::AddFunction(const char* sName, SCRIPT_FUNCTION pThunk, int nFuncID)
{
	return false;
}

bool CScriptObject::AddSetGetHandlers(SCRIPT_FUNCTION pSetThunk, SCRIPT_FUNCTION pGetThunk)
{
	return false;
}

void CScriptObject::RegisterParent(IScriptObjectSink* pSink)
{
}

void CScriptObject::Detach()
{
}

void CScriptObject::Release()
{
}

bool CScriptObject::GetValueRecursive(const char* szPath, IScriptObject* pObj)
{
	return false;
}
