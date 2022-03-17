#ifndef LINUX
	#include <BlackBox/Core/Platform/Platform.hpp>
	#include <BlackBox/Core/Platform/Windows.hpp>
	#include "CryPak.hpp"

	#include <algorithm>
	#include <cctype>
	#include <cstdarg>
	#include <string>

	#define BB_NATIVE_PATH_SEPSTR "/"

using namespace ZipFile;

//FIXME:
using stack_string = string;

namespace my
{
	bool replace(std::string& str, const std::string& from, const std::string& to)
	{
		size_t start_pos = str.find(from);
		if (start_pos == std::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}

	//std::string string("hello $name");
	//replace(string, "$name", "Somename");
} // namespace my

//////////////////////////////////////////////////////////////////////////
bool CCryPak::AdjustAliases(CryPathString& dst)
{
	bool foundAlias = false;
	for (tNameAlias* pAlias : m_arrAliases)
	{
		// Replace the alias if it's the first item in the path
		if (strncmp(dst.c_str(), pAlias->szName, pAlias->nLen1) == 0 && dst[pAlias->nLen1] == g_cNativeSlash)
		{
			foundAlias = true;
			dst.replace(0, pAlias->nLen1, pAlias->szAlias);
		}

		// Strip extra aliases from path
		stack_string searchAlias(32, ' ');
	#if 0
		searchAlias.Format("%c%s%c", g_cNativeSlash, pAlias->szName, g_cNativeSlash);
	#else
		StringFormat(searchAlias, "%c%s%c", g_cNativeSlash, pAlias->szName, g_cNativeSlash);
	#endif
		my::replace(dst, searchAlias.c_str(), "");
	}
	return foundAlias;
}

void CCryPak::SetLog(IMiniLog* pLog)
{
	m_pLog = pLog;
}

// makes the path lower-case and removes the duplicate and non native slashes
// may make some other fool-proof stuff
// may NOT write beyond the string buffer (may not make it longer)
// returns: the pointer to the ending terminator \0
char* CCryPak::BeautifyPath(char* dst, bool bMakeLowercase)
{
	// make the path lower-letters and with native slashes
	char *p, *q;
	// there's a special case: two slashes at the beginning mean UNC filepath
	p = q = dst;
	if (*p == g_cNonNativeSlash || *p == g_cNativeSlash)
		++p, ++q; // start normalization/beautifications from the second symbol; if it's a slash, we'll add it, too

	bool bMakeLower = false;

	while (*p)
	{
		if (*p == g_cNonNativeSlash || *p == g_cNativeSlash)
		{
			*q = g_cNativeSlash;
			++p, ++q;
			while (*p == g_cNonNativeSlash || *p == g_cNativeSlash)
				++p; // skip the extra slashes
		}
		else
		{
			if (*p == '%')
				bMakeLower = !bMakeLower;

			if (bMakeLower || bMakeLowercase)
				//*q = CryStringUtils::toLowerAscii(*p);
				*q = std::tolower(*p);
			else
				*q = *p;
			++q, ++p;
		}
	}
	*q = '\0';
	return q;
}

// remove all '%s/..' or '.' parts from the path (needs beautified path - only single native slashes)
// e.g. Game/Scripts/AI/../Entities/foo -> Game/Scripts/Entities/foo
// e.g. Game/Scripts/./Entities/foo -> Game/Scripts/Entities/foo
void CCryPak::RemoveRelativeParts(char* dst)
{
	char* q = dst;
	char* p = nullptr;

	PREFAST_ASSUME(q);

	// replace all '/./' with '/'
	const char slashDotSlashString[4] = {CCryPak::g_cNativeSlash, '.', CCryPak::g_cNativeSlash, 0};

	while ((p = strstr(q, slashDotSlashString)) != nullptr)
	{
		//Move the string and the null terminator
		memmove(p, p + 2, strlen(p + 2) + 1);
	}

	// replace all '/%s/../' with '/'
	const char slashDotDotSlashString[5] = {CCryPak::g_cNativeSlash, '.', '.', CCryPak::g_cNativeSlash, 0};

	while ((p = strstr(q, slashDotDotSlashString)) != nullptr)
	{
		if (p != q) // only remove if not in front of a path
		{
			int  i        = 4;
			bool bSpecial = true;
			while (*(--p) != CCryPak::g_cNativeSlash && p != q)
			{
				if (*p != '.')
				{
					bSpecial = false;
				}
				i++;
			}
			if (!bSpecial)
			{
				memmove(p, p + i, strlen(p + i) + 1);
				continue;
			}
		}
		q += 3;
	}
}

namespace filehelpers
{
	//////////////////////////////////////////////////////////////////////////
	inline bool CheckPrefix(const char* str, const char* prefix)
	{
		//this should rather be a case insensitive check here, so strnicmp is used instead of strncmp
		return (strnicmp(str, prefix, strlen(prefix)) == 0);
	}

	//////////////////////////////////////////////////////////////////////////
	//inline ICryPak::SignedFileSize GetFileSizeOnDisk(const char* filename)
	//{
	//	return gEnv->pCryPak->GetFileSizeOnDisk(filename);
	//}

