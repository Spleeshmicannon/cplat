#define CP_ASSERT_ENABLED
#define CP_ABORT_ON_ASSERT_FAIL
#include <cplat.h>

int main()
{
    if(!CP_init())
    {
        CP_log_fatal("CP initialise failed");
        return -1;
    } 

    CP_Window* window = nullptr;
    CP_WindowConfig config = {
        .width = 1080,
        .height = 1920,
        .x = 100,
        .y = 100 };

    CP_create_window(window, &config);
    if(nullptr == window)
    {
        CP_log_fatal("CP failed to create window");
        return -2;
    }

    CP_destroy_window(window);
    CP_deinit();
}
