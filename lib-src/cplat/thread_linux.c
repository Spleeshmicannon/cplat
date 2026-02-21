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
#ifdef CP_LINUX

#include <pthread.h>

#include "memory.h"
#include "thread.h"
#include "logger.h"

bool CP_threadCreate(CP_Thread* thread, CP_ThreadCallbackType callback, void* thread_arg)
{
    const int errorValue = pthread_create(
        &thread->platformThread,
        NULL,
        callback,
        thread_arg
    );

    if(0 != errorValue)
    {
        CP_free(thread);
        CP_log_error("Failed to create pthread with error %d", errorValue);
        return false;
    }

    return true;
}

void CP_threadJoin(CP_Thread* thread)
{
    pthread_join(thread->platformThread, NULL);
}

void CP_threadDetach(CP_Thread* thread)
{
    pthread_detach(thread->platformThread);
}

#endif // CP_LINUX
