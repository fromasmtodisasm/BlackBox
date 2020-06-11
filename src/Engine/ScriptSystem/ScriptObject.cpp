#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/ScriptSystem/ScriptObject.hpp>
#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
#include <BlackBox/ScriptSystem/StackGuard.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <cassert>

lua_State* CScriptObject::L = nullptr;
CScriptSystem* CScriptObject::m_pSS = nullptr;

///////////////////////////////////////////////////////////////////
namespace {
  ///////////////////////////////////////////////////////////////////
  template<typename T>
  bool GetValueAny(CScriptObject* pSO, const char* sKey, T& Val, bool bChain = false)
  {
    auto L = CScriptObject::L;
    CHECK_STACK(L);
    int top = lua_gettop(L);
    if (!bChain)
      pSO->PushRef();
    lua_pushstring(L, sKey);
    lua_gettable(L, -2);
    bool res = CScriptObject::m_pSS->PopAny(Val);
    lua_settop(L, top);
    return true;
  }
  ///////////////////////////////////////////////////////////////////

  template<typename T>
  bool PushAnyTemplate(T& val)
  {
    CScriptObject::m_pSS->PushAny(val);
    //lua_pop(CScriptObject::L, 1);
    return true;
  }
  ///////////////////////////////////////////////////////////////////
  template<typename T>
  void SetValueTemplate(CScriptObject* pSO, const char* sKey, T& Val, bool bChain = false)
  {
    auto L = CScriptObject::L;
    CHECK_STACK(L);
    int top = lua_gettop(L);

    T oldValue;
    if (top && lua_getmetatable(L, -1))     // if there is no metatable nothing is pushed
    {
      lua_pop(L, 1);    // pop the metatable - we only care that it exists, not about the value
      if (GetValueAny(pSO, sKey, oldValue) && oldValue == Val)
        return;
    }

    if (!bChain)
      pSO->PushRef();

    assert(sKey);
    size_t len = strlen(sKey);
    lua_pushlstring(L, sKey, len);
    PushAnyTemplate(Val);
    lua_settable(L, -3);
    lua_settop(L, top);
  }
  ///////////////////////////////////////////////////////////////////

  template<typename T>
  void SetAtAny(CScriptObject* pSO, int nIndex, T& any)
  {
    auto L = CScriptObject::L;
    CHECK_STACK(L);
    pSO->PushRef();
    CScriptObject::m_pSS->PushAny(any);
    lua_rawseti(L, -2, nIndex);
    lua_pop(L, 1); // Pop table.
  }

  template<typename T>
  bool GetAtAny(CScriptObject* pSO, int nIndex, T& any)
  {
    auto L = CScriptObject::L;
    CHECK_STACK(L);
    pSO->PushRef();
    lua_rawgeti(L, -1, nIndex);
    bool res = CScriptObject::m_pSS->PopAny(any);
    lua_pop(L, 1); // Pop table.

    return res;
  }
  ///////////////////////////////////////////////////////////////////
}

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

void CScriptObject::Delegate(IScriptObject* pMetatable)
{
  if (!pMetatable)
    return;

  CHECK_STACK(L);

  PushRef(pMetatable);
  lua_pushstring(L, "__index"); // push key.
  PushRef(pMetatable);
  lua_rawset(L, -3); // sets metatable.__index = metatable

  lua_pushstring(L, "__newindex"); // push key.
  PushRef(pMetatable);
  lua_rawset(L, -3); // sets metatable.__newindex = metatable
  lua_pop(L, 1);     // pop metatable from stack.

  SetMetatable(pMetatable);
}

void CScriptObject::PushBack(int nVal)
{
	m_pSS->PushAny(nVal);
}

void CScriptObject::PushBack(float fVal)
{
	m_pSS->PushAny(fVal);
}

void CScriptObject::PushBack(const char* sVal)
{
	m_pSS->PushAny(sVal);
}

void CScriptObject::PushBack(bool bVal)
{
	m_pSS->PushAny(bVal);
}

void CScriptObject::PushBack(IScriptObject* pObj)
{
	m_pSS->PushAny(pObj);
}

void CScriptObject::SetValue(const char* sKey, int Val)
{
  SetValueTemplate(this, sKey, Val);
}

void CScriptObject::SetValue(const char* sKey, float Val)
{
  SetValueTemplate(this, sKey, Val);
}

void CScriptObject::SetValue(const char* sKey, const char* Val)
{
  SetValueTemplate(this, sKey, Val);
}

