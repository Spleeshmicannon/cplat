# CPLAT (C Platform Library)
This is a platform library written in c99. It's made to be simple to use, while giving easy acces
to internals from whatever platform is being targeted.

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
Going to the src/main.cpp file you'll see the following:
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

    CP_ERROR error = CP_create_window(&window, &config);
    if(error != CP_ERROR_SUCCESS)
    {
        CP_log_fatal("CP failed to create window");
        return -2;
    }

    for(CP_WindowEvent event = {}; // zero initialise
        event.type != CP_EVENT_QUIT; // end loop if user quits
        event = CP_get_next_event(&window)) // get next event
    {

    }

    CP_destroy_window(&window);
}
```
