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
        struct {

        } vulkan; // TODO: implement vulkan init
    };
};

#else
#error "can't define two native window types at once!"
#endif // WINDOW_NATIVE
