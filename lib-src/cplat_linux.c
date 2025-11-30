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

#include "cplat.h"
#ifdef CP_LINUX

#include <string.h>
#include <stdlib.h>

#include <xcb/xcb_icccm.h>

#define XK_LATIN1
#define XK_MISCELLANY
#include <X11/keysymdef.h>

#include <GL/gl.h>

CP_INLINE CP_KEY CP_xcbKeyToCPkey(xcb_keysym_t keycode);

void CP_getScreenWH(const CP_Window*const window, int*const width, int*const height)
{
    if(NULL != width) *width = window->screen->width_in_pixels;
    if(NULL != height) *height = window->screen->height_in_pixels;
}

CP_ERROR CP_createWindow(CP_Window*const window, const CP_WindowConfig* const config)
{
    int screen_count = 0;
    memset(window, 0, sizeof(CP_Window));

    window->xDisplay = XOpenDisplay(NULL);
    if(!window->xDisplay)
    {
        CP_log_error("Failed to open X Display, is the display variable set?");
        return CP_ERROR_OS_CALL_FAILED;
    }
    
    // grab X11 proto connection
    window->connection = XGetXCBConnection(window->xDisplay);
    if(window->connection == NULL)
    {
        CP_log_error("null xcb connection");
        return CP_ERROR_OS_CALL_FAILED;
    }

    xcb_aux_get_screen(window->connection, screen_count);
    
    // get the default screen
    window->screen = xcb_aux_get_screen(window->connection, screen_count);
    if(window->screen == NULL)
    {
        CP_log_error("unable to get screen");
        return CP_ERROR_OS_CALL_FAILED;
    }
    
    // get the window id
    window->windowId = xcb_generate_id(window->connection);
    if(window->windowId == 0)
    {
        CP_log_error("failed to generate window id");
        return CP_ERROR_OS_CALL_FAILED;
    }
    
    // creating the window
    uint32_t event_values = XCB_EVENT_MASK_BUTTON_PRESS | 
        XCB_EVENT_MASK_BUTTON_RELEASE |
		XCB_EVENT_MASK_KEY_PRESS | 
        XCB_EVENT_MASK_KEY_RELEASE |
		XCB_EVENT_MASK_EXPOSURE | 
        XCB_EVENT_MASK_POINTER_MOTION |
		XCB_EVENT_MASK_STRUCTURE_NOTIFY;

    uint32_t value_list[] = { window->screen->black_pixel, event_values };
    
    uint16_t win_width = config->width;
    uint16_t win_height = config->height;

    if(config->flags & CP_WINDOW_FLAGS_FULLSCREEN)
    {
        win_width = window->screen->width_in_pixels;
        win_height = window->screen->height_in_pixels;
    }
    
    xcb_create_window(
        window->connection, 
        0,
        window->windowId,
        window->screen->root,
        0,
        0,
        win_width,
        win_height,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        window->screen->root_visual,
        XCB_CW_EVENT_MASK | XCB_CW_BACK_PIXEL,
        value_list
    );

    // creating data for window manager protocl
    const char* window_manager_protocol_name = "WM_PROTOCOLS";
    uint16_t window_manager_protocol_name_size = (uint16_t)strlen(window_manager_protocol_name);
    xcb_intern_atom_cookie_t intern_atom_cookie = xcb_intern_atom(
        window->connection, 
        true, 
        window_manager_protocol_name_size, 
        window_manager_protocol_name
    );
    
    xcb_intern_atom_reply_t* intern_atom_reply = xcb_intern_atom_reply(
        window->connection, 
        intern_atom_cookie, 
        NULL
    );
    xcb_atom_t window_manager_protocols_property = intern_atom_reply->atom;
    free(intern_atom_reply);
    
    // creating data for window manager deletion protocol
    const char* window_manager_delete_protocol_name = "WM_DELETE_WINDOW";
    uint16_t window_manager_delete_protocol_name_size = (uint16_t)strlen(window_manager_delete_protocol_name);
    intern_atom_cookie = xcb_intern_atom(
        window->connection, 
        true, 
        window_manager_delete_protocol_name_size, 
        window_manager_delete_protocol_name
    );
    intern_atom_reply = xcb_intern_atom_reply(
        window->connection, 
        intern_atom_cookie, 
        NULL
    );
    xcb_atom_t window_manager_window_delete_protocol = intern_atom_reply->atom;
    free(intern_atom_reply);
    
    // setting window deletion and title up
    xcb_icccm_set_wm_name(
        window->connection, 
        window->windowId, 
        XCB_ATOM_STRING, 
        sizeof(char) * 8, 
        (uint16_t)strlen(config->windowName), 
        config->windowName
    );
    xcb_icccm_set_wm_protocols(
        window->connection, 
        window->windowId, 
        window_manager_protocols_property, 
        1, 
        &window_manager_window_delete_protocol
    );
    window->wmDeleteProtocol = window_manager_window_delete_protocol;
    window->wmProtocols = window_manager_protocols_property;

    if (config->flags & CP_WINDOW_FLAGS_BORDERLESS)
    {
        xcb_atom_t motif_hints_atom;
        {
            xcb_intern_atom_cookie_t cookie = 
                xcb_intern_atom(
                    window->connection, 
                    0, 
                    strlen("_MOTIF_WM_HINTS"), 
                    "_MOTIF_WM_HINTS"
                );

            xcb_intern_atom_reply_t *reply = xcb_intern_atom_reply(
                window->connection, 
                cookie, 
                NULL
            );

            motif_hints_atom = reply->atom;
            free(reply);
        }

        struct  
        {
            uint32_t flags;
            uint32_t functions;
            uint32_t decorations;
            int32_t  input_mode;
            uint32_t status;
        }
        hints = {0};

        hints.flags = 1L << 1; // necessary?
        hints.decorations = 0; // no borders, no title

        xcb_change_property(
            window->connection,
            XCB_PROP_MODE_REPLACE,
            window->windowId,
            motif_hints_atom,
            motif_hints_atom,
            32,
            sizeof(hints) / 4,
            &hints
        );
    }

    if (!(config->flags & CP_WINDOW_FLAGS_RESIZEABLE))
    {
        // setting window size 
        xcb_size_hints_t window_size_hints;
        xcb_icccm_size_hints_set_min_size(&window_size_hints, config->width, config->height);
        xcb_icccm_size_hints_set_max_size(&window_size_hints, config->width, config->height);
        xcb_icccm_set_wm_size_hints(
            window->connection, 
            window->windowId, 
            XCB_ATOM_WM_NORMAL_HINTS, 
            &window_size_hints
        );
    }

    // showing the window
    xcb_map_window(window->connection, window->windowId);
    if(xcb_flush(window->connection) <= 0)
    {
        xcb_destroy_window(window->connection, window->windowId);
        xcb_disconnect(window->connection);

        CP_log_error("failed to xcb flush");
        return CP_ERROR_OS_CALL_FAILED;
    }

    window->keySymbols = xcb_key_symbols_alloc(window->connection);

    if(config->flags & CP_WINDOW_FLAGS_INIT_OPENGL)
    {
        // Getting the egl display
        window->opengl.display = eglGetDisplay((EGLNativeDisplayType)window->xDisplay);

        if(window->opengl.display == EGL_NO_DISPLAY)
        {
            CP_log_error("Failed to get OpenGL display");
            return CP_ERROR_GL_CALL_FAILED;
        }

        // initialize EGL
        int major, minor;
        if(!eglInitialize(window->opengl.display, &major, &minor))
        {
            CP_log_error("Failed to initialise EGL");
            return CP_ERROR_GL_CALL_FAILED;
        }

        CP_log_info("initialized EGL with version %d.%d", major, minor);

        // configure EGL
        const EGLint configAttribs[] = {
            EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
            EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,   // For desktop OpenGL
            EGL_RED_SIZE,        8,
            EGL_GREEN_SIZE,      8,
            EGL_BLUE_SIZE,       8,
            EGL_ALPHA_SIZE,      8,
            EGL_DEPTH_SIZE,      24,
            EGL_STENCIL_SIZE,    8,
            EGL_NONE
        };
        EGLConfig eglConfig;
        EGLint eglConfigNum;

        if(!eglChooseConfig(window->opengl.display, configAttribs, &eglConfig, 1, &eglConfigNum) || 
        eglConfigNum< 1)
        {
            CP_log_error("Failed to setup EGL config");
            return CP_ERROR_GL_CALL_FAILED;
        }

        window->opengl.surface = eglCreateWindowSurface(
            window->opengl.display, 
            eglConfig, 
            (EGLNativeWindowType)window->windowId, 
            NULL
        );

        if(window->opengl.surface == EGL_NO_SURFACE)
        {
            CP_log_error("Failed to create EGL surface");
            return CP_ERROR_GL_CALL_FAILED;
        }

        if(!eglBindAPI(EGL_OPENGL_API))
        {
            CP_log_error("Failed to bind OpenGL API");
            return CP_ERROR_GL_CALL_FAILED;
        }

        const EGLint ctxAttribs[] = {
            EGL_CONTEXT_MAJOR_VERSION, config->major,
            EGL_CONTEXT_MINOR_VERSION, config->minor,
            EGL_CONTEXT_OPENGL_PROFILE_MASK, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT,
            EGL_NONE
        };

        window->opengl.context = eglCreateContext(
            window->opengl.display, 
            eglConfig, 
            EGL_NO_CONTEXT, 
            ctxAttribs
        );


        if(window->opengl.context == EGL_NO_CONTEXT)
        {
            CP_log_error("Failed to create OpenGL context");
            return CP_ERROR_GL_CALL_FAILED;
        }

        if(!eglMakeCurrent(
            window->opengl.display, 
            window->opengl.surface, 
            window->opengl.surface, 
            window->opengl.context))
        {
            CP_log_error("Failed to make context current");

        }

        const char* ver = (const char*)glGetString(GL_VERSION);

        if(ver != NULL)
        {
            CP_log_info("OpenGL version set to: %s", ver);
        }
        else
        {
            CP_log_error("Failed to set OpenGL version");
            return CP_ERROR_OS_CALL_FAILED;
        }
    }

    return CP_ERROR_SUCCESS;
}

