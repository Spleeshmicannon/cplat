/* zlib license
 * Copyright (C) 2025 J. Benson
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
*/

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
