
//////////////////////////////////////////////////////////////////////////
//
//	Crytek Source code
//	Copyright (c) Crytek 2001-2004
//
//	File: Cryfile.h
//	Description: Pak file wrapper.
//
//	History:
//	- 3/7/2003: Created by Timur Davidenko
//	- February 2005: Modified by Marco Corbetta for SDK release
//
//////////////////////////////////////////////////////////////////////////

#ifndef __cryfile_h__
#define __cryfile_h__
#pragma once

//#include "ISystem.h"
//#include "ICryPak.h"
#include <BlackBox/System/ISystem.hpp>
#include <BlackBox/System/File/ICryPak.hpp>
#include <BlackBox/Core/Path.hpp>
#include <algorithm>

#define IfPak(PakFunc, stdfunc, args) (m_pIPak ? m_pIPak->PakFunc args : stdfunc args)

//////////////////////////////////////////////////////////////////////////
// Wrapper on file system.
//////////////////////////////////////////////////////////////////////////
class CCryFile
{
public:
	CCryFile();
	CCryFile(CCryFile&& other);
	CCryFile(const char* filename, const char* mode);
	virtual ~CCryFile();

	virtual bool   Open(std::string_view path, std::string_view mode, int nOpenFlagsEx = 0);
	virtual bool   Open(const char* filename, const char* mode, int nOpenFlagsEx = 0);
	virtual void   Close();

	//! Writes data in a file to the current file position.
	virtual size_t Write(void* lpBuf, size_t nSize);
	//! Reads data from a file at the current file position.
	size_t         ReadRaw(void* lpBuf, size_t nSize);
	//! Reads data from a file at the current file position.
	virtual size_t Read(void* lpBuf, size_t nSize);
	//! Retrieves the length of the file.
	virtual size_t GetLength() const;

	//! Positions the current file pointer.
	virtual size_t Seek(size_t seek, int mode);
	//! Positions the current file pointer at the beginning of the file.
	void           SeekToBegin();
	//! Positions the current file pointer at the end of the file.
	size_t         SeekToEnd();
	//! Retrieves the current file pointer.
	size_t         GetPosition() const;

	//! Tests for end-of-file on a selected file.
	virtual bool   IsEof();

	//! Flushes any data yet to be written.
	virtual void   Flush();

	//! A handle to a pack object.
	FILE*          GetHandle() const { return m_file; };

	// Retrieves the filename of the selected file.
	const char*    GetFilename() const { return m_filename.c_str(); };

	//! Retrieves the filename after adjustment to the real relative to engine root path.
	//! Example:
	//! Original filename "textures/red.dds" adjusted filename will look like "game/textures/red.dds"
	//! \return Adjusted filename, this is a pointer to a static string, copy return value if you want to keep it.
	const char* GetAdjustedFilename() const;

	//! Check if file is opened from pak file.
	bool           IsInPak() const;

	//! Get path of archive this file is in.
	const char*    GetPakPath() const;

	               operator bool()
	{
		return m_file != nullptr;
	}

private:
	string   m_filename;
	FILE*    m_file;
	ICryPak* m_pIPak;
};

//////////////////////////////////////////////////////////////////////////
// CCryFile implementation.
//////////////////////////////////////////////////////////////////////////
inline CCryFile::CCryFile()
{
	m_file  = 0;
	m_pIPak = GetISystem()->GetIPak();
}
inline CCryFile::CCryFile(CCryFile&& other)
{
	m_file        = other.m_file;
	m_filename    = std::move(other.m_filename);
	m_pIPak       = other.m_pIPak;

	other.m_file  = 0;
	other.m_pIPak = 0;
}

//////////////////////////////////////////////////////////////////////////
inline CCryFile::CCryFile(const char* filename, const char* mode)
{
	m_file  = 0;
	m_pIPak = GetISystem()->GetIPak();
	Open(filename, mode);
}

//////////////////////////////////////////////////////////////////////////
inline CCryFile::~CCryFile()
{
	Close();
}

