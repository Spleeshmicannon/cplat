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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

#ifndef __gl_h_
#include <GL/gl.h>
#endif

#include <GL/wglext.h>

#include <stdint.h>

typedef struct
{
    HINSTANCE hinst;
    HWND hwnd;
    union {
        struct {
            HDC device;
            HGLRC context;
        } opengl;
        struct {

        } vulkan; // TODO: implement vulkan init
    };
} CP_Window;

CP_INLINE void CP_OpenGLSwapBuffers(CP_Window*const window)
{
    SwapBuffers(window->opengl.device);
}

typedef struct
{
    int64_t freq, startCounter;
}
ClockData;

CP_INLINE static ClockData cp_getClockData(void)
{
    static int64_t freq = 0;
    static int64_t startCounter = 0;

    if(freq == 0)
    {
        LARGE_INTEGER liFreq, liCounter;
        QueryPerformanceCounter(&liCounter);
        QueryPerformanceFrequency(&liFreq);
        freq = liFreq.QuadPart;
        startCounter = liCounter.QuadPart;
    }

    return (ClockData){ freq, startCounter };
}

CP_INLINE static int64_t CP_getNanos(void) 
{
    const ClockData clockData = cp_getClockData();

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    // Scale to nanoseconds carefully to avoid overflow
    return ((counter.QuadPart - clockData.startCounter) * CP_NANOS_IN_ONE) / clockData.freq;
}


CP_INLINE static int64_t CP_getMicros(void) 
{
    const ClockData clockData = cp_getClockData();

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return ((counter.QuadPart - clockData.startCounter) * CP_MICROS_IN_ONE) / clockData.freq;
}

CP_INLINE static int64_t CP_getMillis(void) 
{
    const ClockData clockData = cp_getClockData();

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return ((counter.QuadPart - clockData.startCounter) * CP_MILLIS_IN_ONE) / clockData.freq;
}

CP_INLINE static int64_t CP_getSeconds(void) 
{
    const ClockData clockData = cp_getClockData();

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return (counter.QuadPart - clockData.startCounter) / clockData.freq;
}

#else
#error "can't define two native window types at once!"
#endif // WINDOW_NATIVE
