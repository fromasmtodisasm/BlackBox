#include "Utils.h"
#include <ShlObj.h>

void BrowseURL(const char* szURL)
{
	// for security reasons, check if it really a url
	if (strlen(szURL) >= 10)
	{
		// check for http and : as in http://
		// : might be on position 5, for https://

		if (!strncmp("http", szURL, 4) && ((szURL[4] == ':') || (szURL[5] == ':')))
		{
#ifdef WIN32

			ShellExecute(0, "open", szURL, 0, 0, SW_MAXIMIZE);

#else

#pragma message("WARNING: CScriptObjectSystem::BrowseURL() not implemented on this platform!")

#endif
		}
	}
}
