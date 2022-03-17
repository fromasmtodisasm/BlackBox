#undef UNICODE
#include "pch.hpp"
#include "ResourceCompiler.h"
#include "ZipFileFormat.h"

#include <BlackBox/Core/Platform/platform_impl.inl>

#include <cassert>
#include <regex>

#include "ini.h"

SOptions g_Options;

namespace ZipFile
{
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
} // namespace ZipFile

int  config_handler(void* user, const char* section, const char* name, const char* value);
void parse_cmd(int argc, char* argv[]);

int  config_handler(void* user, const char* section,
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
		match("testing", 0)
		{
			g_Options.testing = true;
			continue;
		}
	}
#undef match
}

int main(int argc, char* argv[])
{
	using namespace ZipFile;

	parse_cmd(argc, argv);

	ResourceCompiler RC;
	(void)RC;
	//RC.RegisterConverters();

	#if 1
	if (g_Options.list)
	{
		List(g_Options.input_file);
		//return 0;
	}
	else
	{
		if (g_Options.create)
			CreateArchive(g_Options.input_folder, g_Options.output_file);
		else if (g_Options.extract)
			Extract(g_Options.extract_file, g_Options.extract_base, g_Options.extract_pattern);
	}

	if (g_Options.testing)
	{
		CPak testPak;

		bool ok{true};
		//for (auto pak : {"Shaders.pak", "Include.pak", "Code.pak"})
		//for (auto pak : {"GameData/Data.pak"})
		for (auto pak : {"GameData/Textures.pak"})
		{
			ok &= testPak.OpenPak(pak);
		}
		if (ok)
		{
			testPak.Dump();

			for (auto name : {
			         //"Shaders/fx/AuxGeom.cfx",
			         //"Include/BlackBox/System/ISystem.hpp",
			         //"Include/BlackBox/Renderer/IRender.hpp"
			         //"Include/BlackBox/System/ISystem.hpp"
			         //"Data/minecraft/bbox.obj"
			         "Textures/hud/white_dot.dds"
			     })
			{
				auto              file = testPak.FOpen(name);
				auto              size = file->Size();
				std::vector<char> data;

				data.resize(size);
				file->FRead(&data[0], size, 1, 0);

				printf("file: %*.*s", size, size, &data[0]);
			}
		}
	}
	#endif

	ZipFile::CDREnd t{0};

	auto           ti = (CStructInfo*) &t.TypeInfo();

	if (ti->HasSubVars())
	{
		//cstr name = "";
		//for (int i = 0; (name = ti.EnumElem(i)) != 0; i++)
		//{
		//	printf("%s\n", name);
		//}
	}

	printf("CDREnd str: %s", ti->ToString(&t, FToString().NamedFields(true).SkipDefault(false).Sub(true)).c_str());




	return 0;
}