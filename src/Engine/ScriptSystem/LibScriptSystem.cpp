#include <BlackBox/Core/Platform/platform_impl.inl>

#if BB_PLATFORM_WINDOWS
#include <BlackBox/Core/Platform/Windows.hpp>
	#ifndef _LIB
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	return TRUE;
}
	#endif
#endif


