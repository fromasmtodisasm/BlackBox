#pragma once

constexpr bool IsCVarWhitelisted(const char* szName)
{
#if defined(CVARS_WHITELIST) && !defined(USE_RUNTIME_CVAR_OVERRIDES)
	#if defined(CVAR_WHITELIST_ENTRIES)
	return detail::IsCVarWhitelistedImpl(CVAR_WHITELIST_ENTRIES, CRY_ARRAY_COUNT(CVAR_WHITELIST_ENTRIES), CCrc32::ComputeLowercase_CompileTime(szName), 0);
	#else
	return false;
	#endif // defined(CVAR_WHITELIST_ENTRIES)
#else
	return true;
#endif // defined(CVARS_WHITELIST)
}
