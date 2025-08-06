#include "macros.h"
#ifdef CP_WIN32

#include "memory.h"

#include <windows.h>

void* CP_allocate(const size_t bytes)
{
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes);
}

void CP_free(void* block)
{
    HeapFree(GetProcessHeap(),0, block);
}

CP_sysMem CP_sysAllocate(const size_t bytes)
{
    return (CP_sysMem){
        .size = bytes,
        .block = VirtualAlloc(
            0, 
            bytes, 
            MEM_COMMIT | MEM_RESERVE | 
            (bytes >= GetLargePageMinimum() ? MEM_LARGE_PAGES : 0), 
            PAGE_READWRITE
        )
    };
}

void CP_sysFree(const CP_sysMem* const mem)
{
    VirtualFree(mem->block, mem->size, MEM_DECOMMIT);
}

#endif // CP_WIN32
