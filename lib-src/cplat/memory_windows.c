#include "macros.h"
#ifdef CP_WIN32

#include "memory.h"

#include "logger.h"

#include <windows.h>

void* CP_allocate(const size_t bytes)
{
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, bytes);
}

void* CP_reallocate(void*const oldBlock, const size_t newSize)
{
    return HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, oldBlock, newSize);
}

void CP_free(void* block)
{
    HeapFree(GetProcessHeap(),0, block);
}

void* CP_sysAllocate(const size_t bytes)
{
    if(0 == bytes)
    {
        return NULL;
    }

    void* ptr = VirtualAlloc(
        0, 
        bytes, 
        MEM_COMMIT | MEM_RESERVE, 
        PAGE_READWRITE
    );

    if(NULL == ptr)
    {
        CP_log_error("VirtualAlloc failed with error 0x%x", GetLastError())
    }

    return ptr;
}

void* CP_sysReallocate(void* oldBlock, const size_t newSize)
{
    MEMORY_BASIC_INFORMATION mbi;
    if(0 == VirtualQuery(oldBlock, &mbi, sizeof(mbi)))
    {
        return NULL;
    }

    void* newBlock = CP_sysAllocate(newSize);
    if(NULL == newBlock)
    {
        return NULL;
    }

    memcpy(newBlock, oldBlock, mbi.RegionSize);
    VirtualFree(oldBlock, 0, MEM_RELEASE);

    return newBlock;
}

bool CP_sysFree(void* block)
{
    return VirtualFree(block, 0, MEM_RELEASE);
}

#endif // CP_WIN32