	////////////////////////////////////////////////////////////////////////////
	//inline bool CheckFileExistOnDisk(const char* filename)
	//{
	//	return GetFileSizeOnDisk(filename) != ICryPak::FILE_NOT_PRESENT;
	//}

}; // namespace filehelpers

CCryPak::CCryPak(IMiniLog* pLog, PakVars* pPakVars, const bool bLvlRes)
    : m_pLog(pLog)
{
	std::replace(m_strDataRootWithSlash.begin(), m_strDataRootWithSlash.end(), g_cNativeSlash, g_cNonNativeSlash);
	m_strDataRootWithSlash[m_strDataRootWithSlash.size() - 1] = g_cNativeSlash;
	m_arrOpenFiles.resize(128);

	m_pPakVars = pPakVars;
}

CCryPak::~CCryPak()
{
}

bool CCryPak::Init(const char* szBasePath)
{
	return InitPack(szBasePath);
}

bool CCryPak::IsModPath(const char* szPath)
{
	#if !defined(PURE_CLIENT)
	CryPathString path = szPath;
	std::replace(path.begin(), path.end(), g_cNativeSlash, g_cNonNativeSlash);
	for (auto it = m_arrMods.begin(); it != m_arrMods.end(); ++it)
	{
		#if 0
		if (filehelpers::CheckPrefix(path.c_str(), it->path.c_str()))
		#else
		if (filehelpers::CheckPrefix(path.c_str(), it->c_str()))
		#endif
		{
			return true;
		}
	}
	#endif //!defined(PURE_CLIENT)
	return false;
}

void CCryPak::Release()
{
	if (this != nullptr)
		delete this;
}

bool CCryPak::OpenPackCommon(const char* szBindRoot, const char* szFullPath, unsigned int nPakFlags, IMemoryBlock* pData)
{
	SArchiveHandle ar{szFullPath};
	if (!ar)
	{
		ar = SArchiveHandle{szFullPath};
		if (!ar) return false;
	}

	CryComment("Opening pak file %s", szFullPath);

	for (auto& entry : ar)
	{
		File file{create_file(entry, ar.header)};

		if (auto it = m_Files.find(file.name); it != m_Files.end()) printf("Eroror, file %s already mapped\n", file.name.data());

		m_Files[file.name] = file;
	}

	m_Archives.emplace_back(std::move(ar));

	return true;
}

bool CCryPak::OpenPack(const char* szPath, unsigned nFlags)
{
	CryPathString fullPath;
	fullPath.resize(_MAX_PATH);
	AdjustFileName(szPath, fullPath.data(), nFlags);

	CryPathString bindRoot;
	size_t        lastSlashIdx = fullPath.rfind(g_cNativeSlash);

	#if BB_PLATFORM_LINUX || BB_PLATFORM_ANDROID || BB_PLATFORM_APPLE || BB_PLATFORM_ORBIS
	if (lastSlashIdx == CryPathString::npos)
	{
		lastSlashIdx = fullPath.rfind(g_cNonNativeSlash);
	}
	#endif
	if (lastSlashIdx != CryPathString::npos)
	{
		bindRoot.assign(fullPath, lastSlashIdx + 1);
	}
	else
	{
		m_pLog->LogError("Pak file %s has absolute path %s, which is strange", szPath, fullPath.c_str());
	}

	bool result = OpenPackCommon(bindRoot.data(), fullPath.data(), nFlags, nullptr);

	#if 0
	if (pFullPath)
	{
		pFullPath->assign(fullPath);
	}
	#endif

	return result;
}

bool CCryPak::OpenPack(const char* pBindingRoot, const char* pName, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	return OpenPack(pName, nFlags);
}

bool CCryPak::ClosePack(const char* pName, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	return false;
}

bool CCryPak::OpenPacksCommon(const char* szDir, const char* pWildcardIn, CryPathString& wildcardPath, int nPakFlags, std::vector<CryPathString>* pFullPaths)
{
	if (wildcardPath.find('*') == CryPathString::npos && wildcardPath.find('?') != CryPathString::npos)
	{
		// No wildcards, just open pack
		if (OpenPackCommon(szDir, wildcardPath.data(), nPakFlags))
		{
			if (pFullPaths)
			{
				pFullPaths->push_back(wildcardPath);
			}
		}
		return true;
	}

	// Note this code suffers from a common FindFirstFile problem - a search
	// for *.pak will also find *.pak? (since the short filename version of *.pakx,
	// for example, is *.pak). For more information, see
	// http://blogs.msdn.com/oldnewthing/archive/2005/07/20/440918.aspx
	// Therefore this code performs an additional check to make sure that the
	// found filenames match the spec.
	_finddata_t   fd;
	intptr_t      h = FindFirst(wildcardPath.data(), &fd /* , FLAGS_PATH_REAL */);

	CryPathString wildcardFullPath;
	#if 0
	wildcardFullPath.Format("*.%s", PathUtil::GetExt(pWildcardIn));
	#else
	wildcardFullPath.resize(256);
	sprintf(wildcardFullPath.data(), "*.%s", PathUtil::GetExt(pWildcardIn));
	#endif

	// buffer to concatenate filenames to the directory part of wildcardPath
	//CryPathString::MAX_SIZE = 256
	char destName[256];
	strcpy(destName, wildcardPath.data());
	char* pDirectoriesEnd = strrchr(destName, g_cNativeSlash);

	#if BB_PLATFORM_LINUX || BB_PLATFORM_ANDROID || BB_PLATFORM_APPLE || BB_PLATFORM_ORBIS
	if (!pDirectoriesEnd)
		pDirectoriesEnd = strrchr(destName, g_cNonNativeSlash);
	#endif
	if (!pDirectoriesEnd)
		pDirectoriesEnd = destName;
	else
		++pDirectoriesEnd;

	const size_t remainingBufferCapacity = sizeof(destName) - (pDirectoriesEnd - destName);
	if (h != -1)
	{
		std::vector<string> files;
		do
		{
			strncpy(pDirectoriesEnd, fd.name, remainingBufferCapacity);
			if (PathUtil::MatchWildcard(destName, wildcardFullPath.data()))
				files.push_back(strlwr(destName));
		} while (FindNext(h, &fd) >= 0);

		// Open files in alphabet order.
		std::sort(files.begin(), files.end());
		bool bAllOk = true;
		for (int i = 0; i < (int)files.size(); i++)
		{
			bAllOk = OpenPackCommon(szDir, files[i].c_str(), nPakFlags) && bAllOk;

			if (pFullPaths)
			{
				pFullPaths->push_back(files[i].c_str());
			}
		}

		FindClose(h);
		return bAllOk;
	}

	return false;
}

bool CCryPak::OpenPacks(const char* pWildcardIn, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	CryPathString wildcardPath;
	wildcardPath.resize(_MAX_PATH);
	AdjustFileName(pWildcardIn, wildcardPath.data(), nFlags | FLAGS_COPY_DEST_ALWAYS);
	string strBindRoot = PathUtil::GetParentDirectory(wildcardPath);
	strBindRoot += g_cNativeSlash;
	return OpenPacksCommon(strBindRoot.c_str(), pWildcardIn, wildcardPath, nFlags /*, pFullPaths*/);
}

bool CCryPak::OpenPacks(const char* szBindRoot, const char* pWildcardIn, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	CryPathString wildcardPath;
	wildcardPath.resize(_MAX_PATH);
	AdjustFileName(pWildcardIn, wildcardPath.data(), nFlags | FLAGS_COPY_DEST_ALWAYS);

	CryPathString bindRoot;
	bindRoot.resize(_MAX_PATH);
	AdjustFileName(szBindRoot, bindRoot.data(), FLAGS_ADD_TRAILING_SLASH | FLAGS_PATH_REAL);

	return OpenPacksCommon(bindRoot.data(), pWildcardIn, wildcardPath, nFlags /*, pFullPaths*/);
}

bool CCryPak::ClosePacks(const char* pWildcard, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	return false;
}

void CCryPak::AddMod(const char* szMod)
{
	std::string strPrepend = szMod;
	std::replace(strPrepend.begin(), strPrepend.end(), g_cNativeSlash, g_cNonNativeSlash);
	std::transform(strPrepend.begin(), strPrepend.end(), strPrepend.begin(),
	               [](unsigned char c)
	               { return std::tolower(c); });

	std::vector<std::string>::iterator strit;
	for (strit = m_arrMods.begin(); strit != m_arrMods.end(); ++strit)
	{
		std::string& sMOD = *strit;
		if (_stricmp(sMOD.c_str(), strPrepend.c_str()) == 0)
			return; // already added
	}
	m_arrMods.push_back(strPrepend);
}

void CCryPak::RemoveMod(const char* szMod)
{
	std::string strPrepend = szMod;
	strPrepend.replace(strPrepend.begin(), strPrepend.end(), g_cNativeSlash, g_cNonNativeSlash);
	std::transform(strPrepend.begin(), strPrepend.end(), strPrepend.begin(),
	               [](unsigned char c)
	               { return std::tolower(c); });

	std::vector<std::string>::iterator it;
	for (it = m_arrMods.begin(); it != m_arrMods.end(); ++it)
	{
		std::string& sMOD = *it;
		if (_stricmp(sMOD.c_str(), strPrepend.c_str()) == 0)
		{
			m_arrMods.erase(it);
			break;
		}
	} //it
}

//////////////////////////////////////////////////////////////////////////
const char* CCryPak::GetMod(int index)
{
	#if 0
	return index >= 0 && index < (int)m_arrMods.size() ? m_arrMods[index].path.c_str() : NULL;
	#else
	return "";
	#endif
}

//////////////////////////////////////////////////////////////////////////
void CCryPak::ParseAliases(const char* szCommandLine)
{
	const char* szVal = szCommandLine;
	for (;;)
	{
		// this is a list of pairs separated by commas, i.e. Folder1,FolderNew,Textures,TestBuildTextures etc.
		const char* const szSep = strchr(szVal, ',');
		if (!szSep)
		{
			// bogus string passed
			break;
		}

		char szName[256];
		char szAlias[256];

		// get old folder name
		strncpy(szName, szVal, (size_t)(szSep - szVal));

		// find next pair
		const char* szSepNext = strchr(szSep + 1, ',');

		// get alias name
		if (!szSepNext)
		{
			// we may receive whole command line, not just alias pairs. so we must
			// check if there are other commands in the command line and skip them.
			const char* const szTail = strchr(szSep + 1, ' ');
			if (szTail)
			{
				strncpy(szAlias, szSep + 1, (size_t)(szTail - (szSep + 1)));
			}
			else
			{
				strcpy(szAlias, szSep + 1);
			}
		}
		else
		{
			strncpy(szAlias, szSep + 1, (size_t)(szSepNext - (szSep + 1)));
		}

		// inform the pak system
		SetAlias(szName, szAlias, true);

		CryLogAlways("PAK ALIAS:%s,%s\n", szName, szAlias);

		if (!szSepNext)
		{
			// no more aliases
			break;
		}

		// move over to the next pair
		szVal = szSepNext + 1;
	}
}

//////////////////////////////////////////////////////////////////////////
void CCryPak::SetAlias(const char* szName, const char* szAlias, bool bAdd)
{
	// Strip ./ or .\ at the beginning of the szAlias path.
	if (szAlias && szAlias[0] == '.' && (szAlias[1] == '/' || szAlias[1] == '\\'))
	{
		szAlias += 2;
	}

	// find out if it is already there
	TAliasList::iterator it;
	tNameAlias*          tPrev = NULL;
	for (it = m_arrAliases.begin(); it != m_arrAliases.end(); ++it)
	{
		tNameAlias* tTemp = (*it);
		if (stricmp(tTemp->szName, szName) == 0)
		{
			tPrev = tTemp;
			if (!bAdd)
			{
				//remove it
				SAFE_DELETE(tPrev->szName);
				SAFE_DELETE(tPrev->szAlias);
				delete tPrev;
				m_arrAliases.erase(it);
			}
			break;
		}
	} //it

	if (!bAdd)
		return;

	if (tPrev)
	{
		// replace existing alias
		if (stricmp(tPrev->szAlias, szAlias) != 0)
		{
			SAFE_DELETE(tPrev->szAlias);
			tPrev->nLen2   = strlen(szAlias);
			tPrev->szAlias = new char[tPrev->nLen2 + 1]; // includes /0
			strcpy(tPrev->szAlias, szAlias);
			// make it lowercase
	#if !CRY_PLATFORM_IOS && !CRY_PLATFORM_LINUX && !CRY_PLATFORM_ANDROID
			strlwr(tPrev->szAlias);
	#endif
		}
	}
	else
	{
		// add a new one
		tNameAlias* tNew = new tNameAlias;

		tNew->nLen1      = strlen(szName);
		tNew->szName     = new char[tNew->nLen1 + 1]; // includes /0
		strcpy(tNew->szName, szName);
		// make it lowercase
		strlwr(tNew->szName);

		tNew->nLen2   = strlen(szAlias);
		tNew->szAlias = new char[tNew->nLen2 + 1]; // includes /0
		strcpy(tNew->szAlias, szAlias);
		// make it lowercase
	#if !CRY_PLATFORM_IOS && !CRY_PLATFORM_LINUX && !CRY_PLATFORM_ANDROID
		strlwr(tNew->szAlias);
	#endif
		std::replace(tNew->szAlias, tNew->szAlias + tNew->nLen2 + 1, g_cNonNativeSlash, g_cNativeSlash);
		m_arrAliases.push_back(tNew);
	}
}

//////////////////////////////////////////////////////////////////////////
//! if bReturnSame==true, it will return the input name if an alias doesn't exist. Otherwise returns NULL
const char* CCryPak::GetAlias(const char* szName, bool bReturnSame)
{
	const TAliasList::const_iterator cAliasEnd = m_arrAliases.end();
	for (TAliasList::const_iterator it = m_arrAliases.begin(); it != cAliasEnd; ++it)
	{
		tNameAlias* tTemp = (*it);
		if (stricmp(tTemp->szName, szName) == 0)
			return (tTemp->szAlias);
	} //it
	if (bReturnSame)
		return (szName);
	return (NULL);
}

//////////////////////////////////////////////////////////////////////////
//! Set "Game" folder (/Game, /Game04, ...)
void CCryPak::SetGameFolder(const char* szFolder)
{
	assert(szFolder);
	m_strDataRoot = GetAlias(szFolder, true);
	#if 0
	m_strDataRoot.MakeLower();
	#else
	std::transform(m_strDataRoot.begin(), m_strDataRoot.end(), m_strDataRoot.begin(),
	               [](unsigned char c)
	               { return std::tolower(c); });
	#endif

	#if BB_PLATFORM_WINDOWS
	// Check that game folder exist, produce fatal error if missing.
	{
		__finddata64_t fd;
		ZeroStruct(fd);
		intptr_t hfile = 0;
		hfile          = _findfirst64(m_strDataRoot.c_str(), &fd);
		_findclose(hfile);
		if (!(fd.attrib & _A_SUBDIR))
		{
			const char* dataRoot = m_strDataRoot.c_str();

			m_pLog->LogWarning("Game folder %s not found, trying to create an empty one", dataRoot);

			if (!MakeDir(dataRoot))
			{
				CryFatalError("Couldn't create an empty %s game folder", dataRoot);
			}
		}
		else if (g_cvars.sys_filesystemCaseSensitivity > 0)
		{
			if (strcmp(szFolder, fd.name) != 0)
			{
				CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_ERROR, "Wrong letter casing of the game root folder! Should be '%s' instead of '%s'.", fd.name, m_strDataRoot.c_str());
			}
			m_strDataRoot = fd.name;
		}
	}
	#endif

