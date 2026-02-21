/* zlib license
 * Copyright (C) 2026-02-20 22:52:10 J. Benson
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

#include "macros.h"
#ifdef CP_WIN32

#include <windows.h> 
#include <synchapi.h>

#include "memory.h"
#include "thread.h"
#include "logger.h"

typedef struct
{
    CP_ThreadCallbackType* callback;
    void* thread_arg;
} _cp_ThreadShim;

static DWORD WINAPI threadShim(LPVOID param)
{
    _cp_ThreadShim* shim = (_cp_ThreadShim*)param;
    void* val = shim->callback(shim->callback);
    CP_free(shim);
    return 0;
}

bool CP_threadCreate(CP_Thread* thread, CP_ThreadCallbackType callback, void* thread_arg)
{
    _cp_ThreadShim* shim = CP_allocate(sizeof(_cp_ThreadShim));
    shim->thread_arg = thread_arg;
    shim->callback = callback;

    thread->platformThread = CreateThread(
        NULL,
        0,
        threadShim,
        shim,
        0,
        NULL
    );

    if(0 == thread->platformThread)
    {
        CP_free(thread);
        CP_log_error("Failed to create win32 thread");
        return NULL;
    }

    return thread;
}

void CP_threadJoin(CP_Thread* thread)
{
    WaitForSingleObject(thread->platformThread, INFINITE);
    CloseHandle(thread->platformThread);
}

void CP_threadDetach(CP_Thread* thread)
{
    CloseHandle(thread->platformThread);
}

#endif // CP_WIN32
