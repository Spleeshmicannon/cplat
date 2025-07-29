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
