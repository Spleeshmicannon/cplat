#ifndef WINDOW_NATIVE
#define WINDOW_NATIVE

#include <X11/Xlib-xcb.h> 
#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xcb_keysyms.h>

#include <EGL/egl.h>
#include <EGL/eglext.h>

typedef struct
{
    Display* xDisplay;
    xcb_connection_t *connection;
    xcb_screen_t* screen;
    xcb_window_t windowId;
    xcb_atom_t wmDeleteProtocol;
    xcb_atom_t wmProtocols;
    xcb_key_symbols_t* keySymbols;

    union {
        struct {
            EGLDisplay display;
            EGLSurface surface;
            EGLContext context;
        } opengl;
        struct {

        } vulkan; // TODO: implement vulkan init
    };
} CP_Window;


static inline void CP_OpenGLSwapBuffers(CP_Window*const window)
{
    (void)window;
}

#else
#error "can't define two native window types at once!"
#endif // WINDOW_NATIVE
