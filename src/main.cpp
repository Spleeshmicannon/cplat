#define CP_ASSERT_ENABLED
#define CP_ABORT_ON_ASSERT_FAIL
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