void CScriptObject::SetValue(const char* sKey, bool Val)
{
  SetValueTemplate(this, sKey, Val);
}

void CScriptObject::SetValue(const char* sKey, IScriptObject* Val)
{
  SetValueTemplate(this, sKey, Val);
}

void CScriptObject::SetValue(const char* sKey, USER_DATA ud)
{
  SetValueTemplate(this, sKey, ud);
}

void CScriptObject::SetToNull(const char* sKey)
{
  //assert(0 && __FUNCTION__ " Not implemented");
}

bool CScriptObject::GetValue(const char* sKey, int& nVal)
{
  return GetValueAny(this, sKey, nVal);
}

bool CScriptObject::GetValue(const char* sKey, float& fVal)
{
  return GetValueAny(this, sKey, fVal);
}

bool CScriptObject::GetValue(const char* sKey, bool& bVal)
{
  return GetValueAny(this, sKey, bVal);
}

bool CScriptObject::GetValue(const char* sKey, const char*& sVal)
{
  return GetValueAny(this, sKey, sVal);
}

bool CScriptObject::GetValue(const char* sKey, IScriptObject* pObj)
{
  return GetValueAny(this, sKey, pObj);
}

bool CScriptObject::GetValue(const char* sKey, HSCRIPTFUNCTION& funcVal)
{
  return GetValueAny(this, sKey, funcVal);
}

bool CScriptObject::GetUDValue(const char* sKey, USER_DATA& nValue, int& nCookie)
{
  auto result =  GetValueAny(this, sKey, nValue);
	if (result && nValue != nullptr)
	{
		UserDataInfo* ud = (UserDataInfo*)nValue;
		nCookie = ud->cookie;
		nValue = (void*)ud->ptr;
	}
	return result;
}

bool CScriptObject::GetFuncData(const char* sKey, unsigned int*& pCode, int& iSize)
{
  return false;
}

bool CScriptObject::BeginSetGetChain()
{
  PushRef();
  return true;
}

bool CScriptObject::GetValueChain(const char* sKey, int& Val)
{
  return GetValueAny(this, sKey, Val, true);
}

bool CScriptObject::GetValueChain(const char* sKey, float& Val)
{
  return GetValueAny(this, sKey, Val, true);
}

bool CScriptObject::GetValueChain(const char* sKey, bool& Val)
{
  return GetValueAny(this, sKey, Val, true);
}

bool CScriptObject::GetValueChain(const char* sKey, const char*& Val)
{
  return GetValueAny(this, sKey, Val, true);
}

bool CScriptObject::GetValueChain(const char* sKey, IScriptObject* Val)
{
  return GetValueAny(this, sKey, Val, true);
}

bool CScriptObject::GetValueChain(const char* sKey, HSCRIPTFUNCTION& Val)
{
  return GetValueAny(this, sKey, Val, true);
}

bool CScriptObject::GetUDValueChain(const char* sKey, USER_DATA& nValue, int& nCookie)
{
  return GetValueAny(this, sKey, nValue, true);
}

void CScriptObject::SetValueChain(const char* sKey, int Val)
{
  SetValueTemplate(this, sKey, Val, true);
}

void CScriptObject::SetValueChain(const char* sKey, float Val)
{
  SetValueTemplate(this, sKey, Val, true);
}

void CScriptObject::SetValueChain(const char* sKey, const char* Val)
{
  SetValueTemplate(this, sKey, Val, true);
}

void CScriptObject::SetValueChain(const char* sKey, bool Val)
{
  SetValueTemplate(this, sKey, Val, true);
}

void CScriptObject::SetValueChain(const char* sKey, IScriptObject* Val)
{
  SetValueTemplate(this, sKey, Val, true);
}

void CScriptObject::SetValueChain(const char* sKey, USER_DATA ud)
{
  SetValueTemplate(this, sKey, ud, true);
}

void CScriptObject::SetToNullChain(const char* sKey)
{
  //assert(0 && __FUNCTION__" Not implemented");
}

void CScriptObject::EndSetGetChain()
{
  if (lua_istable(L, -1))
    lua_pop(L, 1);
  else
  {
    assert(0 && "Mismatch in Set/Get Chain");
  }
}

