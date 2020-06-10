#pragma once

#define CRYSCRIPTSYSTEM_EXPORTS

#include <BlackBox/System/ISystem.hpp>

//! Static branch-prediction helpers
#define IF(condition, hint)    if (condition)
//! Compiler-supported type-checking helper
#define PRINTF_PARAMS(...)

//////////////////////////////////////////////////////////////////////////
//! Reports a Game Warning to validator with WARNING severity.
#ifdef _RELEASE
	#define ScriptWarning(...) ((void)0)
#else
void        ScriptWarning(const char*, ...) PRINTF_PARAMS(1, 2);
inline void ScriptWarning(const char* format, ...)
{
	IF (!format, 0)
		return;

	char buffer[MAX_WARNING_LENGTH];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format, args);
	va_end(args);
	CryWarning(VALIDATOR_MODULE_SCRIPTSYSTEM, VALIDATOR_WARNING, "%s", buffer);
}
#endif