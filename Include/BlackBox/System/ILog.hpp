#pragma once
#include "IMiniLog.hpp"

struct ILog : public IMiniLog
{
	virtual void Release() = 0;

	//set the file used to log to disk
	virtual void SetFileName(const char* command = NULL) = 0;
	//
	virtual const char* GetFileName() = 0;

	//will log the text both to file and console
	virtual void Log(const char* szCommand, ...) = 0;

	virtual void LogWarning(const char* szCommand, ...) = 0;

	virtual void LogError(const char* szCommand, ...) = 0;

	//will log the text both to the end of file and console
	virtual void LogPlus(const char* command, ...) = 0;

	//log to the file specified in setfilename
	virtual void LogToFile(const char* command, ...) = 0;

	//
	virtual void LogToFilePlus(const char* command, ...) = 0;

	//log to console only
	virtual void LogToConsole(const char* command, ...) = 0;

	//
	virtual void LogToConsolePlus(const char* command, ...) = 0;

	//
	virtual void UpdateLoadingScreen(const char* command, ...) = 0;

	//
	virtual void UpdateLoadingScreenPlus(const char* command, ...) = 0;

	//
	virtual void EnableVerbosity(bool bEnable) = 0;

	//
	virtual void SetVerbosity(int verbosity) = 0;

	virtual int GetVerbosityLevel() = 0;
};
