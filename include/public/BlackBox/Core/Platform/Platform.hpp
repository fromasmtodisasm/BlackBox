#pragma once

//#include <BlackBox/Core/Platform/PlatformDefines.hpp>
#include "PlatformDefines.hpp"

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

//#define RC_EXECUTABLE "rc.exe"
#endif

#if defined(LINUX64)
//#include <BlackBox/Linux64Specific.h>
#define _CPU_AMD64
#endif

#include <cstring>

// Safe memory helpers
#if 0
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
#else
// Safe memory helpers
#define SAFE_DELETE(p)        { if (p) { delete (p);          (p) = nullptr; } }
#define SAFE_DELETE_ARRAY(p)  { if (p) { delete[] (p);        (p) = nullptr; } }
#define SAFE_RELEASE(p)       { if (p) { (p)->Release();      (p) = nullptr; } }
#define SAFE_RELEASE_FORCE(p) { if (p) { (p)->ReleaseForce(); (p) = nullptr; } }
#endif



///////////////////////////////////////////////////////////////////////////////
// common Typedef                                                                   //
///////////////////////////////////////////////////////////////////////////////
typedef double real;
typedef int index_t;
typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;


#if defined(WIN32) && defined(__GNUC__) || defined(BB_PLATFORM_LINUX)
#if 0
	#define DLL_EXPORT [[gnu::dllexport]]
	#define DLL_IMPORT [[gnu::dllimport]]
#else
	#define DLL_EXPORT
	#define DLL_IMPORT
#endif
#else
	#define DLL_EXPORT __declspec(dllexport)
	#define DLL_IMPORT __declspec(dllimport)
#endif

#define BIT(x)    (1u << (x))
#define BIT64(x)  (1ull << (x))
#define MASK(x)   (BIT(x) - 1U)
#define MASK64(x) (BIT64(x) - 1ULL)

#define FUNCTION_PROFILER(...)

#if defined(USE_CRY_ASSERT)
#else
	//! Use the platform's default assert.
	#include <assert.h>
	#define CRY_ASSERT_TRACE(condition, parenthese_message) assert(condition)
	#define CRY_ASSERT_MESSAGE_IMPL(condition, szCondition, file, line, ...) assert(condition)
	#define CRY_ASSERT_MESSAGE(condition, ... )             assert(condition)
	#define CRY_ASSERT(condition, ...)                      assert(condition)
#endif
namespace Cry
{
	template<typename T, typename ... Args>
	inline T const& VerifyWithMessage(T const& expr, const char* szExpr, const char* szFile, int line, Args&& ... args)
	{
		CRY_ASSERT_MESSAGE_IMPL(expr, szExpr, szFile, line, std::forward<Args>(args) ...);
		return expr;
	}
}

#define CRY_VERIFY(expr, ...)              Cry::VerifyWithMessage(expr, # expr, __FILE__, __LINE__, ##__VA_ARGS__)

#define CRY_FUNCTION_NOT_IMPLEMENTED       CRY_ASSERT(false, "Call to not implemented function: %s", __func__)

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

class MyString;
#ifndef MY_STRING
using string = std::string;
#else
using string = MyString;
#endif



#ifdef SendMessage
#undef SendMessage
#endif

#if BB_PLATFORM_WINDOWS && BB_PLATFORM_64BIT
#include <BlackBox/Core/Platform/WindowsSpecific.hpp>
#endif

#if BB_PLATFORM_LINUX
#include <BlackBox/Core/Platform/Linux64Specific.h>
#endif

#if BB_PLATFORM_ANDROID
#include <BlackBox/Core/Platform/Android64Specific.h>
#endif

// Wrapper code for non-windows builds.
#if BB_PLATFORM_LINUX || BB_PLATFORM_MAC || BB_PLATFORM_IOS || BB_PLATFORM_ANDROID
  #include <BlackBox/Core/Platform/Linux_Win32Wrapper.h>
#elif BB_PLATFORM_ORBIS
  #include <BlackBox/Core/Platform/Orbis_Win32Wrapper.h>
#endif

// CryModule memory manager routines must always be included.
// They are used by any module which doesn't define NOT_USE_CRY_MEMORY_MANAGER
// No Any STL includes must be before this line.
#if 1  //#ifndef NOT_USE_CRY_MEMORY_MANAGER
	#define USE_NEWPOOL
	#include <BlackBox/Memory/CryMemoryManager.h>
#else
inline int IsHeapValid()
{
	#if defined(_DEBUG) && !defined(RELEASE_RUNTIME)
	return _CrtCheckMemory();
	#else
	return true;
	#endif
}
#endif // NOT_USE_CRY_MEMORY_MANAGER

// Memory manager breaks strdup
// Use something higher level, like CryString
//#undef strdup
//#define strdup dont_use_strdup

//! Loads System from disk and initializes the engine, commonly called from the Launcher implementation
//! \param bManualEngineLoop Whether or not the caller will start and maintain the engine loop themselves. Otherwise the loop is started and engine shut down automatically inside the function.
bool			InitializeEngine(struct SSystemInitParams& startupParams, bool bManualEngineLoop = false);


#undef STATIC_CHECK
#define STATIC_CHECK(expr, msg) static_assert((expr) != 0, # msg)

// Conditionally execute code in debug only
#ifdef _DEBUG
	#define IF_DEBUG(expr) (expr)
#else
	#define IF_DEBUG(expr)
#endif

#define DECLARE_SHARED_POINTERS(name)                   \
  typedef std::shared_ptr<name> name ##       Ptr;      \
  typedef std::shared_ptr<const name> name ## ConstPtr; \
  typedef std::weak_ptr<name> name ##         WeakPtr;  \
  typedef std::weak_ptr<const name> name ##   ConstWeakPtr;

