#include "logger.h"

#include <stdio.h>
#include <time.h>

void CP_log_message(CP_LOG_LEVEL level, const char* message, ...)
{
    // create time string
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    char time_buffer[CV_TIME_BUFFER];
    strftime(time_buffer, CV_TIME_BUFFER, "%Y-%m-%d %H:%M:%S", tm_info);
    
    // process args
    va_list args;
    va_start(args, message);
    char formatted_message[CV_MESSAGE_BUFFER];
    vsnprintf(formatted_message, CV_MESSAGE_BUFFER, message, args);
    va_end(args);

    // print formatted message
    if(level >= CP_LOG_ERROR)
    {
        fprintf(stderr, "[%s][%s] %s\n", time_buffer, CP_log_level_to_string(level), formatted_message);
    }
    else
    {
        printf("[%s][%s] %s\n", time_buffer, CP_log_level_to_string(level), formatted_message);
    }
}
