#define CP_ASSERT_ENABLED
#define CP_ABORT_ON_ASSERT_FAIL
#include <cplat.h>

int main()
{
    CP_Window window;
    const CP_WindowConfig config = {
        .width = 1920,
        .height = 1080,
        .flags = CP_WINDOW_FLAGS_INIT_OPENGL,
        .major = 4,
        .minor = 3,
        .windowName = "test"
    };

    CP_ERROR error = CP_createWindow(&window, &config);
    if(error != CP_ERROR_SUCCESS)
    {
        CP_log_fatal("CP failed to create window");
        return -2;
    }
    
    const int64_t incrementNanos = CP_NANOS_IN_ONE*3;
    int64_t nanoSeconds = CP_getNanos();
    for(CP_WindowEvent event = CP_getNextEvent(&window); // zero initialise
        event.type != CP_EVENT_QUIT; // end loop if user quits
        event = CP_getNextEvent(&window)) // get next event
    {
        const int64_t nanos = CP_getNanos();
        if((nanoSeconds / incrementNanos) < (nanos / incrementNanos))
        {
            nanoSeconds = nanos;
            CP_log_info("3s interval: %lld nanos | %lld micros | %lld millis | %lld seconds", 
                    nanos, CP_getMicros(), CP_getMillis(), CP_getSeconds());
        }
    }

    CP_destroyWindow(&window);
}
