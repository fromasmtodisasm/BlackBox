//////////////////////////////////////////////////////////////////////
//
//	Crytek Source code 
//	Copyright (c) Crytek 2001-2004
// 
//	File: 
//
//  Description:  
//
//	History:
//
//////////////////////////////////////////////////////////////////////

#ifndef _CHUNK_FILE_READER_HDR_
#define _CHUNK_FILE_READER_HDR_

#include "CryHeaders.h"
#include "smartptr.h"

#include "FileMapping.h"

//class CFileMapping;
//TYPEDEF_AUTOPTR(CFileMapping);

class MemoryFileMapping
{
	MemoryFileMapping(void* data, size_t size)
	    : m_Data(data)
	    , m_Size(size)
	{
	}
	MemoryFileMapping* operator->()
	{
		return this;
	}

	void*  getData() { return m_Data; }
	size_t getSize() { return m_Size; }

	void* m_Data;
	size_t m_Size;
};

////////////////////////////////////////////////////////////////////////
// Chunk file reader. 
// Accesses a chunked file structure through file mapping object.
// Opens a chunk file and checks for its validity.
// If it's invalid, closes it as if there was no open operation.
// Error handling is performed through the return value of open: it must
// be true for successfully open files
////////////////////////////////////////////////////////////////////////