	m_strDataRootWithSlash = m_strDataRoot + (char)g_cNativeSlash;
}

//////////////////////////////////////////////////////////////////////////
//! Get "Game" folder (/Game, /Game04, ...)
const char* CCryPak::GetGameFolder() const
{
	return m_strDataRoot.c_str();
}
static char* cry_strdup(const char* szSource)
{
	size_t len      = strlen(szSource);
	char*  szResult = (char*)malloc(len + 1);
	memcpy(szResult, szSource, len + 1);
	return szResult;
}

ICryPak::PakInfo* CCryPak::GetPakInfo()
{
	#if 0
	AUTO_READLOCK(m_csZips);
	#endif
	PakInfo* pResult = (PakInfo*)malloc(sizeof(PakInfo) + sizeof(PakInfo::Pak) * m_arrZips.size());
	assert(pResult);
	pResult->numOpenPaks = m_arrZips.size();
	for (unsigned i = 0; i < m_arrZips.size(); ++i)
	{
		pResult->arrPaks[i].szBindRoot = cry_strdup(m_arrZips[i].strBindRoot.c_str());
	//FIXME:
	#if 0
		pResult->arrPaks[i].szFilePath = cry_strdup(m_arrZips[i].GetFullPath());
		pResult->arrPaks[i].nUsedMem   = m_arrZips[i].sizeofThis();
	#endif
	}
	return pResult;
}

