#pragma once

#define IGAME_EXPORTS

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Platform/Windows.hpp>

//////////////////////////////////////////////////////////////////////////
#ifdef GetClassName
#undef GetClassName
#endif

#include <stdlib.h>
#include <stdio.h>

#if defined(LINUX)
#	include <stdarg.h>
#	include "platform.h"
#	include "IGame.h"
#	include "string.h"
#endif

#if defined(_AMD64_) && !defined(LINUX)
#include <io.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// VARIOUS MACROS ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

template <class T> inline void ZeroStruct( T &t ) { memset( &t,0,sizeof(t) ); }

_inline void __cdecl __DLL_TRACE(const char *sFormat, ... )
{
	va_list vl;
	static char sTraceString[1024];
	
	va_start(vl, sFormat);
	vsprintf(sTraceString, sFormat, vl);
	va_end(vl);

	strcat(sTraceString, "\n");

	OutputDebugString(sTraceString);	
}

#ifdef ENABLE_NET_TRACE
#define NET_TRACE __DLL_TRACE
#else
#	if defined(LINUX)
#		define NET_TRACE //
#	else
#		define NET_TRACE __noop
#	endif
#endif

#ifdef ASSERT
#undef ASSERT
#endif

#if defined(WIN64) || defined(LINUX64)
#define ASSERT(x) {assert(x);}
#else
#define ASSERT(x)	{ if (!(x)) { TRACE("Assertion Failed (%s) File: \"%s\" Line: %d\n", #x, __FILE__, __LINE__); DEBUG_BREAK; } }
#endif

/////////////////////////////////////////////////////////////////////////////
// STL //////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#pragma warning (disable : 4786)
#include <vector>
#include <list>
#include <map>	
#include <set>
#include <string>
#include <algorithm>
#include <memory>

#if defined(_DEBUG) && !defined(LINUX)
#include <crtdbg.h>
#define DEBUG_CLIENTBLOCK new( _NORMAL_BLOCK, __FILE__, __LINE__) 
#define new DEBUG_CLIENTBLOCK
#endif

/////////////////////////////////////////////////////////////////////////////
// Interfaces ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Network/INetwork.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/System/ITimer.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/Renderer/IRender.hpp>
#include <BlackBox/EntitySystem/EntityDesc.hpp>

/////////////////////////////////////////////////////////////////////////////
#include "Game.hpp"

//////////////////////////////////////////////////////////////////////////
//! Reports a Game Warning to validator with WARNING severity.
inline void GameWarning( const char *format,... )
{
	if (!format)
		return;

	char buffer[MAX_WARNING_LENGTH];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	CryWarning( VALIDATOR_MODULE_GAME,VALIDATOR_WARNING,buffer );
}

