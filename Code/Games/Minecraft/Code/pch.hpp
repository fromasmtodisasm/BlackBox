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

#include <Cry_Color4.h>
#include <TArray.h>

#include <BlackBox/3DEngine/I3DEngine.hpp>
#include <BlackBox/Core/IMarkers.hpp>
#include <BlackBox/Core/Utils.hpp>

#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/ITimer.hpp>
#include <BlackBox/Network/INetwork.hpp>
#include <BlackBox/System/FrameProfiler.hpp>
#include <BlackBox/EntitySystem/EntityDesc.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/EntitySystem/IEntitySystem.hpp>
#include <BlackBox/Input/IHardwareMouse.hpp>
#include <BlackBox/Input/IInput.hpp>
#include <BlackBox/Renderer/IFont.hpp>
#include <BlackBox/Renderer/IRenderAuxGeom.hpp>
#include <BlackBox/ScriptSystem/IScriptSystem.hpp>

#include <IMovieSystem.h>
#include <ISound.h>

typedef string String;
typedef SmartScriptObject _SmartScriptObject;

/////////////////////////////////////////////////////////////////////////////
// Classes used often/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include "IXSystem.hpp"
#include "GameShared.hpp"

/////////////////////////////////////////////////////////////////////////////
#include "Game.hpp"
#include "Network/XNetwork.hpp"
#include "Server/XServer.hpp"

#include "Server/XServerSlot.hpp"
#include "Client/XClient.hpp"

#include "Legacy/IInput.h"

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
#if 0
struct ISoundSystem
{
	virtual IVisArea* GetListenerArea() = 0;
	virtual Legacy::Vec3	  GetListenerPos()	= 0;
};
#endif

Legacy::IInput* GetLegacyInput();
#define GET_GUI_INPUT() GetLegacyInput()

#ifdef NOT_IMPLEMENTED_V
#	undef NOT_IMPLEMENTED_V
#	define NOT_IMPLEMENTED_V                                   \
		CryError("Function: %s not implemented", __FUNCTION__); \
	return {};
#endif

#ifdef NOT_IMPLEMENTED
#	undef NOT_IMPLEMENTED
#	define NOT_IMPLEMENTED                                     \
		CryError("Function: %s not implemented", __FUNCTION__); \
		//assert(0 && __FUNCTION__);
#endif


#ifndef PS2
#if defined(WIN64) || defined(LINUX)
#define FIXME_ASSERT(cond) { if(!(cond)) abort(); }
#else
#define FIXME_ASSERT(cond) { if(!(cond)) { DEBUG_BREAK; } }
#endif
#else //PS2
#define FIXME_ASSERT(cond) { if(!(cond)) { FORCE_EXIT();} }
#endif

//////////////////////////////////////////////////////////////////////////
inline void __DumpEntity(ILog* pLog, IEntity* pEntity)
{
	const char* sTemp = NULL;
	Legacy::Vec3		v;
	pLog->Log("************************************");
	if (!pEntity)
	{
		pLog->Log("DUMPING ENTITY .... the pEntity IS NULL");
		return;
	}
	pLog->Log("DUMPING ENTITY %d", pEntity->GetId());
	pLog->Log("CLASSID [%03d]", (int)pEntity->GetClassId());
	sTemp = pEntity->GetEntityClassName();
	pLog->Log("GetClassName return [%p]", sTemp);
	pLog->Log("CLASSNAME %s", sTemp);
	sTemp = pEntity->GetName();
	pLog->Log("GetName return [%p]", sTemp);
	pLog->Log("NAME %s", sTemp);
	v = pEntity->GetPos();
	pLog->Log("POS %f,%f,%f", v.x, v.y, v.z);
	pLog->Log("CONTAINER (ptr)[%p]", pEntity->GetContainer());
	pLog->Log("************************************");
}

//////////////////////////////////////////////////////////////////////////
inline void __DumpEntity(ILog* pLog, const CEntityDesc& desc)
{
	const char* sTemp = NULL;
	Legacy::Vec3 v;
	pLog->Log("*************ENTITYDESCDUMP****************");
	pLog->Log("CLASSID [%03d]", (int)desc.ClassId);
	pLog->Log("CLASSNAME %s", desc.className.c_str());
	v = desc.pos;
	pLog->Log("POS %f,%f,%f", v.x, v.y, v.z);
	pLog->Log("************************************");
}

// redundant functions

//////////////////////////////////////////////////////////////////////////
//clamps angles to be within min-max range. Check for special case when min>max -- for example min=350 max=40
inline float ClampAngle360(float min, float max, float angl)
{
	if (min > max)
	{
		if (angl > min || angl < max)
			return angl;
		if (fabs(angl - min) < fabs(angl - max))
			angl = min;
		else
			angl = max;
		return angl;
	}

	if (angl < min)
		angl = min;
	else if (angl > max)
		angl = max;
	return angl;
}

//////////////////////////////////////////////////////////////////////////
//gets angles difference, checks for case when a1 and a2 on different sides of 0. for example a1=350 a2=10
inline float GetAngleDifference360(float a1, float a2)
{
	float res = a1 - a2;

	if (res > 180)
		res = res - 360;
	else if (res < -180)
		res = 360 + res;
	return res;
}

//////////////////////////////////////////////////////////////////////////
// clamps angles to be within min-max range. Check fro special case when min>max -- for example min=350 max=40
// all angles have to be in range (0, 360)
inline float ClampAngle(float minA, float maxA, float angl, bool& bClamped)
{
	bClamped = false;
	if (minA > maxA)
	{
		if (angl > minA || angl < maxA)
			return angl;
	}
	else if (angl > minA && angl < maxA)
		return angl;

	bClamped = true;

	if (fabs(GetAngleDifference360(minA, angl)) < fabs(GetAngleDifference360(maxA, angl)))
		return minA;
	return maxA;
}

//////////////////////////////////////////////////////////////////////////
// clamps angles to be within min-max range. Check for special case when min>max -- for example min=350 max=40
// all angles have to be in range (0, 360)
inline float ClampAngle(float minA, float maxA, float angl)
{
	if (minA > maxA)
	{
		if (angl > minA || angl < maxA)
			return angl;
	}
	else if (angl > minA && angl < maxA)
		return angl;

	if (fabs(GetAngleDifference360(minA, angl)) < fabs(GetAngleDifference360(maxA, angl)))
		return minA;
	return maxA;
}

#if !defined(min) && !defined(LINUX)
#	define max(a, b) (((a) > (b)) ? (a) : (b))
#	define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

//////////////////////////////////////////////////////////////////////
#if defined(_CPU_X86)
ILINE float __fastcall Ffabs(float f) {
	*((unsigned *) & f) &= ~0x80000000;
	return (f);
}
#else
//inline float Ffabs(float x) { return fabsf(x); }
#endif

namespace Legacy
{
	inline vectorf from(const Vec3& v) {
		return vectorf{v.x, v.y, v.z};
	}

	inline Vec3 to(const vectorf& v) {
		return Vec3{v.x, v.y, v.z};
	}
}

