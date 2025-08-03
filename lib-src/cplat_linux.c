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
#include <xcb/xcb_icccm.h>

CP_ERROR CP_createWindow(CP_Window*const window, const CP_WindowConfig* const config)
{
    int screen_count = 0;
    memset(window, 0, sizeof(CP_Window));
    
    // grab X11 proto connection
    window->connection = xcb_connect(NULL, &screen_count);
    if(window->connection == NULL)
    {
        return CP_ERROR_OS_CALL_FAILED;
    }
    
    // get the default screen
    window->screen = xcb_aux_get_screen(window->connection, screen_count);
    if(window->screen == NULL)
    {
        return CP_ERROR_OS_CALL_FAILED;
    }
    
    // get the window id
    window->windowId = xcb_generate_id(window->connection);
    if(window->windowId == 0)
    {
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
    
    xcb_create_window(
        window->connection, 
        0,
        window->windowId,
        window->screen->root,
        (int16_t)config->x,
        (int16_t)config->y,
        config->width,
        config->height,
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

    // setting window size (not resizable)
    xcb_size_hints_t window_size_hints;
    xcb_icccm_size_hints_set_min_size(&window_size_hints, config->width, config->height);
    xcb_icccm_size_hints_set_max_size(&window_size_hints, config->width, config->height);
    xcb_icccm_set_wm_size_hints(window->connection, window->windowId, XCB_ATOM_WM_NORMAL_HINTS, &window_size_hints);
    
    // showing the window
    xcb_map_window(window->connection, window->windowId);
    if(xcb_flush(window->connection) <= 0)
    {
        return CP_ERROR_OS_CALL_FAILED;
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

    switch(XCB_EVENT_RESPONSE_TYPE(xcb_event))
    {
        case XCB_CLIENT_MESSAGE:
        {
            xcb_client_message_event_t* client_message_event = (xcb_client_message_event_t*)xcb_event;
            if((client_message_event->window == window->windowId) && 
               (client_message_event->data.data32[0] == window->wmDeleteProtocol))
            {
                event.type = CP_EVENT_QUIT;
            }
            break;
        }
        default:break;
    }


    return event;
}

void CP_destroyWindow(CP_Window*const window)
{
    xcb_destroy_window(window->connection, window->windowId);
    xcb_disconnect(window->connection);
}

#endif // CP_LINUX
