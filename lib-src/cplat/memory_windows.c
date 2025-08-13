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

void* CP_sysAllocate(const size_t bytes)
{
    const size_t largePageSizeMin = GetLargePageMinimum();
    size_t roundedSize = bytes;
    if (bytes >= largePageSizeMin)
    {
        // bitwise rounding
        roundedSize = (bytes + largePageSizeMin - 1) & ~(largePageSizeMin - 1);
    }

    return VirtualAlloc(
        0, 
        roundedSize, 
        MEM_COMMIT | MEM_RESERVE | 
        (bytes >= GetLargePageMinimum() ? MEM_LARGE_PAGES : 0), 
        PAGE_READWRITE
    );
}

bool CP_sysFree(void* block)
{
    return VirtualFree(block, 0, MEM_RELEASE);
}

#endif // CP_WIN32
