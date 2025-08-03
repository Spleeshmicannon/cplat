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

#include "cplat/input.h"
#include "cplat/macros.h"
#include "cplat/logger.h"
#include "cplat/asserts.h"
#include "cplat/memory.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct st_cp_window CP_Window;

#ifdef CP_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

struct st_cp_window
{
    HINSTANCE hinst;
    HWND hwnd;
};
#elif defined(CP_LINUX)
#include <xcb/xcb.h>
#include <xcb/xcb_util.h>

struct st_cp_window
{
    xcb_connection_t *connection;
    xcb_screen_t* screen;
    xcb_window_t windowId;
    xcb_atom_t wmDeleteProtocol;
    xcb_atom_t wmProtocols;
};
#endif

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
        uint32_t mouseButton;
        int32_t mWheel;
        struct 
        {
            uint16_t mx, my;
        };
    };
} 
CP_WindowEvent;

CP_ERROR CP_createWindow(CP_Window*const window, const CP_WindowConfig* const configm);
CP_WindowEvent CP_getNextEvent(CP_Window*const window);
void CP_destroyWindow(CP_Window*const window);

#ifdef __cplusplus
}
#endif

#endif // CPLAT_H
