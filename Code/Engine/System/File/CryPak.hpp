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

	#include ZIP_FORMAT_H

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

interface IMemoryBlock {
};

//////////////////////////////////////////////////////////////////////////
typedef struct
{
	char* szName;  // folder or name to be replaced
	int   nLen1;   // string length, for faster operation
	char* szAlias; // new folder name
	int   nLen2;   // string length, for faster operation
} tNameAlias;

template<class K, class V>
using VectorMap = std::map<K, V>;

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

	//Pak file comment data. Supplied in key=value pairs in the zip archive file comment
	typedef VectorMap<string, string> TCommentDataMap;
	typedef std::pair<string, string> TCommentDataPair;

	// the array of opened caches - they get destructed by themselves (these are auto-pointers, see the ZipDir::Cache documentation)
	struct PackDesc
	{
		string          strBindRoot; // the zip binding root WITH the trailing native slash
		string          strFileName; // the zip file name (with path) - very useful for debugging so please don't remove

		TCommentDataMap m_commentData; //VectorMap of key=value pairs from the zip archive comments
#if 0
		const char*     GetFullPath() const { return pZip->GetFilePath(); }
#endif

		ICryArchive_AutoPtr pArchive;
#if 0
		ZipDir::CachePtr    pZip;
#endif
#if 0
		size_t sizeofThis()
		{
			return strBindRoot.capacity() + strFileName.capacity() + pZip->GetSize();
		}
#endif

		void GetMemoryUsage(ICrySizer* pSizer) const;
	};
	typedef std::vector<PackDesc /*, stl::STLGlobalAllocator<PackDesc>*/> ZipArray;
#if 0
	CryReadModifyLock m_csZips;
#endif
	ZipArray m_arrZips;

public:
	void  RemoveRelativeParts(char* dst);
	bool  AdjustAliases(CryPathString& dst);
	void  SetLog(IMiniLog* pLog);
	char* BeautifyPath(char* dst, bool bMakeLowercase);
	CCryPak(IMiniLog* pLog, PakVars* pPakVars, const bool bLvlRes);
	~CCryPak();
	// Inherited via ICryPak
	virtual bool         Init(const char* szBasePath) override;
	bool                 IsModPath(const char* szPath);
	void                 AdjustFileNameInternal(const char* src, CryPathString& dst, unsigned nFlags);
	virtual const char*  AdjustFileName(const char* szSourcePath, char szDestPath[g_nMaxPath], unsigned nFlags = 0) override;
	virtual void         Release() override;
	virtual bool         OpenPack(const char* pName, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool         OpenPack(const char* pBindingRoot, const char* pName, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool         ClosePack(const char* pName, unsigned nFlags = FLAGS_PATH_REAL) override;
	bool                 OpenPackCommon(const char* szBindRoot, const char* pName, unsigned int nPakFlags, IMemoryBlock* pData = 0);
	bool                 OpenPacksCommon(const char* szDir, const char* pWildcardIn, CryPathString& wildcardPath, int nPakFlags, std::vector<CryPathString>* pFullPaths = NULL);
	virtual bool         OpenPacks(const char* pWildcard, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool         OpenPacks(const char* pBindingRoot, const char* pWildcard, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool         ClosePacks(const char* pWildcard, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual void         AddMod(const char* szMod) override;
	virtual void         RemoveMod(const char* szMod) override;
	//! returns indexed mod path, or NULL if out of range
	virtual const char*  GetMod(int index) override;

	//! Processes an alias command line containing multiple aliases.
	virtual void         ParseAliases(const char* szCommandLine) override;
	//! adds or removes an alias from the list - if bAdd set to false will remove it
	virtual void         SetAlias(const char* szName, const char* szAlias, bool bAdd) override;
	//! gets an alias from the list, if any exist.
	//! if bReturnSame==true, it will return the input name if an alias doesn't exist. Otherwise returns NULL
	virtual const char*  GetAlias(const char* szName, bool bReturnSame = true) override;

	// Set and Get "Game" folder (/Game, /Game04, ...)
	virtual void         SetGameFolder(const char* szFolder) override;
	virtual const char*  GetGameFolder() const override;

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

private:
	bool          InitPack(const char* szBasePath, unsigned nFlags = FLAGS_PATH_REAL);

	ZipFile::File create_file(ZipFile::CentralDirectory& entry, void* header);

	using KeyType = string_view;
	template<typename Type>

	using MapType =
#if 0
		std::unordered_map<KeyType,Type>
#else
	    std::map<KeyType, Type, ci_less>
#endif
	    ;

	using FileList = MapType<ZipFile::File>;

private:
	IMiniLog*                m_pLog = gEnv->pLog;

	// this is the list of MOD subdirectories that will be prepended to the actual relative file path
	// they all have trailing forward slash. "" means the root dir
	std::vector<std::string> m_arrMods;

#if 0
	std::map<string, SArchiveHandle> m_Archives;
#else
	std::vector<ZipFile::SArchiveHandle> m_Archives;
#endif
	//std::vector< libzippp::ZipArchive> m_Archives;
	std::string                      m_strDataRoot                = "GameData/";
	std::string                      m_strDataRootWithSlash       = PathUtil::GetEnginePath() + "/" + m_strDataRoot;

	char                             m_szEngineRootDir[_MAX_PATH] = {};
	size_t                           m_szEngineRootDirStrLen      = 0;

	// this is the list of aliases, used to replace certain folder(s) or file name(s).
	typedef std::vector<tNameAlias*> TAliasList;
	TAliasList                       m_arrAliases;

	FileList                         m_Files;

	std::vector<ZipFile::MyFile*>    m_arrOpenFiles;

	bool                             m_RecordFileOpen = true;

	typedef std::set<string>         ResourceSet;
	//CryCriticalSection               m_lock;
	ResourceSet                      m_ResourceSet;

	PakVars*                         m_pPakVars;
};