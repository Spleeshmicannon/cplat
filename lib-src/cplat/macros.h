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

#ifndef CP_MACROS
#define CP_MACROS

// inlining
#ifdef __GNUC__
#define CP_INLINE __attribute__((always_inline)) inline
#define CP_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define CP_INLINE __forceinline
#define CP_NOINLINE __declspec(noinline)
#else
#define CP_INLINE inline
#define CP_NOINLINE
#endif

// platform
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#define CP_WIN32
#elif __APPLE__
#error no apple support yet!
#elif __linux__
#define CP_LINUX
#endif 

#endif // CP_MACROS
