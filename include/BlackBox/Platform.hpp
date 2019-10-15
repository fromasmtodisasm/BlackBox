#pragma once

#if defined(WIN32) || defined(WIN64)
#define DEBUG_BREAK _asm { int 3 }
#else
#define DEBUG_BREAK
#endif

#if defined(WIN32) && !defined(WIN64)
#define _CPU_X86
// Insert your headers here
//#include <windows.h>
//#include <Win32specific.h>

//#define RC_EXECUTABLE "rc.exe"
#endif

#include <cstring>
