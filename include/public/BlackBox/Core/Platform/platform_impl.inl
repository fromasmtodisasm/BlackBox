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

#	if BB_PLATFORM_WINDOWS
void CryPureCallHandler()
{
	CryFatalError("Pure function call");
}

void CryInvalidParameterHandler(
	const wchar_t* expression,
	const wchar_t* function,
	const wchar_t* file,
	unsigned int   line,
	uintptr_t	   pReserved)
{
	size_t i;
	char   sFunc[128];
	char   sExpression[128];
	char   sFile[128];
	wcstombs_s(&i, sFunc, sizeof(sFunc), function, _TRUNCATE);
	wcstombs_s(&i, sExpression, sizeof(sExpression), expression, _TRUNCATE);
	wcstombs_s(&i, sFile, sizeof(sFile), file, _TRUNCATE);
	CryFatalError("Invalid parameter detected in function %s. File: %s Line: %d, Expression: %s", sFunc, sFile, line, sExpression);
}

void InitCRTHandlers()
{
	_set_purecall_handler(CryPureCallHandler);
	_set_invalid_parameter_handler(CryInvalidParameterHandler);
}
#	else
void							InitCRTHandlers()
{
}
#	endif

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

	InitCRTHandlers();

	#if !defined(SYS_ENV_AS_STRUCT)
	if (pSystem) // DONT REMOVE THIS. ITS FOR RESOURCE COMPILER!!!!
		gEnv = pSystem->GetGlobalEnvironment();
	#endif
}

//////////////////////////////////////////////////////////////////////////
void CrySleep(unsigned int dwMilliseconds)
{
	::Sleep(dwMilliseconds);
}

//////////////////////////////////////////////////////////////////////////
bool InitializeEngine(SSystemInitParams& startupParams, bool bManualEngineLoop)
{
	//CryFindRootFolderAndSetAsCurrentWorkingDirectory();

#if !defined(CRY_IS_MONOLITHIC_BUILD)
	CCryLibrary systemLibrary(CryLibraryDefName("System"));
	if (!systemLibrary.IsLoaded())
	{
		string errorStr = string(512, '\0');
		sprintf(errorStr.data(), "%s", "Failed to load the " CryLibraryDefName("CrySystem") " library!");
		CryMessageBox(errorStr.c_str(), "Engine initialization failed!");
		return false;
	}

	PFNCREATESYSTEMINTERFACE CreateSystemInterface = systemLibrary.GetProcedureAddress<PFNCREATESYSTEMINTERFACE>("CreateSystemInterface");
	if (CreateSystemInterface == nullptr)
	{
		string errorStr = string(512, '\0');
		sprintf(errorStr.data(), "%s", CryLibraryDefName("CrySystem") " library was invalid, entry-point not found!");
		CryMessageBox(errorStr.c_str(), "Engine initialization failed!");

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
//////////////////////////////////////////////////////////////////////////

// If we use cry memory manager this should be also included in every module.
#if defined(USING_CRY_MEMORY_MANAGER)
	#include <BlackBox/Memory/CryMemoryManager_impl.h>
#endif

#if BB_PLATFORM_WINAPI
#include "platform_impl_winapi.inl"
#elif BB_PLATFORM_ANDROID
#include "platform_impl_android.inl"
#elif BB_PLATFORM_ORBIS
#include "platform_impl_orbis.inl"
#elif BB_PLATFORM_MAC
#include "platform_impl_mac.inl"
#endif

#if BB_PLATFORM_POSIX
#include "platform_impl_posix.inl"
#endif
// Functions that depend on the platform-specific includes below

EQuestionResult CryMessageBox(const char* szText, const char* szCaption, EMessageBox type)
{
	#if 0
	if (gEnv && gEnv->bUnattendedMode)
	{
		return eQR_None;
	}

	if (gEnv && gEnv->pSystem && gEnv->pSystem->GetUserCallback() != nullptr)
	{
		return gEnv->pSystem->GetUserCallback()->ShowMessage(szText, szCaption, type);
	}
	#endif

#if !defined(CRY_PLATFORM_MOBILE) && !defined(CRY_PLATFORM_LINUX)
	// Invoke platform-specific implementation
	EQuestionResult result = CryMessageBoxImpl(szText, szCaption, type);
#else
	EQuestionResult result = eQR_None;
#endif

	if (gEnv && gEnv->pSystem && gEnv->pLog)
	{
		CryLogAlways("Messagebox: cap: %s  text:%s\n", szCaption != nullptr ? szCaption : " ", szText != nullptr ? szText : " ");
	}
	else
	{
		printf("Messagebox: cap: %s  text:%s\n", szCaption != nullptr ? szCaption : " ", szText != nullptr ? szText : " ");
	}

	return result;
}

#if 0
EQuestionResult CryMessageBox(const wchar_t* szText, const wchar_t* szCaption, EMessageBox type)
{
	string text;
	Unicode::Convert(text, szText);
	string caption;
	Unicode::Convert(caption, szCaption);
	return CryMessageBox(text, caption, type);
}
#endif

#endif
