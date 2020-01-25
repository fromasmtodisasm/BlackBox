#pragma once

//////////////////////////////////////////////////////////////////////////
// Global environment variable.
//////////////////////////////////////////////////////////////////////////
#if defined(SYS_ENV_AS_STRUCT)
	#if defined(_LAUNCHER)
SSystemGlobalEnvironment gEnv;
	#else
extern SSystemGlobalEnvironment gEnv;
	#endif
#else
struct SSystemGlobalEnvironment* gEnv = nullptr;
#endif

#include <BlackBox/Core/Platform/Platform.hpp>

int64_t  bbGetTicks()
{
	return 0;
}

void bbSleep(unsigned int dwMilliseconds)
{

}
