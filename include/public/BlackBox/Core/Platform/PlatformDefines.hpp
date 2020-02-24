// Copyright 2001-2017 Crytek GmbH / Crytek Group. All rights reserved. 

#pragma once

// Note: we use '#define xxx 1' because in this case both '#if xxx' and '#if defined(xxx)' work for checking.

//! This define was added just to let programmers check that CryPlatformDefines.h is
//! already included (so BB_PLATFORM_xxx are already available).
#define BB_PLATFORM 1

// Detecting CPU. See:
// http://nadeausoftware.com/articles/2012/02/c_c_tip_how_detect_processor_type_using_compiler_predefined_macros
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0491c/BABJFEFG.html
#if defined(__x86_64__) || defined(_M_X64)
#define BB_PLATFORM_X64       1
#define BB_PLATFORM_64BIT     1
#define BB_PLATFORM_SSE2      1
#elif defined(__i386) || defined(_M_IX86)
#define BB_PLATFORM_X86       1
#define BB_PLATFORM_32BIT     1
#elif defined(__arm__)
#define BB_PLATFORM_ARM    1
#define BB_PLATFORM_32BIT  1
#if defined(__ARM_NEON__)
#define BB_PLATFORM_NEON 1
#endif
#elif defined(__aarch64__)
#define BB_PLATFORM_ARM    1
#define BB_PLATFORM_64BIT  1
#else
#define BB_PLATFORM_UNKNOWNCPU 1
#endif

#if defined(__APPLE__)

#define BB_PLATFORM_APPLE 1
#define BB_PLATFORM_POSIX 1
#include <TargetConditionals.h>
#if TARGET_OS_IPHONE
#define BB_PLATFORM_MOBILE 1
#define BB_PLATFORM_IOS    1
#if !BB_PLATFORM_UNKNOWNCPU
#error iOS: Unsupported CPU
#endif
#define BB_PLATFORM_64BIT   1
#elif TARGET_OS_MAC
#define BB_PLATFORM_DESKTOP 1
#define BB_PLATFORM_MAC     1
#if !BB_PLATFORM_X64
#error Unsupported Mac CPU (the only supported is x86-64).
#endif
#else
#error Unknown Apple platform.
#endif

#elif defined(_DURANGO) || defined(_XBOX_ONE)

#define BB_PLATFORM_CONSOLE 1
#define BB_PLATFORM_DURANGO 1
#define BB_PLATFORM_WINAPI  1
#if !BB_PLATFORM_X64
#error Unsupported Durango CPU (the only supported is x86-64).
#endif
#define BB_PLATFORM_SSE4    1
#define BB_PLATFORM_F16C    1
#define BB_PLATFORM_BMI1    1
#define BB_PLATFORM_TBM     1

#elif defined(_ORBIS) || defined(__ORBIS__)

#define BB_PLATFORM_CONSOLE 1
#define BB_PLATFORM_ORBIS   1
#define BB_PLATFORM_POSIX   1
#if !BB_PLATFORM_X64
#error Unsupported Orbis CPU (the only supported is x86-64).
#endif
#define BB_PLATFORM_SSE4    1
#define BB_PLATFORM_F16C    1
#define BB_PLATFORM_BMI1    1
#define BB_PLATFORM_TBM     0

#elif defined(ANDROID) || defined(__ANDROID__)

#define BB_PLATFORM_MOBILE  1
#define BB_PLATFORM_ANDROID 1
#define BB_PLATFORM_POSIX   1
#if BB_PLATFORM_ARM && !(BB_PLATFORM_64BIT || BB_PLATFORM_32BIT)
#error Unsupported Android CPU (only 32-bit and 64-bit ARM are supported).
#endif

#elif defined(_WIN32)

#define BB_PLATFORM_DESKTOP 1
#define BB_PLATFORM_WINDOWS 1
#define BB_PLATFORM_WINAPI  1
#if defined(_WIN64)
#if !BB_PLATFORM_X64
#error Unsupported Windows 64 CPU (the only supported is x86-64).
#endif
#else
#if !BB_PLATFORM_X86
#error Unsupported Windows 32 CPU (the only supported is x86).
#endif
#endif

#elif defined(__linux__) || defined(__linux)

#define BB_PLATFORM_DESKTOP 1
#define BB_PLATFORM_LINUX   1
#define BB_PLATFORM_POSIX   1
#if !BB_PLATFORM_X64 && !BB_PLATFORM_X86
#error Unsupported Linux CPU (the only supported are x86 and x86-64).
#endif

#else

#error Unknown target platform.

#endif

#if BB_PLATFORM_AVX
#define BB_PLATFORM_ALIGNMENT 32
#elif BB_PLATFORM_SSE2 || BB_PLATFORM_SSE4 || BB_PLATFORM_NEON
#define BB_PLATFORM_ALIGNMENT 16U
#else
#define BB_PLATFORM_ALIGNMENT 1U
#endif