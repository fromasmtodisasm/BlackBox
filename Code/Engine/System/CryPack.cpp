#include <BlackBox/System/File/CryPack.hpp>

CCryPak::CCryPak()
{
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
}

bool CCryPak::OpenPack(const char* pName, unsigned nFlags/* = FLAGS_PATH_REAL*/)
{
  return false;
}

bool CCryPak::OpenPack(const char* pBindingRoot, const char* pName, unsigned nFlags/* = FLAGS_PATH_REAL*/)
{
  return false;
}

bool CCryPak::ClosePack(const char* pName, unsigned nFlags/* = FLAGS_PATH_REAL*/)
{
  return false;
}

bool CCryPak::OpenPacks(const char* pWildcard, unsigned nFlags/* = FLAGS_PATH_REAL*/)
{
  return false;
}

bool CCryPak::OpenPacks(const char* pBindingRoot, const char* pWildcard, unsigned nFlags/* = FLAGS_PATH_REAL*/)
{
  return false;
}

bool CCryPak::ClosePacks(const char* pWildcard, unsigned nFlags/* = FLAGS_PATH_REAL*/)
{
  return false;
}

void CCryPak::AddMod(const char* szMod)
{
}

void CCryPak::RemoveMod(const char* szMod)
{
}

ICryPak::PakInfo* CCryPak::GetPakInfo()
{
  return nullptr;
}

void CCryPak::FreePakInfo(PakInfo*)
{
}

FILE* CCryPak::FOpen(const char* pName, const char* mode, unsigned nFlags = 0)
{
  return nullptr;
}

FILE* CCryPak::FOpen(const char* pName, const char* mode, char* szFileGamePath, int nLen)
{
  return nullptr;
}

size_t CCryPak::FRead(void* data, size_t length, size_t elems, FILE* handle)
{
  return size_t();
}

size_t CCryPak::FWrite(void* data, size_t length, size_t elems, FILE* handle)
{
  return size_t();
}

int CCryPak::FSeek(FILE* handle, long seek, int mode)
{
  return 0;
}

long CCryPak::FTell(FILE* handle)
{
  return 0;
}

int CCryPak::FClose(FILE* handle)
{
  return 0;
}

int CCryPak::FEof(FILE* handle)
{
  return 0;
}

int CCryPak::FFlush(FILE* handle)
{
  return 0;
}

int CCryPak::FPrintf(FILE* handle, const char* format, ...)
{
  return 0;
}

char* CCryPak::FGets(char*, int, FILE*)
{
  return nullptr;
}

int CCryPak::Getc(FILE*)
{
  return 0;
}

int CCryPak::Ungetc(int c, FILE*)
{
  return 0;
}

intptr_t CCryPak::FindFirst(const char* pDir, _finddata_t* fd)
{
  return intptr_t();
}

int CCryPak::FindNext(intptr_t handle, _finddata_t* fd)
{
  return 0;
}

int CCryPak::FindClose(intptr_t handle)
{
  return 0;
}

FILETIME CCryPak::GetModificationTime(FILE* f)
{
  return FILETIME();
}

bool CCryPak::MakeDir(const char* szPath)
{
  return false;
}

ICryArchive* CCryPak::OpenArchive(const char* szPath, unsigned nFlags = FLAGS_PATH_REAL)
{
  return nullptr;
}

const char* CCryPak::GetFileArchivePath(FILE* f)
{
  return nullptr;
}

int CCryPak::RawCompress(const void* pUncompressed, unsigned long* pDestSize, void* pCompressed, unsigned long nSrcSize, int nLevel = -1)
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
