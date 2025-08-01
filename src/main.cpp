#define CP_ASSERT_ENABLED
#define CP_ABORT_ON_ASSERT_FAIL
#include <cplat.h>

int main()
{
    CP_MemPool* pool = nullptr;
    CP_Window* window = nullptr;
    CP_WindowConfig config = {
        .width = 1920,
        .height = 1080,
        .x = 100,
        .y = 100,
        .windowName = "test"
    };

    pool = CP_PoolCreate(1024);

    CP_create_window(&window, &config, pool);
    if(nullptr == window)
    {
        CP_log_fatal("CP failed to create window");
        CP_PoolDestory(pool);
        return -2;
    }

    while(CP_EVENT_QUIT != CP_get_next_event(window).event);

    CP_destroy_window(window, pool);

    CP_PoolDestory(pool);
}
