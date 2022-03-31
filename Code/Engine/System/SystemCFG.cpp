#include "pch.hpp"
#include "System.hpp"

#include <BlackBox/System/File/CryFile.h>
#include "SystemCFG.hpp"

namespace detail
{
	string Trim(string& str)
	{
		int begin = 0;
		int end   = 0;
		for (int i = 0; i < str.size() && isspace(str[i]); i++)
		{
			begin++;
		}
		for (int i = str.size() - 1; i >= 0 && isspace(str[i]); i--)
		{
			end++;
		}
		str.resize(str.size() - end);
		str.erase(0, begin);
		return str;
	}
} // namespace detail

#ifndef EXE_VERSION_INFO_0
	#define EXE_VERSION_INFO_0 1
#endif

#ifndef EXE_VERSION_INFO_1
	#define EXE_VERSION_INFO_1 0
#endif

#ifndef EXE_VERSION_INFO_2
	#define EXE_VERSION_INFO_2 0
#endif

#ifndef EXE_VERSION_INFO_3
	#define EXE_VERSION_INFO_3 1
#endif

#if CRY_PLATFORM_DURANGO
	#include <xdk.h>
#if _XDK_EDITION < 180400
	#error "Outdated XDK, please update to at least XDK edition April 2018"
#endif // #if _XDK_EDITION
#include <BlackBox/System/File/CryFile.h>
#endif // #if CRY_PLATFORM_DURANGO

//////////////////////////////////////////////////////////////////////////
const SFileVersion& CSystem::GetFileVersion()
{
	return m_FileVersion;
}

//////////////////////////////////////////////////////////////////////////
const SFileVersion& CSystem::GetProductVersion()
{
	return m_ProductVersion;
}

//////////////////////////////////////////////////////////////////////////
const SFileVersion& CSystem::GetBuildVersion()
{
	return m_BuildVersion;
}

//////////////////////////////////////////////////////////////////////////
void CSystem::SystemVersionChanged(ICVar* pCVar)
{
	if (CSystem* pThis = static_cast<CSystem*>(Env::System()))
	{
		pThis->SetVersionInfo(pCVar->GetString());
	}
}

void CSystem::SetVersionInfo(const char* const szVersion)
{
#ifndef _RELEASE
	m_FileVersion.Set(szVersion);
	m_ProductVersion.Set(szVersion);
	m_BuildVersion.Set(szVersion);
	CryLog("SetVersionInfo '%s'", szVersion);
	CryLog("FileVersion: %s", m_FileVersion.ToString().c_str());
	CryLog("ProductVersion: %s", m_ProductVersion.ToString().c_str());
	CryLog("BuildVersion: %s", m_BuildVersion.ToString().c_str());
#endif
}

//////////////////////////////////////////////////////////////////////////
void CSystem::QueryVersionInfo()
{
#if !BB_PLATFORM_WINDOWS
	//do we need some other values here?
	m_FileVersion[0] = m_ProductVersion[0] = EXE_VERSION_INFO_3;
	m_FileVersion[1] = m_ProductVersion[1] = EXE_VERSION_INFO_2;
	m_FileVersion[2] = m_ProductVersion[2] = EXE_VERSION_INFO_1;
	m_FileVersion[3] = m_ProductVersion[3] = EXE_VERSION_INFO_0;
	m_BuildVersion = m_FileVersion;
#else
	char moduleName[_MAX_PATH];
	DWORD dwHandle;
	UINT len;

	char ver[1024 * 8];

	GetModuleFileName(NULL, moduleName, _MAX_PATH);  //retrieves the PATH for the current module

	#ifdef _LIB
	GetModuleFileName(NULL, moduleName, _MAX_PATH);  //retrieves the PATH for the current module
	#else //_LIB
	cry_strcpy(moduleName, "CrySystem.dll"); // we want to version from the system dll
	#endif //_LIB

	int verSize = GetFileVersionInfoSize(moduleName, &dwHandle);
	if (verSize > 0)
	{
		GetFileVersionInfo(moduleName, dwHandle, 1024 * 8, ver);
		VS_FIXEDFILEINFO* vinfo;
		VerQueryValue(ver, "\\", (void**)&vinfo, &len);

		m_FileVersion[0] = m_ProductVersion[0] = vinfo->dwFileVersionLS & 0xFFFF;
		m_FileVersion[1] = m_ProductVersion[1] = vinfo->dwFileVersionLS >> 16;
		m_FileVersion[2] = m_ProductVersion[2] = vinfo->dwFileVersionMS & 0xFFFF;
		m_FileVersion[3] = m_ProductVersion[3] = vinfo->dwFileVersionMS >> 16;
		m_BuildVersion = m_FileVersion;

		struct LANGANDCODEPAGE
		{
			WORD wLanguage;
			WORD wCodePage;
		}* lpTranslate;

		UINT count = 0;
		char path[256];
		char* version = NULL;

		VerQueryValue(ver, "\\VarFileInfo\\Translation", (LPVOID*)&lpTranslate, &count);
		if (lpTranslate != NULL)
		{
			sprintf(path, "\\StringFileInfo\\%04x%04x\\InternalName", lpTranslate[0].wLanguage, lpTranslate[0].wCodePage);
			VerQueryValue(ver, path, (LPVOID*)&version, &count);
			if (version)
			{
				m_BuildVersion.Set(version);
			}
		}
	}
#endif // !CRY_PLATFORM_WINDOWS
}


