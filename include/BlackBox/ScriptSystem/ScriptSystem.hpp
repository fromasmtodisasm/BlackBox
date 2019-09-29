#pragma once
#include <BlackBox/IScriptSystem.hpp>

#ifdef _WIN32
#include <windows.h>
#endif // Win32


#include <lua.hpp>
#include <lualib.h>


class CScriptSystem : public IScriptSystem
{
public:
	CScriptSystem(ISystem *system);
	~CScriptSystem();

	virtual bool ExecuteFile(const char* sFileName, bool bRaiseError = true, bool bForceReload = false) override;
	virtual bool ExecuteBuffer(const char* sBuffer, size_t nSize) override;
	virtual bool GetGlobalValue(const char* sKey, int& nVal) override;
	virtual bool GetGlobalValue(const char* sKey, float& fVal) override;
	virtual bool GetGlobalValue(const char* sKey, const char*& sVal) override;

private:

private:
	ISystem* m_System;
	lua_State* L;
};

