#pragma once

#include <BlackBox/System/File/ICryPak.hpp>
#include <BlackBox/System/IMiniLog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/Core/Path.hpp>

//#include <PakVars.hpp>
#define USE_FILE_MAPPING

#if 0
	#include <zip.h>
#else
	#include <filesystem>
	#include <string>
	#include <string_view>
	#include <vector>

	#include <io.h>
	#define NOMINMAX
	#include <Windows.h>
	#include "FileMapping.h"

//#include <../Code/Tools/RC/ResourceCompiler/FileMapping.h>

using byte = std::uint8_t;
using std::string;
using std::string_view;
using std::wstring;

namespace fs = std::filesystem;
	#include <../Code/Tools/RC/ResourceCompiler/Pak.h>

#endif

using CFileDataPtr = void*;

struct ci_less
{
	// case-independent (ci) compare_less binary function
	struct nocase_compare
	{
		bool operator()(const unsigned char& c1, const unsigned char& c2) const
		{
			return tolower(c1) < tolower(c2);
		}
	};

	bool operator()(const std::string_view& s1, const std::string_view s2) const
	{
		return std::lexicographical_compare(s1.begin(), s1.end(), // source range
		                                    s2.begin(), s2.end(), // dest range
		                                    nocase_compare());    // comparison
	}
};

class CCryPak : public ICryPak, public ISystemEventListener
{
public:
	// this defines which slash will be kept internally
#if BB_PLATFORM_LINUX || BB_PLATFORM_ANDROID
	enum {g_cNativeSlash = '/', g_cNonNativeSlash = '\\'};
#else
	enum
	{
		g_cNativeSlash    = '\\',
		g_cNonNativeSlash = '/'
	};
#endif
	// this is the start of indexation of pseudofiles:
	// to the actual index , this offset is added to get the valid handle
	enum
	{
		g_nPseudoFileIdxOffset = 1
	};

public:
	void  RemoveRelativeParts(char* dst);
	char* BeautifyPath(char* dst, bool bMakeLowercase);
	CCryPak(IMiniLog* pLog, PakVars* pPakVars, const bool bLvlRes);
	~CCryPak();
	// Inherited via ICryPak
	virtual bool         Init(const char* szBasePath) override;
	virtual const char*  AdjustFileName(const char* szSourcePath, char szDestPath[g_nMaxPath], unsigned nFlags = 0) override;
	virtual void         Release() override;
	virtual bool         OpenPack(const char* pName, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool         OpenPack(const char* pBindingRoot, const char* pName, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool         ClosePack(const char* pName, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool         OpenPacks(const char* pWildcard, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool         OpenPacks(const char* pBindingRoot, const char* pWildcard, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool         ClosePacks(const char* pWildcard, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual void         AddMod(const char* szMod) override;
	virtual void         RemoveMod(const char* szMod) override;
	virtual PakInfo*     GetPakInfo() override;
	virtual void         FreePakInfo(PakInfo*) override;
	virtual FILE*        FOpen(const char* pName, const char* mode, unsigned nFlags = 0) override;
	virtual FILE*        FOpen(const char* pName, const char* mode, char* szFileGamePath, int nLen) override;
	virtual size_t       FRead(void* data, size_t length, size_t elems, FILE* handle) override;
	virtual size_t       FWrite(void* data, size_t length, size_t elems, FILE* handle) override;
	virtual int          FSeek(FILE* handle, long seek, int mode) override;
	virtual long         FTell(FILE* handle) override;
	virtual int          FClose(FILE* handle) override;
	virtual int          FEof(FILE* handle) override;
	virtual int          FFlush(FILE* handle) override;
	virtual int          FPrintf(FILE* handle, const char* format, ...) override;
	virtual char*        FGets(char*, int, FILE*) override;
	virtual int          Getc(FILE*) override;
	virtual int          Ungetc(int c, FILE*) override;
	virtual intptr_t     FindFirst(const char* pDir, struct _finddata_t* fd) override;
	virtual int          FindNext(intptr_t handle, struct _finddata_t* fd) override;
	virtual int          FindClose(intptr_t handle) override;
	virtual FILETIME     GetModificationTime(FILE* f) override;
	virtual bool         MakeDir(const char* szPath) override;
	virtual ICryArchive* OpenArchive(const char* szPath, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual const char*  GetFileArchivePath(FILE* f) override;
	virtual int          RawCompress(const void* pUncompressed, unsigned long* pDestSize, void* pCompressed, unsigned long nSrcSize, int nLevel = -1) override;
	virtual int          RawUncompress(void* pUncompressed, unsigned long* pDestSize, const void* pCompressed, unsigned long nSrcSize) override;
	virtual void         RecordFileOpen(bool bEnable) override;
	virtual void         RecordFile(const char* szFilename) override;
	virtual void         EnumerateRecordedFiles(RecordedFilesEnumCallback enumCallback) override;
	virtual FILE*        FOpenRaw(const char* pName, const char* mode) override;
	virtual size_t       FReadRaw(void* data, size_t length, size_t elems, FILE* handle) override;

	// this function gets the file data for the given file, if found.
	// The file data object may be created in this function,
	// and it's important that the autoptr is returned: another thread may release the existing
	// cached data before the function returns
	// the path must be absolute normalized lower-case with forward-slashes
	CFileDataPtr         GetFileData(const char* szName, unsigned int& nArchiveFlags);
	CFileDataPtr         GetFileData(const char* szName)
	{
		unsigned int archiveFlags;
		return GetFileData(szName, archiveFlags);
	}

	// Inherited via ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;

	using KeyType = string_view;
	template<typename Type>

	using MapType =
#if 0
		std::unordered_map<KeyType,Type>
#else
	    std::map<KeyType, Type, ci_less>
#endif
	    ;

	using FileList = MapType<File>;

private:
	IMiniLog*                m_pLog;

	// this is the list of MOD subdirectories that will be prepended to the actual relative file path
	// they all have trailing forward slash. "" means the root dir
	std::vector<std::string> m_arrMods;

#if 0
	std::map<string, SArchiveHandle> m_Archives;
#else
	std::vector<SArchiveHandle> m_Archives;
#endif
	//std::vector< libzippp::ZipArchive> m_Archives;
	std::string          m_DataRoot             = "GameData/";
	std::string          m_strDataRootWithSlash = PathUtil::GetEnginePath() + "/" + m_DataRoot;

	FileList             m_Files;

	std::vector<MyFile*> m_arrOpenFiles;
};