//template<class CFileMapping = MemoryFileMapping>
template<class CFileMapping = ::CFileMapping>
class CChunkFileReader:
	public _reference_target_t
{
public:
	typedef FILE_HEADER FileHeader;
	typedef CHUNK_HEADER ChunkHeader;
	typedef _smart_ptr<CFileMapping> CFileMapping_AutoPtr;

	CChunkFileReader()
	    : m_pChunks(NULL)
	//,m_arrChunkSize ("CChunkFileReader.ChunkSize")
	{
	}
	~CChunkFileReader()
	{
		close();
	}

	// attaches the file mapping object to this file reader and checks
	// whether the file is a valid chunked file
	//////////////////////////////////////////////////////////////////////////
	// attaches the file mapping object to this file reader and checks
	// whether the file is a valid chunked file
	bool open(CFileMapping* pFile)
	{
		close();
		m_pFile       = pFile;

		bool bSuccess = false;

		if ((m_pFile != (CFileMapping*)NULL) && (m_pFile->getData() != NULL))
		{
			if (m_pFile->getSize() >= sizeof(FileHeader))
			{ // the file must contain the header
				const FileHeader& fileHeader = getFileHeader();
				if (m_pFile->getSize() >= fileHeader.ChunkTableOffset + sizeof(int) && (int)fileHeader.ChunkTableOffset > (int)sizeof(fileHeader))
				{ // there must be room for the chunk table header
					unsigned numChunks = *static_cast<const unsigned*>(m_pFile->getData(fileHeader.ChunkTableOffset));
					unsigned nChunk;
					if (m_pFile->getSize() >= fileHeader.ChunkTableOffset + sizeof(int) + numChunks * sizeof(ChunkHeader) && numChunks <= (pFile->getSize() - fileHeader.ChunkTableOffset - sizeof(int)) / sizeof(ChunkHeader))
					{
						// the file must contain the full chunk table
						m_pChunks                               = (const ChunkHeader*)m_pFile->getData(fileHeader.ChunkTableOffset + sizeof(int));

						bool          bInvalidChunkOffsetsFound = false; // sets to true if there are chunks pointing outside the raw data section of the file

						// step through all the chunks to fetch file offsets
						std::set<int> setOffsets;
						for (nChunk = 0; nChunk < numChunks; ++nChunk)
						{
							int nOffset = m_pChunks[nChunk].FileOffset;

							if (nOffset < sizeof(FileHeader) || nOffset >= fileHeader.ChunkTableOffset)
							{
								gLastError                = "CryFile is corrupted: chunk table is corrupted (invalid chunk offsets found)";
								bInvalidChunkOffsetsFound = true;
							}

							setOffsets.insert(nOffset);
						}
						m_arrChunkSize.clear();
						m_arrChunkSize.resize(numChunks);
						for (nChunk = 0; nChunk < numChunks; ++nChunk)
						{
							int nOffset = m_pChunks[nChunk].FileOffset;
							int nSize   = 0; // the size for invalid chunks (with invalid offsets)
							if (nOffset >= sizeof(FileHeader) && nOffset < fileHeader.ChunkTableOffset)
							{
								// find the next offset
								std::set<int>::const_iterator it = setOffsets.find(nOffset);
								assert(it != setOffsets.end());
								assert(*it == nOffset);
								++it;
								nSize = (it == setOffsets.end() ? fileHeader.ChunkTableOffset : *it) - nOffset;
							}

							assert(nSize >= 0);
							m_arrChunkSize[nChunk] = nSize;
						}

						bSuccess = true;
						// don't let the files with invalid chunks..
						//bSuccess = !bInvalidChunkOffsetsFound;
					}
					else
						gLastError = "CryFile is corrupted: chunk table is corrupted or truncated (file too small)";
				}
				else
					gLastError = "CryFile is corrupted: chunk table is trucated (file header is probably corrupted)";
			}
			else
				gLastError = "CryFile is corrupted: header is truncated (file too small)";
		}
		else
			gLastError = "Invalid file mapping passed to CChunkFileReader::open";

		if (!bSuccess)
			close();

		return bSuccess;
	}

	// attaches a new file mapping object to this file reader and checks
	// whether the file is a valid chunked file
	//////////////////////////////////////////////////////////////////////////
	// attaches a new file mapping object to this file reader and checks
	// whether the file is a valid chunked file
	bool open(const char* szFileName, unsigned nFlags = 0)
	{
		auto pFileMapping = _smart_ptr<CFileMapping>(new CFileMapping(szFileName, nFlags));
		if (!pFileMapping->getData())
		{
			gLastError = "Cannot open file";
			return false;
		}
		return open(pFileMapping);
	}

	bool open(const string& strFileName, unsigned nFlags = 0)
	{
		return open(strFileName.c_str(), nFlags);
	}

	// closes the file mapping object and thus detaches the file from this reader
	void close()
	{
		m_arrChunkSize.clear();
		m_pFile   = NULL;
		m_pChunks = NULL;
	}

	// returns the raw data of the file from the given offset
	// returns the raw data of the file from the given offset
	    const void*
	    getRawData(unsigned nOffset) const
	{
		if ((m_pFile != (CFileMapping*)NULL) && m_pFile->getData())
			return ((char*)m_pFile->getData()) + nOffset;
		else
			return NULL;
	}

	// returns the raw data of the i-th chunk
	// returns the raw data of the i-th chunk
	const void* getChunkData(int nChunkIdx) const
	{
		if (nChunkIdx >= 0 && nChunkIdx < numChunks())
		{
			int nOffset = m_pChunks[nChunkIdx].FileOffset;
			if (nOffset < sizeof(FileHeader) || nOffset >= getFileHeader().ChunkTableOffset)
				return 0;
			else
				return m_pFile->getData(nOffset);
		}
		else
			return 0;
	}

	// retrieves the raw chunk header, as it appears in the file
	// retrieves the raw chunk header, as it appears in the file
	const ChunkHeader& getChunkHeader(int nChunkIdx) const
	{
		return m_pChunks[nChunkIdx];
	}

	// calculates the chunk size, based on the very next chunk with greater offset
	// or the end of the raw data portion of the file
	//////////////////////////////////////////////////////////////////////////
	// calculates the chunk size, based on the very next chunk with greater offset
	// or the end of the raw data portion of the file
	int getChunkSize(int nChunkIdx) const
	{
		assert(nChunkIdx >= 0 && nChunkIdx < numChunks());
		return m_arrChunkSize[nChunkIdx];
	}

	// number of chunks
	// number of chunks
	int numChunks() const
	{
		return (int)m_arrChunkSize.size();
	}

	// number of chunks of the specified type
	// number of chunks of the specified type
	int numChunksOfType(ChunkTypes nChunkType) const
	{
		int nResult = 0;
		for (int i = 0; i < numChunks(); ++i)
		{
			if (m_pChunks[i].ChunkType == nChunkType)
				++nResult;
		}
		return nResult;
	}

	// returns the file headers
	// returns the file headers
	const FileHeader& getFileHeader() const
	{
		return m_pFile ? *((const FileHeader*)(m_pFile->getData())) : *(const FileHeader*)NULL;
	}

	bool isValid() const
	{
		return m_pFile != (CFileMapping*)NULL;
	}

    const char* getLastError() const { return gLastError; }

protected:
	// this variable contains the last error occured in this class
	const char* gLastError;

	CFileMapping_AutoPtr m_pFile;
	// array of offsets used by the chunks
	typedef std::vector<int> ChunkSizeArray;
	ChunkSizeArray m_arrChunkSize;
	// pointer to the array of chunks in the m_pFile
	const ChunkHeader* m_pChunks;
};

