#ifndef WINDOW_NATIVE
#define WINDOW_NATIVE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/wglext.h>

struct st_cp_window
{
    HINSTANCE hinst;
    HWND hwnd;
    union {
        struct {
            HDC device;
            HGLRC context;
        } opengl;
    };
};


#define GLV_MAJOR_VERSION_1  (int)1
#define GLV_MAJOR_VERSION_2  (int)2
#define GLV_MAJOR_VERSION_3  (int)3
#define GLV_MAJOR_VERSION_4  (int)4

#define GLV_MINOR_VERSION_0  (int)0
#define GLV_MINOR_VERSION_1  (int)1
#define GLV_MINOR_VERSION_2  (int)2
#define GLV_MINOR_VERSION_3  (int)3

#else
#error "can't define two native window types at once!"
#endif // WINDOW_NATIVE
