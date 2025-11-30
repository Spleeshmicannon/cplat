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
CP_ERROR cp_setOpenGLVersion(CP_Window*const window, int majorVersion, int minorVersion);

void CP_getScreenWH(const CP_Window*const window, int*const width, int*const height)
{
    (void)window;
    if(NULL != width) *width = GetSystemMetrics(SM_CXSCREEN);
    if(NULL != height) *height = GetSystemMetrics(SM_CYSCREEN);
}

CP_ERROR CP_createWindow(CP_Window*const window, const CP_WindowConfig* const config)
{
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
    uint32_t windowStyle = WS_OVERLAPPED | WS_SYSMENU;
    
    if(!(config->flags & CP_WINDOW_FLAGS_BORDERLESS))
    {
        windowStyle |= WS_BORDER;
    }

    int window_width = config->width;
    int window_height = config->height;

    if(config->flags & CP_WINDOW_FLAGS_FULLSCREEN)
    {
        window_width = GetSystemMetrics(SM_CXSCREEN);
        window_height = GetSystemMetrics(SM_CYSCREEN);
    }

    uint32_t windowExStyle = WS_EX_APPWINDOW | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

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
        window_width,
        window_height,
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

    if(config->flags & CP_WINDOW_FLAGS_MOUSE_CAP)
    {
        SetCapture(window->hwnd);
    }

    if(config->flags & CP_WINDOW_FLAGS_INIT_OPENGL)
    {
        window->opengl.device = GetDC(window->hwnd);
        
        PIXELFORMATDESCRIPTOR pfd = {0};
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cAlphaBits = 0;
		pfd.cAccumBits = 0;
		pfd.cDepthBits = 0;
		pfd.cStencilBits = 0;
		pfd.cAuxBuffers = 0;
		pfd.iLayerType = PFD_MAIN_PLANE;

        SetPixelFormat(window->opengl.device, ChoosePixelFormat(window->opengl.device, &pfd), &pfd);
        window->opengl.context = wglCreateContext(window->opengl.device);

        if(!wglMakeCurrent(window->opengl.device, window->opengl.context))
        {
            CP_log_fatal("Failed to make gl context current");
            CP_destroyWindow(window);
            return CP_ERROR_OS_CALL_FAILED;
        }
        
        if(config->major > 0)
        {
            return CP_setOpenGLVersion(window, config->major,config->minor);
        }

    }

    return CP_ERROR_SUCCESS;
}

CP_ERROR CP_setOpenGLVersion(CP_Window*const window, int majorVersion, int minorVersion)
{
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;

    wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
        (void*)wglGetProcAddress("wglCreateContextAttribsARB");

    if (!wglCreateContextAttribsARB) {
        CP_log_error("Failed to retrieve context attributions function")
        return CP_ERROR_OS_CALL_FAILED;
    }

    const int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, majorVersion,
        WGL_CONTEXT_MINOR_VERSION_ARB, minorVersion,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB, 
        0
    };

    const HGLRC context = wglCreateContextAttribsARB(window->opengl.device, 0, attribs);
    
    if(!wglMakeCurrent(window->opengl.device, context))
    {
        CP_log_error("Failed to make gl context current", majorVersion, minorVersion);
        wglDeleteContext(context);
        return CP_ERROR_OS_CALL_FAILED;
    }

    window->opengl.context = context;
    
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

    return CP_ERROR_SUCCESS; 
}

void CP_destroyWindow(CP_Window*const window)
{
    if(window->hwnd != NULL)
    {
        if(window->opengl.context != NULL)
        {
            wglMakeCurrent(window->opengl.device, NULL);
            ReleaseDC(window->hwnd, window->opengl.device);
            wglDeleteContext(window->opengl.context);
        }

        DestroyWindow(window->hwnd);
        window->hwnd = NULL;
        window->hinst = NULL;
    }
}

void* CP_getGLProcAddress()
{
    return wglGetProcAddress;
}

