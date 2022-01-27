#include <BlackBox/Core/Platform/platform_impl.inl>

#ifndef	_XBOX
#if !defined(LINUX)
#endif//LINUX
#endif //_XBOX

//////////////////////////////////////////////////////////////////////////
#if !defined(_LIB) && !defined(PS2) && !defined(_XBOX) && !defined(LINUX)
#include <BlackBox/Core/Platform/Windows.hpp>
// DLL-EntryPoint
BOOL CALLBACK DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
#ifdef USE_MEM_POOL
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if(!CHECK_POOL())
			return FALSE;
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
#endif
    return TRUE;
}
#endif
 
//#include <CrtDebugStats.h>
