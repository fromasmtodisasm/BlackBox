#include <BlackBox/ScriptSystem/FunctionHandler.hpp>
#include <BlackBox/ScriptSystem/ScriptObject.hpp>
#include <BlackBox/IEngine.hpp>
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
#if 0
	int top = lua_gettop(L);
	auto cnt = GetParamCount();
	auto type = lua_type(L, 1);
	if (lua_isstring(L, 1))
	{
		auto str = lua_tostring(L, 1);
		GetISystem()->Log(str);
	}
	if (lua_isstring(L, 2))
	{
		auto str = lua_tostring(L, 2);
		GetISystem()->Log(str);
	}
	//CScriptObject::member_ptr* pBuffer = (CScriptObject::member_ptr*)lua_touserdata(L, lua_upvalueindex(1));
	//return pBuffer->this_ptr;
	CScriptObject::member_ptr *pBuffer = (CScriptObject::member_ptr*)lua_touserdata(L, lua_upvalueindex(1));
	int fID = pBuffer->fID;
	int ref = pBuffer->ref;
	//lua_getref(L, ref);

	void* ptr = NULL;
	// Get implicit self table.
	if (lua_type(L, 1) == LUA_TTABLE)
	{
		lua_pushvalue(L, 1);
		// index "__this" member.
		lua_pushstring(L, "__this");
		lua_rawget(L, -2);
		auto type = lua_type(L, -1);
		if (lua_islightuserdata(L, -1))
		{
			ptr = lua_touserdata(L, -1);
		}
		lua_pop(L, 1); // pop result.
	}
	lua_settop(L, top);
	return ptr;
#endif
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
	n = lua_tointeger(L, nRealIdx);
	return true;

}

bool CFunctionHandler::GetParam(int nIdx, float& f)
{
	int nRealIdx = nIdx + m_paramIdOffset;
	f = lua_tonumber(L, nRealIdx);
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
	ScriptVarType type = svtNull;
	int luatype = lua_type(L, nRealIdx);
	switch (luatype)
	{
	case LUA_TNIL:
		type = svtNull;
		break;
	case LUA_TBOOLEAN:
		type = svtBool;
		break;
	case LUA_TNUMBER:
		type = svtNumber;
		break;
	case LUA_TSTRING:
		type = svtString;
		break;
	case LUA_TFUNCTION:
		type = svtFunction;
		break;
	case LUA_TLIGHTUSERDATA:
		type = svtPointer;
		break;
	case LUA_TTABLE:
		type = svtObject;
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
	m_pSS->PushFuncParam(nRetVal);
	return 1;
}

int CFunctionHandler::EndFunction(float fRetVal)
{
	m_pSS->PushFuncParam(fRetVal);
	return 1;
}

int CFunctionHandler::EndFunction(const char* cRetVal)
{
	m_pSS->PushFuncParam(cRetVal);
	return 1;
}

int CFunctionHandler::EndFunction(bool bRetVal)
{
	m_pSS->PushFuncParam(bRetVal);
	return 1;
}

int CFunctionHandler::EndFunction(IScriptObject* pObj)
{
	m_pSS->PushFuncParam(pObj);
	return 1;
}

int CFunctionHandler::EndFunction(HSCRIPTFUNCTION hFunc)
{
	//m_pSS->PushFuncParam(hFunc);
	NOT_IMPL();
	return 1;
}

int CFunctionHandler::EndFunction()
{
	return 0;
}

int CFunctionHandler::EndFunction(int nRetVal1, int nRetVal2)
{
	return 2;
}

int CFunctionHandler::EndFunction(float fRetVal1, float fRetVal2)
{
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
