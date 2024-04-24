#include "pch.hpp"
#include <BlackBox/Core/Platform/platform_impl.inl>

#if BB_PLATFORM_WINDOWS
	#include <BlackBox/Core/Platform/Windows.hpp>
	#ifndef _LIB
HANDLE        gDLLHandle = NULL;
BOOL CALLBACK DllMain(HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved)
{
	gDLLHandle = hModule;
	return TRUE;
}

	#endif
#endif