//////////////////////////////////////////////////////////////////////////
void CCryPak::FreePakInfo(PakInfo* pPakInfo)
{
	for (unsigned i = 0; i < pPakInfo->numOpenPaks; ++i)
	{
		free((void*)pPakInfo->arrPaks[i].szBindRoot);
		free((void*)pPakInfo->arrPaks[i].szFilePath);
	}
	free(pPakInfo);
}

namespace CIOWrapper
{
	inline FILE* FopenEx(string_view path, string_view mode) { return fopen(path.data(), mode.data()); }
	#define FopenLocked FopenEx
}

FILE* CCryPak::FOpen(const char* pName, const char* szMode, unsigned nInputFlags /* = 0*/)
{
	FILE* fp               = NULL;

	bool  bFileCanBeOnDisk = 0 != (nInputFlags & FOPEN_ONDISK);

	bool  bFileOpenLocked  = 0 != (nInputFlags & FOPEN_LOCKED_OPEN);

	// get the priority into local variable to avoid it changing in the course of
	// this function execution (?)
	int   nVarPakPriority  = m_pPakVars->nPriority;

	int   nAdjustFlags     = 0;
	#if CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID || CRY_PLATFORM_APPLE
	unsigned nOSFlags = _O_RDONLY;
	#else
	unsigned nOSFlags = _O_BINARY | _O_RDONLY;
	#endif

	// check the szMode
	for (const char* pModeChar = szMode; *pModeChar; ++pModeChar)
		switch (*pModeChar)
		{
		case 'r':
			nOSFlags &= ~(_O_WRONLY | _O_RDWR);
			// read mode is the only mode we can open the file in
			break;
		case 'w':
			nOSFlags |= _O_WRONLY;
			nAdjustFlags |= FLAGS_FOR_WRITING;
			break;
		case 'a':
			nOSFlags |= _O_RDWR;
			nAdjustFlags |= FLAGS_FOR_WRITING;
			break;
		case '+':
			nOSFlags |= _O_RDWR;
			nAdjustFlags |= FLAGS_FOR_WRITING;
			break;

		case 'b':
			nOSFlags &= ~_O_TEXT;
			nOSFlags |= _O_BINARY;
			break;
		case 't':
			nOSFlags &= ~_O_BINARY;
			nOSFlags |= _O_TEXT;
			break;

	//FIXME:
	#if 0
		case 'c':
		case 'C':
			nOSFlags |= (uint32)CZipPseudoFile::_O_COMMIT_FLUSH_MODE;
			break;
		case 'n':
		case 'N':
			nOSFlags &= ~CZipPseudoFile::_O_COMMIT_FLUSH_MODE;
			break;
	#endif

		case 'S':
			nOSFlags |= _O_SEQUENTIAL;
			break;

		case 'R':
			nOSFlags |= _O_RANDOM;
			break;

		case 'T':
			nOSFlags |= _O_SHORT_LIVED;
			break;

		case 'D':
			nOSFlags |= _O_TEMPORARY;
			break;

		case 'x':
		case 'X':
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Using deprecated file open mode 'x' on file %s.", pName);
			break;
		default:
			CryWarning(VALIDATOR_MODULE_SYSTEM, VALIDATOR_WARNING, "Using unknown file open mode '%c' on file %s.", *pModeChar, pName);
		}

	if (nInputFlags & FLAGS_PATH_REAL)
	{
		nAdjustFlags |= FLAGS_PATH_REAL;
	}

	if (nInputFlags & FLAGS_NO_LOWCASE)
	{
		nAdjustFlags |= FLAGS_NO_LOWCASE;
	}

	CryPathString fullPath;
	fullPath.resize(256);
	AdjustFileName(pName, fullPath.data(), nAdjustFlags);

	if (nOSFlags & (_O_WRONLY | _O_RDWR))
	{
		#if 0
		CheckFileAccessDisabled(fullPath, szMode);
		#endif

		// we need to open the file for writing, but we failed to do so.
		// the only reason that can be is that there are no directories for that file.
		// now create those dirs
		if (!MakeDir(PathUtil::GetParentDirectory(fullPath).c_str()))
		{
			return NULL;
		}
		FILE* file = NULL;
		if (!bFileOpenLocked)
			file = CIOWrapper::FopenEx(fullPath, szMode);
		else
			file = CIOWrapper::FopenLocked(fullPath, szMode);

#if !defined(_RELEASE)
		if (file && g_cvars.pakVars.nLogAllFileAccess)
		{
			CryLog("<PAK LOG FILE ACCESS> CCryPak::FOpen() has directly opened requested file %s for writing", fullPath.c_str());
		}
#endif

		return file;
	}


	if (nVarPakPriority == ePakPriorityFileFirst ||
	    (nVarPakPriority == ePakPriorityFileFirstModsOnly && IsModPath(fullPath.c_str()))) // if the file system files have priority now..
	{
		fp = fopen(fullPath.data(), szMode);

		if (fp)
		{
	#if !defined(_RELEASE)
			if (g_cvars.pakVars.nLogAllFileAccess)
			{
				CryLog("$6<PAK LOG FILE ACCESS> CCryPak::FOpen() has directly opened requested file %s with FileFirst priority", fullPath.c_str());
			}
	#endif

			RecordFile(pName);
			return fp;
		}
	}

	if (auto data = GetFileData(pName); data)
	{
		INT_PTR nFile = m_arrOpenFiles.size() + CCryPak::g_nPseudoFileIdxOffset;
		m_arrOpenFiles.push_back((MyFile*)data);

		CryLog("$3<PAK LOG FILE ACCESS> CCryPak::FOpen() has opened requested file %s from %s pak %s, disk offset %u",
		       fullPath.c_str(),
	#if 0
			pZip->IsInMemory() ? "memory" : "disk", 
			pZipFilePath, 
			pFileData->GetFileEntry()->nFileDataOffset
	#else
			"memory",
			pName,
			((MyFile*)data)->m_File->offset
	#endif
		);

		RecordFile(pName);
		return (FILE*)nFile;
	}
	else
	{
		if (nVarPakPriority != ePakPriorityPakOnly || bFileCanBeOnDisk)
		{
			fp = fopen(fullPath.data(), szMode);

			if (fp)
			{
	#if !defined(_RELEASE)
				if (g_cvars.pakVars.nLogAllFileAccess)
				{
					CryLog("$4<PAK LOG FILE ACCESS> CCryPak::FOpen() has directly opened requested file %s with FileFirst priority", fullPath.c_str());
				}
	#endif

				RecordFile(pName);
				return fp;
			}
		}
		return NULL;
	}
}

