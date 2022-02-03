// Copyright 2001-2019 Crytek GmbH / Crytek Group. All rights reserved.

#if !defined(AFX_STDAFX_H__844E5BAB_B810_40FC_8939_167146C07AED__INCLUDED_)
#define AFX_STDAFX_H__844E5BAB_B810_40FC_8939_167146C07AED__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

//#include <CryCore/Project/CryModuleDefs.h>
#define eCryModule   eCryM_ScriptSystem
#define RWI_NAME_TAG "RayWorldIntersection(Script)"
#define PWI_NAME_TAG "PrimitiveWorldIntersection(Script)"

#define CRYSCRIPTSYSTEM_EXPORTS
#include <BlackBox/Platform/Platform.hpp>

#define MY_STRING
#include <BlackBox/Platform/Platform.hpp>
#include <BlackBox/Platform/Windows.hpp>
#include <BlackBox/Utils/Utils.hpp>

class MyString : public std::string
{
  public:
	using std::string::string;
	using std::string::operator=;
	using std::string::operator+=;
	//! cast to C string operator.
	operator const char*() const
	{
		return data();
	} // make this explicit

	string& MakeLower()
	{
		for (auto c = begin(); c != end(); c++)
		{
			*c = std::tolower(*c);
		}
		return *this;
	}

	string& Trim()
	{
		auto c = begin();
		auto e = end();

		while (c != e && isspace(*c))
		{
			c++;
		}
		auto s = substr(c - begin(), npos);
		std::swap(static_cast<string&>(*this), s);
		return *this;
	}

	string& replace(char c1, char c2)
	{
		for (auto c = begin(); c != end(); c++)
		{
			if (*c == c1)
			{
				*c = c2;
			}
		}
		return *this;
	}

	string& replace(const char* c1, const char* c2)
	{
		findAndReplaceAll(*this, c1, c2);
		return *this;
	}
};

#include <cassert>
#include <cstdarg>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

//#define DEBUG_LUA_STATE

#include <BlackBox/Utils/StlUtils.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>
//#include <CryCore/StlUtils.h>
//#include <CryMemory/CrySizer.h>
//#include <CryMemory/PoolAllocator.h>

//////////////////////////////////////////////////////////////////////////
//! Reports a Game Warning to validator with WARNING severity.
#ifdef _RELEASE
	#define ScriptWarning(...) ((void)0)
#else
void ScriptWarning(const char*, ...); //PRINTF_PARAMS(1, 2);
inline void ScriptWarning(const char* format, ...)
{
	IF (!format, 0)
		return;

	char buffer[MAX_WARNING_LENGTH];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	CryWarning(VALIDATOR_MODULE_SCRIPTSYSTEM, VALIDATOR_WARNING, "%s", buffer);
}
#endif

template <class K, class V>
using VectorMap = std::map<K,V>;

#define CONST_TEMP_STRING(a) a


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__844E5BAB_B810_40FC_8939_167146C07AED__INCLUDED_)
