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

#ifndef CPLAT_H
#define CPLAT_H

#define CP_DEBUG

#ifdef __cplusplus
extern "C" {
#endif

#include "cplat/macros.h"
#include "cplat/logger.h"
#include "cplat/asserts.h"
#include "cplat//memory.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct st_cp_window CP_Window;

typedef struct 
{
    uint16_t width, height, x, y;
    const char* windowName;
} 
CP_WindowConfig;

typedef enum 
{
    CP_ERROR_SUCCESS = 0,
    CP_ERROR_ALLOCATION = 1,
    CP_ERROR_OS_CALL_FAILED = 2,
    CP_ERROR_NULL_CONFIG = 3
} 
CP_ERROR;

typedef enum
{
    CP_EVENT_NONE = 0,
    CP_EVENT_QUIT
}
CP_EVENT;

typedef struct 
{
    CP_EVENT event;
    union {
        uint32_t key;
        int32_t mWheel;
        struct {
            int16_t mx, my;
        };
    };
} CP_WindowEvent;

CP_ERROR CP_create_window(CP_Window** window, const CP_WindowConfig* const configm, CP_MemPool* pool);
CP_WindowEvent CP_get_next_event(CP_Window* window);
void CP_destroy_window(CP_Window* window, CP_MemPool* pool);

#ifdef __cplusplus
}
#endif

#endif // CPLAT_H
