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