ScriptVarType CScriptObject::GetValueType(const char* sKey)
{
  CHECK_STACK(L);
  ScriptVarType type = ScriptVarType::Null;

  PushRef();
  lua_pushstring(L, sKey);
  lua_gettable(L, -2);
  int luatype = lua_type(L, -1);
  switch (luatype)
  {
  case LUA_TNIL:
    type = ScriptVarType::Null;
    break;
  case LUA_TBOOLEAN:
    type = ScriptVarType::Bool;
    break;
  case LUA_TNUMBER:
    type = ScriptVarType::Number;
    break;
  case LUA_TSTRING:
    type = ScriptVarType::String;
    break;
  case LUA_TFUNCTION:
    type = ScriptVarType::Function;
    break;
  case LUA_TLIGHTUSERDATA:
    type = ScriptVarType::Pointer;
    break;
  case LUA_TTABLE:
    type = ScriptVarType::Object;
    break;
  }
  lua_pop(L, 2); // Pop value and table.
  return type;
}

ScriptVarType CScriptObject::GetAtType(int nIdx)
{
  CHECK_STACK(L);
  ScriptVarType svtRetVal = ScriptVarType::Null;
  PushRef();

  if (luaL_len(L, -1) < nIdx)
  {
    lua_pop(L, 1);
    return ScriptVarType::Null;
  }

  lua_rawgeti(L, -1, nIdx);

  switch (lua_type(L, -1))
  {
  case LUA_TNIL:
    svtRetVal = ScriptVarType::Null;
    break;
  case LUA_TBOOLEAN:
    svtRetVal = ScriptVarType::Bool;
    break;
  case LUA_TNUMBER:
    svtRetVal = ScriptVarType::Number;
    break;
  case LUA_TSTRING:
    svtRetVal = ScriptVarType::String;
    break;
  case LUA_TTABLE:
    svtRetVal = ScriptVarType::Object;
    break;
  case LUA_TFUNCTION:
    svtRetVal = ScriptVarType::Function;
    break;
  }

  lua_pop(L, 2);
  return svtRetVal;
}

void CScriptObject::SetAt(int nIdx, int Val)
{
  SetAtAny(this, nIdx, Val);
}

void CScriptObject::SetAt(int nIdx, float Val)
{
  SetAtAny(this, nIdx, Val);
}

void CScriptObject::SetAt(int nIdx, bool Val)
{
  SetAtAny(this, nIdx, Val);
}

void CScriptObject::SetAt(int nIdx, const char* Val)
{
  SetAtAny(this, nIdx, Val);
}

void CScriptObject::SetAt(int nIdx, IScriptObject* Val)
{
  SetAtAny(this, nIdx, Val);
}

void CScriptObject::SetAtUD(int nIdx, USER_DATA nValue)
{
  SetAtAny(this, nIdx, nValue);
}

void CScriptObject::SetNullAt(int nIdx)
{
}

bool CScriptObject::GetAt(int nIdx, int& Val)
{
  return GetAtAny(this, nIdx, Val);
}

bool CScriptObject::GetAt(int nIdx, float& Val)
{
  return GetAtAny(this, nIdx, Val);
}

bool CScriptObject::GetAt(int nIdx, bool& Val)
{
  return GetAtAny(this, nIdx, Val);
}

bool CScriptObject::GetAt(int nIdx, const char*& Val)
{
  return GetAtAny(this, nIdx, Val);
}

bool CScriptObject::GetAt(int nIdx, IScriptObject* Val)
{
  return GetAtAny(this, nIdx, Val);
}