FILE* CCryPak::FOpen(const char* pName, const char* mode, char* szFileGamePath, int nLen)
{
	assert(0);
	return fopen((szFileGamePath + std::string(pName)).c_str(), mode);
}

size_t CCryPak::FRead(void* data, size_t length, size_t elems, FILE* handle)
{
	return FReadRaw(data, length, elems, handle);
}

size_t CCryPak::FWrite(void* data, size_t length, size_t elems, FILE* handle)
{
	return fwrite(data, length, elems, handle);
}

int CCryPak::FSeek(FILE* handle, long seek, int mode)
{
	//SAutoCollectFileAcessTime accessTime(this);

	{
		INT_PTR nPseudoFile = ((INT_PTR)handle) - g_nPseudoFileIdxOffset;
		//AUTO_READLOCK(m_csOpenFiles);
		if ((UINT_PTR)nPseudoFile < m_arrOpenFiles.size())
			return m_arrOpenFiles[nPseudoFile]->FSeek(seek, mode);
	}

	//int nResult = CIOWrapper::Fseek(hFile, seek, mode);
	int nResult = fseek(handle, seek, mode);
	assert(nResult == 0);
	return nResult;
}

long CCryPak::FTell(FILE* handle)
{
	{
		INT_PTR nPseudoFile = ((INT_PTR)handle) - g_nPseudoFileIdxOffset;
		//AUTO_READLOCK(m_csOpenFiles);
		if ((UINT_PTR)nPseudoFile < m_arrOpenFiles.size())
		{
			return m_arrOpenFiles[nPseudoFile]->FTell();
		}
	}
	//return (long)CIOWrapper::FTell(hFile);

	return ftell(handle);
}

