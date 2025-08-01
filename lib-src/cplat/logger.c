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

#include "logger.h"
#include "macros.h"

#include <stdio.h>
#include <time.h>

static CP_INLINE const char* CP_log_level_to_string(CP_LOG_LEVEL level)
{
    static const char* ll_strings[] = {
        "FATAL",
        "ERROR",
        "WARN",
        "INFO",
        "DEBUG",
        "TRACE"
    };

    return ll_strings[level];
}

void CP_log_message(CP_LOG_LEVEL level, const char* message, ...)
{
    // create time string
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char time_buffer[CV_TIME_BUFFER];
    strftime(time_buffer, CV_TIME_BUFFER, "%Y-%m-%d %H:%M:%S", tm_info);

    // get nanoseconds
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    
    // process args
    va_list args;
    va_start(args, message);
    char formatted_message[CV_MESSAGE_BUFFER];
    vsnprintf(formatted_message, CV_MESSAGE_BUFFER, message, args);
    va_end(args);

    // print formatted message
    if(level <= CP_LOG_ERROR)
    {
        fprintf(stderr, "[%s.%09ld][%s] %s\n", time_buffer, ts.tv_nsec, CP_log_level_to_string(level), formatted_message);
    }
    else
    {
        printf("[%s.%09ld][%s] %s\n", time_buffer, ts.tv_nsec, CP_log_level_to_string(level), formatted_message);
    }
}
