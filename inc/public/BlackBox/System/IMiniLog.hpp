#pragma once
#include <stdarg.h>

#define PRINTF_PARAMS(...)

////////////////////////////////////////////////////////////////////////////
struct IMiniLog
{
	enum ELogType
	{
		eMessage,
		eWarning,
		eError,
		eAlways,
		eWarningAlways,
		eErrorAlways,
		eInput,			//!< e.g. "e_CaptureFolder ?" or all printouts from history or auto completion.
		eInputResponse, //!< e.g. "Set output folder for video capturing" in response to "e_CaptureFolder ?".
		eComment,
		eAssert
	};

	//! you only have to implement this function
	virtual void LogV(const ELogType nType, const char* szFormat, va_list args) = 0;
	virtual void LogV(const ELogType nType, int flags, const char* szFormat, va_list args) PRINTF_PARAMS(4, 0) = 0;

	//! Logs using type.
	inline void LogWithType(ELogType nType, const char* szFormat, ...) PRINTF_PARAMS(3, 4);
	inline void LogWithType(ELogType nType, int flags, const char* szFormat, ...) PRINTF_PARAMS(4, 5);

	//! destructor
	virtual ~IMiniLog()
	{
	}

	//! this is the simplest log function for messages
	//! with the default implementation
	virtual void Log(const char* szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		LogV(eMessage, szFormat, args);
		va_end(args);
	}

	//! this is the simplest log function for warnings
	//! with the default implementation
	virtual void LogWarning(const char* szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		LogV(eWarning, szFormat, args);
		va_end(args);
	}

	//! this is the simplest log function for errors
	//! with the default implementation
	virtual void LogError(const char* szFormat, ...)
	{
		va_list args;
		va_start(args, szFormat);
		LogV(eError, szFormat, args);
		va_end(args);
	}
};

inline void IMiniLog::LogWithType(ELogType nType, int flags, const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);
	LogV(nType, flags, szFormat, args);
	va_end(args);
}

inline void IMiniLog::LogWithType(ELogType nType, const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);
	LogV(nType, szFormat, args);
	va_end(args);
}

////////////////////////////////////////////////////////////////////////////
// By default, to make it possible not to implement the log at the beginning at all,
// empty implementations of the two functions are given
struct CNullMiniLog : public IMiniLog
{
	// the default implementation just won't do anything
	void LogV(const ELogType nType, const char* szFormat, va_list args){};
};
