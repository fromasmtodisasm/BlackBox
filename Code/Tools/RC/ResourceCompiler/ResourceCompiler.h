#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include <io.h>
#define NOMINMAX
#include <Windows.h>

#include "FileMapping.h"

using byte = std::uint8_t;
using std::string;
using std::wstring;
using std::string_view;

namespace fs = std::filesystem;


struct IResourceCompiler
{
};

#define RCLog(f, ...) printf(f "\n", __VA_ARGS__)
#define RCLogError    RCLog

#define GetExePath()  "./"

// this is the plugin function that's exported by plugins
// Registers all converters residing in this DLL
extern "C"
{
	typedef void(__stdcall* FnRegisterConverters)(IResourceCompiler* pRC);
}

class ResourceCompiler : public IResourceCompiler
{
public:
	ResourceCompiler() = default;
	//////////////////////////////////////////////////////////////////////////
	bool RegisterConverters()
	{
		using namespace std;
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
};

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
extern SOptions g_Options;