//////////////////////////////////////////////////////////////////////////
inline bool CCryFile::Open(std::string_view path, std::string_view mode, int nOpenFlagsEx)
{
	return Open(path.data(), mode.data(), nOpenFlagsEx);
}

//////////////////////////////////////////////////////////////////////////
inline bool CCryFile::Open(const char* filename, const char* mode, int nOpenFlagsEx)
{
	CryPathString tempfilename = filename;

#if !defined(_RELEASE)
	if (Env::Get() && Env::Get()->IsEditor())
	{
		ICVar* const pCvar = Env::Console()->GetCVar("ed_lowercasepaths");
		if (pCvar)
		{
			const int lowercasePaths = pCvar->GetIVal();
			if (lowercasePaths)
			{
				assert(0);
	//FIXME:
	#if 0
				tempfilename.MakeLower();
	#endif
			}
		}
	}
#endif
	if (m_file)
	{
		Close();
	}
	m_filename = tempfilename;

	m_file     = m_pIPak ? m_pIPak->FOpen(tempfilename.c_str(), mode /*, nOpenFlagsEx*/) : fopen(tempfilename.c_str(), mode);
	return m_file != NULL;
}

//////////////////////////////////////////////////////////////////////////
inline void CCryFile::Close()
{
	if (m_file)
	{
		m_pIPak->FClose(m_file);
		m_file     = 0;
		m_filename = "";
	}
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::Write(void* lpBuf, size_t nSize)
{
	assert(m_file);
	return m_pIPak->FWrite(lpBuf, 1, nSize, m_file);
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::ReadRaw(void* lpBuf, size_t nSize)
{
	assert(m_file);
	return IfPak(FReadRaw, fread, (lpBuf, 1, nSize, m_file));
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::Read(void* lpBuf, size_t nSize)
{
	assert(m_file);
	return m_pIPak->FRead(lpBuf, 1, nSize, m_file);
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::GetLength() const
{
	assert(m_file);
	long curr = m_pIPak->FTell(m_file);
	m_pIPak->FSeek(m_file, 0, SEEK_END);
	long size = m_pIPak->FTell(m_file);
	m_pIPak->FSeek(m_file, curr, SEEK_SET);
	return size;
}

#ifdef WIN64
	#pragma warning(push) //AMD Port
	#pragma warning(disable : 4267)
#endif

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::Seek(size_t seek, int mode)
{
	assert(m_file);
	return m_pIPak->FSeek(m_file, seek, mode);
}

#ifdef WIN64
	#pragma warning(pop) //AMD Port
#endif

//////////////////////////////////////////////////////////////////////////
inline void CCryFile::SeekToBegin()
{
	Seek(0, SEEK_SET);
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::SeekToEnd()
{
	Seek(0, SEEK_END);
}

//////////////////////////////////////////////////////////////////////////
inline size_t CCryFile::GetPosition() const
{
	assert(m_file);
	return m_pIPak->FTell(m_file);
}

//////////////////////////////////////////////////////////////////////////
inline bool CCryFile::IsEof()
{
	assert(m_file);
	return m_pIPak->FEof(m_file) != 0;
}

//////////////////////////////////////////////////////////////////////////
inline void CCryFile::Flush()
{
	assert(m_file);
	m_pIPak->FFlush(m_file);
}

//////////////////////////////////////////////////////////////////////////
inline bool CCryFile::IsInPak() const
{
	if (m_file)
	{
		if (m_pIPak->GetFileArchivePath(m_file) != NULL)
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
inline const char* CCryFile::GetPakPath() const
{
	if (m_file)
	{
		const char* sPath = m_pIPak->GetFileArchivePath(m_file);
		if (sPath != NULL)
			return sPath;
	}
	return "";
}

//////////////////////////////////////////////////////////////////////////
inline const char* CCryFile::GetAdjustedFilename() const
{
	assert(m_pIPak);
	if (!m_pIPak)
		return "";

	//! Gets mod path to file.
	static CryPathString adjustedFileName(MAX_PATH, 0);
	m_pIPak->AdjustFileName(m_filename.c_str(), adjustedFileName.data(), 0);
	return adjustedFileName.c_str();
}

#endif // __cryfile_h__
