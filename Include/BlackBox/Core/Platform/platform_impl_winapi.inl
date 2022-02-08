#include <BlackBox\Core\Platform\Windows.hpp>

#include <string>
using std::wstring;
using std::string;

#include <codecvt> // for std::codecvt_utf8
#include <locale>  // for std::wstring_convert
#include <string>

//std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv_utf8_utf32;

namespace Unicode
{
	wstring Convert(wstring& widt, const char*& narow)
	{
		::std::wstring wideWhat;
		{
			int convertResult = MultiByteToWideChar(CP_UTF8, 0, narow, (int)strlen(narow), NULL, 0);
			if (convertResult <= 0)
			{
				wideWhat = L"Exception occurred: Failure to convert its message text using MultiByteToWideChar: convertResult=";
				wideWhat += std::to_wstring(convertResult);
				wideWhat += L"  GetLastError()=";
				wideWhat += std::to_wstring(GetLastError());
			}
			else
			{
				wideWhat.resize(convertResult + 10);
				convertResult = MultiByteToWideChar(CP_UTF8, 0, narow, (int)strlen(narow), &wideWhat[0], (int)wideWhat.size());
			}
		}
		widt = wideWhat;
		return widt;
	}
} // namespace Unicode

//////////////////////////////////////////////////////////////////////////
bool CryCreateDirectory(const char* lpPathName)
{
	// Convert from UTF-8 to UNICODE
	wstring widePath;
	Unicode::Convert(widePath, lpPathName);

	const DWORD dwAttr = ::GetFileAttributesW(widePath.c_str());
	if (dwAttr != INVALID_FILE_ATTRIBUTES && (dwAttr & FILE_ATTRIBUTE_DIRECTORY) != 0)
	{
		return true;
	}

	return ::CreateDirectoryW(widePath.c_str(), 0) != 0;
}

//////////////////////////////////////////////////////////////////////////
bool CryDirectoryExists(const char* szPath)
{
	// Convert from UTF-8 to UNICODE
	wstring widePath;
	Unicode::Convert(widePath, szPath);

	const DWORD dwAttr = ::GetFileAttributesW(widePath.c_str());
	if (dwAttr != INVALID_FILE_ATTRIBUTES && (dwAttr & FILE_ATTRIBUTE_DIRECTORY) != 0)
	{
		return true;
	}

	return false;
}

#if 0
//////////////////////////////////////////////////////////////////////////
void CryGetCurrentDirectory(unsigned int nBufferLength, char* lpBuffer)
{
	if (nBufferLength <= 0 || !lpBuffer)
	{
		return;
	}

	*lpBuffer = 0;

	// Get directory in UTF-16
	std::vector<wchar_t> buffer;
	{
		const size_t requiredLength = ::GetCurrentDirectoryW(0, 0);

		if (requiredLength <= 0)
		{
			return;
		}

		buffer.resize(requiredLength, 0);

		if (::GetCurrentDirectoryW(requiredLength, &buffer[0]) != requiredLength - 1)
		{
			return;
		}
	}

	// Convert to UTF-8
	if (Unicode::Convert<Unicode::eEncoding_UTF8, Unicode::eEncoding_UTF16>(lpBuffer, nBufferLength, &buffer[0]) > nBufferLength)
	{
		*lpBuffer = 0;
	}
}
#endif
//////////////////////////////////////////////////////////////////////////
void CrySetCurrentWorkingDirectory(const char* szWorkingDirectory)
{
	wstring cwd;
	Unicode::Convert(cwd, szWorkingDirectory).c_str();
	SetCurrentDirectoryW(cwd.data());
}

//////////////////////////////////////////////////////////////////////////
#include <BlackBox/Core/Path.hpp>
void CryGetExecutableFolder(unsigned int pathSize, char* szPath)
{
	/*W*/CHAR filePath[512];
	size_t nLen = GetModuleFileNameA(CryGetCurrentModule(), filePath, CRY_ARRAY_COUNT(filePath));

	if (nLen >= CRY_ARRAY_COUNT(filePath))
	{
		CryFatalError("The path to the current executable exceeds the expected length. TruncatedPath:%s", filePath);
	}

	if (nLen <= 0)
	{
		CryFatalError("Unexpected error encountered trying to get executable path. GetModuleFileNameW failed.");
	}

	if (auto pathEnd = strrchr(filePath, L'\\'))
	{
		pathEnd[1] = L'\0';
	}

	//size_t requiredLength = Unicode::Convert(szPath, pathSize, filePath);
	size_t requiredLength = strlen(filePath);
	if (requiredLength > pathSize)
	{
		CryFatalError("Executable path is to long. MaxPathSize:%u, PathSize:%u, Path:%s", pathSize, (uint)requiredLength, filePath);
	}
	strncpy(szPath, filePath, pathSize);
}