using CChunkFileReader_AutoPtr = _smart_ptr<CChunkFileReader<>>;

// this function eats the given number of elements from the raw data pointer pRawData
// and increments the pRawData to point to the end of data just eaten
template <typename T>
void EatRawData (T*pArray, unsigned nSize, const void*&pRawData)
{
	memcpy (pArray, pRawData, sizeof(T)*nSize);
	pRawData = static_cast<const T*>(pRawData) + nSize;
}

// this function eats the given number of elements from the raw data pointer pRawData
// and increments the pRawData to point to the end of data just eaten
// RETURNS:
//   false if failed to read the data
template <typename T>
bool EatRawData (T*pArray, unsigned nSize, const void*&pRawData, unsigned& nDataSize)
{
	if (sizeof(T)*nSize <= nDataSize)
	{
		memcpy (pArray, pRawData, sizeof(T)*nSize);
		pRawData = static_cast <const T*> (pRawData) + nSize;
		nDataSize -= sizeof(T)*nSize;
		return true;
	}
	else
		return false;
}

template <typename T>
bool EatRawData (T*pArray, unsigned nSize, const void*&pRawData, const void* pRawDataEnd)
{
	if ((const char*)pRawData + sizeof(T)*nSize <= (const char*)pRawDataEnd)
	{
		memcpy (pArray, pRawData, sizeof(T)*nSize);
		pRawData = static_cast <const T*> (pRawData) + nSize;
		return true;
	}
	else
		return false;
}


// this function puts the pointer to the data to the given pointer, and moves
// the raw data pointer further; if fails, nothing happens and false is returned
// PARAMETERS:
//   pArray - reference to the (pointer) variable to which the pointer to the actual data will be stored
//   nSize  - number of elements in the array (depending on this, the raw data pointer will be moved)
//   pRawData - reference to the actual raw data pointer, this will be incremented
//   nDataSize - reference to the data size counter, this will be decremented upon success
// RETURNS:
//   false if failed to read the data
template <typename T>
bool EatRawDataPtr(const T*&pArray, unsigned nSize, const void*&pRawData, unsigned& nDataSize)
{
	if (sizeof(T)*nSize <= nDataSize)
	{
		pArray = (const T*)pRawData;
		pRawData = static_cast <const T*> (pRawData) + nSize;
		nDataSize -= sizeof(T)*nSize;
		return true;
	}
	else
		return false;
}

template <typename T>
bool EatRawDataPtr(const T*&pArray, unsigned nSize, const void*&pRawData, const void* pRawDataEnd)
{
	if ((const char*)pRawData + sizeof(T)*nSize <= (const char*)pRawDataEnd)
	{
		pArray = (const T*)pRawData;
		pRawData = static_cast <const T*> (pRawData) + nSize;
		return true;
	}
	else
		return false;
}

// ... non-const version, this will hardly be ever needed
/*
template <typename T>
bool EatRawDataPtr(T*&pArray, unsigned nSize, void*&pRawData, unsigned& nDataSize)
{
	if (sizeof(T)*nSize <= nDataSize)
	{
		pArray = (const T*)pRawData;
		pRawData = static_cast <const T*> (pRawData) + nSize;
		nDataSize -= sizeof(T)*nSize;
		return true;
	}
	else
		return false;
}
*/

#endif