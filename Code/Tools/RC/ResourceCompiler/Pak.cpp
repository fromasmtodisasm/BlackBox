#include "ResourceCompiler.h"
#include "ZipFileFormat.h"

#include <regex>
#include <fstream>
#include <iostream>

//#include <crc32c/crc32c.h>
#include <zlib.h>

namespace ZipFile
{
	MemoryArena g_FileDataArena;
	MemoryArena g_CentralDirArena{100};

	class CLocalFileHeader : public LocalFileHeader
	{
		CLocalFileHeader()
		{
		}
	};

	void MakeLocalHeader(void* header, string_view path, uint32 offset, uint32 data_size, void* data)
	{
		LocalFileHeader& lfh = *(LocalFileHeader*)(char*)header;

		auto             crc = crc32(0, (const Bytef*)data, data_size);

		lfh.Signature        = LocalFileHeader::SIGNATURE;
		lfh.VersionNeeded    = SPakFileVersion{0, 10};
		lfh.Flags            = 0;
		lfh.Method           = 0;
		lfh.nLastModTime     = 0;
		lfh.nLastModDate     = 0;
		// If file uncompressed then sizeCompressed and sizeUncompressed should match
		lfh.desc             = DataDescriptor{.CRC32 = crc, .SizeCompressed = data_size, .SizeUncompressed = data_size};
		lfh.FileNameLength   = path.length();
		lfh.ExtraFieldLength = 0;

		WriteTime(lfh);

		strncpy((char*)header + sizeof LocalFileHeader, path.data(), path.length());
	}

	void MakeCentralDirectory(void* header, uint32 offset, string_view path, LocalFileHeader& lfh)
	{
		CentralDirectory& cd  = *(CentralDirectory*)(char*)header;

		cd.lSignature         = cd.SIGNATURE;
		cd.nVersionMadeBy     = 20;
		cd.nVersionNeeded     = 10;
		cd.nFlags             = 0;
		cd.nMethod            = METHOD_STORE;
		cd.nLastModTime       = lfh.nLastModTime;
		cd.nLastModDate       = lfh.nLastModDate;
		cd.desc               = lfh.desc;
		cd.nFileNameLength    = path.length();
		cd.nExtraFieldLength  = 0;
		cd.nFileCommentLength = 0;
		cd.nDiskNumberStart   = 0;
		cd.nAttrInternal      = 0;
		cd.lAttrExternal      = 0;
		cd.lLocalHeaderOffset = offset;

		strncpy((char*)header + sizeof CentralDirectory, path.data(), path.length());
	}
	// conversion routines for the date/time fields used in Zip
	uint16 DOSDate(tm* t)
	{
		return ((t->tm_year - 80) << 9) | (t->tm_mon << 5) | t->tm_mday;
	}

	uint16 DOSTime(tm* t)
	{
		return ((t->tm_hour) << 11) | ((t->tm_min) << 5) | ((t->tm_sec) >> 1);
	}

	template<class Header>
	void WriteTime(Header& header)
	{
		time_t nTime;
		time(&nTime);
		tm* t = localtime(&nTime);

		// While local time converts the month to a 0 to 11 interval...
		// ...the pack file expects months from 1 to 12...
		// Therefore, for correct date, we have to do t->tm_mon+=1;
		t->tm_mon += 1;

		header.nLastModTime = DOSTime(t);
		header.nLastModDate = DOSDate(t);
	}

	auto FillLocalHeader(string_view path, void* file_data = 0, uint32 data_size = 0)
	{
		auto              alloc_size = sizeof LocalFileHeader + path.length();
		std::vector<char> data;

		data.resize(alloc_size);
		MakeLocalHeader(&data[0], path, 0, data_size, file_data);
	}

