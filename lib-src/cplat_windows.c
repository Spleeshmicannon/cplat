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

#include <windows.h>

#include <stdint.h>

struct st_cp_window
{
    HINSTANCE hinst;
    HWND hwnd;
};

LRESULT CALLBACK WIN32_processMessage(HWND hwnd, uint32_t msg, WPARAM wparam, LPARAM lparam);

CP_ERROR CP_create_window(CP_Window** window, const CP_WindowConfig* const config, CP_MemPool *pool)
{
    CP_Window* localWin = NULL;

    CP_log_trace("entering CP_create_window");

    if(config == NULL)
    {
        return CP_ERROR_NULL_CONFIG;
    }

    if(window == NULL)
    {
        return CP_ERROR_NULL_CONFIG;
    }
    
    // allocating memory for the window object (using pool if available)
    if(pool == NULL)
    {
        localWin = (CP_Window*)CP_allocate(sizeof(CP_Window));
    }
    else
    {
        localWin = (CP_Window*)CP_PoolAllocate(pool, sizeof(CP_Window));
    }

    if(localWin == NULL)
    {
        CP_log_fatal("Failed to allocate window memory");
        return CP_ERROR_ALLOCATION;
    }
    
    // setting local win data to 0, 
    // since values are checked against NULL
    memset(localWin, 0, sizeof(CP_Window));
    
    // get the HINSTANCE
    localWin->hinst = GetModuleHandleA(0);
    if(localWin->hinst == NULL)
    {
        CP_log_fatal("failed to get HINSTANCE");
        return CP_ERROR_OS_CALL_FAILED;
    }
    
    // Setting up the window class
    WNDCLASSA wc = {0};

    const char* className = "class_test";

    wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WIN32_processMessage;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = localWin->hinst;
	wc.hIcon = LoadIcon(localWin->hinst, IDI_APPLICATION);
	wc.hCursor = LoadCursor(localWin->hinst, IDC_ARROW);
	wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
	wc.lpszClassName = (LPCSTR)className;

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
    localWin->hwnd = CreateWindowExA(
        windowExStyle, 
        (LPCSTR)className,
        (LPCSTR)config->windowName,
        windowStyle,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (int)config->width,
        (int)config->height,
        NULL,
        NULL,
        localWin->hinst,
        NULL
    );

    if(localWin->hwnd == NULL)
    {
        CP_log_fatal("Failed to create window - %u", GetLastError());
        return CP_ERROR_OS_CALL_FAILED;
    }

    ShowWindow(localWin->hwnd, SW_SHOW);

    // passing the window back to the caller
    *window = localWin;

    CP_log_trace("Leaving CP_create_window");

    return CP_ERROR_SUCCESS;
}

void CP_destroy_window(CP_Window* window, CP_MemPool* pool)
{
    CP_log_trace("Entering CP_destroy_window");

    if (window == NULL)
    {
        CP_log_fatal("Passed NULL window to CP_destroy_window");
        return;
    }

    if(window->hwnd != NULL)
    {
        DestroyWindow(window->hwnd);
        window->hwnd = NULL;
    }
    window->hinst = NULL;

    if(pool == NULL)
    {
        CP_free(window);
    }

    CP_log_trace("Leaving CP_destroy_window");
}

CP_WindowEvent CP_get_next_event(CP_Window* window)
{
    MSG message;

    CP_WindowEvent event = {0};

    SetWindowLongPtrA(window->hwnd, GWLP_USERDATA, (long long)&event);
    
    while(PeekMessageA(&message, window->hwnd, 0, 0, PM_REMOVE) != 0)
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
            event->event = CP_EVENT_QUIT;
            CP_log_trace("close event");
            return 0;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }
    }

    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

#endif // CP_WIN32