bool CScriptObject::GetAtUD(int nIdx, USER_DATA& nValue, int& nCookie)
{
	UserDataInfo *ud;
  auto result = GetAtAny(this, nIdx, (USER_DATA&)ud);
	nValue = (USER_DATA&)ud->ptr;
	nCookie = ud->cookie;
	return result;
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

void CScriptObject::SetNativeData(void* data)
{
  PushRef();
  lua_pushstring(L, "__this");
  lua_pushlightuserdata(L, data);
  lua_rawset(L, -3);
  lua_pop(L, 1);
}

void* CScriptObject::GetNativeData()
{
  void* result = nullptr;
  PushRef();
  lua_pushstring(L, "__this");
  lua_rawget(L, -2);
  if (lua_islightuserdata(L, -1))
  {
    result = lua_touserdata(L, -1);
  }
  lua_pop(L, 1);
  return result;
}

void CScriptObject::Clear()
{
  CHECK_STACK(L);

  PushRef();
  int trgTable = lua_gettop(L);

  lua_pushnil(L);  // first key
  while (lua_next(L, trgTable) != 0)
  {
    lua_pop(L, 1);        // pop value, leave index.
    lua_pushvalue(L, -1); // Push again index.
    lua_pushnil(L);
    lua_rawset(L, trgTable);
  }
  assert(lua_istable(L, -1));
  lua_pop(L, 1);
}

int CScriptObject::Count()
{
  CHECK_STACK(L);

  PushRef();
  int count = static_cast<int>(luaL_len(L, -1));
  lua_pop(L, 1);

  return count;
}

bool CScriptObject::Clone(IScriptObject* pObj)
{
  int top = lua_gettop(L);
  bool bDeepCopy = true;
  bool bCopyByReference = false;

  PushRef(pObj);	// src object at top + 1
  PushRef();			// target object at top + 2

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

static void IterTable(lua_State* L, IScriptObjectDumpSink* p)
{
  lua_pushnil(L); // first key
  while (lua_next(L, -2))
  {
    int keyType = lua_type(L, -2);
    int valType = lua_type(L, -1);

    if (keyType == LUA_TSTRING)
    {
      const char* key = lua_tostring(L, -2);
      switch (valType)
      {
      case LUA_TNIL:
        p->OnElementFound(key, ScriptVarType::Null);
        break;
      case LUA_TBOOLEAN:
        p->OnElementFound(key, ScriptVarType::Bool);
        break;
      case LUA_TLIGHTUSERDATA:
        p->OnElementFound(key, ScriptVarType::Pointer);
        break;
      case LUA_TNUMBER:
        p->OnElementFound(key, ScriptVarType::Number);
        break;
      case LUA_TSTRING:
        p->OnElementFound(key, ScriptVarType::String);
        break;
      case LUA_TTABLE:
        if (strcmp(key, "__index") != 0)
          p->OnElementFound(key, ScriptVarType::Object);
        break;
      case LUA_TFUNCTION:
        p->OnElementFound(key, ScriptVarType::Function);
        break;
      case LUA_TUSERDATA:
        p->OnElementFound(key, ScriptVarType::UserData);
        break;
      }
    }
    else
    {
      int idx = (int)lua_tonumber(L, -2);
      switch (valType)
      {
      case LUA_TNIL:
        p->OnElementFound(idx, ScriptVarType::Null);
        break;
      case LUA_TBOOLEAN:
        p->OnElementFound(idx, ScriptVarType::Bool);
        break;
      case LUA_TLIGHTUSERDATA:
        p->OnElementFound(idx, ScriptVarType::Pointer);
        break;
      case LUA_TNUMBER:
        p->OnElementFound(idx, ScriptVarType::Number);
        break;
      case LUA_TSTRING:
        p->OnElementFound(idx, ScriptVarType::String);
        break;
      case LUA_TTABLE:
        p->OnElementFound(idx, ScriptVarType::Object);
        break;
      case LUA_TFUNCTION:
        p->OnElementFound(idx, ScriptVarType::Function);
        break;
      case LUA_TUSERDATA:
        p->OnElementFound(idx, ScriptVarType::UserData);
        break;
      }
    }

    lua_pop(L, 1);  // pop value, keep key
  }
}

void CScriptObject::Dump(IScriptObjectDumpSink* p)
{
  if (!p) return;
  CHECK_STACK(L);
  int top = lua_gettop(L);

  PushRef();

  int trgTable = top + 1;

  lua_pushnil(L);  // first key
  int reftop = lua_gettop(L);
  while (lua_next(L, trgTable) != 0)
  {
    // `key' is at index -2 and `value' at index -1
    if (lua_type(L, -2) == LUA_TSTRING)
    {
      const char* sName = lua_tostring(L, -2); // again index
      switch (lua_type(L, -1))
      {
      case LUA_TNIL:
        p->OnElementFound(sName, ScriptVarType::Null);
        break;
      case LUA_TBOOLEAN:
        p->OnElementFound(sName, ScriptVarType::Bool);
        break;
      case LUA_TLIGHTUSERDATA:
        p->OnElementFound(sName, ScriptVarType::Pointer);
        break;
      case LUA_TNUMBER:
        p->OnElementFound(sName, ScriptVarType::Number);
        break;
      case LUA_TSTRING:
        p->OnElementFound(sName, ScriptVarType::String);
        break;
      case LUA_TTABLE:
        if (strcmp(sName, "__index") != 0)
          p->OnElementFound(sName, ScriptVarType::Object);
        break;
      case LUA_TFUNCTION:
        p->OnElementFound(sName, ScriptVarType::Function);
        break;
      case LUA_TUSERDATA:
        p->OnElementFound(sName, ScriptVarType::UserData);
        break;
      }
      ;
    }
    else
    {
      int nIdx = (int)lua_tonumber(L, -2);  // again index
      switch (lua_type(L, -1))
      {
      case LUA_TNIL:
        p->OnElementFound(nIdx, ScriptVarType::Null);
        break;
      case LUA_TBOOLEAN:
        p->OnElementFound(nIdx, ScriptVarType::Bool);
        break;
      case LUA_TLIGHTUSERDATA:
        p->OnElementFound(nIdx, ScriptVarType::Pointer);
        break;
      case LUA_TNUMBER:
        p->OnElementFound(nIdx, ScriptVarType::Number);
        break;
      case LUA_TSTRING:
        p->OnElementFound(nIdx, ScriptVarType::String);
        break;
      case LUA_TTABLE:
        p->OnElementFound(nIdx, ScriptVarType::Object);
        break;
      case LUA_TFUNCTION:
        p->OnElementFound(nIdx, ScriptVarType::Function);
        break;
      case LUA_TUSERDATA:
        p->OnElementFound(nIdx, ScriptVarType::UserData);
        break;
      }
      ;
    }
    lua_settop(L, reftop); // pop value, leave index.
  }

  if (lua_getmetatable(L, -1))
  {
    lua_pushstring(L, "__index");
    lua_rawget(L, -2);
    if (lua_type(L, -1) == LUA_TTABLE)
      IterTable(L, p);
  }
  lua_settop(L, trgTable);

  lua_pop(L, 1); // pop table ref
}

bool CScriptObject::AddFunction(const char* sName, SCRIPT_FUNCTION pThunk, const INT_PTR nFuncID)
{
  PushRef();
  lua_pushstring(L, sName);

  if (pThunk)
  {
    member_ptr* pBuffer = (member_ptr*)lua_newuserdata(L, sizeof(member_ptr));
    pBuffer->fID = nFuncID;
    pBuffer->ref = GetRef();
    lua_pushcclosure(L, reinterpret_cast<lua_CFunction>(pThunk), 1);
  }

  lua_rawset(L, -3);
  lua_pop(L, 1); // pop table.
  return true;
}

bool CScriptObject::AddSetGetHandlers(SCRIPT_FUNCTION pSetThunk, SCRIPT_FUNCTION pGetThunk)
{
	PushRef();
	lua_getmetatable(L, -1);

	if (!lua_istable(L, -1))
		return false;
  CScriptObject* metaObject = new CScriptObject;
	metaObject->Attach();
	
	metaObject->AddFunction("__newindex", pSetThunk, -1);
	metaObject->AddFunction("__index", pGetThunk, -1);
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
	delete this;
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
  CHECK_STACK(L);
  //////////////////////////////////////////////////////////////////////////
  // Set metatable for this script object.
  //////////////////////////////////////////////////////////////////////////
  PushRef();           // -2
  PushRef(pMetatable); // -1
  lua_setmetatable(L, -2);
  lua_pop(L, 1); // pop table
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
  CHECK_STACK(L);
  int top = lua_gettop(L);
  lua_pushnil(L);  // first key
  while (lua_next(L, srcTable) != 0)
  {
    // `key' is at index -2 and `value' at index -1
    lua_pushvalue(L, -2); // Push again index.
    lua_pushvalue(L, -2); // Push value.
    lua_rawset(L, trgTable);
    lua_pop(L, 1); // pop value, leave index.
  }
  lua_settop(L, top); // Restore stack.
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

void CScriptObject::ReferenceTable_r(int srcTable, int trgTable)
{
  CHECK_STACK(L);
  int top = lua_gettop(L);

  lua_newtable(L);                                  // push new meta table
  lua_pushlstring(L, "__index", strlen("__index")); // push __index
  lua_pushvalue(L, srcTable);                       // push src table
  lua_rawset(L, -3);                                // meta.__index==src table
  lua_setmetatable(L, trgTable);                    // set meta table

  lua_pushnil(L);  // first key
  while (lua_next(L, srcTable) != 0)
  {
    if (lua_type(L, -1) == LUA_TTABLE)
    {
      int srct = lua_gettop(L);
      lua_pushvalue(L, -2); // Push again index.
      lua_newtable(L);      // Make value.
      int trgt = lua_gettop(L);
      ReferenceTable_r(srct, trgt);
      lua_rawset(L, trgTable); // Set new table to trgtable.
    }
    lua_pop(L, 1); // pop value, leave index.
  }
  lua_settop(L, top); // Restore stack.
}
