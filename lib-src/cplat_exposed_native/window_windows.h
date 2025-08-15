#ifndef WINDOW_NATIVE
#define WINDOW_NATIVE

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#include <GL/gl.h>
#include <GL/wglext.h>

typedef struct
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
} CP_Window;

static inline void CP_OpenGLSwapBuffers(CP_Window*const window)
{
    SwapBuffers(window->opengl.device);
}

#else
#error "can't define two native window types at once!"
#endif // WINDOW_NATIVE
