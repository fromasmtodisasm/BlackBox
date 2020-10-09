#pragma once
#include <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/Core/Utils.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>

template<class T, int = 32>
using StackStringT = std::string;

class CLog : 
	public ILog
	, public ISystemEventListener
{
  public:
	typedef StackStringT<char, 256> LogStringType;
	
	CLog::CLog(ISystem* pSystem)
		: m_pSystem(pSystem)
		, m_fLastLoadingUpdateTime(-1.0f)
		, m_logFormat("%Y-%m-%dT%H:%M:%S:fffzzz")
	{
		gEnv->pSystem->GetISystemEventDispatcher()->RegisterListener(this, "CLog");

	}

	~CLog()
	{
		if (output)
		{
			Shutdown();
			fclose(output);
		}
	}

  private:
	void Shutdown()
	{
		for (auto& str : log)
		{
			fputs(str.c_str(), output);
		}
		bool hasConsole = GetISystem()->GetIConsole() != nullptr;
		if (hasConsole && GET_CVAR("stpo_running"))
		{
			fputs("\n\n*****Game stopped*****", output);
		}
		else if (hasConsole && GET_CVAR("window_closed"))
		{
			fputs("\n\n*****Window Closed*****", output);
		}
		else
		{
			fputs("\n\n*****Unknown stop cause*****", output);
		}
	}

	virtual void Release() override;
	virtual void LogV(const ELogType nType, const char* szFormat, va_list args) override;
	virtual void LogToConsole(const char* command, ...) override;
	virtual void SetFileName(const char* command = NULL) override;
	virtual const char* GetFileName() override;
	virtual void LogToFile(const char* command, ...) override;
	virtual void LogV(IMiniLog::ELogType nType, int flags, const char* szFormat, va_list args);
	const char* LogTypeToString(IMiniLog::ELogType type);
	virtual void Log(const char* szCommand, ...) override;
	virtual void LogWarning(const char* szCommand, ...) override;
	virtual void LogError(const char* szCommand, ...) override;
	virtual void LogPlus(const char* command, ...) override;
	virtual void LogToFilePlus(const char* command, ...) override;
	virtual void LogToConsolePlus(const char* command, ...) override;
	virtual void UpdateLoadingScreen(const char* command, ...) override;
	virtual void UpdateLoadingScreenPlus(const char* command, ...) override;
	virtual void EnableVerbosity(bool bEnable) override;
	virtual void SetVerbosity(int verbosity) override;
	virtual int GetVerbosityLevel() override;

	void LogStringToFile(const char* szString, bool bAdd, bool bError = false);
	void LogStringToConsole(const char* szString, bool bAdd = false);

	ISystem*                  m_pSystem;
	float                     m_fLastLoadingUpdateTime; // for non-frequent streamingEngine update
	string                    m_filename;               // Contains only the file name and the extension
	string                    m_filePath;               // Contains the full absolute path to the log file
	string                    m_backupFilePath;         // Contains the full absolute path to the backup log file
	FILE*                     m_pLogFile = nullptr;
	StackStringT<char, 32> m_LogMode;                // mode m_pLogFile has been opened with
	FILE*                     m_pErrFile = nullptr;
	int                       m_nErrCount = 0;
	string                    m_logFormat;              // Time logging format
  private:
	// checks the verbosity of the message and returns NULL if the message must NOT be
	// logged, or the pointer to the part of the message that should be logged
	const char* CheckAgainstVerbosity(const char* pText, bool& logtofile, bool& logtoconsole, const uint8 DefaultVerbosity = 2);

	FILE* output;
	const char* filename = "log.txt";
	bool inited			 = false;
	std::vector<std::string> log;
	char buf[4096] = {0};


	ICVar* m_pLogVerbosity					   = nullptr; //
	ICVar* m_pLogWriteToFile				   = nullptr; //
	ICVar* m_pLogWriteToFileVerbosity		   = nullptr; //
	ICVar* m_pLogVerbosityOverridesWriteToFile = nullptr; //
	ICVar* m_pLogSpamDelay					   = nullptr; //
	ICVar* m_pLogModule						   = nullptr;

	// Inherited via ISystemEventListener
	virtual void OnSystemEvent(ESystemEvent event, UINT_PTR wparam, UINT_PTR lparam) override;
	// Module filter for log
};
