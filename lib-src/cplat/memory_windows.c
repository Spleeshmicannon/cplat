#include "macros.h"
#ifdef CP_WIN32

#include "memory.h"

#include <windows.h>

struct st_cp_mempool
{
    HANDLE handle;
};

CP_MemPool* CP_PoolCreate(size_t bytes)
{
    HANDLE handle = HeapCreate(0, bytes, 0);
    if(handle == 0)
    {
        return NULL;
    }

    CP_MemPool* pool = CP_allocate(sizeof(CP_MemPool));
    pool->handle = handle;

    return pool;
}

void* CP_PoolAllocate(CP_MemPool* pool, size_t bytes)
{
    return HeapAlloc(pool->handle, HEAP_ZERO_MEMORY, bytes);
}

void CP_PoolDestory(CP_MemPool* pool)
{
    HeapDestroy(pool->handle);
    CP_free(pool);
}

void* CP_allocate(const size_t bytes)
{
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes);
}

void CP_free(void* block)
{
    HeapFree(GetProcessHeap(),0, block);
}

#endif // CP_WIN32
