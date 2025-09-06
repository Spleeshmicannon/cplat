#include "logger.h"
#include "macros.h"

#include <stdio.h>

#ifdef CP_LINUX
#ifndef __USE_POSIX199309
#define __USE_POSIX199309 1
#endif
#include <sys/time.h>
#endif
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

void CP_logMessage(CP_LOG_LEVEL level, const char* message, ...)
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
        fflush(stdout);
    }
}
