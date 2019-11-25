#pragma once

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

#include <BlackBox/WindowsSpecific.hpp>

//#define RC_EXECUTABLE "rc.exe"
#endif

#if defined(LINUX64)
#include <Linux64Specific.h>
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
