#include <BlackBox/System/NullLog.hpp>

void CLog::Log(const char* szFormat, ...)
{
	va_list arg;
	va_start(arg, szFormat);
	LogV(eMessage, szFormat, arg);
	va_end(arg);
}


//////////////////////////////////////////////////////////////////////
static void RemoveColorCodeInPlace(CLog::LogStringType& rStr)
{
	char* s = (char*)rStr.c_str();
	char* d = s;

	while (*s != 0)
	{
		if (*s == '$' && *(s + 1) >= '0' && *(s + 1) <= '9')
		{
			s += 2;
			continue;
		}

		*d++ = *s++;
	}
	*d = 0;

	rStr.resize(d - rStr.c_str());
}
CLog::CLog(ISystem* pSystem)
	: m_pSystem(pSystem)
	, m_fLastLoadingUpdateTime(-1.0f)
	, m_logFormat("%Y-%m-%dT%H:%M:%S:fffzzz")
{
	gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CLog");

	m_nMainThreadId = std::this_thread::get_id();

	IConsole* console = m_pSystem->GetIConsole();

#ifdef  _RELEASE
	#if defined(RELEASE_LOGGING)
		#define DEFAULT_VERBOSITY 0
	#elif defined(DEDICATED_SERVER)
		#define DEFAULT_VERBOSITY 0
	#else
		#define DEFAULT_VERBOSITY -1
	#endif
#else
	#define DEFAULT_VERBOSITY 3
#endif

#if defined(DEDICATED_SERVER) && defined(BB_PLATFORM_LINUX)
	#define DEFAULT_LOG_INCLUDE_TIME (0)
#else
	#define DEFAULT_LOG_INCLUDE_TIME (1)
#endif

	if (console)
	{
		m_pLogVerbosity = REGISTER_INT("log_Verbosity", DEFAULT_VERBOSITY, VF_DUMPTODISK,
									   "defines the verbosity level for log messages written to console\n"
									   "-1=suppress all logs (including eAlways)\n"
									   "0=suppress all logs(except eAlways)\n"
									   "1=additional errors\n"
									   "2=additional warnings\n"
									   "3=additional messages\n"
									   "4=additional comments");

		//writing to game.log during game play causes stalls on consoles
		m_pLogWriteToFile = REGISTER_INT("log_WriteToFile", 1, VF_DUMPTODISK, "toggle whether to write log to file (game.log)");

		// put time into begin of the string if requested by cvar
		m_pLogIncludeTime = REGISTER_INT("log_IncludeTime", 1, 0,
										 "Toggles time stamping of log entries.\n"
										 "Usage: log_IncludeTime [0/1/2/3/4/5]\n"
										 "  0=off (default)\n"
										 "  1=current time\n"
										 "  2=relative time\n"
										 "  3=current+relative time\n"
										 "  4=absolute time in seconds since this mode was started\n"
										 "  5=current time+server time");
	}
}

CLog::~CLog()
{
	Shutdown();
}


//////////////////////////////////////////////////////////////////////////
void CLog::CloseLogFile(bool forceClose)
{
	#if 0
	if (m_pLogFile)
	{
		LockNoneExclusiveAccess(&m_exclusiveLogFileThreadAccessLock);
		fclose(m_pLogFile);
		m_pLogFile = NULL;
		UnlockNoneExclusiveAccess(&m_exclusiveLogFileThreadAccessLock);
	}
	#else
	fclose(m_pLogFile);
	#endif
}

