/* zlib license
 * Copyright (C) 2025 J. Benson
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
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
#include <string.h>

#define __USE_GNU
#include <sys/mman.h>

void* CP_allocate(const size_t bytes)
{
    void* block = malloc(bytes);
    memset(block, 0, bytes);
    return block;
}

void CP_free(void* block)
{
    free(block);
}

void* CP_sysAllocate(const size_t bytes)
{
    void* block = mmap(NULL, 
        bytes + sizeof(size_t), 
        PROT_READ | PROT_WRITE, 
        MAP_ANONYMOUS | MAP_PRIVATE, // MAP_ANONYMOUS zeros memory
        -1, 
        0
    );

    if(block == MAP_FAILED)
    {
        return NULL;
    }

    // storing size in memory allocation
    *(size_t*)block = bytes + sizeof(size_t);

    return (char*)block + sizeof(size_t);
}

bool CP_sysFree(void* block)
{
    // decrementing block to get memory allocation size
    block = (char*)block - sizeof(size_t);
    return munmap(block, *(size_t*)block) == 0;
}

#endif // CP_LINUX
