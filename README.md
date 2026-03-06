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
- [x] OpenGL context initialisation on Windows
- [x] OpenGL context initialisation on Linux
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

    for(CP_WindowEvent event = CP_getNextEvent(&window);
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

### Aiming to be minimalistic 
I aim for Cplat to be minimalistic. SDL will get you to choose an x,y coordinate for instance but
I don't give you that option here. There are some edge cases I'm sure where that matters and if
someone asks for it I'll add it, but I think it adds unecessary complexity at this stage at least.

So given that I'm aiming for that. What use cases am I minimising for?
I'm mostly making this for:
 - Games
 - Rendering Demos
 - Basic Desktop Applications

### What did I develope this on?
I've developed this on Arch Linux using DWM as my desktop environment. That said, I've tested
the windows version of this extensively with wine (with an exception for the threading stuff).

I've used a mixture of Bazel 9.0.0, CMake 4.2.3, gcc 15.2.1 to build and compile applications using this library.

I'm currently developing a task manager application that will only run on linux and a simple
game that will target windows and linux. I'm hoping these applications will help me iron out
some of the kinks in the library.
