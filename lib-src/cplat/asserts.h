#ifndef CP_ASSERTS
#define CP_ASSERTS

#include "macros.h"
#include "logger.h"

#include <stdbool.h>
#include <stdlib.h>

#ifdef CP_ASSERT_ENABLED
static CP_INLINE void cp_assert(const char* const file, const int line, bool expr)
{
    if(!expr)
    {
        CP_log_error("Assertion Failure: %s:%d", file, line);
#ifdef CP_EXIT_ON_ASSERT_FAIL
        exit(128);
#endif
    }
}

#define CP_assert(expr) cp_assert(__FILE__, __LINE__, expr)
#else
#define CP_assert(expr) (void)expr
#endif

#endif // CP_ASSERTS
