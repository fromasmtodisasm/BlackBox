// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved. 

// -------------------------------------------------------------------------
//  File name:   CryWindows.h
//  Version:     v1.00
//  Created:     02/05/2012 by James Chilvers.
//  Compilers:   Visual Studio.NET
//  Description: Specific header to handle Windows.h include
// -------------------------------------------------------------------------
//  History:
//
////////////////////////////////////////////////////////////////////////////

#pragma once

#if BB_PLATFORM_WINAPI

	#ifndef WIN32_LEAN_AND_MEAN
		#define WIN32_LEAN_AND_MEAN
	#endif

// Do not define min/max in windows.h
	#define NOMINMAX

// Prevents <Windows.h> from #including <Winsock.h>
// Manually define your <Winsock2.h> inclusion point elsewhere instead.
	#ifndef _WINSOCKAPI_
		#define _WINSOCKAPI_
	#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS

	#if defined(_WINDOWS_) && !defined(CRY_INCLUDE_WINDOWS_VIA_MFC_OR_ATL_INCLUDES)
		#error "<windows.h> has been included by other means than CryWindows.h"
	#endif

	#include <windows.h>

	#if !defined(BB_SUPPRESS_CRYENGINE_WINDOWS_FUNCTION_RENAMING)
		#undef min
		#undef max
		#undef GetCommandLine
		#undef GetObject
		#undef PlaySound
		#undef GetClassName
		#undef DrawText
		#undef GetCharWidth
		#undef GetUserName
    #undef SendMessage
		//#undef LoadLibrary
	#endif
	#undef GetCompressedFileSize

	#ifdef CRY_PLATFORM_DURANGO
		#include <CryCore/Platform/Durango_Win32Legacy.h>
	#endif

// In RELEASE disable OutputDebugString
	#if defined(_RELEASE) && !BB_PLATFORM_DESKTOP && !defined(RELEASE_LOGGING)
		#undef OutputDebugString
		#define OutputDebugString(...) (void) 0
	#else
		#undef OutputDebugString
		#define OutputDebugString(...) (void) 0
	#endif

  inline int64_t  bbGetTicks()
  {
    return 0;
  }

  inline void bbSleep(unsigned int dwMilliseconds)
  {

  }

#endif
