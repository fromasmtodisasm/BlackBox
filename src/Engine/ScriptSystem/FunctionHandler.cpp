#include <BlackBox/ScriptSystem/FunctionHandler.hpp>
#include <BlackBox/ScriptSystem/ScriptObject.hpp>

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
	CScriptObject::member_ptr* pBuffer = (CScriptObject::member_ptr*)lua_touserdata(L, lua_upvalueindex(1));
	return pBuffer->this_ptr;
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
	n = lua_tointeger(L, nIdx);
	return true;

}

bool CFunctionHandler::GetParam(int nIdx, float& f)
{
	f = lua_tonumber(L, nIdx);
	return true;
}

bool CFunctionHandler::GetParam(int nIdx, const char*& s)
{
	s = lua_tostring(L, nIdx);
	return true;
}

bool CFunctionHandler::GetParam(int nIdx, bool& b)
{
	b = lua_toboolean(L, nIdx);
	return true;
}

bool CFunctionHandler::GetParam(int nIdx, IScriptObject* pObj)
{
	return false;
}

bool CFunctionHandler::GetParam(int nIdx, HSCRIPTFUNCTION& hFunc, int nReference/* = 0*/)
{
	//hFunc = lua_tocfunction(L, nIdx);
	return false;
}

ScriptVarType CFunctionHandler::GetParamType(int nIdx)
{
	return ScriptVarType();
}

int CFunctionHandler::EndFunctionNull()
{
	return 0;
}

int CFunctionHandler::EndFunction(int nRetVal)
{
	return 0;
}

int CFunctionHandler::EndFunction(float fRetVal)
{
	return 0;
}

int CFunctionHandler::EndFunction(const char* fRetVal)
{
	return 0;
}

int CFunctionHandler::EndFunction(bool bRetVal)
{
	return 0;
}

int CFunctionHandler::EndFunction(IScriptObject* pObj)
{
	return 0;
}

int CFunctionHandler::EndFunction(HSCRIPTFUNCTION hFunc)
{
	return 0;
}

int CFunctionHandler::EndFunction()
{
	return 0;
}

int CFunctionHandler::EndFunction(int nRetVal1, int nRetVal2)
{
	return 0;
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
