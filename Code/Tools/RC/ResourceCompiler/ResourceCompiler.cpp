#undef UNICODE
#include "ResourceCompiler.h"
#include "FileMapping.h"

#include <string>
#include <string_view>
#include <io.h>
#include <Windows.h>
#include <fstream>

#include <filesystem>
#include <cassert>
#include <regex>

#include "ini.h"
#include <iostream>

using std::string;
using std::wstring;

namespace fs = std::filesystem;

#define RCLog(f, ...) printf(f "\n", __VA_ARGS__)
#define RCLogError    RCLog

#define GetExePath()  "./"

std::string_view remove_leading_ups(std::string_view str)
{
	std::string_view result = str;
	while (true)
	{
		if (auto pos = result.find("../"); pos == 0)
		{
			result = std::string_view{result.data() + 3};
		}
		else if (auto pos = result.find("./"); pos == 0)
		{
			result = std::string_view{result.data() + 2};
		}
		else
		{
			break;
		}
	}
	return result;
}

struct SOptions
{
	bool   list{false};
	string input_file;

	bool   create{false};
	string input_folder;
	string output_file;

	bool   extract{false};
	string extract_file;
	string extract_base;
	string extract_pattern;

	bool   verbose{false};
	bool   recursion{true};

	string exclude;
};

SOptions g_Options;

//////////////////////////////////////////////////////////////////////////
bool     ResourceCompiler::RegisterConverters()
{
	const string strDir  = GetExePath();
	const string strMask = strDir + "ResourceCompiler*.dll";

	RCLog("Searching for %s", strMask.c_str());

	string      widePath = strMask, wstrDir = strDir;
	//Unicode::Convert(widePath, strMask.c_str());
	//Unicode::Convert(wstrDir, strDir.c_str());

	_finddata_t fd;
	intptr_t    hSearch = _findfirst(widePath.data(), &fd);
	if (hSearch == -1)
	{
		RCLog("Cannot find any %s", strMask.c_str());
		return false;
	}

	do
	{
		const string pluginFilename = wstrDir + fd.name;
		RCLog("Loading \"%s\"", pluginFilename.c_str());
		HMODULE hPlugin = LoadLibrary(pluginFilename.c_str());
		if (!hPlugin)
		{
			const DWORD errCode             = GetLastError();
			char        messageBuffer[1024] = {'?', 0};
			FormatMessageA(
			    FORMAT_MESSAGE_FROM_SYSTEM,
			    NULL,
			    errCode,
			    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			    messageBuffer,
			    sizeof(messageBuffer) - 1,
			    NULL);
			RCLogError("Couldn't load plug-in module \"%s\"", pluginFilename.c_str());
			RCLogError("Error code: 0x%x (%s)", errCode, messageBuffer);
			return false;
		}
		FnRegisterConverters fnRegister =
		    hPlugin
		        ? (FnRegisterConverters)GetProcAddress(hPlugin, "RegisterConverters")
		        : NULL;
		if (!fnRegister)
		{
			RCLog("Error: plug-in module \"%s\" doesn't have RegisterConverters function", pluginFilename.c_str());
			return false;
		}

		RCLog("  Loaded \"%s\"", fd.name);

		//const int oldErrorCount = GetNumErrors();
		fnRegister(this);
		//const int newErrorCount = GetNumErrors();
		//if (newErrorCount > oldErrorCount)
		//{
		//	return false;
		//}

	} while (_findnext(hSearch, &fd) != -1);

	RCLog("");

	_findclose(hSearch);

	return true;
}

using byte = std::uint8_t;

struct SFileVersion
{
	SFileVersion() = default;
	SFileVersion(byte major, byte minor, byte patch)
	    : version(0)
	{
		version = major << 16 | minor << 8 | patch;
	}

public:
	std::uint32_t version;
};

struct MemoryArena
{
	static constexpr std::uint32_t _size           = 10 * 0xffff;
	static constexpr float         _growing_factor = 1.6f;
	size_t                         _offset{};
	std::vector<char>              _data{};
	MemoryArena()
	{
		_data.resize(this->_size);
	}