//////////////////////////////////////////////////////////////////////////
FILE* CLog::OpenLogFile(const char* filename, const char* mode)
{
	#if 0
	SCOPED_ALLOW_FILE_ACCESS_FROM_THIS_THREAD();

#if CRY_PLATFORM_IOS
	char buffer[1024];
	cry_strcpy(buffer, "");
	if (AppleGetUserLibraryDirectory(buffer, sizeof(buffer)))
	{
		cry_strcat(buffer, "/");
		cry_strcat(buffer, filename);
		LockNoneExclusiveAccess(&m_exclusiveLogFileThreadAccessLock);
		m_pLogFile = fxopen(buffer, mode);
		UnlockNoneExclusiveAccess(&m_exclusiveLogFileThreadAccessLock);
	}
	else
	{
		m_pLogFile = NULL;
	}
#else
	LockNoneExclusiveAccess(&m_exclusiveLogFileThreadAccessLock);
	m_pLogFile = fxopen(filename, mode);
	UnlockNoneExclusiveAccess(&m_exclusiveLogFileThreadAccessLock);
#endif

	if (m_pLogFile)
	{
#if KEEP_LOG_FILE_OPEN
		m_bFirstLine = true;
		setvbuf(m_pLogFile, m_logBuffer, _IOFBF, sizeof(m_logBuffer));
#endif
	}
	else
	{
#if CRY_PLATFORM_LINUX || CRY_PLATFORM_ANDROID || CRY_PLATFORM_APPLE
		syslog(LOG_NOTICE, "Failed to open log file [%s], mode [%s]", filename, mode);
#endif
	}
	#else
	m_pLogFile = fopen(filename, mode);
	#endif

	return m_pLogFile;
}


void CLog::Release()
{
	delete this;
}

void CLog::LogV(const ELogType nType, const char* szFormat, va_list args)
{
	LogV(nType, 0, szFormat, args);
}

void CLog::LogToConsole(const char* szFormat, ...)
{
	if (m_pLogVerbosity && m_pLogVerbosity->GetIVal() < 0)
	{
		return;
	}

	if (!szFormat)
	{
		return;
	}

	bool bfile = false, bconsole = false;
	const char* szszFormat = CheckAgainstVerbosity(szFormat, bfile, bconsole);
	if (!bconsole)
	{
		return;
	}

	char temp[256];
	va_list arglist;
	va_start(arglist, szFormat);
	vsprintf(temp, szszFormat, arglist);
	va_end(arglist);

	LogStringToConsole(temp);
}

void CLog::SetFileName(const char* filename)
{
	string temp(filename);
	if (temp.empty() || temp.size() >= sizeof(m_szFilename))
		return ;

	strcpy(m_szFilename, temp.c_str());

	//CreateBackupFile();

	FILE* fp = OpenLogFile(m_szFilename, "wt");
	if (fp)
	{
		CloseLogFile(true);
	}
}

const char* CLog::GetFileName()
{
	return m_szFilename;
}

void CLog::LogToFile(const char* szFormat, ...)
{
	if (m_pLogVerbosity && m_pLogVerbosity->GetIVal() < 0)
	{
		return;
	}

	if (m_filePath.empty() || !szFormat)
	{
		return;
	}

	bool bfile = false, bconsole = false;
	const char* szszFormat = CheckAgainstVerbosity(szFormat, bfile, bconsole);
	if (!bfile)
		return;

	char temp[256];
	va_list arglist;
	va_start(arglist, szFormat);
	vsprintf(temp, szszFormat, arglist);
	va_end(arglist);

	LogStringToFile(temp, false);
}

