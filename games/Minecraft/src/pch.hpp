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
#include <filesystem>

namespace fs = std::filesystem;
#if defined(LINUX)
#	include <stdarg.h>
//#	include "platform.h"
//#	include "IGame.h"
//#	include "string.h"
#define OutputDebugString(...) void(0)
#include <sys/types.h>
#include <sys/stat.h>

#define MakeSureDirectoryPathExists(path) mkdir((path), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)

typedef struct _SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
}SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

inline void GetLocalTime(LPSYSTEMTIME lpSystemTime)
{
    ZeroStruct(lpSystemTime);
}
#else
inline bool MakeSureDirectoryPathExists(string path)
{
	if (!fs::exists(path)) {
		fs::create_directories(path);
	}
	return fs::exists(path);
}
#endif

#if defined(_AMD64_) && !defined(LINUX)
#include <io.h>
#endif

/////////////////////////////////////////////////////////////////////////////
// VARIOUS MACROS ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

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

#define TRACE __DLL_TRACE

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
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <list>
#include <map>	
#include <memory>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#if 0
#if defined USE_DEBUG_NEW
    #if defined(_DEBUG) && !defined(LINUX)
        #include <crtdbg.h>
        #define DEBUG_CLIENTBLOCK new( _NORMAL_BLOCK, __FILE__, __LINE__) 
        #define new DEBUG_CLIENTBLOCK
    #endif
#endif
#endif

/////////////////////////////////////////////////////////////////////////////
// Interfaces ///////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include <BlackBox/Renderer/IRender.hpp>

#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/Core/IMarkers.hpp>
#include <BlackBox/Core/Utils.hpp>
#include <BlackBox/EntitySystem/EntityDesc.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/Input/IHardwareMouse.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Network/INetwork.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>
#include <BlackBox/Scene/IScene.hpp>
#include <BlackBox/System/ConsoleCommands.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ITimer.hpp>
#include <BlackBox/World/IWorld.hpp>


/////////////////////////////////////////////////////////////////////////////
#include "Game.hpp"
#include "GameShared.hpp"

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

using CXGame = CXGame;
struct ISoundSystem
{
	virtual IVisArea* GetListenerArea() = 0;
	virtual Vec3	  GetListenerPos()	= 0;
};
