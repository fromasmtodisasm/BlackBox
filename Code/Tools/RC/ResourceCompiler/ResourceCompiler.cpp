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

#include <ini.h>

using std::string;
using std::wstring;

namespace fs = std::filesystem;

#define RCLog(f, ...) printf(f "\n", __VA_ARGS__)
#define RCLogError    RCLog

#define GetExePath()  "./"

struct SOptions
{
	bool   dump{};
	string input_file;
	bool   create_pak;
	string input_folder;
	string output_file;
	bool   unpak;
	string unpak_file;
	string unpak_base;
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
	static constexpr std::uint32_t _size = 10 * 0xffff;
	size_t                         _offset{};
	std::vector<char>              _data{};
	MemoryArena()
	{
		_data.resize(this->_size);
	}

	void* alloc(size_t size)
	{
		assert(_offset + size < this->_size);
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

	SToc(const char* file_name, std::uint16_t file_name_size, std::int32_t offset, std::uint32_t size)
	    : file_name(file_name_size, (char*)file_name)
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

std::vector<SToc*> write_archive_recursive(SArchive& ar, const std::string& pattern, std::ofstream& of)
{
	auto               it = std::filesystem::directory_iterator{pattern};
	std::vector<SToc*> vtoc;
	for (auto const& dir_entry : std::filesystem::directory_iterator{pattern})
	{
		if (dir_entry.is_regular_file())
		{
			string       path = dir_entry.path().u8string();
			CFileMapping fm(path.c_str());

			auto         alloc_size = sizeof SToc + path.size() - 1; // why -2 ???
			SToc*        toc        = SToc::alloc(alloc_size);

			new (toc) SToc(path.data(), std::uint16_t(path.size()), ar.toc_offset, fm.getSize());

			of.write((const char*)fm.getData(), fm.getSize());
			ar.toc_offset += fm.getSize();
			ar.number_of_files++;

#if 0
			vtoc.push_back(toc);
#endif
		}
		else
		{
			auto toc = write_archive_recursive(ar, dir_entry.path().u8string(), of);
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

	auto toc = write_archive_recursive(ar, pattern, of);
	of.write((char*)SToc::arena.data(), SToc::arena.size());
	auto _offset = offsetof(SArchive, number_of_files);
	of.seekp(_offset);
	of.write((const char*)&ar.number_of_files, sizeof ar.number_of_files);
	of.write((const char*)&ar.toc_offset, sizeof ar.toc_offset);

	return ar;
}

template<typename F>
void iterate(SArchive& ar, F func)
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
};

SArchiveHandle archive_open(std::string_view file)
{
	return SArchiveHandle(file);
}

void dump(const string& file)
{
	if (auto ar = archive_open(file); ar)
	{
		int i = 0;
		printf("number of files: %d\n", ((SArchive&)ar).number_of_files);
		puts("id\tpath\toffset\tsize");
		iterate(ar, [&i](SArchive& ar, SToc* entry)
		        { printf("%d\t%*.*s\t%d\t%d\n",
			             i++,
			             entry->file_name.size,
			             entry->file_name.size,
			             entry->file_name.data,
			             entry->offset + 1,
			             entry->size); });
	}
}
using std::string_view;

void create_file(SArchive& ar, fs::path filename, std::int32_t offset, std::uint32_t size)
{
	char* base = (char*)&ar;

	auto  path = (fs::path("./") / fs::path(filename)).parent_path();
	if (!fs::exists(path) && !path.has_extension())
	{
		auto tmp = path.u8string();
		if (path.u8string().find(".cpp") != tmp.npos)
		{
			printf("OMG!!!");
			//DebugBreak();
		}

		fs::create_directories(path);
	}
	std::ofstream of{"./" + string(filename.u8string()), std::ios_base::binary};
	of.write(base + offset, size);
}

void unpak(const string& file, const string& base)
{
	if (auto ar = archive_open(file); ar)
	{
		iterate(ar, [&base](SArchive& ar, SToc* entry)
		        {
				auto offset = 0;
				auto strv = string_view(entry->file_name.data, entry->file_name.size);
				if (auto pos = strv.find(base.c_str()); pos == 0) {
				    offset = base.size();
                }
                create_file(ar, fs::path(string(string_view(entry->file_name.data + offset, entry->file_name.size))), entry->offset, entry->size); });
	}
}

int config_handler(void* user, const char* section,
                   const char* name, const char* value)
{
#define match(a) if ((std::string_view(a) == name))
	match("dump")
	{
		g_Options.dump = *value - '0';
		return true;
	}
	match("create-pak")
	{
		g_Options.create_pak = *value - '0';
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
#undef match
}

void parse_cmd(int argc, char* argv[])
{
	string config = argv[1];

	ini_parse(config.c_str(), config_handler, nullptr);

	for (int i = 2; i < argc; i++)
	{
#define match(s, n) \
	if ((std::regex_match(argv[i], std::regex{"--" s})) && (argc > i + n))
		match("dump", 1)
		{
			g_Options.input_file = argv[++i];
			g_Options.dump       = !g_Options.input_file.empty();
			continue;
		}
		match("create-pak", 2)
		{
			g_Options.input_folder = argv[++i];
			g_Options.output_file  = argv[++i];
			g_Options.create_pak   = !g_Options.input_folder.empty() && !g_Options.output_file.empty();
			continue;
		}

		match("unpak", 1)
		{
			g_Options.unpak_file = argv[++i];
			g_Options.unpak      = !g_Options.unpak_file.empty();
			continue;
		}
		match("unpak-base", 1)
		{
			g_Options.unpak_base = argv[++i];
			continue;
		}
        //string_view(argv[i]).find_first_of("--",)
	}
#undef match
}

int main(int argc, char* argv[])
{
	parse_cmd(argc, argv);

	ResourceCompiler RC;
	//RC.RegisterConverters();

	if (g_Options.dump)
	{
		dump(g_Options.input_file);
		return 0;
	}
	else if (g_Options.create_pak)
		write_archive(g_Options.input_folder, g_Options.output_file);
	else if (g_Options.unpak)
		unpak(g_Options.unpak_file, g_Options.unpak_base);

	return 0;
}