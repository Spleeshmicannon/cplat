/* zlib license
 * Copyright (C) 2026-02-20 22:49:48 J. Benson
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

#ifndef THREAD_H
#define THREAD_H

#define NANOS_IN_ONE_SECOND 1000000000

#include "macros.h"

#include <stdbool.h>

#ifdef CP_LINUX
#include <pthread.h>
typedef struct
{
    pthread_t platformThread;
} CP_Thread;

#elif defined(CP_WIN32)
#include <windows.h>
typedef struct
{
    HANDLE platformThread;
} CP_Thread;
#endif

typedef void* (CP_ThreadCallbackType(void*));

bool CP_threadCreate(CP_Thread* thread, CP_ThreadCallbackType callback, void* thread_arg);
void CP_threadJoin(CP_Thread* thread); // deallocates thread
void CP_threadDetach(CP_Thread* thread); // deallocates thread





#ifdef CP_LINUX
#include <pthread.h>
#include <time.h>

typedef struct
{
    pthread_mutex_t platformMutex;
} CP_Mutex;

static inline void CP_mutexInit(CP_Mutex*const mutex)
{
    pthread_mutex_init(&mutex->platformMutex, NULL);
}

static inline void CP_mutexLock(CP_Mutex*const mutex)
{
    pthread_mutex_lock(&mutex->platformMutex);
}

static inline void CP_mutexUnlock(CP_Mutex*const mutex)
{
    pthread_mutex_unlock(&mutex->platformMutex);
}

static inline void CP_destroy(CP_Mutex*const mutex)
{
    pthread_mutex_destroy(&mutex->platformMutex);
}

static inline void CP_Sleep(unsigned long long unix_nanoseconds)
{
    const struct timespec ts = {
        .tv_sec = unix_nanoseconds / NANOS_IN_ONE_SECOND,
        .tv_nsec = unix_nanoseconds % NANOS_IN_ONE_SECOND
    };

    nanosleep(&ts, NULL);
}

#elif defined(CP_WIN32)
#include <windows.h>

typedef struct
{
    CRITICAL_SECTION platformMutex;
} CP_Mutex;

static inline void CP_mutexInit(CP_Mutex*const mutex)
{
    InitializeCriticalSection(&mutex->platformMutex);
}

static inline void CP_mutexLock(CP_Mutex*const mutex)
{
    EnterCriticalSection(&mutex->platformMutex);
}

static inline void CP_mutexUnlock(CP_Mutex*const mutex)
{
    LeaveCriticalSection(&mutex->platformMutex);
}

static inline void CP_destroy(CP_Mutex*const mutex)
{
    DeleteCriticalSection(&mutex->platformMutex);
}

static inline void CP_Sleep(unsigned long long unix_nanoseconds)
{
    Sleep(unix_nanoseconds / 1000);
}
#endif

#endif // THREAD_H