void CLog::LogV(IMiniLog::ELogType nType, int flags, const char* szFormat, va_list args)
{
	if (!szFormat)
		return;

	if (m_pLogVerbosityOverridesWriteToFile && m_pLogVerbosityOverridesWriteToFile->GetIVal())
	{
		if (m_pLogVerbosity && m_pLogVerbosity->GetIVal() < 0)
		{
			return;
		}
	}

	FUNCTION_PROFILER(GetISystem(), PROFILE_SYSTEM);
	//LOADING_TIME_PROFILE_SECTION(GetISystem());

	bool bfile = false, bconsole = false;
	const char* szCommand = szFormat;

	uint8 DefaultVerbosity = 0; // 0 == Always log (except for special -1 verbosity overrides)

	switch (nType)
	{
	case eAlways:
	case eWarningAlways:
	case eErrorAlways:
	case eInput:
	case eInputResponse:
		DefaultVerbosity = 0;
		break;
	case eError:
	case eAssert:
		DefaultVerbosity = 1;
		break;
	case eWarning:
		DefaultVerbosity = 2;
		break;
	case eMessage:
		DefaultVerbosity = 3;
		break;
	case eComment:
		DefaultVerbosity = 4;
		break;
	default:
		CryFatalError("Unsupported ELogType type");
		break;
	}

	szCommand = CheckAgainstVerbosity(szFormat, bfile, bconsole, DefaultVerbosity);
	if (!bfile && !bconsole)
	{
		return;
	}

	bool bError = false;

	const char* szPrefix = nullptr;
	switch (nType)
	{
	case eWarning:
	case eWarningAlways:
		bError = true;
		szPrefix = "$6[Warning] ";
		break;

	case eError:
	case eErrorAlways:
		bError = true;
		szPrefix = "$4[Error] ";
		break;
	case eAssert:
		bError = true;
		szPrefix = "$4[Assert] ";
	case eMessage:
	case eAlways:
	case eInput:
	case eInputResponse:
	case eComment:
		// Do not modify string
		break;
	default:
		CryFatalError("Unsupported ELogType type");
		break;
	}

	//char temp[512];
	vsprintf(buf, szFormat, args);
	auto formatted = string(buf);

	if (szPrefix)
	{
		formatted.insert(0, szPrefix);
	}

	if (bfile)
	{
		const char* szAfterColor = szPrefix ? formatted.c_str() + 2 : formatted.c_str();
		LogStringToFile(szAfterColor, false, bError);
	}
	if (bconsole)
	{
#ifdef __WITH_PB__
		// Send the console output to PB for audit purposes
		if (gEnv->pNetwork)
			gEnv->pNetwork->PbCaptureConsoleLog(szBuffer, strlen(szBuffer));
#endif
		LogStringToConsole(formatted.c_str());
	}

	switch (nType)
	{
	case eAlways:
	case eInput:
	case eInputResponse:
	case eComment:
	case eMessage:
		GetISystem()->GetIRemoteConsole()->AddLogMessage(formatted.c_str());
		break;
	case eWarning:
	case eWarningAlways:
		GetISystem()->GetIRemoteConsole()->AddLogWarning(formatted.c_str());
		break;
	case eError:
	case eErrorAlways:
	case eAssert:
		GetISystem()->GetIRemoteConsole()->AddLogError(formatted.c_str());
		break;
	default:
		CryFatalError("Unsupported ELogType type");
		break;
	}
	//////////////////////////////////////////////////////////////////////////
	if (nType == eWarningAlways || nType == eWarning || nType == eError || nType == eErrorAlways)
	{
		IValidator* pValidator = m_pSystem->GetIValidator();
		if (pValidator && (flags & VALIDATOR_FLAG_SKIP_VALIDATOR) == 0)
		{
			std::lock_guard scope_lock(m_logCriticalSection);

			SValidatorRecord record;
			record.text = formatted.c_str();
			record.module = VALIDATOR_MODULE_SYSTEM;
			record.severity = VALIDATOR_WARNING;
			//record.assetScope = GetAssetScopeString();
			record.flags = flags;
			if (nType == eError || nType == eErrorAlways)
			{
				record.severity = VALIDATOR_ERROR;
			}
			pValidator->Report(record);
		}
	}
}

const char* CLog::LogTypeToString(IMiniLog::ELogType type)
{
	switch (type)
	{
	case IMiniLog::eMessage:
		return "";
		break;
	case IMiniLog::eWarning:
		return "[$6Warning$1]";
		break;
	case IMiniLog::eError:
		return "$4[Error]";
		break;
	case IMiniLog::eAlways:
		return "";
		break;
	case IMiniLog::eWarningAlways:
		return "";
		break;
	case IMiniLog::eErrorAlways:
		return "[$4Error$1]";
		break;
	case IMiniLog::eInput:
		return "Input";
		break;
	default:
		return "";
	}
}

void CLog::LogWarning(const char* szFormat, ...)
{
	va_list ArgList;
	va_start(ArgList, szFormat);
	LogV(eWarning, szFormat, ArgList);
	va_end(ArgList);
}

void CLog::LogError(const char* szFormat, ...)
{
	va_list ArgList;
	va_start(ArgList, szFormat);
	LogV(eError, szFormat, ArgList);
	va_end(ArgList);
}

