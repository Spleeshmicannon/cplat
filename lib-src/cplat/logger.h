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

#ifndef CP_LOGGER_H
#define CP_LOGGER_H

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

void CP_log_message(CP_LOG_LEVEL level, const char* message, ...);

#ifndef NO_LOGGING
#define CP_log_fatal(message, ...)   CP_log_message(CP_LOG_FATAL, message, ##__VA_ARGS__);
#define CP_log_error(message, ...)   CP_log_message(CP_LOG_ERROR, message, ##__VA_ARGS__);
#define CP_log_warn(message, ...)    CP_log_message(CP_LOG_WARN, message, ##__VA_ARGS__);
#define CP_log_info(message, ...)    CP_log_message(CP_LOG_INFO, message, ##__VA_ARGS__);
#ifdef CP_DEBUG
#define CP_log_debug(message, ...)   CP_log_message(CP_LOG_DEBUG, message, ##__VA_ARGS__);
#define CP_log_trace(message, ...)   CP_log_message(CP_LOG_TRACE, message, ##__VA_ARGS__);
#else
#define CP_log_debug(message, ...)
#define CP_log_trace(message, ...)
#endif
#else
#define CP_log_fatal(message, ...)
#define CP_log_error(message, ...)
#define CP_log_warn(message, ...)
#define CP_log_info(message, ...)
#define CP_log_debug(message, ...)
#define CP_log_trace(message, ...)
#endif

#endif // CP_LOGGER_H