//////////////////////////////////////////////////////////////////////////
int CryGetWritableDirectory(unsigned int nBufferLength, char* lpBuffer)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////////
short CryGetAsyncKeyState(int vKey)
{
#if BB_PLATFORM_WINDOWS
	return GetAsyncKeyState(vKey);
#else
	return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
void* CryCreateCriticalSection()
{
	CRITICAL_SECTION* pCS = new CRITICAL_SECTION;
	InitializeCriticalSection(pCS);
	return pCS;
}

void CryCreateCriticalSectionInplace(void* pCS)
{
	InitializeCriticalSection((CRITICAL_SECTION*)pCS);
}
//////////////////////////////////////////////////////////////////////////
void CryDeleteCriticalSection(void* cs)
{
	CRITICAL_SECTION* pCS = (CRITICAL_SECTION*)cs;
	if (pCS->LockCount >= 0)
		CryFatalError("Critical Section hanging lock");
	DeleteCriticalSection(pCS);
	delete pCS;
}

//////////////////////////////////////////////////////////////////////////
void CryDeleteCriticalSectionInplace(void* cs)
{
	CRITICAL_SECTION* pCS = (CRITICAL_SECTION*)cs;
	if (pCS->LockCount >= 0)
		CryFatalError("Critical Section hanging lock");
	DeleteCriticalSection(pCS);
}

//////////////////////////////////////////////////////////////////////////
void CryEnterCriticalSection(void* cs)
{
	EnterCriticalSection((CRITICAL_SECTION*)cs);
}

//////////////////////////////////////////////////////////////////////////
bool CryTryCriticalSection(void* cs)
{
	return TryEnterCriticalSection((CRITICAL_SECTION*)cs) != 0;
}

//////////////////////////////////////////////////////////////////////////
void CryLeaveCriticalSection(void* cs)
{
	LeaveCriticalSection((CRITICAL_SECTION*)cs);
}

#if 0
//////////////////////////////////////////////////////////////////////////
uint32 CryGetFileAttributes(const char* lpFileName)
{
	// Normal GetFileAttributes not available anymore in non desktop applications (eg Durango)
	WIN32_FILE_ATTRIBUTE_DATA data;
	BOOL res;
#if BB_PLATFORM_DURANGO
	res = GetFileAttributesExA(lpFileName, GetFileExInfoStandard, &data);
#else
	wstring widePath;
	Unicode::Convert(widePath, lpFileName);
	res = GetFileAttributesExW(widePath.c_str(), GetFileExInfoStandard, &data);
#endif
	return res != 0 ? data.dwFileAttributes : -1;
}

//////////////////////////////////////////////////////////////////////////
bool CrySetFileAttributes(const char* lpFileName, uint32 dwFileAttributes)
{
#if BB_PLATFORM_DURANGO
	return SetFileAttributesA(lpFileName, dwFileAttributes) != 0;
#else
	wstring widePath;
	Unicode::Convert(widePath, lpFileName);
	return SetFileAttributesW(widePath.c_str(), dwFileAttributes) != 0;
#endif
}
#endif

namespace
{
	unsigned int GetMessageBoxType(EMessageBox type)
	{
		switch (type)
		{
		case eMB_YesCancel:
			return MB_OKCANCEL;
		case eMB_YesNoCancel:
			return MB_YESNOCANCEL;
			break;
		case eMB_Error:
			return MB_OK | MB_ICONWARNING | MB_SYSTEMMODAL;
			break;
		case eMB_AbortRetryIgnore:
			return MB_ABORTRETRYIGNORE | MB_ICONWARNING | MB_DEFBUTTON2 | MB_SYSTEMMODAL;

		default:
		case eMB_Info:
			return MB_OK;
		}
	}

	EQuestionResult GetMessageBoxResult(int returnValue)
	{
		switch (returnValue)
		{
		case IDABORT:
			return eQR_Abort;
		case IDCANCEL:
			return eQR_Cancel;
		case IDIGNORE:
			return eQR_Ignore;
		case IDNO:
			return eQR_No;
		case IDYES:
		case IDOK:
			return eQR_Yes;
		case IDRETRY:
			return eQR_Retry;
		default:
			return eQR_None;
		}
	}
}

EQuestionResult CryMessageBoxImpl(const char* szText, const char* szCaption, EMessageBox type)
{
#if BB_PLATFORM_WINDOWS
	return GetMessageBoxResult(::MessageBoxA(nullptr, szText, szCaption, GetMessageBoxType(type)));
#else
	BB_ASSERT(false, "Should not be called, the actual implementation is in ApplicationView::DurangoSystemCallback::ShowMessage().");
	return eQR_None;
#endif
}

//////////////////////////////////////////////////////////////////////////
const char* CryGetSystemErrorMessage(DWORD error)
{
	static char szBuffer[2048]; // function will return pointer to this buffer
#if BB_PLATFORM_DURANGO
	cry_sprintf(szBuffer, "Last error code was %d.", error);
	return szBuffer;
#else
	if (error)
	{
		LPVOID lpMsgBuf = 0;

		if (FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR)&lpMsgBuf,
			0,
			NULL))
		{
			strcpy(szBuffer, (char*)lpMsgBuf);
			LocalFree(lpMsgBuf);
			return szBuffer;
		}
	}
	return nullptr;
#endif
}

const char* CryGetLastSystemErrorMessage()
{
	return CryGetSystemErrorMessage(GetLastError());
}

void CryClearSytemError()
{
	SetLastError(0);
}

bool CryIsDebuggerPresent()
{
	return IsDebuggerPresent() != 0;
}