//////////////////////////////////////////////////////////////////////////
void CSystem::LogVersion()
{
	// Get time.
	time_t ltime;
	time(&ltime);
	tm* today = localtime(&ltime);

	char s[1024];

	strftime(s, 128, "%d %b %y (%H %M %S)", today);

#if !defined(EXCLUDE_NORMAL_LOG)
	const SFileVersion& ver = GetFileVersion();
	CryLogAlways("BackupNameAttachment=\" Build(%d) %s\"  -- used by backup system\n", ver[0], s);      // read by CreateBackupFile()
#endif

	// Use strftime to build a customized time string.
	strftime(s, 128, "Log Started at %c", today);
	CryLogAlways("%s", s);

	CryLogAlways("Built on " __DATE__ " " __TIME__);

#if CRY_PLATFORM_DURANGO
	CryLogAlways("Running 64 bit Durango version XDK VER:%d", _XDK_VER);
#elif CRY_PLATFORM_ORBIS
	CryLogAlways("Running 64 bit Orbis version SDK VER:0x%08X", SCE_ORBIS_SDK_VERSION);
#elif CRY_PLATFORM_ANDROID
	CryLogAlways("Running 64 bit Android version API VER:%d", __ANDROID_API__);
#elif CRY_PLATFORM_IOS
	CryLogAlways("Running 64 bit iOS version");
#elif CRY_PLATFORM_WINDOWS
	CryLogAlways("Running 64 bit Windows version");
#elif CRY_PLATFORM_LINUX
	CryLogAlways("Running 64 bit Linux version");
#elif CRY_PLATFORM_MAC
	CryLogAlways("Running 64 bit Mac version");
#endif
	CryLogAlways("Command Line: %s", m_pCmdLine->GetCommandLine());
#if !CRY_PLATFORM_LINUX && !CRY_PLATFORM_ANDROID && !CRY_PLATFORM_APPLE && !CRY_PLATFORM_DURANGO && !CRY_PLATFORM_ORBIS
	GetModuleFileName(NULL, s, sizeof(s));
	CryLogAlways("Executable: %s", s);
#endif

	CryLogAlways("FileVersion: %d.%d.%d.%d", m_FileVersion[3], m_FileVersion[2], m_FileVersion[1], m_FileVersion[0]);
	CryLogAlways("ProductVersion: %d.%d.%d.%d", m_ProductVersion[3], m_ProductVersion[2], m_ProductVersion[1], m_ProductVersion[0]);
}

CSystemConfiguration::CSystemConfiguration(const string& strSysConfigFilePath, CSystem* pSystem, ILoadConfigurationEntrySink* pSink)
    : m_strSysConfigFilePath(strSysConfigFilePath)
    , m_bError(false)
    , m_pSink(pSink)
{
	CRY_ASSERT(pSink);
	m_pSystem = pSystem;
	m_bError  = !ParseSystemConfig();
}

//////////////////////////////////////////////////////////////////////////
bool CSystemConfiguration::OpenFile(const string& filename, CCryFile& file, int flags)
{
	flags |= ICryPak::FOPEN_HINT_QUIET;

	// Absolute paths first
	if (Env::CryPak()->IsAbsPath(filename.c_str()) || filename[0] == '%')
	{
		if (file.Open(filename.c_str(), "rb", flags | ICryPak::FLAGS_PATH_REAL))
		{
			return true;
		}
		else
		{
			// the file is absolute and was not found, it is not useful to search further
			return false;
		}
	}

	// If the path is relative, search relevant folders in given order:
	// First search in current folder.
	if (file.Open((string("./") + filename).c_str(), "rb", flags | ICryPak::FLAGS_PATH_REAL))
	{
		return true;
	}

	string gameFolder = PathUtil::RemoveSlash(Env::CryPak()->GetGameFolder());
	if (!filename.empty() && filename[0] == '%')
	{
		// When file name start with alias, not check game folder.
		gameFolder = "";
	}

	// Next search inside game folder (ex, game/game.cfg)
	if (!gameFolder.empty() && file.Open(gameFolder + "/" + filename, "rb", flags | ICryPak::FLAGS_PATH_REAL))
	{
		return true;
	}

	// Next Search in registered mod folders.
	if (file.Open(filename, "rb", flags))
	{
		return true;
	}

	// Next Search in game/config subfolder.
	if (!gameFolder.empty() && file.Open(gameFolder + "/config/" + filename, "rb", flags | ICryPak::FLAGS_PATH_REAL))
	{
		return true;
	}

	// Next Search in config subfolder.
	if (file.Open(string("config/") + filename, "rb", flags | ICryPak::FLAGS_PATH_REAL))
	{
		return true;
	}

	// Next Search in engine root.
	if (file.Open(string("%ENGINEROOT%/") + filename, "rb", flags))
	{
		return true;
	}

	// Next Search in engine folder.
	if (file.Open(string("%ENGINEROOT%/Engine/") + filename, "rb", flags))
	{
		return true;
	}

	// Next Search in engine config subfolder, in case loosely stored on drive
	if (file.Open(string("%ENGINEROOT%/Engine/config/") + filename, "rb", flags))
	{
		return true;
	}

	return false;
}


