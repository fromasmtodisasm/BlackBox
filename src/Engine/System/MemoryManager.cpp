#include  <BlackBox/Core/Platform/Platform.hpp>
#include <BlackBox/System/ISystem.hpp>

// Undefine malloc for memory manager itself..
#undef malloc
#undef realloc
#undef free

CRYMEMORYMANAGER_API void* CrySystemCrtMalloc(size_t size)
{
	void* ret = NULL;
	ret = malloc(size);
	return ret;
}

CRYMEMORYMANAGER_API void* CrySystemCrtRealloc(void* p, size_t size)
{
	void* ret;
	ret = realloc(p, size);
	return ret;
}

CRYMEMORYMANAGER_API size_t CrySystemCrtFree(void* p)
{
	size_t n = 0;
	free(p);
	return n;
}

//////////////////////////////////////////////////////////////////////////
CRYMEMORYMANAGER_API void* CryMalloc(size_t size)
{
	if (size >= 256 * 1024 * 1024)
	{
		CryLogAlways("Allocation of size %d requested!", size);
	}

	uint8* p = nullptr;
	size_t sizePlus = size;

	if (p == nullptr)
	{
		{
			p = (uint8*)CrySystemCrtMalloc(sizePlus);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	return p;
}

//////////////////////////////////////////////////////////////////////////
CRYMEMORYMANAGER_API void* CryRealloc(void* memblock, size_t size)
{
	return CrySystemCrtRealloc(memblock, size);
}

CRYMEMORYMANAGER_API void CryFree(void* p)
{
	CrySystemCrtFree(p);
}



