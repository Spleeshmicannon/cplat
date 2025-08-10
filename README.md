# CPLAT (C Platform Library)
This is a platform library written in c99. It's made to be simple to use, while giving easy acces
to internals from whatever platform is being targeted.

> [!WARNING]
> This is still a work in progress, do not expect this to be useful yet.

The current development progress is thus:
- [x] Get window opening/closing working on Windows
- [x] Get window opening/closing working on Linux
- [x] Get window event management working on Windows
- [x] Get window event management working on Linux
- [ ] Bug Fixes and extensions for weird keys and controllers


## Building
You can build with the following cmake commands, assuming your in a 
build subdirectory.
```bash
cmake ..
cmake --build .
```
if you want to cross compile with mingw you can do the following:
```bash
cmake .. -DCMAKE_TOOLCHAIN_FILE=../mingw_toolchain.cmake
cmake --build .
```

## Usage
```CPP
#include <cplat.h>

int main()
{
    CP_Window window;
    CP_WindowConfig config = {
        .width = 1920,
        .height = 1080,
        .x = 100,
        .y = 100,
        .windowName = "test"
    };

    CP_ERROR error = CP_createWindow(&window, &config);
    if(error != CP_ERROR_SUCCESS)
    {
        CP_log_fatal("CP failed to create window");
        return -2;
    }

    for(CP_WindowEvent event = CP_getNextEvent(&window); // zero initialise
        event.type != CP_EVENT_QUIT; // end loop if user quits
        event = CP_getNextEvent(&window)) // get next event
    {
    }

    CP_destroyWindow(&window);
}
```

## Explanation of implementation
Here I'm just going to go into some detail about design decisions.

### Exposed the native window APIs
This code excerpt is from `cplat.h` and directly exposes the data in CP_Window (typdefed).
There is a tradeoff here, in that if you access the struct fields in your code it will no longer
be portable. That said, you can add `CP_WIN32/CP_LINUX` and `#ifdefs` to make those sections portable.
If I didn't do this CP_Window (if exposed at all) could only be used as a pointer by user code meaning
that you either need to point to struct in a CPP file (harder to have multiple windows) or you do a
heap allocation (not super efficient).
```CPP
typedef struct st_cp_window CP_Window;

#ifdef CP_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>

struct st_cp_window
{
    HINSTANCE hinst;
    HWND hwnd;
};
#elif defined(CP_LINUX)
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
#endif
```

