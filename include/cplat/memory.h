#ifndef CP_MEMORY_H
#define CP_MEMORY_H

#include <stddef.h>

typedef struct st_cp_mempool CP_MemPool;

CP_MemPool* CP_PoolCreate(size_t bytes);
void* CP_PoolAllocate(CP_MemPool* pool, size_t bytes);
void CP_PoolDestory(CP_MemPool* pool);

void* CP_allocate(const size_t bytes);
void CP_free(void* block);

#endif // CP_MEMORY_H