int CCryPak::FClose(FILE* hFile)
{
	//if (m_pCachedFileData && m_pCachedFileData->m_hFile == hFile) // Free cached data.
	//	m_pCachedFileData = 0;

	//SAutoCollectFileAcessTime accessTime(this);
	{
		INT_PTR nPseudoFile = ((INT_PTR)hFile) - g_nPseudoFileIdxOffset;
		//AUTO_READLOCK(m_csOpenFiles);
		if ((UINT_PTR)nPseudoFile < m_arrOpenFiles.size())
		{
	#if 0
			m_arrOpenFiles[nPseudoFile].Destruct();
	#endif
			return 0;
		}
	}
	return fclose(hFile);
}

int CCryPak::FEof(FILE* handle)
{
	return feof(handle);
}

int CCryPak::FFlush(FILE* handle)
{
	return fflush(handle);
}

int CCryPak::FPrintf(FILE* handle, const char* format, ...)
{
	va_list ptr;
	va_start(ptr, format);
	auto res = vfprintf(handle, format, ptr);
	va_end(ptr);
	return res;
}

char* CCryPak::FGets(char* buffer, int max_count, FILE* file)
{
	return fgets(buffer, max_count, file);
}

int CCryPak::Getc(FILE* file)
{
	return getc(file);
}