void CLog::LogPlus(const char* szFormat, ...)
{
	if (m_pLogVerbosity && m_pLogVerbosity->GetIVal() < 0)
	{
		return;
	}

	if (m_pLogSpamDelay && m_pLogSpamDelay->GetFVal())
	{
		// Vlad: SpamDelay does not work correctly with LogPlus
		return;
	}

	//CRY_PROFILE_FUNCTION(PROFILE_LOADING_ONLY);

	if (!szFormat)
		return;

	bool bfile = false, bconsole = false;
	const char* szCommand = CheckAgainstVerbosity(szFormat, bfile, bconsole);
	if (!bfile && !bconsole)
		return;

	char temp[256];
	va_list arglist;
	va_start(arglist, szFormat);
	vsprintf(temp, szFormat, arglist);
	va_end(arglist);

	if (bfile)
	{
		LogStringToFile(temp, true);
	}
	if (bconsole)
	{
		LogStringToConsole(temp, true);
	}
}



void CLog::LogToFilePlus(const char* szFormat, ...)
{
}

void CLog::LogToConsolePlus(const char* szFormat, ...)
{
	if (m_pLogVerbosity && m_pLogVerbosity->GetIVal() < 0)
	{
		return;
	}

	if (!szFormat)
	{
		return;
	}

	bool bfile = false, bconsole = false;
	const char* szszFormat = CheckAgainstVerbosity(szFormat, bfile, bconsole);
	if (!bconsole)
	{
		return;
	}

	if (!m_pSystem)
	{
		return;
	}

	char temp[256];
	va_list arglist;
	va_start(arglist, szFormat);
	vsprintf(temp, szszFormat, arglist);
	va_end(arglist);

	LogStringToConsole(temp, true);
}

void CLog::UpdateLoadingScreen(const char* szFormat, ...)
{
	if (szFormat)
	{
		va_list args;
		va_start(args, szFormat);

		LogV(ILog::eMessage, szFormat, args);

		va_end(args);
	}
}

void CLog::UpdateLoadingScreenPlus(const char* szFormat, ...)
{
}

void CLog::EnableVerbosity(bool bEnable)
{
}

void CLog::SetVerbosity(int verbosity)
{
	if (m_pLogVerbosity)
		m_pLogVerbosity->Set(verbosity);
}

int CLog::GetVerbosityLevel()
{
	if (m_pLogVerbosity)
	{
		return (m_pLogVerbosity->GetIVal());
	}

	return 0;
}

