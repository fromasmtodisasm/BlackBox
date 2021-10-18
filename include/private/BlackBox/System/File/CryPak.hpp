#pragma once

#include <BlackBox/System/File/ICryPak.hpp>
#include <BlackBox/System/IMiniLog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/PakVars.h>

#include <zip.h>

class CCryPak : public ICryPak
	, public ISystemEventListener
{
  public:
	// this defines which slash will be kept internally
#if BB_PLATFORM_LINUX || BB_PLATFORM_ANDROID
	enum {g_cNativeSlash = '/', g_cNonNativeSlash = '\\'};
#else
	enum
	{
		g_cNativeSlash	  = '\\',
		g_cNonNativeSlash = '/'
	};
#endif

  public:
	CCryPak(IMiniLog* pLog, PakVars* pPakVars, const bool bLvlRes);
	~CCryPak();
	// Inherited via ICryPak
	virtual bool		 Init(const char* szBasePath) override;
	virtual const char*	 AdjustFileName(const char* szSourcePath, char szDestPath[g_nMaxPath], unsigned nFlags = 0) override;
	virtual void		 Release() override;
	virtual bool		 OpenPack(const char* pName, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool		 OpenPack(const char* pBindingRoot, const char* pName, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool		 ClosePack(const char* pName, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool		 OpenPacks(const char* pWildcard, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool		 OpenPacks(const char* pBindingRoot, const char* pWildcard, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual bool		 ClosePacks(const char* pWildcard, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual void		 AddMod(const char* szMod) override;
	virtual void		 RemoveMod(const char* szMod) override;
	virtual PakInfo*	 GetPakInfo() override;
	virtual void		 FreePakInfo(PakInfo*) override;
	virtual FILE*		 FOpen(const char* pName, const char* mode, unsigned nFlags = 0) override;
	virtual FILE*		 FOpen(const char* pName, const char* mode, char* szFileGamePath, int nLen) override;
	virtual size_t		 FRead(void* data, size_t length, size_t elems, FILE* handle) override;
	virtual size_t		 FWrite(void* data, size_t length, size_t elems, FILE* handle) override;
	virtual int			 FSeek(FILE* handle, long seek, int mode) override;
	virtual long		 FTell(FILE* handle) override;
	virtual int			 FClose(FILE* handle) override;
	virtual int			 FEof(FILE* handle) override;
	virtual int			 FFlush(FILE* handle) override;
	virtual int			 FPrintf(FILE* handle, const char* format, ...) override;
	virtual char*		 FGets(char*, int, FILE*) override;
	virtual int			 Getc(FILE*) override;
	virtual int			 Ungetc(int c, FILE*) override;
	virtual intptr_t	 FindFirst(const char* pDir, struct _finddata_t* fd) override;
	virtual int			 FindNext(intptr_t handle, struct _finddata_t* fd) override;
	virtual int			 FindClose(intptr_t handle) override;
	virtual FILETIME	 GetModificationTime(FILE* f) override;
	virtual bool		 MakeDir(const char* szPath) override;
	virtual ICryArchive* OpenArchive(const char* szPath, unsigned nFlags = FLAGS_PATH_REAL) override;
	virtual const char*	 GetFileArchivePath(FILE* f) override;
	virtual int			 RawCompress(const void* pUncompressed, unsigned long* pDestSize, void* pCompressed, unsigned long nSrcSize, int nLevel = -1) override;
	virtual int			 RawUncompress(void* pUncompressed, unsigned long* pDestSize, const void* pCompressed, unsigned long nSrcSize) override;
	virtual void		 RecordFileOpen(bool bEnable) override;
	virtual void		 RecordFile(const char* szFilename) override;
	virtual void		 EnumerateRecordedFiles(RecordedFilesEnumCallback enumCallback) override;
	virtual FILE*		 FOpenRaw(const char* pName, const char* mode) override;
	virtual size_t		 FReadRaw(void* data, size_t length, size_t elems, FILE* handle) override;

	// Inherited via ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;

  private:
	IMiniLog* m_pLog;

	// this is the list of MOD subdirectories that will be prepended to the actual relative file path
	// they all have trailing forward slash. "" means the root dir
	std::vector<std::string> m_arrMods;

	std::map<string, zip*> m_Archives;
	//std::vector< libzippp::ZipArchive> m_Archives;
};