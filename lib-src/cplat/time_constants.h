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


#ifndef TIME_CONSTANTS_H
#define TIME_CONSTANTS_H

#define CP_NANOS_IN_ONE  1000000000ll
#define CP_MICROS_IN_ONE 1000000ll
#define CP_MILLIS_IN_ONE 1000ll

#define CP_MICROS_IN_NANO (long long)(CP_NANOS_IN_ONE / CP_MICROS_IN_ONE)
#define CP_MILLIS_IN_NANO (long long)(CP_NANOS_IN_ONE / CP_MILLIS_IN_ONE)

#endif // TIME_CONSTANTS_H