	void* alloc(size_t size)
	{
		//assert(_offset + size < this->_size && "Not enough memory in arena!!!");

		bool enough_mem = (_offset + size) < this->_size;
		if (!enough_mem)
		{
			_data.resize(size_t(_data.size() * _growing_factor));
		}

		auto result = &_data[_offset];
		_offset += size;

		return result;
	}
	template<class T>
	T* alloc(size_t size)
	{
		return reinterpret_cast<T*>(alloc(size));
	}

	size_t size() { return _offset; }
	void*  data() { return &_data[0]; }
};

struct SVariableString
{
	std::uint16_t size;
	char          data[1];

	SVariableString(std::uint16_t size, char* data)
	    : size(size)
	{
		strcpy((this->data), data);
	}
};

struct SToc
{
	std::int32_t    offset;
	std::uint32_t   size;
	SVariableString file_name;

	SToc(std::string_view file_name, std::int32_t offset, std::uint32_t size)
	    : file_name(std::uint16_t(file_name.length()), (char*)file_name.data())
	    , offset(offset)
	    , size(size)
	{
	}

	static MemoryArena arena;

	static SToc*       alloc(size_t size)
	{
		return (SToc*)arena.alloc(size);
	}
};

MemoryArena SToc::arena;

struct SArchive
{
	std::uint32_t magic;
	SFileVersion  version;
	std::uint32_t number_of_files;
	std::uint32_t toc_offset;

};

std::vector<SToc*> write_archive_recursive(SArchive& ar, const std::string& file, std::ofstream& of)
{
	string pattern = file;
	std::replace(pattern.begin(), pattern.end(), '\\', '/');
	if (!fs::exists(pattern))
	{
		printf("Path [%s] dont exists\n", pattern.c_str());
		abort();
	}
	auto               it = std::filesystem::directory_iterator{pattern};
	std::vector<SToc*> vtoc;
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

			auto         alloc_size = sizeof SToc + reduced_path.size() - 1; // why -2 ???
			SToc*        toc        = SToc::alloc(alloc_size);

			new (toc) SToc(reduced_path, ar.toc_offset, fm.getSize());

			of.write((const char*)fm.getData(), fm.getSize());
			ar.toc_offset += fm.getSize();
			if (g_Options.verbose)
			{
				std::cout << path << std::endl;
			}
			ar.number_of_files++;

#if 0
			vtoc.push_back(toc);
#endif
		}
		else if (g_Options.recursion)
		{
			auto toc = write_archive_recursive(ar, path, of);
#if 0
			vtoc.insert(vtoc.end(), toc.begin(), toc.end());
#endif
		}
	}
	return vtoc;
}
SArchive write_archive(const std::string& pattern, const std::string out_file);

///////
SArchive write_archive(const std::string& pattern, const std::string out_file)
{
	SArchive ar;
	ar.magic   = 'BBAR';
	ar.version = SFileVersion{1, 1, 1};

	std::ofstream of{out_file, std::ios_base::binary};

	ar.number_of_files = 0;
	ar.toc_offset      = sizeof SArchive;

	of.write((const char*)&ar, sizeof SArchive);

	string file = pattern;
	std::replace(file.begin(), file.end(), '\\', '/');

	auto toc = write_archive_recursive(ar, file, of);
	//SToc fakeToc{"", INT32_MAX, UINT32_MAX};
	of.write((char*)SToc::arena.data(), SToc::arena.size());
	//of.write((char*)&fakeToc, sizeof SToc);
	auto _offset = offsetof(SArchive, number_of_files);
	of.seekp(_offset);
	of.write((const char*)&ar.number_of_files, sizeof ar.number_of_files);
	of.write((const char*)&ar.toc_offset, sizeof ar.toc_offset);

	return ar;
}

template<typename F>
void foreach (SArchive& ar, F func)
{
	auto* entry = reinterpret_cast<SToc*>((byte*)&ar + ar.toc_offset);
	for (size_t i = 0; i < ar.number_of_files; i++)
	{
		func(ar, entry);
		entry = (SToc*)((byte*)entry->file_name.data + entry->file_name.size + 1);
	}
}

