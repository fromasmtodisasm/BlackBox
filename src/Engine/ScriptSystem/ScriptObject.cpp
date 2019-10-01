#include <BlackBox/ScriptSystem/ScriptObject.hpp>
#include <BlackBox/ScriptSystem/StackGuard.hpp>

#include <BlackBox/IEngine.hpp>

#include <cassert>

lua_State* CScriptObject::L = nullptr;
IScriptSystem* CScriptObject::m_pSS = nullptr;

CScriptObject::~CScriptObject()
{
}

int CScriptObject::GetRef()
{
	return m_nRef;
}

void CScriptObject::Attach()
{
	if (m_nRef != NULL_REF)
		lua_unref(L, m_nRef);
	m_nRef = lua_ref(L, 1);
}

void CScriptObject::Attach(IScriptObject* so)
{
	PushRef(so);
	Attach();
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
	int top = lua_gettop(L);
	bool bDeepCopy = true;
	bool bCopyByReference = false;

	PushRef(pObj);
	PushRef();

	int srcTable = top + 1;
	int trgTable = top + 2;

	if (bDeepCopy)
	{
		if (bCopyByReference)
		{
			ReferenceTable_r(srcTable, trgTable);
		}
		else
		{
			CloneTable_r(srcTable, trgTable);
		}
	}
	else
	{
		CloneTable(srcTable, trgTable);
	}
	lua_settop(L, top); // Restore stack.

	return true;
}

void CScriptObject::Dump(IScriptObjectDumpSink* p)
{
}

bool CScriptObject::AddFunction(const char* sName, SCRIPT_FUNCTION pThunk, void *this_ptr, int nFuncID)
{
	PushRef();
	lua_pushstring(L, sName);

	//int8 nParamIdOffset = fd.nParamIdOffset;
	if (pThunk)
	{
		// Store functor in first upvalue.
		size_t tp = sizeof(this_ptr);
		size_t i = sizeof(nFuncID);
		member_ptr* pBuffer = (member_ptr*)lua_newuserdata(L, sizeof(member_ptr));
		pBuffer->fID = nFuncID;
		pBuffer->this_ptr = this_ptr;
		//memcpy(pBuffer, &nFuncID, sizeof(nFuncID));
		//memcpy(pBuffer + sizeof(nFuncID), this_ptr, sizeof(this_ptr));
		//memcpy(pBuffer + sizeof(fd.pFunctor) + 1, sFuncSignature, strlen(sFuncSignature) + 1);
		lua_pushcclosure(L, reinterpret_cast<lua_CFunction>(pThunk), 1);
		//lua_setglobal(L, sName);
	}

	lua_rawset(L, -3);
	lua_pop(L, 1); // pop table.
	return true;
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

void CScriptObject::CreateNew()
{
	lua_newtable(L);
	Attach();
}

void CScriptObject::SetMetatable(IScriptObject* pMetatable)
{
}

void CScriptObject::PushRef()
{
	if (m_nRef != DELETED_REF && m_nRef != NULL_REF)
		lua_getref(L, m_nRef);
	else
	{
		lua_pushnil(L);
		if (m_nRef == DELETED_REF)
		{
			assert(0 && "Access to deleted script object");
			CryFatalError("Access to deleted script object %x", (unsigned int)(UINT_PTR)this);
		}
		else
		{
			assert(0 && "Pushing Nil table reference");
			ScriptWarning("Pushing Nil table reference");
		}
	}
}

void CScriptObject::PushRef(IScriptObject* pObj)
{
	int nRef = ((CScriptObject*)pObj)->m_nRef;
	if (nRef != DELETED_REF)
		lua_getref(L, nRef);
	else
		CryFatalError("Access to deleted script object");

}

void CScriptObject::CloneTable(int srcTable, int trgTable)
{
}

void CScriptObject::CloneTable_r(int srcTable, int trgTable)
{
	CHECK_STACK(L);
	int top = lua_gettop(L);
	lua_pushnil(L);  // first key
	while (lua_next(L, srcTable) != 0)
	{
		if (lua_type(L, -1) == LUA_TTABLE)
		{
			int srct = lua_gettop(L);

			lua_pushvalue(L, -2); // Push again index.
			lua_newtable(L);      // Make value.
			int trgt = lua_gettop(L);
			CloneTable_r(srct, trgt);
			lua_rawset(L, trgTable); // Set new table to trgtable.
		}
		else
		{
			// `key' is at index -2 and `value' at index -1
			lua_pushvalue(L, -2); // Push again index.
			lua_pushvalue(L, -2); // Push value.
			lua_rawset(L, trgTable);
		}
		lua_pop(L, 1); // pop value, leave index.
	}
	lua_settop(L, top); // Restore stack.
}

void CScriptObject::ReferenceTable_r(int scrTable, int trgTable)
{
	assert(0 && __FUNCTION__" not implemented");
}