CP_WindowEvent CP_getNextEvent(CP_Window*const window)
{
    CP_WindowEvent event = {};
    xcb_generic_event_t* xcb_event;

    xcb_event = xcb_poll_for_event(window->connection);
    if(xcb_event == NULL)
    {
        return event;
    }

    switch(XCB_EVENT_RESPONSE_TYPE(xcb_event)) // TODO implement mouse wheel
    {
        case XCB_KEY_PRESS:
        {
            const xcb_key_press_event_t*const kb_event = (xcb_key_press_event_t*)xcb_event;

            event.type = CP_EVENT_KEYDOWN;
            event.key = CP_xcbKeyToCPkey(
                xcb_key_symbols_get_keysym(window->keySymbols, kb_event->detail, 0)
            );
            CP_log_trace("key press of %s", CP_keyToString(event.key));
            break;
        }
        case XCB_KEY_RELEASE:
        {
            const xcb_key_release_event_t*const kb_event = (xcb_key_press_event_t*)xcb_event;

            event.type = CP_EVENT_KEYDOWN;
            event.key = CP_xcbKeyToCPkey( xcb_key_symbols_get_keysym(window->keySymbols, kb_event->detail, 0));
            CP_log_trace("key release of %s", CP_keyToString(event.key));
            break;
        }
        case XCB_MOTION_NOTIFY:
        {
            const xcb_motion_notify_event_t*const mn_event = (xcb_motion_notify_event_t*)xcb_event;

            event.type = CP_EVENT_MOUSEMOVE;
            event.mx = (uint16_t)mn_event->root_x;
            event.my = (uint16_t)mn_event->root_y;

            CP_log_trace("mouse motion at %d, %d", event.mx, event.my);
            break;
        }
        case XCB_BUTTON_PRESS:
        {
            const xcb_button_press_event_t*const mn_event = (xcb_button_press_event_t*)xcb_event;
            
            switch(mn_event->detail)
            {
                case XCB_BUTTON_INDEX_1: 
                    event.type = CP_EVENT_LBUTTONDOWN; 
                    CP_log_trace("left mouse button");
                    break;
                case XCB_BUTTON_INDEX_2: 
                    event.type = CP_EVENT_MBUTTONDOWN; 
                    CP_log_trace("middle mouse button");
                    break;
                case XCB_BUTTON_INDEX_3: 
                    event.type = CP_EVENT_RBUTTONDOWN; 
                    CP_log_trace("right mouse button");
                    break;
                case XCB_BUTTON_INDEX_4:
                {
                    event.type = CP_EVENT_MOUSEWHEEL;
                    event.mWheel = 1;
                    CP_log_trace("mouse up");
                    break;
                }
                case XCB_BUTTON_INDEX_5:
                {
                    event.type = CP_EVENT_MOUSEWHEEL;
                    event.mWheel = -1;
                    CP_log_trace("mouse down");
                    break;
                }
                default: break;
            }
            break;
        }
        case XCB_BUTTON_RELEASE:
        {
            const xcb_button_release_event_t*const mn_event = (xcb_button_press_event_t*)xcb_event;
            
            switch(mn_event->detail)
            {
                case XCB_BUTTON_INDEX_1: 
                    event.type = CP_EVENT_LBUTTONUP; 
                    CP_log_trace("left mouse button up");
                    break;
                case XCB_BUTTON_INDEX_2: 
                    event.type = CP_EVENT_MBUTTONUP; 
                    CP_log_trace("middle mouse button up");
                    break;
                case XCB_BUTTON_INDEX_3: 
                    event.type = CP_EVENT_RBUTTONUP; 
                    CP_log_trace("right mouse button up");
                    break;
                default: break;
            }
            break;
        }
        case XCB_CLIENT_MESSAGE:
        {
            xcb_client_message_event_t* client_message_event = (xcb_client_message_event_t*)xcb_event;
            if((client_message_event->window == window->windowId) && 
               (client_message_event->data.data32[0] == window->wmDeleteProtocol))
            {
                event.type = CP_EVENT_QUIT;
                CP_log_trace("quit event");
            }
            break;
        }
        default:break;
    }


    return event;
}

