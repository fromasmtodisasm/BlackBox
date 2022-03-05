#include "ResourceCompiler.h"
#include "Pak.h"

#include <regex>
#include <fstream>
#include <iostream>

MemoryArena SDirEntry::arena;

void write_archive_recursive(SArchive& ar, const std::string& file, std::ofstream& of)
{
	string pattern = file;
	std::replace(pattern.begin(), pattern.end(), '\\', '/');
	if (!fs::exists(pattern))
	{
		printf("Path [%s] dont exists\n", pattern.c_str());
		abort();
	}
	auto                    it = std::filesystem::directory_iterator{pattern};
	std::vector<SDirEntry*> vtoc;
	for (auto const& dir_entry : std::filesystem::directory_iterator{pattern})
	{
		auto path     = dir_entry.path().string();
		auto filename = dir_entry.path().filename().string();
		std::replace(path.begin(), path.end(), '\\', '/');
		if (!g_Options.exclude.empty())
		{
			if (regex_match(path, std::regex{g_Options.exclude, std::regex::extended}))
			{
				continue;
			}
		}
		if (dir_entry.is_regular_file())
		{
			auto         reduced_path = remove_leading_ups(path);
			CFileMapping fm(path.data());

			auto         alloc_size = sizeof SDirEntry + reduced_path.size() - 1; // why -2 ???
			SDirEntry*   toc        = SDirEntry::alloc(alloc_size);

			new (toc) SDirEntry(reduced_path, ar.toc_offset, fm.getSize());

			of.write((const char*)fm.getData(), fm.getSize());
			ar.toc_offset += fm.getSize();
			if (g_Options.verbose)
			{
				std::cout << path << std::endl;
			}
			ar.number_of_files++;
		}
		else if (g_Options.recursion)
		{
			write_archive_recursive(ar, path, of);
		}
	}
}

///////
SArchive write_archive(const std::string& pattern, const std::string out_file)
{
	SArchive ar;
	ar.magic   = 'BBAR';
	ar.version = SPakFileVersion{1, 1, 1};

	std::ofstream of{out_file, std::ios_base::binary};

	ar.number_of_files = 0;
	ar.toc_offset      = sizeof SArchive;

	of.write((const char*)&ar, sizeof SArchive);

	string file = pattern;
	std::replace(file.begin(), file.end(), '\\', '/');

	auto _offset = offsetof(SArchive, number_of_files);

	write_archive_recursive(ar, file, of);
	of.write((char*)SDirEntry::arena.data(), SDirEntry::arena.size());
	of.seekp(_offset);
	of.write((const char*)&ar.number_of_files, sizeof ar.number_of_files);
	of.write((const char*)&ar.toc_offset, sizeof ar.toc_offset);

	return ar;
}

SArchiveHandle archive_open(std::string_view file)
{
	return SArchiveHandle(file);
}

void list(const string& file)
{
	if (auto ar = SArchiveHandle(file); ar)
	{
		int i = 0;
		printf("number of files: %d\n", ((SArchive&)ar).number_of_files);
		puts("id\tpath\toffset\tsize");

		for (auto& entry : ar)
		{
			printf("%d\t%*.*s\t%d\t%d\n",
			       i++,
			       entry.file_name.size,
			       entry.file_name.size,
			       entry.file_name.data,
			       entry.offset + 1,
			       entry.size);
		}
	}
}

void create_file(SArchive& ar, fs::path filename, std::int32_t offset, std::uint32_t size)
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

void extract(const string& file, const string& base, const string& pattern)
{
	if (auto ar = SArchiveHandle(file); ar)
	{
		for (auto& entry : ar)
		{
			size_t offset = 0;
			auto   strv   = string_view(entry.file_name.data, entry.file_name.size);
			if (auto pos = strv.find(base.c_str()); pos == 0)
			{
				offset = base.size();
			}

			auto path = fs::path(strv.substr(offset));
			if (!pattern.empty())
			{
				if (regex_match(path.string(), std::regex{pattern, std::regex::extended}))
					create_file(ar, path, entry.offset, entry.size);
			}
			else
			{
				create_file(ar, path, entry.offset, entry.size);
			}
		}
	}
}