	void WriteArchiveRecursive(ArchiveInfo& ar, std::filesystem::directory_entry dir_entry, std::ofstream& of)
	{
		if (!dir_entry.exists())
		{
			printf("Path [%s] dont exists\n", dir_entry.path().string().c_str());
			abort();
		}

		std::error_code ec;
		for (fs::directory_iterator next(dir_entry, ec), end; next != end; ++next)
		{
			if (!g_Options.exclude.empty())
			{
				if (regex_match(next->path().string(), std::regex{g_Options.exclude, std::regex::extended}))
				{
					continue;
				}
			}
			WriteDirectoryEntry(of, ar, *next);
		}
	}
	void WriteDirectoryEntry(std::ofstream& of, ZipFile::ArchiveInfo& ar, const std::filesystem::directory_entry& dir_entry)
	{
		{
			auto path         = dir_entry.path().string();
			auto reduced_path = remove_leading_ups(path);

			if (!dir_entry.is_regular_file())
			{
				if (path[path.size() - 1] != '/') path += '/';
			}

			CFileMapping fm(path.data());

			auto         file_data = (void*)fm.getData();
			auto         file_size = fm.getSize();

			FillLocalFileHeaderAndCentralDirectory(reduced_path, file_size, file_data, of, ar);

			if (g_Options.verbose)
			{
				std::cout << path << std::endl;
			}
			ar.NumberOfFiles++;

			if (!dir_entry.is_regular_file())
			{
				if (g_Options.recursion)
				{
					WriteArchiveRecursive(ar, dir_entry, of);
				}
			}
		}
	}
	void FillLocalFileHeaderAndCentralDirectory(
	    const std::string_view& path,
	    unsigned int            file_size,
	    void*                   file_data,
	    std::ofstream&          of,
	    ArchiveInfo&            info)
	{
		auto              alloc_size = sizeof LocalFileHeader + path.size();

		std::vector<char> data;
		data.resize(alloc_size);
		memset(&data[0], 0, data.size());
		LocalFileHeader* local_header = (LocalFileHeader*)&data[0];

		MakeLocalHeader(local_header, path, 0, file_size, (void*)file_data);
		of.write((const char*)local_header, sizeof LocalFileHeader + local_header->FileNameLength);
		of.write((const char*)file_data, file_size);

		{
			auto              size       = sizeof CentralDirectory;
			auto              alloc_size = size + path.size();
			CentralDirectory* header     = (CentralDirectory*)g_CentralDirArena.alloc(alloc_size);
			MakeCentralDirectory(header, info.CurrentOffset, path, *local_header);
		}

		info.CurrentOffset += sizeof LocalFileHeader + local_header->ExtraFieldLength + local_header->FileNameLength + file_size;
	}
	LocalFileHeader make_archive()
	{
		LocalFileHeader ar{0};
		ar.Signature     = LocalFileHeader::SIGNATURE;
		ar.VersionNeeded = SPakFileVersion{14, 0};

		return ar;
	}

	///////
	void write_archive(const std::string& pattern, const std::string out_file)
	{
		ArchiveInfo   info{0};
		CDREnd        cdr_end{};

		std::ofstream of{out_file, std::ios_base::binary};
		if (of.is_open())
		{
#if 1
			info.NumberOfFiles = 0;
			info.CurrentOffset = 0;

			string file        = pattern;
			std::replace(file.begin(), file.end(), '\\', '/');

			auto dir_entry = std::filesystem::directory_entry(file);
			if (dir_entry.is_directory())
			{
				if (pattern[pattern.size() - 1] != '/') dir_entry = fs::directory_entry(file += '/');
				info.NumberOfFiles++;
				FillLocalFileHeaderAndCentralDirectory(dir_entry.path().string(), 0, 0, of, info);
				WriteArchiveRecursive(info, dir_entry, of);
			}
			else
			{
				WriteDirectoryEntry(of, info, dir_entry);
			}

			auto offset = (uint32)of.tellp();
			of.write((const char*)g_CentralDirArena.data(), g_CentralDirArena.size());

			cdr_end.lSignature       = cdr_end.SIGNATURE;
			cdr_end.lCDROffset       = offset;
			cdr_end.lCDRSize         = (uint32)of.tellp() - offset;
			cdr_end.numEntriesOnDisk = info.NumberOfFiles;
			cdr_end.numEntriesTotal  = info.NumberOfFiles;
			of.write((const char*)&cdr_end, sizeof cdr_end);
#else
			of.write((const char*)&ar, sizeof LocalFileHeader);
#endif
		}
		else
		{
			std::cerr << "Can't open " << out_file << std::endl;
			abort();
		}
	}

