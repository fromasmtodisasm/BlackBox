#include <BlackBox/System/NullLog.hpp>

void CLog::Log(const char* szFormat, ...)
{
	va_list arg;
	va_start(arg, szFormat);
	LogV(eMessage, szFormat, arg);
	va_end(arg);
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

void CLog::SetFileName(const char* szFormat)
{
	m_filename = szFormat;
}

const char* CLog::GetFileName()
{
	return m_filename.data();
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
	auto len = sprintf(buf, "%s ", LogTypeToString(nType));
	len += vsprintf(buf + len, szFormat, args);
	buf[len] = '\0';
	if (gEnv->pConsole)
		gEnv->pConsole->PrintLine(buf);
	//else

	auto formatted = string(buf);
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

	log.push_back(strdup(buf));
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
