#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/ScriptSystem/FunctionHandler.hpp>
#include <BlackBox/ScriptSystem/ScriptObject.hpp>
#include <BlackBox/System/ISystem.hpp>

#include <cassert>

#define NOT_IMPL() assert(0 && __FUNCTION__" not implemented")

CFunctionHandler::CFunctionHandler(CScriptSystem* pSS, lua_State* L)
  :
  m_pSS(pSS),
  L(L)
{
}

CFunctionHandler::~CFunctionHandler()
{
}

void CFunctionHandler::__Attach(HSCRIPT hScript)
{
  L = static_cast<lua_State*>(hScript);
}

THIS_PTR CFunctionHandler::GetThis()
{
  void* ptr = NULL;
  // Get implicit self table.
  if (m_paramIdOffset > 0 && lua_type(L, 1) == LUA_TTABLE)
  {
    // index "__this" member.
    lua_pushstring(L, "__this");
    lua_rawget(L, 1);
    if (lua_type(L, -1) == LUA_TLIGHTUSERDATA)
      ptr = const_cast<THIS_PTR>(lua_topointer(L, -1));
    lua_pop(L, 1); // pop result.
  }
  return ptr;
}

INT_PTR CFunctionHandler::GetFunctionID()
{
  unsigned char* pBuffer = (unsigned char*)lua_touserdata(L, lua_upvalueindex(1));
	INT_PTR fID = 0;
  memcpy(&fID, pBuffer, sizeof(INT_PTR));
  return fID;
}

int CFunctionHandler::GetParamCount()
{
  auto count = lua_gettop(L);
  if (m_paramIdOffset > 0)
    count -= m_paramIdOffset;
  return count;
}

bool CFunctionHandler::GetParam(int nIdx, int& n)
{
  return GetParamAny(nIdx, n);
}

bool CFunctionHandler::GetParam(int nIdx, float& f)
{
  return GetParamAny(nIdx, f);
}

bool CFunctionHandler::GetParam(int nIdx, const char*& s)
{
  return GetParamAny(nIdx, s);
}

bool CFunctionHandler::GetParam(int nIdx, bool& b)
{
  return GetParamAny(nIdx, b);
}

bool CFunctionHandler::GetParam(int nIdx, IScriptObject* pObj)
{
  return GetParamAny(nIdx, pObj);
}

bool CFunctionHandler::GetParam(int nIdx, HSCRIPTFUNCTION& hFunc, int nReference/* = 0*/)
{
  int nRealIdx = nIdx + m_paramIdOffset;
  //hFunc = lua_tocfunction(L, nIdx);
  return false;
  //return GetParamAny(nIdx, pObj);
}

bool CFunctionHandler::GetParam(int nIdx, USER_DATA& ud)
{
  return GetParamAny(nIdx, ud);
}


ScriptVarType CFunctionHandler::GetParamType(int nIdx)
{
  int nRealIdx = nIdx + m_paramIdOffset;
  ScriptVarType type = ScriptVarType::Null;
  int luatype = lua_type(L, nRealIdx);
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
  return type;
}

int CFunctionHandler::EndFunctionNull()
{
	m_pSS->PushAny(nullptr);
  return 1;
}

int CFunctionHandler::EndFunction(int nRetVal)
{
  m_pSS->PushAny(nRetVal);
  return 1;
}

int CFunctionHandler::EndFunction(float fRetVal)
{
  m_pSS->PushAny(fRetVal);
  return 1;
}

int CFunctionHandler::EndFunction(const char* cRetVal)
{
  m_pSS->PushAny(cRetVal);
  return 1;
}

int CFunctionHandler::EndFunction(bool bRetVal)
{
  m_pSS->PushAny(bRetVal);
  return 1;
}

int CFunctionHandler::EndFunction(IScriptObject* pObj)
{
  m_pSS->PushAny(pObj);
  return 1;
}

int CFunctionHandler::EndFunction(HSCRIPTFUNCTION hFunc)
{
  m_pSS->PushAny(hFunc);
  return 1;
}

int CFunctionHandler::EndFunction()
{
  return 0;
}

int CFunctionHandler::EndFunction(int nRetVal1, int nRetVal2)
{
  m_pSS->PushAny(nRetVal1);
  m_pSS->PushAny(nRetVal2);
  return 2;
}

int CFunctionHandler::EndFunction(float fRetVal1, float fRetVal2)
{
  m_pSS->PushAny(fRetVal1);
  m_pSS->PushAny(fRetVal2);
  return 0;
}

void CFunctionHandler::Unref(HSCRIPTFUNCTION hFunc)
{
}

bool CFunctionHandler::GetParamUDVal(int nIdx, USER_DATA& val, int& cookie)
{
	GetParamAny(nIdx, val);
  return false;
}

int CFunctionHandler::EndFunction(USER_DATA ud)
{
  m_pSS->PushAny(ud);
  return 1;
}
