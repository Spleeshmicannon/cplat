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

#ifdef __cplusplus
extern "C" {
#endif

#include "cplat/keys.h"
#include "cplat/macros.h"
#include "cplat/thread.h"
#include "cplat/logger.h"
#include "cplat/asserts.h"
#include "cplat/memory.h"

#ifdef CP_WIN32
#include "cplat_exposed_native/cplat_exposed_windows.h"
#elif defined(CP_LINUX)
#include "cplat_exposed_native/cplat_exposed_linux.h"
#endif

#include <stdbool.h>
#include <stdint.h>

typedef enum : uint8_t
{
    CP_WINDOW_FLAGS_FULLSCREEN      = 0x01,
    CP_WINDOW_FLAGS_BORDERLESS      = 0x02,
    CP_WINDOW_FLAGS_RESIZEABLE      = 0x04, // no windows support
    // TODO: CP_WINDOW_FLAGS_KEYBOARD_CAP    = 0x08,
    CP_WINDOW_FLAGS_MOUSE_CAP       = 0x10,
    CP_WINDOW_FLAGS_INIT_OPENGL     = 0x20,
    // TODO: CP_WINDOW_FLAGS_INIT_VULKAN     = 0x40,
    CP_WINDOW_FLAGS_NONE            = 0x00
}
CP_WINDOW_FLAGS;


typedef struct 
{
    uint16_t width, height;
    CP_WINDOW_FLAGS flags;
    uint8_t major, minor; // only used for vulkan/opengl
    const char* windowName;
} 
CP_WindowConfig;

typedef enum
{
    CP_ERROR_SUCCESS = 0,
    CP_ERROR_ALLOCATION = 1,
    CP_ERROR_OS_CALL_FAILED = 2,
    CP_ERROR_GL_CALL_FAILED = 3,
    CP_ERROR_NULL_CONFIG = 4
} 
CP_ERROR;

typedef enum
{
    CP_EVENT_NONE = 0,
    CP_EVENT_QUIT = 1,
    CP_EVENT_KEYDOWN = 2,
    CP_EVENT_KEYUP = 3,
    CP_EVENT_MOUSEMOVE = 4,
    CP_EVENT_MOUSEWHEEL = 5,
    CP_EVENT_LBUTTONDOWN = 6,
    CP_EVENT_MBUTTONDOWN = 7,
    CP_EVENT_RBUTTONDOWN = 8,
    CP_EVENT_LBUTTONUP = 9,
    CP_EVENT_MBUTTONUP = 10,
    CP_EVENT_RBUTTONUP = 11,
}
CP_EVENT;

typedef struct 
{
    CP_EVENT type;
    union 
    {
        uint32_t key;
        int32_t mWheel;
        struct 
        {
            uint16_t mx, my;
        };
    };
} 
CP_WindowEvent;

void* CP_getGLProcAddress();

CP_ERROR CP_createWindow(CP_Window*const window, const CP_WindowConfig* const config);
CP_ERROR CP_setOpenGLVersion(CP_Window*const window, int majorVersion, int minorVersion);

void CP_getWindowWH(const CP_Window*const window, int*const width, int*const height);
void CP_getScreenWH(const CP_Window*const window, int*const width, int*const height);
void CP_getScreenXY(const CP_Window*const window, int*const x, int*const y);

CP_WindowEvent CP_getNextEvent(CP_Window*const window);
CP_WindowEvent CP_waitForNextEvent(CP_Window*const window);

void CP_destroyWindow(CP_Window*const window);

#ifdef __cplusplus
}
#endif

#endif // CPLAT_H
