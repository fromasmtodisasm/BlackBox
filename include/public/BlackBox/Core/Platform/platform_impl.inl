#pragma once

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/Platform/CryLibrary.h>

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

#if (defined(_LAUNCHER)/* && defined(CRY_IS_MONOLITHIC_BUILD)*/) || !defined(_LIB)
//The reg factory is used for registering the different modules along the whole project
struct SRegFactoryNode* g_pHeadToRegFactories = nullptr;
std::vector<const char*> g_moduleCommands;
std::vector<const char*> g_moduleCVars;

extern "C" DLL_EXPORT void CleanupModuleCVars()
{
	if (auto pConsole = gEnv->pConsole)
	{
		// Unregister all commands that were registered from within the plugin/module
		for (auto& it : g_moduleCommands)
		{
			pConsole->RemoveCommand(it);
		}
		g_moduleCommands.clear();

		// Unregister all CVars that were registered from within the plugin/module
		for (auto& it : g_moduleCVars)
		{
			pConsole->UnregisterVariable(it);
		}
		g_moduleCVars.clear();
	}
}
#endif

#if /*!defined(CRY_IS_MONOLITHIC_BUILD)  || */defined(_LAUNCHER)
extern "C" DLL_EXPORT SRegFactoryNode* GetHeadToRegFactories()
{
	return g_pHeadToRegFactories;
}
#endif

#if !defined(_LIB) || defined(_LAUNCHER)
//////////////////////////////////////////////////////////////////////////
// If not in static library.

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

//////////////////////////////////////////////////////////////////////////
bool InitializeEngine(SSystemInitParams& startupParams, bool bManualEngineLoop)
{
	//CryFindRootFolderAndSetAsCurrentWorkingDirectory();

#if !defined(CRY_IS_MONOLITHIC_BUILD)
	CCryLibrary systemLibrary(CryLibraryDefName("System"));
	if (!systemLibrary.IsLoaded())
	{
		//string errorStr = string().Format("Failed to load the " CryLibraryDefName("CrySystem") " library!");
		//CryMessageBox(errorStr.c_str(), "Engine initialization failed!");
		return false;
	}

	PFNCREATESYSTEMINTERFACE CreateSystemInterface = systemLibrary.GetProcedureAddress<PFNCREATESYSTEMINTERFACE>("CreateSystemInterface");
	if (CreateSystemInterface == nullptr)
	{
		//string errorStr = string().Format(CryLibraryDefName("CrySystem") " library was invalid, entry-point not found!");
		//CryMessageBox(errorStr.c_str(), "Engine initialization failed!");

		return false;
	}
#endif

	startupParams.bManualEngineLoop = bManualEngineLoop;
	if (CreateSystemInterface(startupParams/*, bManualEngineLoop*/) != nullptr)
	{
#if !defined(IS_MONOLITHIC_BUILD)
		if (bManualEngineLoop)
		{
			// Forward ownership to the function caller
			// This is done since the engine loop will be updated outside of this function scope
			// In other cases we would be exiting the engine at this point.
			systemLibrary.ReleaseOwnership();
		}
#endif
	}
	else
	{
		return false;
	}

	return true;
}

#if BB_PLATFORM_WINDOWS
int64 bbGetTicks()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return li.QuadPart;
}
#endif

//////////////////////////////////////////////////////////////////////////
void bbSleep(unsigned int dwMilliseconds)
{
#if BB_PLATFORM_WINDOWS
	Sleep(dwMilliseconds);
#else
	//sleep(dwMilliseconds);
#endif
}

//////////////////////////////////////////////////////////////////////////

// If we use cry memory manager this should be also included in every module.
#if defined(USING_CRY_MEMORY_MANAGER)
	#include <BlackBox/Memory/CryMemoryManager_impl.h>
#endif

#endif