int CCryPak::Ungetc(int c, FILE* file)
{
	return ungetc(c, file);
}

intptr_t CCryPak::FindFirst(const char* pDir, _finddata_t* fd)
{
	return _findfirst(pDir, fd);
}

int CCryPak::FindNext(intptr_t handle, _finddata_t* fd)
{
	return _findnext(handle, fd);
}

int CCryPak::FindClose(intptr_t handle)
{
	return _findclose(handle);
}

FILETIME CCryPak::GetModificationTime(FILE* f)
{
	return FILETIME();
}

bool CCryPak::MakeDir(const char* szPath)
{
	return _mkdir(szPath);
}

ICryArchive* CCryPak::OpenArchive(const char* szPath, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	return nullptr;
}

const char* CCryPak::GetFileArchivePath(FILE* f)
{
	return nullptr;
}

int CCryPak::RawCompress(const void* pUncompressed, unsigned long* pDestSize, void* pCompressed, unsigned long nSrcSize, int nLevel /* = -1*/)
{
	return 0;
}

int CCryPak::RawUncompress(void* pUncompressed, unsigned long* pDestSize, const void* pCompressed, unsigned long nSrcSize)
{
	return 0;
}

void CCryPak::RecordFileOpen(bool bEnable)
{
	m_RecordFileOpen = bEnable;
}

void CCryPak::RecordFile(const char* szFilename)
{
	if (m_pLog)
		CryComment("File open: %s", szFilename);

	if (m_RecordFileOpen)
	{
		if (strnicmp("%USER%", szFilename, 6) != 0) // ignore path to OS settings
		{
	#if 0
			IResourceList* pList = GetResourceList(m_eRecordFileOpenList);

			if (pList)
				pList->Add(szFilename);
	#else
			m_ResourceSet.insert(szFilename);
	#endif
		}
	}

	#if 0
	std::vector<ICryPakFileAcesssSink*>::iterator it, end = m_FileAccessSinks.end();

	for (it = m_FileAccessSinks.begin(); it != end; ++it)
	{
		ICryPakFileAcesssSink* pSink = *it;

		pSink->ReportFileOpen(in, szFilename);
	}
	#endif
}

void CCryPak::EnumerateRecordedFiles(RecordedFilesEnumCallback enumCallback)
{
}

void CCryPak::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
}

bool CCryPak::InitPack(const char* szBasePath, unsigned nFlags)
{
	#if BB_PLATFORM_IOS
	char buffer[1024];
	if (AppleGetUserLibraryDirectory(buffer, sizeof(buffer)))
	{
		SetAlias("%USER%", buffer, true);
	}
	#endif
	CryFindEngineRootFolder(CRY_ARRAY_COUNT(m_szEngineRootDir), m_szEngineRootDir);
	m_szEngineRootDirStrLen = strlen(m_szEngineRootDir);

	return true;
}

ZipFile::File CCryPak::create_file(ZipFile::CentralDirectory& entry, void* header)
{
	auto name = string_view((char*)header + entry.lLocalHeaderOffset + sizeof LocalFileHeader, entry.nFileNameLength); //
	#if 0                                                                                                              // For test purpose
	if (stricmp(std::string(name.data(), name.length()).c_str(), std::string("textures/console/defaultconsole.dds").c_str()) == 0)
	{
		puts("");
		//CryLog("Here");
	}
	#endif
	bool  compressed = entry.desc.lSizeCompressed != entry.desc.lSizeUncompressed;
	auto& lfh        = *(LocalFileHeader*)((char*)header + entry.lLocalHeaderOffset);
	File  file{entry.lLocalHeaderOffset + sizeof LocalFileHeader + lfh.nFileNameLength + lfh.nExtraFieldLength, entry.desc.lSizeUncompressed, entry.desc.lSizeCompressed, name, (char*)header, compressed};
	return file;
}