struct SArchiveHandle
{
	CFileMapping fm;
	SArchive*    header{};
	SArchiveHandle(std::string_view file)
	    : fm(file.data())
	{
		header = (SArchive*)fm.getData();
	}

	operator SArchive&()
	{
		return *header;
	}

	operator bool()
	{
		return header != nullptr;
	}

	struct Iterator
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = SToc;
		using pointer           = value_type*; 
		using reference         = value_type&;

		Iterator(pointer ptr)
		    : m_ptr(ptr)
		{
		}
		reference operator*() const { return *m_ptr; }
		pointer   operator->() { return m_ptr; }

		// Prefix increment
		Iterator& operator++()
		{
			m_ptr = (SToc*)((byte*)m_ptr->file_name.data + m_ptr->file_name.size + 1);
			return *this;
		}

		// Postfix increment
		Iterator operator++(int)
		{
			Iterator tmp = *this;
			++(*this);
			return tmp;
		}

		friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
		friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
		/////////////////////////////////////////////

	private:
		pointer       m_ptr;
	};

	Iterator begin() { return Iterator(reinterpret_cast<SToc*>((byte*)header + header->toc_offset)); }
	Iterator end() { return Iterator((SToc*)((byte*)header + fm.getSize())); }
};

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
using std::string_view;

void create_file(SArchive& ar, fs::path filename, std::int32_t offset, std::uint32_t size)
{
	char* base = (char*)&ar;

	auto  path = (fs::path("./") / fs::path(filename)).parent_path();
	if (!fs::exists(path) && !path.has_extension())
	{
		fs::create_directories(path);
	}
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

int config_handler(void* user, const char* section,
                   const char* name, const char* value)
{
#define match(a) if ((std::string_view(a) == name))
	match("list")
	{
		g_Options.list = *value - '0';
		return true;
	}
	match("create-pak")
	{
		g_Options.create = *value - '0';
		return true;
	}
	match("input-file")
	{
		g_Options.input_file = value;
		return true;
	}

	match("input-folder")
	{
		g_Options.input_folder = value;
		return true;
	}
	match("output-file")
	{
		g_Options.output_file = value;
		return true;
	}
	return false;
#undef match
}

void parse_cmd(int argc, char* argv[])
{
#if 0
	ini_parse("rc.ini", config_handler, nullptr);
#endif

	for (int i = 1; i < argc; i++)
	{
#define match(s, n) \
	if ((std::regex_match(argv[i], std::regex{"--" s})) && (argc > i + n))
		match("list", 1)
		{
			g_Options.input_file = argv[++i];
			g_Options.list       = !g_Options.input_file.empty();
			continue;
		}
		match("create", 2)
		{
			g_Options.input_folder = argv[++i];
			g_Options.output_file  = argv[++i];
			g_Options.create       = !g_Options.input_folder.empty() && !g_Options.output_file.empty();
			continue;
		}

		match("extract", 1)
		{
			g_Options.extract_file = argv[++i];
			g_Options.extract      = !g_Options.extract_file.empty();
			continue;
		}
		match("extract-base", 1)
		{
			g_Options.extract_base = argv[++i];
			continue;
		}
		match("extract-pattern", 1)
		{
			g_Options.extract_pattern = argv[++i];
			continue;
		}
		match("verbose", 0)
		{
			g_Options.verbose = true;
			continue;
		}
		match("no-recursion", 0)
		{
			g_Options.recursion = false;
			continue;
		}
		match("exclude", 1)
		{
			g_Options.exclude = argv[++i];
			continue;
		}
	}
#undef match
}

int main(int argc, char* argv[])
{
	parse_cmd(argc, argv);

	ResourceCompiler RC;
	(void)RC;
	//RC.RegisterConverters();

	if (g_Options.list)
	{
		list(g_Options.input_file);
		return 0;
	}
	else
	{
		if (g_Options.create)
			write_archive(g_Options.input_folder, g_Options.output_file);
		else if (g_Options.extract)
			extract(g_Options.extract_file, g_Options.extract_base, g_Options.extract_pattern);
	}

	return 0;
}