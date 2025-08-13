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
- [x] (optional) OpenGL context initialisation on Windows
- [ ] (optional) OpenGL context initialisation on Linux
- [ ] (optional) Vulkan context initialisation on Windows
- [ ] (optional) Vulkan context initialisation on Linux
- [ ] Polish and extensions for weird keys and controllers


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
Cplat exposes the native api by directly exposing the data in CP_Window. This makes the library
easy to integrate with other software and easy to extend.
There is a tradeoff here, in that if you access the struct fields in your code it will no longer
be portable. That said, you can add `CP_WIN32/CP_LINUX` in `#ifdefs` to make those sections portable.
