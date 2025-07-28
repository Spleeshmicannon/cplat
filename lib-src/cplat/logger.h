#ifndef CP_LOGGER_H
#define CP_LOGGER_H

#include "macros.h"

#include <stdarg.h>

#define CV_TIME_BUFFER 26
#define CV_MESSAGE_BUFFER 1024

typedef enum
{
    CP_LOG_FATAL = 0,
    CP_LOG_ERROR = 1,
    CP_LOG_WARN  = 2,
    CP_LOG_INFO  = 3,
    CP_LOG_DEBUG = 4,
    CP_LOG_TRACE = 5
}CP_LOG_LEVEL;

static CP_INLINE const char* CP_log_level_to_string(CP_LOG_LEVEL level)
{
    static const char* ll_strings[] = {
        "FATAL",
        "ERROR",
        "WARN",
        "INFO",
        "DEBUG"
    };

    return ll_strings[level];
}

void CP_log_message(CP_LOG_LEVEL level, const char* message, ...);

#define CP_log_fatal(message, ...)   CP_log_message(CP_LOG_FATAL, message, ##__VA_ARGS__);
#define CP_log_error(message, ...)   CP_log_message(CP_LOG_ERROR, message, ##__VA_ARGS__);
#define CP_log_warn(message, ...)    CP_log_message(CP_LOG_WARN, message, ##__VA_ARGS__);
#define CP_log_info(message, ...)    CP_log_message(CP_LOG_INFO, message, ##__VA_ARGS__);
#define CP_log_debug(message, ...)   CP_log_message(CP_LOG_DEBUG, message, ##__VA_ARGS__);

#endif // CP_LOGGER_H
