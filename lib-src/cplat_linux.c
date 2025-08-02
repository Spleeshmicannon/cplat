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

CP_ERROR CP_create_window(CP_Window*const window, const CP_WindowConfig* const config)
{
    int screen_count = 0;
    memset(window, 0, sizeof(CP_Window));

    window->connection = xcb_connect(NULL, &screen_count);
    if(window->connection == NULL)
    {
        return CP_ERROR_OS_CALL_FAILED;
    }

    window->screen = xcb_aux_get_screen(window->connection, screen_count);
    if(window->screen == NULL)
    {
        return CP_ERROR_OS_CALL_FAILED;
    }

    window->windowId = xcb_generate_id(window->connection);
    if(window->windowId == 0)
    {
        return CP_ERROR_OS_CALL_FAILED;
    }
    
    const char* window_manager_protocol_name = "Window Protocl";
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

    intern_atom_cookie = xcb_intern_atom(
        window->connection, 
        true, 
        window_manager_protocol_name_size, 
        window_manager_protocol_name
    );
    intern_atom_reply = xcb_intern_atom_reply(
        window->connection, 
        intern_atom_cookie, 
        NULL
    );
    xcb_atom_t window_manager_window_delete_protocol = intern_atom_reply->atom;
    free(intern_atom_reply);
    
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
        XCB_EVENT_MASK_NO_EVENT,
        NULL
    );

    xcb_icccm_set_wm_name(window->connection, window->windowId, XCB_ATOM_STRING, sizeof(char), (uint16_t)strlen(config->windowName), config->windowName);
    xcb_icccm_set_wm_protocols(window->connection, window->windowId, window_manager_protocols_property, 1, &window_manager_window_delete_protocol);

    xcb_map_window(window->connection, window->windowId);
    xcb_flush(window->connection);

    return CP_ERROR_SUCCESS;
}

CP_WindowEvent CP_get_next_event(CP_Window*const window)
{
    (void)window;
    return (CP_WindowEvent){CP_EVENT_QUIT, {0}};
}

void CP_destroy_window(CP_Window*const window)
{
    xcb_destroy_window(window->connection, window->windowId);
    xcb_disconnect(window->connection);
}

#endif // CP_LINUX