void CLog::LogStringToFile(const char* szString, bool bAdd, bool bError)
{
	#if defined(_RELEASE) && defined(EXCLUDE_NORMAL_LOG) // no file logging in release
	return;
	#endif

	if (!szString)
	{
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	if (std::this_thread::get_id() != m_nMainThreadId)
	{
		// When logging from other thread then main, push all log strings to queue.
		SLogMsg msg;
		msg.msg = szString;
		msg.bAdd = bAdd;
		msg.bError = bError;
		msg.bConsole = false;
		// don't try to store the log message for later in case of out of memory, since then its very likely that this allocation
		// also fails and results in a stack overflow. This way we should at least get a out of memory on-screen message instead of
		// a not obvious crash
		#if 0
		if (gEnv->bIsOutOfMemory == false)
		{
			m_threadSafeMsgQueue.push(msg);
		}
		#else
		m_threadSafeMsgQueue.push(msg);
		#endif
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	if (!m_pSystem)
	{
		return;
	}

	LogStringType tempString;
	tempString = szString;

	// Skip any non character.
	if (tempString.length() > 0 && tempString.at(0) < 32)
	{
		tempString.erase(0, 1);
	}

	RemoveColorCodeInPlace(tempString);

	#if defined(SUPPORT_LOG_IDENTER)
	if (m_topIndenter)
	{
		m_topIndenter->DisplaySectionText();
	}

	tempString = m_indentWithString + tempString;
	#endif

	if (m_pLogIncludeTime && gEnv->pTimer)
	{
		uint32 dwCVarState = m_pLogIncludeTime->GetIVal();
		char sTime[21];
		if (dwCVarState == 5) // Log_IncludeTime
		{
			dwCVarState = 1; // Afterwards insert time as-if Log_IncludeTime == 1
		}
		if (dwCVarState < 4)
		{
			if (dwCVarState & 1) // Log_IncludeTime
			{
				time_t ltime;
				time(&ltime);
				struct tm* today = localtime(&ltime);
				strftime(sTime, ARRAY_COUNT(sTime), "<%H:%M:%S> ", today);
				sTime[ARRAY_COUNT(sTime) - 1] = 0;
				tempString.insert(0, sTime);
			}
			if (dwCVarState & 2) // Log_IncludeTime
			{
				static CTimeValue lasttime;
				const CTimeValue currenttime = gEnv->pTimer->GetAsyncTime();
				if (lasttime != CTimeValue())
				{
					const uint32 dwMs = (uint32)((currenttime - lasttime).GetMilliSeconds());
					sprintf(sTime, "<%3u.%.3u>: ", dwMs / 1000, dwMs % 1000);
					tempString.insert(0, sTime);
				}
				lasttime = currenttime;
			}
		}
		else if (dwCVarState == 4) // Log_IncludeTime
		{
			static bool bFirst = true;
			static CTimeValue lasttime;
			const CTimeValue currenttime = gEnv->pTimer->GetAsyncTime();
			if (lasttime != CTimeValue())
			{
				const uint32 dwMs = (uint32)((currenttime - lasttime).GetMilliSeconds());
				sprintf(sTime, "<%3u.%.3u>: ", dwMs / 1000, dwMs % 1000);
				tempString.insert(0, sTime);
			}
			if (bFirst)
			{
				lasttime = currenttime;
				bFirst = false;
			}
		}
	}

	#if !KEEP_LOG_FILE_OPEN
	// add \n at end.
	if (tempString.empty() || tempString[tempString.length() - 1] != '\n')
	{
		tempString += '\n';
	}
	#endif

	#if 0
	//////////////////////////////////////////////////////////////////////////
	// Call callback function (on invoke if we are not in application crash)
	if (!m_callbacks.empty() && m_eLogMode != eLogMode_AppCrash)
	{
		for (Callbacks::iterator it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
		{
			(*it)->OnWriteToFile(tempString.c_str(), !bAdd);
		}
	}
	#endif
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Write to file.
	//////////////////////////////////////////////////////////////////////////
	int logToFile = m_pLogWriteToFile ? m_pLogWriteToFile->GetIVal() : 1;

	if (logToFile)
	{
		//SCOPED_ALLOW_FILE_ACCESS_FROM_THIS_THREAD();

	#if KEEP_LOG_FILE_OPEN
		if (!m_pLogFile)
		{
			OpenLogFile(m_szFilename, "at");
		}

		if (m_pLogFile)
		{
			if (m_bFirstLine)
			{
				m_bFirstLine = false;
			}
			else
			{
				if (!bAdd)
				{
					tempString = "\n" + tempString;
				}
			}

			fputs(tempString.c_str(), m_pLogFile);
		}
	#else
		if (bAdd)
		{
			FILE* fp = OpenLogFile(m_szFilename, "r+t");
			if (fp)
			{
				int nRes;
				nRes = fseek(fp, 0, SEEK_END);
				assert(nRes == 0);
				nRes = fseek(fp, -2, SEEK_CUR);
				assert(nRes == 0);
				(void)nRes;
				fputs(tempString.c_str(), fp);
				CloseLogFile();
			}
		}
		else
		{
			// comment on bug by TN: Log file misses the last lines of output
			// Temporally forcing the Log to flush before closing the file, so all lines will show up
			if (FILE* fp = OpenLogFile(m_szFilename, "at")) // change to option "atc"
			{
				fputs(tempString.c_str(), fp);
				// fflush(fp);  // enable to flush the file
				CloseLogFile();
			}
		}
	#endif
	}

	#if !defined(_RELEASE) && !defined(DISABLE_OUTPUTDEBUGSTRING)
	// Note: OutputDebugString(A) only accepts current ANSI code-page, and the W variant will call the A variant internally.
	// Here we replace non-ASCII characters with '?', which is the same as OutputDebugStringW will do for non-ANSI.
	// Thus, we discard slightly more characters (ie, those inside the current ANSI code-page, but outside ASCII).
	// In exchange, we save double-converting that would have happened otherwise (UTF-8 -> UTF-16 -> ANSI).
	#if 0
	string asciiString;
	Unicode::Convert<Unicode::eEncoding_ASCII, Unicode::eEncoding_UTF8>(asciiString, tempString);
	OutputDebugString(asciiString.c_str());
	#else
	OutputDebugString(tempString.c_str());
	#endif
	#endif
}

//log to console only
//////////////////////////////////////////////////////////////////////
void CLog::LogStringToConsole(const char* szString, bool bAdd)
{
	#if defined(_RELEASE) && defined(EXCLUDE_NORMAL_LOG) // no console logging in release
	return;
	#endif

	#if 0
	//////////////////////////////////////////////////////////////////////////
	if (CryGetCurrentThreadId() != m_nMainThreadId)
	{
		// When logging from other thread then main, push all log strings to queue.
		SLogMsg msg;
		msg.msg = szString;
		msg.bAdd = bAdd;
		msg.bError = false;
		msg.bConsole = true;
		// don't try to store the log message for later in case of out of memory, since then its very likely that this allocation
		// also fails and results in a stack overflow. This way we should at least get a out of memory on-screen message instead of
		// a not obvious crash
		if (gEnv->bIsOutOfMemory == false)
		{
			m_threadSafeMsgQueue.push(msg);
		}
		return;
	}
	#endif
	//////////////////////////////////////////////////////////////////////////

	if (!m_pSystem || !szString)
	{
		return;
	}

	IConsole* console = m_pSystem->GetIConsole();
	if (!console)
		return;

	LogStringType tempString;
	tempString = szString;

	// add \n at end.
	if (tempString.length() > 0 && tempString[tempString.length() - 1] != '\n')
	{
		tempString += '\n';
	}

	if (bAdd)
	{
		console->PrintLinePlus(tempString.c_str());
	}
	else
	{
		console->PrintLine(tempString.c_str());
	}

	#if 0
	// Call callback function.
	if (!m_callbacks.empty())
	{
		for (Callbacks::iterator it = m_callbacks.begin(); it != m_callbacks.end(); ++it)
		{
			(*it)->OnWriteToConsole(tempString.c_str(), !bAdd);
		}
	}
	#endif
}


//////////////////////////////////////////////////////////////////////////////////////////////////
// Checks the verbosity of the message and returns NULL if the message must NOT be
// logged, or the pointer to the part of the message that should be logged
// NOTE:
//    Normally, this is either the pText pointer itself, or the pText+1, meaning
//    the first verbosity character may be cut off)
//    This is done in order to avoid modification of const char*, which may cause GPF
//    sometimes, or kill the verbosity qualifier in the text that's gonna be passed next time.
const char* CLog::CheckAgainstVerbosity(const char* pText, bool& logtofile, bool& logtoconsole, const uint8 DefaultVerbosity)
{
	// the max verbosity (most detailed level)
#if defined(RELEASE)
	const unsigned char nMaxVerbosity = 0;
#else // #if defined(RELEASE)
	const unsigned char nMaxVerbosity = 8;
#endif // #if defined(RELEASE)

	// the current verbosity of the log
	int nLogVerbosityConsole = m_pLogVerbosity ? m_pLogVerbosity->GetIVal() : nMaxVerbosity;
	int nLogVerbosityFile = m_pLogWriteToFileVerbosity ? m_pLogWriteToFileVerbosity->GetIVal() : nMaxVerbosity;

	logtoconsole = (nLogVerbosityConsole >= DefaultVerbosity);

	//to preserve logging to TTY, logWriteToFile logic has been moved to inside logStringToFile
	//int logToFileCVar = m_pLogWriteToFile ? m_pLogWriteToFile->GetIVal() : 1;

	logtofile = (nLogVerbosityFile >= DefaultVerbosity);

	return pText;
}

void CLog::OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam)
{
}

#if KEEP_LOG_FILE_OPEN
void CLog::LogFlushFile(IConsoleCmdArgs* pArgs)
{
	//gEnv->pLog->Flush();
}
#endif
