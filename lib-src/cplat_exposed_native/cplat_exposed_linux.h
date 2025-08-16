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

#ifndef WINDOW_NATIVE
#define WINDOW_NATIVE

#include "../cplat/macros.h"
#include "../cplat/time_constants.h"

#ifndef __USE_POSIX199309
#define __USE_POSIX199309 1
#endif
#include <sys/time.h>
#include <time.h>


#include <stdint.h>

#include <X11/Xlib-xcb.h> 
#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xcb_keysyms.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

typedef struct
{
    Display* xDisplay;
    xcb_connection_t *connection;
    xcb_screen_t* screen;
    xcb_window_t windowId;
    xcb_atom_t wmDeleteProtocol;
    xcb_atom_t wmProtocols;
    xcb_key_symbols_t* keySymbols;

    union {
        struct {
            EGLDisplay display;
            EGLSurface surface;
            EGLContext context;
        } opengl;
        struct {

        } vulkan; // TODO: implement vulkan init
    };
} CP_Window;


CP_INLINE void CP_OpenGLSwapBuffers(CP_Window*const window)
{
    (void)window;
}


CP_INLINE static int64_t CP_getNanos(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * CP_NANOS_IN_ONE) + ts.tv_nsec;
}

CP_INLINE static int64_t CP_getMicros(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * CP_MICROS_IN_ONE) + 
        (ts.tv_nsec / CP_MICROS_IN_NANO);
}

CP_INLINE static int64_t CP_getMillis(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * CP_MILLIS_IN_ONE) + 
        (ts.tv_nsec / CP_MILLIS_IN_NANO);
}

CP_INLINE static int64_t CP_getSeconds(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec;
}

#else
#error "can't define two native window types at once!"
#endif // WINDOW_NATIVE