FILE* CCryPak::FOpenRaw(const char* pName, const char* mode)
{
	return nullptr;
}
size_t CCryPak::FReadRaw(void* pData, size_t nSize, size_t nCount, FILE* hFile)
{
	//BB_PROFILE_FUNCTION(PROFILE_LOADING_ONLY);

	//SAutoCollectFileAcessTime accessTime(this);

	{
		INT_PTR nPseudoFile = ((INT_PTR)hFile) - g_nPseudoFileIdxOffset;
		//AUTO_READLOCK(m_csOpenFiles);
		if ((UINT_PTR)nPseudoFile < m_arrOpenFiles.size())
		{
			return m_arrOpenFiles[nPseudoFile]->FRead(pData, nSize, nCount, hFile);
		}
	}

	return fread(pData, nSize, nCount, hFile);
}

CFileDataPtr CCryPak::GetFileData(const char* szName, unsigned int& nArchiveFlags)
{
	//BB_PROFILE_FUNCTION(PROFILE_SYSTEM);

	CFileDataPtr* pResult = 0;

	//ZipDir::CachePtr   pZip       = 0;
	//ZipDir::FileEntry* pFileEntry = FindPakFileEntry(szName, nArchiveFlags, &pZip);
	//if (pFileEntry)
	//{
	//	pResult = new CCachedFileData(this, pZip, nArchiveFlags, pFileEntry, szName);
	//}

	if (auto it = m_Files.find(szName); it != m_Files.end())
	{
		pResult = (CFileDataPtr*)new MyFile(File::CreateFrom(&it->second));
	}
	return pResult;
}

ILINE bool IsDirSep(const char c)
{
	return (c == CCryPak::g_cNativeSlash || c == CCryPak::g_cNonNativeSlash);
}

//////////////////////////////////////////////////////////////////////////
bool /*CCryPak::*/ IsAbsPath(const char* pPath)
{
	return (pPath && ((pPath[0] && pPath[1] == ':' && IsDirSep(pPath[2])) || IsDirSep(pPath[0])));
}

const char* CCryPak::AdjustFileName(const char* szSourcePath, char szDestPath[g_nMaxPath], unsigned nFlags)
{
	//BB_PROFILE_FUNCTION(PROFILE_SYSTEM);

	{
		// in many cases, the path will not be long, so there's no need to allocate so much..
		// I'd use _alloca, but I don't like non-portable solutions. besides, it tends to confuse new developers. So I'm just using a big enough array
		//CryPathString::MAX_SIZE = 256;
		char szNewSrc[256];
		strcpy(szNewSrc, szSourcePath);

		{
			BeautifyPath(szNewSrc, (nFlags & FLAGS_NO_LOWCASE) == 0);
			RemoveRelativeParts(szNewSrc);
		}

		strcpy(szDestPath, szNewSrc);
	}

	string     dst(szDestPath);
	const bool bAliasWasUsed = AdjustAliases(dst);

	if (nFlags & FLAGS_NO_FULL_PATH)
	{
		return szSourcePath;
	}

	const bool isAbsolutePath = IsAbsPath(dst.c_str());

	//////////////////////////////////////////////////////////////////////////
	// Strip ./ or .\\ at the beginning of the path when absolute path is given.
	//////////////////////////////////////////////////////////////////////////
	if (nFlags & FLAGS_PATH_REAL)
	{
		if (filehelpers::CheckPrefix(szDestPath, "./") ||
		    filehelpers::CheckPrefix(szDestPath, ".\\"))
		{
			//dst.erase(0, 2);
			assert(0);
			szDestPath += 2;
		}
	}
	if (!isAbsolutePath && !(nFlags & FLAGS_PATH_REAL) && !bAliasWasUsed && !IsModPath(dst.c_str()))
	{
		// This is a relative filename.
		// 1) /root/system.cfg
		// 2) /root/game/system.cfg
		// 3) /root/game/config/system.cfg

		//////////////////////////////////////////////////////////////////////////
		if (
		    (!filehelpers::CheckPrefix(szDestPath, m_strDataRootWithSlash.c_str()) &&
		     !filehelpers::CheckPrefix(szDestPath, "." BB_NATIVE_PATH_SEPSTR) &&
		     !filehelpers::CheckPrefix(szDestPath, ".." BB_NATIVE_PATH_SEPSTR) &&
		     !filehelpers::CheckPrefix(szDestPath, "editor" BB_NATIVE_PATH_SEPSTR) &&
		     !filehelpers::CheckPrefix(szDestPath, "gamedata" BB_NATIVE_PATH_SEPSTR) &&
		     !filehelpers::CheckPrefix(szDestPath, "engine" BB_NATIVE_PATH_SEPSTR)))
		{
			// Add data folder prefix.
			string tmp = m_strDataRootWithSlash + szDestPath;

			strcpy(szDestPath, tmp.c_str());
		}
	}

	return dst.c_str();
}
#endif
