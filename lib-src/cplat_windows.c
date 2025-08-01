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
#ifdef CP_WIN32

LRESULT CALLBACK WIN32_processMessage(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);

CP_ERROR CP_create_window(CP_Window*const window, const CP_WindowConfig* const config)
{
    CP_log_trace("entering CP_create_window");

    if(config == NULL)
    {
        return CP_ERROR_NULL_CONFIG;
    }

    if(window == NULL)
    {
        return CP_ERROR_NULL_CONFIG;
    }
    
    // get the HINSTANCE
    window->hinst = GetModuleHandleA(0);

    if(window->hinst == NULL)
    {
        CP_log_fatal("failed to get HINSTANCE");
        return CP_ERROR_OS_CALL_FAILED;
    }
    
    // Setting up the window class
    WNDCLASSA wc = {0};

    wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WIN32_processMessage;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = window->hinst;
	wc.hIcon = LoadIcon(window->hinst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(window->hinst, IDC_ARROW);
	wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCSTR)config->windowName;

    if(!RegisterClassA(&wc))
    {
        CP_log_fatal("Failed to register window class - %u", GetLastError());
        return CP_ERROR_OS_CALL_FAILED;
    }
    
    // Setting window options
    RECT borderRect = {0,0,0,0};
    const uint32_t windowStyle = WS_OVERLAPPED | WS_SYSMENU;
    const uint32_t windowExStyle = WS_EX_APPWINDOW | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

    AdjustWindowRectEx(
        &borderRect, 
        windowStyle, 
        0, 
        windowExStyle
    );

    // creating the window
    window->hwnd = CreateWindowExA(
        windowExStyle, 
        (LPCSTR)config->windowName,
        (LPCSTR)config->windowName,
        windowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (int)config->width,
        (int)config->height,
        NULL,
        NULL,
        window->hinst,
        NULL
    );

    if(window->hwnd == NULL)
    {
        CP_log_fatal("Failed to create window - %u", GetLastError());
        return CP_ERROR_OS_CALL_FAILED;
    }

    ShowWindow(window->hwnd, SW_SHOW);

    CP_log_trace("Leaving CP_create_window");
    return CP_ERROR_SUCCESS;
}

void CP_destroy_window(CP_Window*const window)
{
    CP_log_trace("Entering CP_destroy_window");

    if(window->hwnd != NULL)
    {
        DestroyWindow(window->hwnd);
        window->hwnd = NULL;
        window->hinst = NULL;
    }

    CP_log_trace("Leaving CP_destroy_window");
}

CP_WindowEvent CP_get_next_event(CP_Window*const window)
{
    MSG message;
    CP_WindowEvent event = {0};

    SetWindowLongPtrA(window->hwnd, GWLP_USERDATA, (long long)&event);
    
    if(PeekMessageA(&message, window->hwnd, 0, 0, PM_REMOVE) != 0)
    {
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }

    return event;
}

LRESULT CALLBACK WIN32_processMessage(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam)
{

    CP_WindowEvent* event = (CP_WindowEvent*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);

    switch(msg)
    {
        case WM_ERASEBKGND:
            return 1;
        case WM_CLOSE:
        {
            event->type= CP_EVENT_QUIT;
            CP_log_info("close event");
            return 0;
        }
        case WM_DESTROY:
        {
            CP_log_info("posting quit message");
            PostQuitMessage(0);
            return 0;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            event->type = CP_EVENT_KEYDOWN;
            event->key = (uint32_t)wparam;
            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            event->type = CP_EVENT_KEYDOWN;
            event->key = (uint32_t)wparam;
            break;
        }
        case WM_MOUSEMOVE:
        {
            event->type = CP_EVENT_MOUSEMOVE;
            event->mx = (uint16_t)GET_X_LPARAM(lparam);
            event->my = (uint16_t)GET_Y_LPARAM(lparam);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            event->type = CP_EVENT_MOUSEWHEEL;
            int zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
            if(zDelta != 0)
            {
                event->mWheel = (zDelta < 0) ? -1 : 1;
            }
            break;
        }
        case WM_LBUTTONDOWN: 
        {
            event->type = CP_EVENT_LBUTTONDOWN;
            break;
        }
        case WM_MBUTTONDOWN: 
        {
            event->type = CP_EVENT_MBUTTONDOWN;
            break;
        }
        case WM_RBUTTONDOWN: 
        {
            event->type = CP_EVENT_RBUTTONDOWN;
            break;
        }
        case WM_LBUTTONUP: 
        {
            event->type = CP_EVENT_LBUTTONUP;
            break;
        }
        case WM_MBUTTONUP: 
        {
            event->type = CP_EVENT_MBUTTONUP;
            break;
        }
        case WM_RBUTTONUP: 
        {
            event->type = CP_EVENT_RBUTTONUP;
            break;
        }
        default: break;
    }

    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

#endif // CP_WIN32
