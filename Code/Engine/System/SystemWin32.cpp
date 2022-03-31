#include "System.hpp"

#if BB_PLATFORM_WINDOWS
	#include <float.h>
	//#include <BlackBox/Core/Platform/Windows.hpp>
#define IFont TestTest
	#include <shellapi.h> // Needed for ShellExecute.
	#include <Aclapi.h>
	#include <tlhelp32.h>

// HAX to resolve IEntity redefinition
	#define __IEntity_INTERFACE_DEFINED__
	#include <shlobj.h>
#undef IFont

	//#include "DebugCallStack.h"
#endif

//////////////////////////////////////////////////////////////////////////
void CSystem::FatalError(const char* format, ...)
{
	// format message
	va_list     ArgList;
	char        szBuffer[MAX_WARNING_LENGTH];
	const char* sPrefix = "";
	strcpy(szBuffer, sPrefix);
	va_start(ArgList, format);
	vsprintf(szBuffer + strlen(szBuffer) /*, sizeof(szBuffer) - strlen(szBuffer)*/, format, ArgList);
	va_end(ArgList);

#
	// get system error message before any attempt to write into log
	const char* szSysErrorMessage = CryGetLastSystemErrorMessage();

	CryLogAlways("=============================================================================");
	CryLogAlways("*ERROR");
	CryLogAlways("=============================================================================");
	// write both messages into log
	CryLogAlways("%s", szBuffer);

	if (szSysErrorMessage)
		CryLogAlways("<System> Last System Error: %s", szSysErrorMessage);

	assert(szBuffer[0] >= ' ');
	//strcpy(szBuffer,szBuffer+1);	// remove verbosity tag since it is not supported by ::MessageBox

	//LogSystemInfo();

	//CollectMemStats(0, nMSP_ForCrashLog);

	OutputDebugString(szBuffer);
	//OnFatalError(szBuffer);

	if (!g_cvars.sys_no_crash_dialog)
	{
		CryMessageBox(szBuffer, "ENGINE FATAL ERROR", eMB_Error);
	}

	//GetITextModeConsole()->OnShutdown();
	DebugBreak();
	//__debugbreak();
}

#if BB_PLATFORM_WINDOWS
//////////////////////////////////////////////////////////////////////////
bool CSystem::GetWinGameFolder(char* szMyDocumentsPath, int maxPathSize)
{
	bool bSucceeded = false;
	// check Vista and later OS first

	HMODULE shell32 = LoadLibraryA("Shell32.dll");
	if (shell32)
	{
		typedef long (__stdcall * T_SHGetKnownFolderPath)(REFKNOWNFOLDERID rfid, unsigned long dwFlags, void* hToken, wchar_t** ppszPath);
		T_SHGetKnownFolderPath _SHGetKnownFolderPath = (T_SHGetKnownFolderPath)GetProcAddress(shell32, "SHGetKnownFolderPath");
		if (_SHGetKnownFolderPath)
		{
			// We must be running Vista or newer
			wchar_t* wMyDocumentsPath;
			HRESULT hr = _SHGetKnownFolderPath(FOLDERID_SavedGames, KF_FLAG_CREATE | KF_FLAG_DONT_UNEXPAND, NULL, &wMyDocumentsPath);
			bSucceeded = SUCCEEDED(hr);
			if (bSucceeded)
			{
				// Convert from UNICODE to UTF-8
				snprintf(szMyDocumentsPath, maxPathSize, "%S", wMyDocumentsPath);
				CoTaskMemFree(wMyDocumentsPath);
			}
		}
		FreeLibrary(shell32);
	}

	if (!bSucceeded)
	{
		// check pre-vista OS if not succeeded before
		char wMyDocumentsPath[MAX_PATH];
		bSucceeded = SUCCEEDED(SHGetFolderPathA(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, 0, wMyDocumentsPath));
		if (bSucceeded)
		{
			strncpy(szMyDocumentsPath, wMyDocumentsPath, maxPathSize);
		}
	}

	return bSucceeded;
}
#endif

