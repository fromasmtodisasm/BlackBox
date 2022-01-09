#include <BlackBox/System/System.hpp>

//////////////////////////////////////////////////////////////////////////
void CSystem::FatalError(const char* format, ...)
{
	// format message
	va_list ArgList;
	char szBuffer[MAX_WARNING_LENGTH];
	const char* sPrefix = "";
	strcpy(szBuffer, sPrefix);
	va_start(ArgList, format);
	vsprintf(szBuffer + strlen(szBuffer)/*, sizeof(szBuffer) - strlen(szBuffer)*/, format, ArgList);
	va_end(ArgList);

	#
	// get system error message before any attempt to write into log
	const char* szSysErrorMessage = CryGetLastSystemErrorMessage();

	CryLogAlways("=============================================================================");
	CryLogAlways("*ERROR");
	CryLogAlways("=============================================================================");
	// write both messages into log
	CryLogAlways("%s", szBuffer);

	if (szSysErrorMessage)
		CryLogAlways("<System> Last System Error: %s", szSysErrorMessage);

	assert(szBuffer[0] >= ' ');
	//strcpy(szBuffer,szBuffer+1);	// remove verbosity tag since it is not supported by ::MessageBox

	//LogSystemInfo();

	//CollectMemStats(0, nMSP_ForCrashLog);

	OutputDebugString(szBuffer);
	//OnFatalError(szBuffer);

	if (!g_cvars.sys_no_crash_dialog)
	{
		CryMessageBox(szBuffer,"ENGINE FATAL ERROR", eMB_Error);
	}

	//GetITextModeConsole()->OnShutdown();
	DebugBreak();
	//__debugbreak();

}


