#undef UNICODE
#include "ResourceCompiler.h"



#include <string>
#include <io.h>
#include <Windows.h>


using std::string;
using std::wstring;

#define RCLog(f, ...) printf(f "\n", __VA_ARGS__)
#define RCLogError RCLog

#define GetExePath()  "./"

//////////////////////////////////////////////////////////////////////////
bool ResourceCompiler::RegisterConverters()
{
	const string strDir  = GetExePath();
	const string strMask = strDir + "ResourceCompiler*.dll";

	RCLog("Searching for %s", strMask.c_str());

	string widePath = strMask, wstrDir = strDir;
	//Unicode::Convert(widePath, strMask.c_str());
	//Unicode::Convert(wstrDir, strDir.c_str());

	_finddata_t fd;
	intptr_t       hSearch = _findfirst(widePath.data(), &fd);
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

int main()
{
	ResourceCompiler RC;

	RC.RegisterConverters();
	
	return 0;
}