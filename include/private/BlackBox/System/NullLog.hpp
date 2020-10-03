#pragma once
#include <BlackBox/Core/Utils.hpp>
#include <BlackBox/System/IConsole.hpp>
#include <BlackBox/System/ILog.hpp>
#include <BlackBox/System/ISystem.hpp>
#include <cstdarg>
#include <cstdio>
#include <string>
#include <vector>

class CLog : public ILog
{
  public:
	CLog(const char* filename)
		: filename(filename)
	{
		output = fopen(filename, "w");
		ZeroMemory(buf, sizeof(buf));
	}
	~CLog()
	{
		if (output)
		{
			Shutdown();
			fclose(output);
		}
	}

	// Inherited via ILog
	virtual void Clear() override
	{
		log.clear();
	}
	virtual void Draw(const char* title, bool* p_open) override
	{
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

  private:
	FILE* output;
	const char* filename = "log.txt";
	bool inited			 = false;
	std::vector<std::string> log;
	char buf[4096] = {0};

	// Inherited via ILog
	virtual void Release() override
	{
		delete this;
	}

	// Inherited via ILog
	virtual void LogV(const ELogType nType, const char* szFormat, va_list args) override
	{
		LogV(nType, 0, szFormat, args);
	}

	// Inherited via ILog
	virtual void LogToConsole(const char* command, ...) override
	{
	}

	// Inherited via ILog
	virtual void SetFileName(const char* command = NULL) override
	{
	}
	virtual const char* GetFileName() override
	{
		return NULL;
	}

	// Inherited via ILog
	virtual void LogToFile(const char* command, ...) override
	{
	}

	// Inherited via ILog
	virtual void LogV(IMiniLog::ELogType nType, int flags, const char* szFormat, va_list args)
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
		{
			buf[len]	 = '\n';
			buf[len + 1] = '\0';
			std::cout << buf;
		}
		log.push_back(strdup(buf));
	}

	inline const char* LogTypeToString(IMiniLog::ELogType type)
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
};