CP_WindowEvent CP_getNextEvent(CP_Window*const window)
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
            CP_log_trace("WM_ERASEBKGND event");
            return 1;
        case WM_CLOSE:
        {
            CP_log_trace("WM_CLOLSE event");
            event->type= CP_EVENT_QUIT;
            return 0;
        }
        case WM_DESTROY:
        {
            CP_log_trace("WM_DESTROY event");
            PostQuitMessage(0);
            return 0;
        }
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
        {
            event->type = CP_EVENT_KEYDOWN;
            switch((uint32_t)wparam)
            {
                case VK_ACCEPT: event->key = CP_KEY_EXECUTE; break;
                case VK_SNAPSHOT: event->key = CP_KEY_PRINT; break;
                case VK_SHIFT: event->key = CP_KEY_LSHIFT; break;
                case VK_CONTROL: event->key = CP_KEY_LCONTROL; break;
                case VK_MENU: event->key = CP_KEY_LALT; break;
                default: event->key = (uint32_t)wparam; break;
            }
            
            CP_log_trace("key press of %s", CP_keyToString(event->key));

            break;
        }
        case WM_KEYUP:
        case WM_SYSKEYUP:
        {
            event->type = CP_EVENT_KEYDOWN;
            switch((uint32_t)wparam)
            {
                case VK_ACCEPT: event->key = CP_KEY_EXECUTE; break;
                case VK_SNAPSHOT: event->key = CP_KEY_PRINT; break;
                case VK_SHIFT: event->key = CP_KEY_LSHIFT; break;
                case VK_CONTROL: event->key = CP_KEY_LCONTROL; break;
                case VK_MENU: event->key = CP_KEY_LALT; break;
                default: event->key = (uint32_t)wparam; break;
            }

            CP_log_trace("key press of %s", CP_keyToString(event->key));

            break;
        }
        case WM_MOUSEMOVE:
        {
            event->type = CP_EVENT_MOUSEMOVE;
            event->mx = (uint16_t)GET_X_LPARAM(lparam);
            event->my = (uint16_t)GET_Y_LPARAM(lparam);
            CP_log_trace("Mouse move to %d,%d", event->mx, event->my);
            break;
        }
        case WM_MOUSEWHEEL:
        {
            int zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
            if(zDelta != 0)
            {
                event->type = CP_EVENT_MOUSEWHEEL;
                event->mWheel = (zDelta < 0) ? -1 : 1;
#ifdef CP_DEBUG
                if(event->mWheel == 1)
                {
                    CP_log_trace("Mouse Wheel Up");
                }
                else
                {
                    CP_log_trace("Mouse Wheel Down");
                }
#endif
            }
            break;
        }
        case WM_LBUTTONDOWN: 
        {
            event->type = CP_EVENT_LBUTTONDOWN;
            CP_log_trace("CP_EVENT_LBUTTONDOWN event");
            break;
        }
        case WM_MBUTTONDOWN: 
        {
            event->type = CP_EVENT_MBUTTONDOWN;
            CP_log_trace("CP_EVENT_MBUTTONDOWN event");
            break;
        }
        case WM_RBUTTONDOWN: 
        {
            event->type = CP_EVENT_RBUTTONDOWN;
            CP_log_trace("CP_EVENT_RBUTTONDOWN event");
            break;
        }
        case WM_LBUTTONUP: 
        {
            event->type = CP_EVENT_LBUTTONUP;
            CP_log_trace("CP_EVENT_LBUTTONUP event");
            break;
        }
        case WM_MBUTTONUP: 
        {
            event->type = CP_EVENT_MBUTTONUP;
            CP_log_trace("CP_EVENT_MBUTTONUP event");
            break;
        }
        case WM_RBUTTONUP: 
        {
            event->type = CP_EVENT_RBUTTONUP;
            CP_log_trace("CP_EVENT_RBUTTONUP event");
            break;
        }
        default: break;
    }

    return DefWindowProcA(hwnd, msg, wparam, lparam);
}

#endif // CP_WIN32