//////////////////////////////////////////////////////////////////////////
void CSystem::ChangeUserPath(const char* sUserPath)
{
	string userFolder = sUserPath;

#if BB_PLATFORM_WINDOWS
	bool folderCreated = false;

	#if defined(DEDICATED_SERVER)
	userFolder = "";              // Enforce userfolder as empty which will cause USER%d creation if root is not specified
	if (!(m_root.empty()))
	{
		m_sys_user_folder->Set(m_root.c_str());
		userFolder = m_root.c_str();
		folderCreated = true;
	}
	else
	#endif // defined(DEDICATED_SERVER)
	{
		if (userFolder.empty())
		{
			userFolder.resize(MAX_PATH);
			userFolder = "USER";
			m_env.pCryPak->MakeDir(userFolder.c_str());
		}
		else
		{
			char szMyDocumentsPath[MAX_PATH];
			if (GetWinGameFolder(szMyDocumentsPath, MAX_PATH))
			{
				string mydocs = string(szMyDocumentsPath) + "\\" + userFolder;
				mydocs = PathUtil::RemoveSlash(mydocs);
				mydocs = PathUtil::ToDosPath(mydocs);
				userFolder = mydocs;
				m_env.pCryPak->MakeDir(mydocs.c_str());
				folderCreated = true;
			}
		}
	}

	if (!folderCreated)
	{
		// pick a unique dir name for this instance
		int instance = GetApplicationInstance();
		if (instance != 0)
		{
			//;
			StringFormat(userFolder, "USER(%d)", instance);
			m_sys_user_folder->Set(userFolder.c_str());
		}

		// Make the userFolder path absolute
		char cwdBuffer[MAX_PATH];
		CryGetCurrentDirectory(MAX_PATH, cwdBuffer);
		string tempBuffer(MAX_PATH, '\0');
		#if 0
		tempBuffer.Format("%s\\%s", cwdBuffer, userFolder.c_str());
		#else
		StringFormat(tempBuffer, "%s\\%s", cwdBuffer, userFolder.c_str());
		#endif
		tempBuffer = PathUtil::RemoveSlash(tempBuffer);
		tempBuffer = PathUtil::ToDosPath(tempBuffer);
		userFolder = tempBuffer;

		Env::CryPak()->MakeDir(userFolder.c_str());
	}

#elif BB_PLATFORM_DURANGO
	userFolder = "USER";
	m_env.pCryPak->MakeDir(userFolder.c_str());
	m_sys_user_folder->Set(userFolder.c_str());
#elif BB_PLATFORM_ANDROID
	userFolder = CryGetUserStoragePath();
	m_sys_user_folder->Set(userFolder.c_str());
	m_env.pCryPak->MakeDir(userFolder.c_str());
#elif BB_PLATFORM_LINUX || BB_PLATFORM_ANDROID
	if (userFolder.empty())
	{
		userFolder = "USER";
		m_env.pCryPak->MakeDir(userFolder.c_str());
	}
	else
	{
		//TODO: Use Cocoa Shared Application folder to store info on Mac OS X
		const char* home = getenv("HOME");
		if (home != NULL)
		{
			string mydocs = string(home) + "/" + userFolder;
			mydocs = PathUtil::RemoveSlash(mydocs);
			userFolder = mydocs;
			m_env.pCryPak->MakeDir(userFolder.c_str());
		}
	}
#elif BB_PLATFORM_ORBIS
	userFolder = PathUtil::Make(m_env.pSystem->GetRootFolder(), "user");
	m_env.pCryPak->MakeDir(userFolder);
#endif

	m_env.pCryPak->SetAlias("%USER%", userFolder.c_str(), true);
}

//////////////////////////////////////////////////////////////////////////
int CSystem::GetApplicationInstance()
{
#if BB_PLATFORM_WINDOWS
	if (m_iApplicationInstance == -1)
	{
		string suffix;
		suffix.resize(MAX_PATH);
		for (int instance = 0;; ++instance)
		{
			StringFormat(suffix,"(%d)", instance);

			CreateMutex(NULL, TRUE, (string("BlackBoxApplication") + suffix).c_str());
			// search for duplicates
			if (GetLastError() != ERROR_ALREADY_EXISTS)
			{
				m_iApplicationInstance = instance;
				break;
			}
		}
	}

	return m_iApplicationInstance;
#elif BB_PLATFORM_LINUX
	if (m_iApplicationInstance == -1)
	{
		string path;
		for (int i = 0; i < std::numeric_limits<int>::max(); ++i)
		{
			path.Format("/tmp/CrytekApplication%d.lock", i);

			int fd = open(path.c_str(), O_CREAT | O_RDWR, 0666);
			BB_ASSERT(fd >= 0, "Failed to create mutex file, errno %d", errno);
			if (fd >= 0)
			{
				flock lock;
				memset(&lock, 0, sizeof(lock));
				lock.l_type = F_WRLCK;
				lock.l_whence = SEEK_SET;
				lock.l_start = 0;
				lock.l_len = 0; // Lock entire file
				if (fcntl(fd, F_SETLK, &lock) != -1)
				{
					// We're not calling close in case of success as
					// that would release the lock on the file
					m_iApplicationInstance = i;
					break;
				}
				else
				{
					close(fd);
				}
			}
		}
	}

	// Set a valid value in case something went wrong
	if (m_iApplicationInstance == -1)
	{
		m_iApplicationInstance = 0;
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "Unable to tetermine application ID");
	}

	return m_iApplicationInstance;
#else
	return 0;
#endif
}


