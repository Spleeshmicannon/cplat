#ifndef WINDOW_NATIVE
#define WINDOW_NATIVE

#include <xcb/xcb.h>
#include <xcb/xcb_util.h>
#include <xcb/xcb_keysyms.h>

struct st_cp_window
{
    xcb_connection_t *connection;
    xcb_screen_t* screen;
    xcb_window_t windowId;
    xcb_atom_t wmDeleteProtocol;
    xcb_atom_t wmProtocols;
    xcb_key_symbols_t* keySymbols;
};

#else
#error "can't define two native window types at once!"
#endif // WINDOW_NATIVE
