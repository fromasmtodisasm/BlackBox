#pragma once

#ifndef _PLATFORM_IMPL_H_
#define _PLATFORM_IMPL_H_
#endif

#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/Platform/CryLibrary.h>

#include <BlackBox/Math/Cry_Math.h>

//////////////////////////////////////////////////////////////////////////
// Global environment variable.
//////////////////////////////////////////////////////////////////////////
namespace Env
{
#if defined(SYS_ENV_AS_STRUCT)
	#if defined(_LAUNCHER)
	SSystemGlobalEnvironment gEnv;
	#else
	extern SSystemGlobalEnvironment gEnv;
	#endif
#else
	struct SSystemGlobalEnvironment* gEnv = nullptr;
#endif
}

#if (defined(_LAUNCHER) && defined(IS_MONOLITHIC_BUILD)) || !defined(_LIB)
//The reg factory is used for registering the different modules along the whole project
struct SRegFactoryNode*    g_pHeadToRegFactories = nullptr;
std::vector<const char*>   g_moduleCommands;
std::vector<const char*>   g_moduleCVars;

extern "C" DLL_EXPORT void CleanupModuleCVars()
{
	if (auto pConsole = Env::Console())
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

#if !defined(_LIB) || defined(_LAUNCHER)
//////////////////////////////////////////////////////////////////////////
// If not in static library.

	#include <BlackBox/Math/Cry_Math.h>
	#include <BlackBox/Core/TypeInfo_impl.h>

	#define CRY_PLATFORM_IMPL_H_FILE 1
	#include <BlackBox/Core/CryTypeInfo.inl>
	#if CRY_PLATFORM_POSIX
		#include "WinBase.inl"
	#endif
	#undef CRY_PLATFORM_IMPL_H_FILE

	#if BB_PLATFORM_WINDOWS
void CryPureCallHandler()
{
	CryFatalError("Pure function call");
}

void CryInvalidParameterHandler(
    const wchar_t* expression,
    const wchar_t* function,
    const wchar_t* file,
    unsigned int   line,
    uintptr_t      pReserved)
{
	return;
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
	#else
void InitCRTHandlers()
{
}
	#endif

//////////////////////////////////////////////////////////////////////////
// This is an entry to DLL initialization function that must be called for each loaded module
//////////////////////////////////////////////////////////////////////////
extern "C" DLL_EXPORT void ModuleInitISystem(ISystem* pSystem, const char* moduleName)
{
	if (Env::Get()) // Already registered.
		return;

	#if defined(USE_CRY_ASSERT)
	CryAssertSetGlobalFlagAddress(pSystem->GetAssertFlagAddress());
	#endif

	#if !defined(SYS_ENV_AS_STRUCT)
	if (pSystem) // DONT REMOVE THIS. ITS FOR RESOURCE COMPILER!!!!
		Env::Set(pSystem->GetGlobalEnvironment());
	#endif

	InitCRTHandlers();

	#if !defined(SYS_ENV_AS_STRUCT)
	if (pSystem) // DONT REMOVE THIS. ITS FOR RESOURCE COMPILER!!!!
		Env::Set(pSystem->GetGlobalEnvironment());
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

	#if !defined(IS_MONOLITHIC_BUILD)
	CCryLibrary systemLibrary(CryLibraryDefName("System"));
	if (!systemLibrary.IsLoaded())
	{
		string errorStr = string(512, '\0');
		sprintf(errorStr.data(), "%s", "Failed to load the " CryLibraryDefName("System") " library!");
		CryMessageBox(errorStr.c_str(), "Engine initialization failed!");
		return false;
	}

	PFNCREATESYSTEMINTERFACE CreateSystemInterface = systemLibrary.GetProcedureAddress<PFNCREATESYSTEMINTERFACE>("CreateSystemInterface");
	if (CreateSystemInterface == nullptr)
	{
		string errorStr = string(512, '\0');
		sprintf(errorStr.data(), "%s", CryLibraryDefName("System") " library was invalid, entry-point not found!");
		CryMessageBox(errorStr.c_str(), "Engine initialization failed!");

		return false;
	}
	#endif

	startupParams.bManualEngineLoop = bManualEngineLoop;
	if (CreateSystemInterface(startupParams /*, bManualEngineLoop*/) != nullptr)
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

	#if BB_PLATFORM_WINAPI || BB_PLATFORM_LINUX
//////////////////////////////////////////////////////////////////////////
void CryFindEngineRootFolder(unsigned int engineRootPathSize, char* szEngineRootPath)
{
		#if BB_PLATFORM_WINAPI
	char osSeperator = '\\';
		#elif BB_PLATFORM_POSIX
	char osSeperator = '/';
		#endif
	char szExecFilePath[_MAX_PATH] = "";
	CryGetExecutableFolder(ARRAY_COUNT(szExecFilePath), szExecFilePath);

	string strTempPath(szExecFilePath);
	size_t nCurDirSlashPos = strTempPath.size() - 1;

	// Try to find directory named "Engine" or "engine"
	do
	{
		bool bFoundMatch = false;
		for (int i = 0; i < 2; ++i)
		{
			strTempPath.erase(nCurDirSlashPos + 1, string::npos);
			strTempPath.append(i == 0 ? "Engine" : "engine");

			if (CryDirectoryExists(strTempPath.c_str()))
			{
				bFoundMatch = true;
				break;
			}
		}
		if (bFoundMatch)
		{
			break;
		}
		// Move up to parent directory
		nCurDirSlashPos = strTempPath.rfind(osSeperator, nCurDirSlashPos - 1);

	} while (nCurDirSlashPos != 0 && nCurDirSlashPos != string::npos);

	if (nCurDirSlashPos == 0 || nCurDirSlashPos == string::npos)
	{
		CryFatalError("Unable to locate CryEngine root folder. Ensure that the 'engine' folder exists in your CryEngine root directory");
		return;
	}

	strTempPath.erase(strTempPath.rfind(osSeperator) + 1, string::npos);
	if (!strcpy(szEngineRootPath, /*engineRootPathSize, */ strTempPath.c_str()))
	{
		CryFatalError("CryEngine root path is to long. MaxPathSize:%u, PathSize:%u, Path:%s", engineRootPathSize, (uint)strTempPath.length() + 1, strTempPath.c_str());
	}
}

	#endif

int64 CryGetTicks()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return li.QuadPart;
}

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
	#if 1
		#if 0
	if (Env::Get() && Env::Get()->bUnattendedMode)
	{
		return eQR_None;
	}
		#endif

	if (Env::Get() && Env::System() && Env::System()->GetUserCallback() != nullptr)
	{
		return Env::System()->GetUserCallback()->ShowMessage(szText, szCaption, type);
	}
	#endif

	#if !defined(CRY_PLATFORM_MOBILE) && !defined(CRY_PLATFORM_LINUX)
	// Invoke platform-specific implementation
	EQuestionResult result = CryMessageBoxImpl(szText, szCaption, type);
	#else
	EQuestionResult result = eQR_None;
	#endif

	if (Env::Get() && Env::System() && Env::Log())
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
