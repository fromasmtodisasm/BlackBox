#include <BlackBox/ScriptSystem/FunctionHandler.hpp>
#include <BlackBox/ScriptSystem/ScriptObject.hpp>
#include <BlackBox/ISystem.hpp>
#include <cassert>

	
#define NOT_IMPL() assert(0 && __FUNCTION__" not implemented") 

CFunctionHandler::CFunctionHandler(CScriptSystem *pSS, lua_State *L)
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

int CFunctionHandler::GetFunctionID()
{
	unsigned char* pBuffer = (unsigned char*)lua_touserdata(L, lua_upvalueindex(1));
	int fID = (int)(*pBuffer);
	return fID;
}

int CFunctionHandler::GetParamCount()
{
	return lua_gettop(L);
}

bool CFunctionHandler::GetParam(int nIdx, int& n)
{
	int nRealIdx = nIdx + m_paramIdOffset;
	n = static_cast<int>(lua_tointeger(L, nRealIdx));
	return true;

}

bool CFunctionHandler::GetParam(int nIdx, float& f)
{
	int nRealIdx = nIdx + m_paramIdOffset;
	f = static_cast<float>(lua_tonumber(L, nRealIdx));
	return true;
}

bool CFunctionHandler::GetParam(int nIdx, const char*& s)
{
	int nRealIdx = nIdx + m_paramIdOffset;
	s = lua_tostring(L, nRealIdx);
	return true;
}

bool CFunctionHandler::GetParam(int nIdx, bool& b)
{
	int nRealIdx = nIdx + m_paramIdOffset;
	b = lua_toboolean(L, nRealIdx);
	return true;
}

bool CFunctionHandler::GetParam(int nIdx, IScriptObject* pObj)
{
	int nRealIdx = nIdx + m_paramIdOffset;
	return false;
}

bool CFunctionHandler::GetParam(int nIdx, HSCRIPTFUNCTION& hFunc, int nReference/* = 0*/)
{
	int nRealIdx = nIdx + m_paramIdOffset;
	//hFunc = lua_tocfunction(L, nIdx);
	return false;
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
	return 0;
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

bool CFunctionHandler::GetParam(int nIdx, USER_DATA& ud)
{
	return false;
}

bool CFunctionHandler::GetParamUDVal(int nIdx, USER_DATA& val, int& cookie)
{
	return false;
}

int CFunctionHandler::EndFunction(USER_DATA ud)
{
	return 0;
}