bool CSystemConfiguration::ParseSystemConfig()
{
	
	string filename = m_strSysConfigFilePath;
	if (strlen(PathUtil::GetExt(filename.c_str())) == 0)
	{
		filename = PathUtil::ReplaceExtension(filename.c_str(), "cfg");
	}

	CCryFile file;

	{
		string filenameLog;

		int flags = ICryPak::FOPEN_HINT_QUIET | ICryPak::FOPEN_ONDISK;

		if (!OpenFile(filename, file, flags))
		{
			if (ELoadConfigurationFlags::None == ELoadConfigurationFlags((uint32(m_flags) & uint32(ELoadConfigurationFlags::SuppressConfigNotFoundWarning))))
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Config file %s not found!", filename.c_str());
			}
			return false;
		}
		filenameLog = file.GetAdjustedFilename();

		CryLog("Loading Config file %s (%s)", filename.c_str(), filenameLog.c_str());
	}

	//INDENT_LOG_DURING_SCOPE();

	int nLen = file.GetLength();
	if (nLen == 0)
	{
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Couldn't get length for Config file %s", filename.c_str());
		return false;
	}
	char* szFullText = new char[nLen + 16];
	if (file.ReadRaw(szFullText, nLen) < (size_t)nLen)
	{
		CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Couldn't read Config file %s", filename.c_str());
		return false;
	}
	szFullText[nLen] = '\0';
	szFullText[nLen + 1] = '\0';

	string strGroup; // current group e.g. "[General]"

	char* strLast = szFullText + nLen;
	char* str = szFullText;
	while (str < strLast)
	{
		char* szLineStart = str;
		// find the first line break
		while (str < strLast && *str != '\n' && *str != '\r')
			str++;
		// clear it to handle the line as one string
		*str = '\0';
		str++;
		// and skip any remaining line break chars
		while (str < strLast && (*str == '\n' || *str == '\r'))
			str++;

		//trim all whitespace characters at the beginning and the end of the current line
		string strLine = szLineStart;
		strLine        = detail::Trim(strLine);

		//skip empty lines
		if (strLine.empty())
			continue;

// detect groups e.g. "[General]" should set strGroup="General"
#if 0
		const size_t strLineSize = strLine.size();
		if (strLineSize >= 3)
		{
			if (strLine[0] == '[' && strLine[strLineSize - 1] == ']') // currently no comments are allowed to be behind groups
			{
				strGroup = &strLine[1]; // removes '['
				strGroup.pop_back();    // removes ']'
				continue;               // next line
			}
		}
#endif

		//skip comments, comments start with ";" or "--" (any preceding whitespaces have already been trimmed)
		if (strLine[0] == ';' || strLine.find("--") == 0)
			continue;

		//if line contains a '=' try to read and assign console variable
		const string::size_type posEq(strLine.find("=", 0));
		if (string::npos != posEq)
		{
			string strKey(strLine, 0, posEq);
			strKey = detail::Trim(strKey);

			// extract value and remove surrounding quotes, if present
			string strValue(strLine, posEq + 1, strLine.size() - (posEq + 1));
			strValue = detail::Trim(strValue);
			if (strValue.front() == '"' && strValue.back() == '"')
			{
				strValue.pop_back();
				strValue.erase(0, 1);
			}

			//strValue.replace()
			//strValue.replace("\\\\", "\\");
			//strValue.replace("\\\"", "\"");

			if (strKey == "r_DisplayIndex")
			{
				CryLog("here r_DisplayIndex");
			}
			m_pSink->OnLoadConfigurationEntry(strKey.c_str(), strValue.c_str(), nullptr);
		}
		else
		{
			Env::Log()->LogWarning("%s -> invalid configuration line: %s", filename.c_str(), strLine.c_str());
		}
	}

	m_pSink->OnLoadConfigurationEntry_End();
	return true;
	return false;
}

//////////////////////////////////////////////////////////////////////////
void CSystem::OnLoadConfigurationEntry(const char* szKey, const char* szValue, const char* szGroup)
{
	if (!Env::Console())
		return;

	if (*szKey != 0)
		Env::Console()->LoadConfigVar(szKey, szValue);
}

void CSystem::LoadConfiguration(const char* sFilename, ILoadConfigurationEntrySink* pSink, ELoadConfigurationType configType, ELoadConfigurationFlags flags)
{
	if (sFilename && strlen(sFilename) > 0)
	{
		CSystemConfiguration tempConfig(sFilename, this, this);
	}
	else
	{
		CSystemConfiguration tempConfig("system.cfg", this, this);
	}
}
