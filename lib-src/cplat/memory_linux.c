#include "macros.h"
#ifdef CP_LINUX

#include "memory.h"

#include <stdlib.h>

#define __USE_GNU
#include <sys/mman.h>

struct st_cp_mempool
{
    void* base;
    size_t used;
    size_t size;
};

CP_MemPool* CP_PoolCreate(size_t bytes)
{
    void* base = mmap(NULL, bytes, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
    if(base == MAP_FAILED)
    {
        return NULL;
    }
    
    CP_MemPool* pool = CP_allocate(sizeof(CP_MemPool));
    pool->base = base;
    pool->size = bytes;

    return pool;
}

void* CP_PoolAllocate(CP_MemPool* pool, size_t bytes)
{
    if((pool->used + bytes) >= pool->size)
    {
        void* newMap = mremap(pool->base, pool->size, pool->size*2, 0);
        if(newMap == MAP_FAILED)
        {
            return NULL;
        }
        pool->base = newMap;
        pool->size = pool->size*2;
    }
    void* block = pool->base + pool->used;
    pool->used += bytes;
    return block;
}

void CP_PoolDestory(CP_MemPool* pool)
{
    munmap(pool->base, pool->size);
    free(pool);
}

void* CP_allocate(const size_t bytes)
{
    return malloc(bytes);
}

void CP_free(void* block)
{
    free(block);
}

#endif // CP_LINUX
