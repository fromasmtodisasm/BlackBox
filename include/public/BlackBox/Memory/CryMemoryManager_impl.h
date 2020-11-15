#ifndef __CryMemoryManager_impl_h__
#define __CryMemoryManager_impl_h__
#pragma once

#ifdef _LIB
	#include <CrySystem/ISystem.h> // <> required for Interfuscator
#endif

#include <BlackBox/Core/Platform/CryLibrary.h>
#include <BlackBox/Core/Platform/Windows.hpp>

#define DLL_ENTRY_CRYMALLOC     "CryMalloc"
#define DLL_ENTRY_CRYFREE       "CryFree"
#define DLL_ENTRY_CRYREALLOC    "CryRealloc"
#define DLL_ENTRY_CRYGETMEMSIZE "CryGetMemSize"
#define DLL_ENTRY_CRYCRTMALLOC  "CrySystemCrtMalloc"
#define DLL_ENTRY_CRYCRTFREE    "CrySystemCrtFree"
#define DLL_ENTRY_CRYCRTSIZE    "CrySystemCrtSize"
#define DLL_ENTRY_GETMEMMANAGER "CryGetIMemoryManagerInterface"

//////////////////////////////////////////////////////////////////////////
// _PoolHelper definition.
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
struct _CryMemoryManagerPoolHelper
{
	uint64 allocatedMemory;
	uint64 freedMemory;
	int numAllocations;
	typedef void *(*FNC_CryMalloc)(size_t size);
	typedef void *(*FNC_CryRealloc)(void *memblock,size_t size);
	typedef void (*FNC_CryFree)(void *p);
	typedef int (*FNC_CryStats)(char *buf);
	FNC_CryMalloc _CryMalloc;
	FNC_CryRealloc _CryRealloc;
	FNC_CryFree _CryFree;

	explicit _CryMemoryManagerPoolHelper( void *pHandle = NULL )
	{
		allocatedMemory = 0;
		freedMemory = 0;
		numAllocations = 0;
		// On first iteration check ourself
		HMODULE hMod = CryGetCurrentModule();
		for (int i = 0; i < 2; i++)
		{
			if (hMod)
			{
				_CryMalloc = (FNC_CryMalloc)CryGetProcAddress(hMod, DLL_ENTRY_CRYMALLOC);
				_CryRealloc = (FNC_CryRealloc)CryGetProcAddress(hMod, DLL_ENTRY_CRYREALLOC);
				_CryFree = (FNC_CryFree)CryGetProcAddress(hMod, DLL_ENTRY_CRYFREE);

				if ((_CryMalloc && _CryRealloc && _CryFree))
					break;
			}

			hMod = CryLoadLibraryDefName("System");
		}

		if (!hMod || !_CryMalloc || !_CryRealloc || !_CryFree )
		{
			char errMsg[10240];
			if (hMod)
			{
				sprintf(errMsg, "%s", "Memory Manager: Unable to bind memory management functions.");
			}
			else
			{
#ifdef CRY_PLATFORM_LINUX
				sprintf(errMsg, "%s\nError details: %s", "Memory Manager: Unable to bind memory management functions. Could not access " CryLibraryDefName("CrySystem")" (check working directory)", dlerror());
#else
				sprintf(errMsg, "%s", "Memory Manager: Unable to bind memory management functions. Could not access " CryLibraryDefName("CrySystem")" (check working directory)");
#endif
			}

			//CryMessageBox(errMsg, "Memory Manager", eMB_Error);
			__debugbreak();
			abort();
		}
	}
	~_CryMemoryManagerPoolHelper()
	{
#if defined(LINUX) && defined(LKJDF)
		if (hSystem)
			::dlclose( hSystem );
#endif
	}
	void GetMemoryInfo( CryModuleMemoryInfo *pMmemInfo )
	{
		pMmemInfo->allocated = allocatedMemory;
		pMmemInfo->freed = freedMemory;
		pMmemInfo->num_allocations = numAllocations;
	}
	//////////////////////////////////////////////////////////////////////////
	// Local version of allocations, does memory counting per module.
	//////////////////////////////////////////////////////////////////////////
	__forceinline void*	Malloc(size_t size)
	{
		allocatedMemory += size;
		numAllocations++;
		return _CryMalloc( size );
	}
	//////////////////////////////////////////////////////////////////////////
	__forceinline void*	Realloc(void *memblock,size_t size)
	{
		if (memblock == NULL)
		{
			allocatedMemory += size;
			numAllocations++;
		}
		else
		{
			numAllocations++;
			size_t oldsize = ((int*)memblock)[-1];
			allocatedMemory += size;
			freedMemory += oldsize;
		}
		return _CryRealloc( memblock,size );
	}
	//////////////////////////////////////////////////////////////////////////
	__forceinline void Free( void *memblock )
	{
		if (memblock != 0)
		{
			size_t size = ((int*)memblock)[-1];
			freedMemory += size;
		}
		_CryFree( memblock );
	}
	//////////////////////////////////////////////////////////////////////////
};
#endif //__cplusplus

//////////////////////////////////////////////////////////////////////
#ifndef _XBOX
#undef _ACCESS_POOL
#define _ACCESS_POOL \
  _CryMemoryManagerPoolHelper g_CryMemoryManagerHelper;\
	void* CryModuleMalloc( size_t size ) throw(){ return g_CryMemoryManagerHelper.Malloc(size); };\
	void* CryModuleRealloc( void *ptr,size_t size )  throw(){ return g_CryMemoryManagerHelper.Realloc(ptr,size); };\
	void  CryModuleFree( void *ptr ) throw() { g_CryMemoryManagerHelper.Free(ptr); };\
	CRY_MEM_USAGE_API void CryModuleGetMemoryInfo( CryModuleMemoryInfo *pMemInfo ) { g_CryMemoryManagerHelper.GetMemoryInfo(pMemInfo); };

// To be created inside CrySystem.
#define _SYSTEM_POOL( hSystemHandle ) \
	_CryMemoryManagerPoolHelper g_CryMemoryManagerHelper( hSystemHandle );\
	void* CryModuleMalloc( size_t size ) throw() {return g_CryMemoryManagerHelper.Malloc(size); };\
	void* CryModuleRealloc( void *ptr,size_t size ) throw(){ return g_CryMemoryManagerHelper.Realloc(ptr,size); };\
	void  CryModuleFree( void *ptr ) { g_CryMemoryManagerHelper.Free(ptr); };\
	CRY_MEM_USAGE_API void CryModuleGetMemoryInfo( CryModuleMemoryInfo *pMemInfo ) { g_CryMemoryManagerHelper.GetMemoryInfo(pMemInfo); };
#endif

_ACCESS_POOL

#endif
