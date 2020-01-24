#pragma once

#ifdef _WINDOWS_
#error windows.h should not be included prior to platform.h
#endif

enum class EPlatform
{
  Windows,
  Linux,
  MacOS,
  XboxOne,
  PS4,
  Android,
  iOS,

#ifdef BB_PLATFORM_WINDOWS
  Current = Windows
#elif defined(BB_PLATFORM_LINUX)
  Current = Linux
#elif defined(BB_PLATFORM_APPLE) && !defined(BB_PLATFORM_MOBILE)
  Current = MacOS
#elif defined(BB_PLATFORM_DURANGO)
  Current = XboxOne
#elif defined(BB_PLATFORM_ORBIS)
  Current = PS4
#elif defined(BB_PLATFORM_ANDROID)
  Current = Android
#elif defined(BB_PLATFORM_APPLE) && defined(BB_PLATFORM_MOBILE)
  Current = iOS
#endif
};

#if defined(WIN32) || defined(WIN64)
#define DEBUG_BREAK _asm { int 3 }
#else
#define DEBUG_BREAK
#endif

#if defined(WIN32) && !defined(WIN64)
#define _CPU_X86
// Insert your headers here
#define WIN32_LEAN_AND_MEAN
#ifdef APIENTRY
#undef APIENTRY
#endif
#include <Windows.h>
#undef min
#undef max


//#define RC_EXECUTABLE "rc.exe"
#endif

#if defined(LINUX64)
#include <BlackBox/Linux64Specific.h>
#define _CPU_AMD64
#endif

#include <cstring>

// Safe memory helpers
template<typename T>
inline auto SAFE_DELETE(T const* t) -> decltype((void)(t->Release()), void())
{
  t->Release();
}

template<typename T>
inline auto SAFE_DELETE(T*& t)
{
  if (t != nullptr)
  {
    delete t;
    t = nullptr;
  }
}

///////////////////////////////////////////////////////////////////////////////
// common Typedef                                                                   //
///////////////////////////////////////////////////////////////////////////////
typedef double real;
typedef int index_t;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;


#define BIT(x)    (1u << (x))
#define BIT64(x)  (1ull << (x))
#define MASK(x)   (BIT(x) - 1U)
#define MASK64(x) (BIT64(x) - 1ULL)

#define ASSERT(msg) assert(msg)
#define FUNCTION_PROFILER(...)
#define LogAlways(...) void(0);

#define TRACE(...)

//! ILINE always maps to CRY_FORCE_INLINE, which is the strongest possible inline preference.
//! Note: Only use when shown that the end-result is faster when ILINE macro is used instead of inline.
#if !defined(_DEBUG) && !defined(CRY_UBSAN)
#define ILINE
#else
#define ILINE inline
#endif
#include <cstdint>

int64_t    bbGetTicks();
void       bbSleep(unsigned int dwMilliseconds);

// Include most commonly used STL headers.
// They end up in precompiled header and make compilation faster.
#include <memory>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cstdint>

#ifdef SendMessage
#undef SendMessage
#endif

#if BB_PLATFORM_WINDOWS
#include <BlackBox/Core/Platform/WindowsSpecific.hpp>
#endif

#if BB_PLATFORM_LINUX
#include <CryCore/Platform/Linux64Specific.h>
#endif

#if BB_PLATFORM_ANDROID
#include <CryCore/Platform/Android64Specific.h>
#endif

#define DECLARE_SHARED_POINTERS(name)                   \
  typedef std::shared_ptr<name> name ##       Ptr;      \
  typedef std::shared_ptr<const name> name ## ConstPtr; \
  typedef std::weak_ptr<name> name ##         WeakPtr;  \
  typedef std::weak_ptr<const name> name ##   ConstWeakPtr;

