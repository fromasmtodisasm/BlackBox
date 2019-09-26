#include <BlackBox/ScriptSystem/ScriptSystem.hpp>
#include <BlackBox/ILog.hpp>
#include <BlackBox/IConsole.hpp>

#include <string>
#include <iostream>


CScriptSystem::CScriptSystem(ISystem *system)
	:
	m_System(system)
{
	L = luaL_newstate();
	luaL_openlibs(L);
}

CScriptSystem::~CScriptSystem()
{
	lua_close(L);
}

bool CScriptSystem::ExecuteFile(const char* sFileName, bool bRaiseError/* = true*/, bool bForceReload/* = false*/)
{
	return false;
}

bool CScriptSystem::ExecuteBuffer(const char* sBuffer, size_t nSize)
{
	int result = 0;

	if ((result = luaL_dostring(L, sBuffer)) == LUA_OK)
	{

	}
	else
	{
		std::string errormsg = lua_tostring(L, -1);
		m_System->getIConsole()->PrintLine("LUA ERROR: %s", errormsg.c_str());
	}

	return result == LUA_OK;
}

bool CScriptSystem::GetGlobalValue(const char* sKey, int& nVal)
{
	bool result = false;
	lua_getglobal(L, sKey);
	if (lua_isinteger(L, -1))
	{
		result = true;
		nVal = lua_tointeger(L, -1);
	}
	return result;
}

bool CScriptSystem::GetGlobalValue(const char* sKey, float& fVal)
{
	bool result = false;
	lua_getglobal(L, sKey);
	if (lua_isnumber(L, -1))
	{
		result = true;
		fVal = lua_tonumber(L, -1);
	}
	return result;
}

bool CScriptSystem::GetGlobalValue(const char* sKey, const char*& sVal)
{
	bool result = false;
	lua_getglobal(L, sKey);
	if (lua_isstring(L, -1))
	{
		result = true;
		sVal = lua_tostring(L, -1);
	}
	return result;
}
