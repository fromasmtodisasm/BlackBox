#pragma once

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>

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

//////////////////////////////////////////////////////////////////////////
// This is an entry to DLL initialization function that must be called for each loaded module
//////////////////////////////////////////////////////////////////////////
extern "C" DLL_EXPORT void ModuleInitISystem(ISystem* pSystem, const char* moduleName)
{
	#if defined(USE_CRY_ASSERT)
	CryAssertSetGlobalFlagAddress(pSystem->GetAssertFlagAddress());
	#endif

	if (gEnv) // Already registered.
		return;


	#if !defined(SYS_ENV_AS_STRUCT)
	if (pSystem) // DONT REMOVE THIS. ITS FOR RESOURCE COMPILER!!!!
		gEnv = pSystem->GetGlobalEnvironment();
	#endif
}


