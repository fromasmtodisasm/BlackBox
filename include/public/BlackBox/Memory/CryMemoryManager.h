
//////////////////////////////////////////////////////////////////////////
//
//	Crytek Source code 
//	Copyright (c) Crytek 2001-2004
//
//	File: CryMemoryManager.h
//  Description: Memory manager.
//
//	History:
//	- September 2002: File created
//	- February 2005: Modified by Marco Corbetta for SDK release	
//
//////////////////////////////////////////////////////////////////////

#ifndef _CRY_MEMORY_MANAGER_H_
#define _CRY_MEMORY_MANAGER_H_

#include <malloc.h>
//#include <platform.h>
#include <stdlib.h>

#ifdef WIN32
	#ifdef SYSTEM_EXPORTS
		#define CRYMEMORYMANAGER_API __declspec(dllexport)
	#else
		#define CRYMEMORYMANAGER_API __declspec(dllimport)
	#endif
#endif //WIN32 
#if defined(LINUX)
	#define CRYMEMORYMANAGER_API
#endif //LINUX 

#if defined(LINUX)
	#define HMODULE void*
	#include <dlfcn.h>
#endif

//////////////////////////////////////////////////////////////////////
//! Structure filled by call to CryModuleGetMemoryInfo()
struct CryModuleMemoryInfo
{
	//! Total Ammount of memory allocated.
	uint64 allocated;
	//! Total Ammount of memory freed.
	uint64 freed;
	//! Total number of memory allocations.
	int num_allocations;
};

//////////////////////////////////////////////////////////////////////////
// Used by overrided new and delete operators.
//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
	// C++ methods.
	extern "C"
	{
		void* CryModuleMalloc(size_t size) throw();
		void* CryModuleRealloc(void *memblock,size_t size) throw();
		void  CryModuleFree(void *ptr) throw();
		void* CryModuleReallocSize(void *memblock,size_t oldsize,size_t size);
		void  CryModuleFreeSize(void *ptr,size_t size);
	}
	#else
		// C methods.
		extern void* CryModuleMalloc(size_t size);
		extern void* CryModuleRealloc(void *memblock,size_t size);
		extern void  CryModuleFree(void *ptr);
		extern void* CryModuleReallocSize(void *memblock,size_t oldsize,size_t size);
		extern void  CryModuleFreeSize(void *ptr,size_t size);
#endif //__cplusplus
//////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
	extern "C" {
#endif

#if defined(SYSTEM_EXPORTS) || (!defined(WIN32) && !defined(LINUX))
	CRYMEMORYMANAGER_API void *CryMalloc(size_t size);
	CRYMEMORYMANAGER_API void *CryRealloc(void *memblock,size_t size);
	CRYMEMORYMANAGER_API void CryFree(void *p);
	CRYMEMORYMANAGER_API void*  CrySystemCrtMalloc(size_t size);
	CRYMEMORYMANAGER_API void*  CrySystemCrtRealloc(void* p, size_t size);
	CRYMEMORYMANAGER_API size_t CrySystemCrtFree(void* p);

#endif

#ifdef __cplusplus
	}
#endif 


//////////////////////////////////////////////////////////////////////
//#ifndef CRYSYSTEM_EXPORTS
#define _ACCESS_POOL
//#if defined(_DEBUG) || defined(DONT_USE_CRY_MEMORY_MANAGER)
#if defined(DONT_USE_CRY_MEMORY_MANAGER)
	#define CryModuleMalloc malloc
	#define CryModuleRealloc realloc
	#define CryModuleFree free
#else
	#ifdef USE_NEWPOOL
		#define USING_CRY_MEMORY_MANAGER
	// - check this covers all prototypes
	// - way to check memory in use by old malloc?
	// issues
	// only release
	// - globals with allocs -> can make it possible but rather not
	// - calloc? also malloc -> new


#ifdef _USRDLL
#define CRY_MEM_USAGE_API extern "C" __declspec(dllexport)
#else
#define CRY_MEM_USAGE_API
#endif

#ifdef __cplusplus
#include <new>
#endif


#undef malloc
#undef realloc
#undef free


#define malloc				CryModuleMalloc
#define realloc				CryModuleRealloc
#define free				CryModuleFree
#define realloc_size		CryModuleReallocSize
#define free_size			CryModuleFreeSize

#ifdef __cplusplus
	#ifndef GAMECUBE //I don't know how to compile this on GC
		inline void * __cdecl operator new   (size_t  size) { return CryModuleMalloc(size); } 
		inline void * __cdecl operator new[](size_t size) { return CryModuleMalloc(size); }; 
		inline void __cdecl operator delete  (void *p) noexcept { CryModuleFree(p); };
		inline void __cdecl operator delete[](void *p) noexcept { CryModuleFree(p); };
	#endif //GAMECUBE
#endif //__cplusplus

#endif // USE_NEWPOOL

#endif // _DEBUG
#if defined USE_DEBUG_NEW
#	if defined(_DEBUG) && !defined(LINUX)
#		error WTF??????????????/
#		include <crtdbg.h>
#		define DEBUG_CLIENTBLOCK new (_NORMAL_BLOCK, __FILE__, __LINE__)
#		define new DEBUG_CLIENTBLOCK
#	endif
#endif
#if defined(_DEBUG) && !defined(LINUX)
#	undef malloc
#	undef realloc
#	undef free

#	define _CRTDBG_MAP_ALLOC
#	include <crtdbg.h>
#	define DEBUG_CLIENTBLOCK new (_NORMAL_BLOCK, __FILE__, __LINE__)
#	define new DEBUG_CLIENTBLOCK
#endif


//#endif // CRYSYSTEM_EXPORTS
//#endif //LINUX
#endif //_CRY_MEMORY_MANAGER_H_
