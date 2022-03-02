#undef UNICODE
#include "ResourceCompiler.h"
#include "FileMapping.h"

#include <string>
#include <string_view>
#include <io.h>
#include <Windows.h>
#include <fstream>

#include <filesystem>

using std::string;
using std::wstring;

namespace fs = std::filesystem;

#define RCLog(f, ...) printf(f "\n", __VA_ARGS__)
#define RCLogError    RCLog

#define GetExePath()  "./"

//////////////////////////////////////////////////////////////////////////
bool ResourceCompiler::RegisterConverters()
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

struct SVariableString
{
	std::uint16_t size;
	char          data[64];
};

struct SToc
{
	SVariableString file_name;
	std::int32_t    offset;
};

struct SArchive
{
	std::uint32_t magic;
	SFileVersion  version;
	std::uint32_t number_of_files;
	std::uint32_t toc_offset;
};

template<typename fn>
void write(const SArchive& ar, const std::string& file_name, fn func)
{
	using namespace std;
	ofstream of(file_name.data(), ios_base::binary);
	if (of.is_open())
	{
		func(of, (const char*)&ar.magic, sizeof ar.magic);
		func(of, (const char*)&ar.version.version, sizeof ar.version.version);
		func(of, (const char*)&ar.number_of_files, sizeof ar.number_of_files);
		auto* entry = &ar.toc[0];
		for (auto count{0}; count < ar.number_of_files; count++)
		{
			func(of, (const char*)&entry[count].file_name, sizeof SVariableString);
			func(of, (const char*)&entry[count].offset, sizeof entry[count].offset);
		}
	}
	else
	{
		printf("All bad");
	}
}

template<typename fn>
void read(SArchive& ar, const std::string& file_name, fn func)
{
	using namespace std;
	ifstream in(file_name.data(), ios_base::binary);
	if (in.is_open())
	{
		func(in, (char*)&ar.magic, sizeof ar.magic);
		func(in, (char*)&ar.version.version, sizeof ar.version.version);
		func(in, (char*)&ar.number_of_files, sizeof ar.number_of_files);
		auto* entry = &ar.toc[0];
		for (auto count{0}; count < ar.number_of_files; count++)
		{
			func(in, (char*)&entry[count].file_name, sizeof entry[count].file_name);
			func(in, (char*)&entry[count].offset, sizeof entry[count].offset);
		}
	}
	else
	{
		printf("All bad");
	}
}

bool cat(const string& source, const string& dest)
{
	const string strDir  = "./test_folder/";
	const string strMask = strDir + "*.txt";

	RCLog("Searching for %s", strMask.c_str());

	string      widePath = strMask, wstrDir = strDir;

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
		CFileMapping fileMapping(pluginFilename.c_str());
		if (!fileMapping.getData())
		{
			RCLogError("Error");
			return false;
		}

		RCLog("  Loaded \"%s\"", fd.name);
		//std::printf("%s\n", fileMapping.getData());

	} while (_findnext(hSearch, &fd) != -1);
}

static constexpr const char* fn = "../../CMakeLists.txt";

///////
void                         write_archive(SArchive& ar, const std::string& pattern, const std::string out_file)
{
	std::ofstream     of{out_file};

	auto              it = std::filesystem::directory_iterator{pattern};
	std::vector<SToc> vtoc;
	ar.number_of_files = 0;
	ar.toc_offset = sizeof SArchive;

	of.write((const char*)&ar, sizeof SArchive);

	for (auto const& dir_entry : std::filesystem::directory_iterator{pattern})
	{
		if (dir_entry.is_regular_file())
		{
			string       path = dir_entry.path().u8string();
			CFileMapping fm(path.c_str());
			SToc         toc;
			toc.offset         = ar.toc_offset;
			toc.file_name.size = fm.getSize();
			strcpy(toc.file_name.data, path.data());

			of.write((const char*)fm.getData(), fm.getSize());
			//printf("data: \n%s\n", fm.getData());
			ar.toc_offset += fm.getSize();
			ar.number_of_files++;

			vtoc.push_back(toc);
		}
	}
	//ar.toc_offset += 3;
	of.write((const char*)vtoc.data(), vtoc.size() * sizeof(SToc));
	auto _offset = offsetof(SArchive, number_of_files);
	of.seekp(_offset);
	of.write((const char*)&ar.number_of_files, sizeof ar.number_of_files);
	of.write((const char*)&ar.toc_offset, sizeof ar.toc_offset);
}

void list_of_files(SArchive& ar)
{
	auto* entry = reinterpret_cast<SToc*>((byte*) & ar + ar.toc_offset);
	for (size_t i = 0; i < ar.number_of_files; i++)
	{
		printf("file: %s, offset: %d\n", entry[i].file_name.data, entry[i].offset + 1);
	}
}

void read_archive()
{
}

int main()
{
	ResourceCompiler RC;

	CFileMapping     fm(fn);
	auto             data = fm.getData();
	auto             fv   = SFileVersion(1, 1, 1);

	RC.RegisterConverters();
	SArchive         ar;
	ar.magic   = 0xff;
	ar.version = SFileVersion{1, 1, 1};
	write_archive(ar, "./test_folder/", "my.pak");

	CFileMapping archive("my.pak");

	list_of_files((SArchive&)*((SArchive*)(archive.getData())));



	return 0;
}