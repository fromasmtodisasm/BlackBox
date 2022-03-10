#ifndef LINUX
	#include <BlackBox/Core/Platform/Platform.hpp>
	#include <BlackBox/Core/Platform/Windows.hpp>
	#include "CryPak.hpp"

	#include <algorithm>
	#include <cctype>
	#include <cstdarg>
	#include <string>

	#define CRY_NATIVE_PATH_SEPSTR "/"

// makes the path lower-case and removes the duplicate and non native slashes
// may make some other fool-proof stuff
// may NOT write beyond the string buffer (may not make it longer)
// returns: the pointer to the ending terminator \0
char* CCryPak::BeautifyPath(char* dst, bool bMakeLowercase)
{
	// make the path lower-letters and with native slashes
	char* p, * q;
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
	const char slashDotSlashString[4] = { CCryPak::g_cNativeSlash, '.', CCryPak::g_cNativeSlash, 0 };

	while ((p = strstr(q, slashDotSlashString)) != nullptr)
	{
		//Move the string and the null terminator
		memmove(p, p + 2, strlen(p + 2) + 1);
	}

	// replace all '/%s/../' with '/'
	const char slashDotDotSlashString[5] = { CCryPak::g_cNativeSlash, '.', '.', CCryPak::g_cNativeSlash, 0 };

	while ((p = strstr(q, slashDotDotSlashString)) != nullptr)
	{
		if (p != q) // only remove if not in front of a path
		{
			int i = 4;
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
	m_arrOpenFiles.resize(128);
}

CCryPak::~CCryPak()
{
}

bool CCryPak::Init(const char* szBasePath)
{
	return false;
}

void CCryPak::Release()
{
	if (this != nullptr)
		delete this;
}

bool CCryPak::OpenPack(const char* pName, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	string fullPath;
	fullPath.resize(_MAX_PATH);
	AdjustFileName(pName, fullPath.data(), 0);
	SArchiveHandle ar{fullPath.c_str()};
	if (!ar)
	{
		ar = SArchiveHandle{fullPath.c_str()};
		if (!ar) return false;
	}

	CryComment("Opening pak file %s", fullPath.c_str());

	for (auto& entry : ar)
	{
		File file{entry.offset, entry.size, entry.Name(), (char*)ar.header};

		if (auto it = m_Files.find(file.name); it != m_Files.end()) printf("Eroror, file %s already mapped\n", file.name.data());

		m_Files[file.name] = file;
	}
	m_Archives.emplace_back(std::move(ar));

	return true;
}

bool CCryPak::OpenPack(const char* pBindingRoot, const char* pName, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	return OpenPack(pName, nFlags);
}

bool CCryPak::ClosePack(const char* pName, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	return false;
}

bool CCryPak::OpenPacks(const char* pWildcard, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	return false;
}

bool CCryPak::OpenPacks(const char* pBindingRoot, const char* pWildcard, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	return false;
}

bool CCryPak::ClosePacks(const char* pWildcard, unsigned nFlags /* = FLAGS_PATH_REAL*/)
{
	return false;
}

void CCryPak::AddMod(const char* szMod)
{
	std::string strPrepend = szMod;
	strPrepend.replace(strPrepend.begin(), strPrepend.end(), g_cNativeSlash, g_cNonNativeSlash);
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

ICryPak::PakInfo* CCryPak::GetPakInfo()
{
	return nullptr;
}

void CCryPak::FreePakInfo(PakInfo*)
{
}

FILE* CCryPak::FOpen(const char* pName, const char* mode, unsigned nFlags /* = 0*/)
{
	if (auto data = GetFileData(pName); data)
	{
		INT_PTR nFile = m_arrOpenFiles.size() + CCryPak::g_nPseudoFileIdxOffset;
		m_arrOpenFiles.push_back((MyFile*)data);
		return (FILE*)nFile;
	}
	else
	{
		auto file = fopen(pName, mode);
		if (!file)
		{
			string adjustedName = m_DataRoot + pName;
			file                = fopen(adjustedName.data(), mode);
		}
		return file;
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
}

void CCryPak::RecordFile(const char* szFilename)
{
}

void CCryPak::EnumerateRecordedFiles(RecordedFilesEnumCallback enumCallback)
{
}

void CCryPak::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
}
FILE* CCryPak::FOpenRaw(const char* pName, const char* mode)
{
	return nullptr;
}
size_t CCryPak::FReadRaw(void* pData, size_t nSize, size_t nCount, FILE* hFile)
{
	//CRY_PROFILE_FUNCTION(PROFILE_LOADING_ONLY);

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
	//CRY_PROFILE_FUNCTION(PROFILE_SYSTEM);

	CFileDataPtr* pResult = 0;

	//ZipDir::CachePtr   pZip       = 0;
	//ZipDir::FileEntry* pFileEntry = FindPakFileEntry(szName, nArchiveFlags, &pZip);
	//if (pFileEntry)
	//{
	//	pResult = new CCachedFileData(this, pZip, nArchiveFlags, pFileEntry, szName);
	//}

	if (auto it = m_Files.find(szName); it != m_Files.end())
	{
		pResult = (CFileDataPtr*)new MyFile(&it->second);
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
	//CRY_PROFILE_FUNCTION(PROFILE_SYSTEM);

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

	const bool isAbsolutePath = IsAbsPath(szDestPath);

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
	//////////////////////////////////////////////////////////////////////////
	if (
	    (!filehelpers::CheckPrefix(szDestPath, m_strDataRootWithSlash.c_str()) &&
	     !filehelpers::CheckPrefix(szDestPath, "." CRY_NATIVE_PATH_SEPSTR) &&
	     !filehelpers::CheckPrefix(szDestPath, ".." CRY_NATIVE_PATH_SEPSTR) &&
	     !filehelpers::CheckPrefix(szDestPath, "editor" CRY_NATIVE_PATH_SEPSTR) &&
	     !filehelpers::CheckPrefix(szDestPath, "gamedata" CRY_NATIVE_PATH_SEPSTR) &&
	     !filehelpers::CheckPrefix(szDestPath, "engine" CRY_NATIVE_PATH_SEPSTR)))
	{
		// Add data folder prefix.
		string tmp = m_strDataRootWithSlash + szDestPath;

		strcpy(szDestPath, tmp.c_str());
	}

	return szSourcePath;
}
#endif