void CP_destroyWindow(CP_Window*const window)
{
    // TODO destroy opengl vars, if necessary
    xcb_key_symbols_free(window->keySymbols);
    xcb_destroy_window(window->connection, window->windowId);
    xcb_disconnect(window->connection);
}

void* CP_getGLProcAddress()
{
    return eglGetProcAddress;
}

CP_INLINE CP_KEY CP_xcbKeyToCPkey(xcb_keysym_t keysym) // Todo numpad, / and " don't work
{
    switch(keysym)
    {
		case XK_BackSpace: return CP_KEY_BACKSPACE;
		case XK_KP_Enter:
        case XK_Return: return CP_KEY_ENTER;
		case XK_Tab: return CP_KEY_TAB;
		case XK_Pause: return CP_KEY_PAUSE;
		case XK_Caps_Lock: return CP_KEY_CAPITAL;
		case XK_Escape: return CP_KEY_ESCAPE;
        case XK_Henkan: return CP_KEY_CONVERT;
        case XK_Muhenkan: return CP_KEY_NONCONVERT;
		case XK_Mode_switch: return CP_KEY_MODECHANGE;
		case XK_space: return CP_KEY_SPACE;
		case XK_KP_Prior:
        case XK_Prior: return CP_KEY_PRIOR;
		case XK_KP_Next:
        case XK_Next: return CP_KEY_NEXT;
		case XK_KP_End:
        case XK_End: return CP_KEY_END;
		case XK_KP_Home:
        case XK_Home: return CP_KEY_HOME;
        case XK_KP_Left:
        case XK_Left: return CP_KEY_LEFT;
		case XK_KP_Up:
        case XK_Up: return CP_KEY_UP;
		case XK_KP_Right:
        case XK_Right: return CP_KEY_RIGHT;
		case XK_KP_Down:
        case XK_Down: return CP_KEY_DOWN;
		case XK_Select: return CP_KEY_SELECT;
		case XK_Print: return CP_KEY_PRINT;
		case XK_Execute:return CP_KEY_EXECUTE;
		case XK_KP_Insert:
        case XK_Insert: return CP_KEY_INSERT;
		case XK_KP_Delete:
        case XK_Delete: return CP_KEY_TDELETE;
		case XK_Help: return CP_KEY_HELP;
        case XK_Super_L:
		case XK_Meta_L: return CP_KEY_LWIN;
        case XK_Super_R:
        case XK_Meta_R: return CP_KEY_RWIN;
        case XK_Menu: return CP_KEY_APPS;
        case XK_KP_Begin: return CP_KEY_BEGIN;
        case XK_0: return CP_KEY_0;
		case XK_KP_0: return CP_KEY_NUMPAD0;
        case XK_1: return CP_KEY_1;
        case XK_KP_1: return CP_KEY_NUMPAD1;
        case XK_2: return CP_KEY_2;
        case XK_KP_2: return CP_KEY_NUMPAD2;
        case XK_3: return CP_KEY_3;
        case XK_KP_3: return CP_KEY_NUMPAD3;
        case XK_4: return CP_KEY_4;
        case XK_KP_4: return CP_KEY_NUMPAD4;
        case XK_5: return CP_KEY_5;
        case XK_KP_5: return CP_KEY_NUMPAD5;
        case XK_6: return CP_KEY_6;
        case XK_KP_6: return CP_KEY_NUMPAD6;
        case XK_7: return CP_KEY_7;
        case XK_KP_7: return CP_KEY_NUMPAD7;
        case XK_8: return CP_KEY_8;
        case XK_KP_8: return CP_KEY_NUMPAD8;
        case XK_9: return CP_KEY_9;
        case XK_KP_9: return CP_KEY_NUMPAD9;
        case XK_KP_Multiply:
		case XK_multiply: return CP_KEY_MULTIPLY;
		case XK_KP_Add: return CP_KEY_ADD;
		case XK_KP_Separator: return CP_KEY_SEPARATOR;
		case XK_KP_Subtract: return CP_KEY_SUBTRACT;
		case XK_KP_Decimal: return CP_KEY_DECIMAL;
		case XK_KP_Divide: return CP_KEY_DIVIDE;
        case XK_backslash: return CP_KEY_BACK_SLASH;
        case XK_bracketleft: return CP_KEY_OPEN_BRACKET;
        case XK_bracketright: return CP_KEY_CLOSE_BRACKET;
        case XK_apostrophe: return CP_KEY_APOSTRAPHE;
        case XK_F1: return CP_KEY_F1;
		case XK_F2: return CP_KEY_F2;
		case XK_F3: return CP_KEY_F3;
		case XK_F4: return CP_KEY_F4;
		case XK_F5: return CP_KEY_F5;
		case XK_F6: return CP_KEY_F6;
		case XK_F7: return CP_KEY_F7;
		case XK_F8: return CP_KEY_F8;
		case XK_F9: return CP_KEY_F9;
		case XK_F10: return CP_KEY_F10;
		case XK_F11: return CP_KEY_F11;
		case XK_F12: return CP_KEY_F12;
		case XK_F13: return CP_KEY_F13;
		case XK_F14: return CP_KEY_F14;
		case XK_F15: return CP_KEY_F15;
		case XK_F16: return CP_KEY_F16;
		case XK_F17: return CP_KEY_F17;
		case XK_F18: return CP_KEY_F18;
		case XK_F19: return CP_KEY_F19;
		case XK_F20: return CP_KEY_F20;
		case XK_F21: return CP_KEY_F21;
		case XK_F22: return CP_KEY_F22;
		case XK_F23: return CP_KEY_F23;
		case XK_F24: return CP_KEY_F24;
		case XK_Num_Lock: return CP_KEY_NUMLOCK;
		case XK_Scroll_Lock: return CP_KEY_SCROLL;
		case XK_KP_Equal: return CP_KEY_NUMPAD_EQUAL;
		case XK_Shift_L: return CP_KEY_LSHIFT;
		case XK_Shift_R: return CP_KEY_RSHIFT;
		case XK_Control_L: return CP_KEY_LCONTROL;
		case XK_Control_R: return CP_KEY_RCONTROL;
		case XK_Alt_L: return CP_KEY_LALT;
		case XK_Alt_R: return CP_KEY_RALT;
		case XK_semicolon: return CP_KEY_SEMICOLON;
		case XK_plus: return CP_KEY_PLUS;
		case XK_comma: return CP_KEY_COMMA;
		case XK_minus: return CP_KEY_MINUS;
		case XK_period: return CP_KEY_PERIOD;
		case XK_slash: return CP_KEY_FORWARD_SLASH;
		case XK_grave: return CP_KEY_GRAVE;
		case XK_a:
		case XK_A: return CP_KEY_A;
		case XK_b:
		case XK_B: return CP_KEY_B;
		case XK_c:
		case XK_C: return CP_KEY_C;
		case XK_d:
		case XK_D: return CP_KEY_D;
		case XK_e:
		case XK_E: return CP_KEY_E;
		case XK_f:
		case XK_F: return CP_KEY_F;
		case XK_g:
		case XK_G: return CP_KEY_G;
		case XK_h:
		case XK_H: return CP_KEY_H;
		case XK_i:
		case XK_I: return CP_KEY_I;
		case XK_j:
		case XK_J: return CP_KEY_J;
		case XK_k:
		case XK_K: return CP_KEY_K;
		case XK_l:
		case XK_L: return CP_KEY_L;
		case XK_m:
		case XK_M: return CP_KEY_M;
		case XK_n:
		case XK_N: return CP_KEY_N;
		case XK_o:
		case XK_O: return CP_KEY_O;
		case XK_p:
		case XK_P: return CP_KEY_P;
		case XK_q:
		case XK_Q: return CP_KEY_Q;
		case XK_r:
		case XK_R: return CP_KEY_R;
		case XK_s:
		case XK_S: return CP_KEY_S;
		case XK_t:
		case XK_T: return CP_KEY_T;
		case XK_u:
		case XK_U: return CP_KEY_U;
		case XK_v:
		case XK_V: return CP_KEY_V;
		case XK_w:
		case XK_W: return CP_KEY_W;
		case XK_x:
		case XK_X: return CP_KEY_X;
		case XK_y:
		case XK_Y: return CP_KEY_Y;
		case XK_z:
		case XK_Z: return CP_KEY_Z;
        default: 
            CP_log_trace("Key 0x%x not found", keysym);
            return CP_KEY_MAX_KEYS;
    }
}

#endif // CP_LINUX