	SArchiveHandle archive_open(std::string_view file)
	{
		return SArchiveHandle(file);
	}

	void list(const string& file)
	{
		if (auto ar = SArchiveHandle(file); ar)
		{
			// Print end of central directory
			auto     data{(char*)ar.fm->getData()};
			unsigned size{ar.fm->getSize()};
			CDREnd&  cdr = *(CDREnd*)(data + size - sizeof CDREnd);
			if (!(cdr.lSignature == cdr.SIGNATURE))
			{
				puts("Error signature of Central Directory End");
				return;
			}
			CentralDirectory* cd = (CentralDirectory*)(data + cdr.lCDROffset);
			std::cout << "Num Entries on disk: " << cdr.numEntriesOnDisk << "\nSize of Central Directory: " << cdr.lCDRSize << std::endl;
			std::cout << "lCDROffset: " << cdr.lCDROffset << std::endl;

			for (size_t i = 0; i < cdr.numEntriesOnDisk; i++)
			{
				if (!(cd->lSignature == CentralDirectory::SIGNATURE))
				{
					puts("Error signature of Central Directory");
					return;
				}

				auto& lh   = *(LocalFileHeader*)(data + cd->lLocalHeaderOffset);
				auto  path = string_view((char*)(&lh + 1), lh.FileNameLength);
				std::cout << path << "\t" << lh.desc.CRC32 << "\t" << lh.desc.SizeUncompressed << std::endl;

				cd = (CentralDirectory*)((char*)(cd) + sizeof CentralDirectory + cd->nFileNameLength + cd->nExtraFieldLength + cd->nFileCommentLength);
			}
		}
	}

	void create_file(LocalFileHeader& ar, fs::path filename, SRange range)
	{
		create_file(ar, filename, range.begin, range.end);
	}

	void create_file(LocalFileHeader& ar, fs::path filename, uint32 offset, uint32 size)
	{
		char* base = (char*)&ar;
		auto  path = (fs::path("./") / fs::path(filename)).parent_path();

		if (!fs::exists(path) && !path.has_extension())
			fs::create_directories(path);

		if (g_Options.verbose)
			std::cout << filename.string() << std::endl;

		std::ofstream of{filename.string(), std::ios_base::binary};
		of.write(base + offset, size);
	}

	SRange file_range(CentralDirectory& entry)
	{
		return SRange{entry.lLocalHeaderOffset + uint32(sizeof LocalFileHeader) + entry.nFileNameLength, entry.desc.SizeUncompressed};
	}

	string_view file_name(CentralDirectory& entry, void* header)
	{
		return string_view((char*)header + entry.lLocalHeaderOffset + sizeof LocalFileHeader, entry.nFileNameLength); //
	}

	void extract(const string& file, const string& base, const string& pattern)
	{
		if (auto ar = SArchiveHandle(file); ar)
		{
			for (auto& entry : ar)
			{
				size_t offset = 0;
				auto   strv   = file_name(entry, ar.header); // string_view(entry.file_name.data, entry.file_name.size);
				if (auto pos = strv.find(base.c_str()); pos == 0)
				{
					offset = base.size();
				}

				auto path = fs::path(strv.substr(offset));
				if (!pattern.empty())
				{
					if (regex_match(path.string(), std::regex{pattern, std::regex::extended}))
						create_file(ar, path, file_range(entry));
				}
				else
				{
					create_file(ar, path, file_range(entry));
				}
			}
		}
	}

} // namespace ZipFile
