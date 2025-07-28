#ifdef __linux__
#include "cplat.h"
#include <stdio.h>

CP_ERROR CP_create_window(CP_Window* window, const CP_WindowConfig* const config)
{
    (void)window;
    (void)config;
    return CP_ERROR_SUCCESS;
}

void CP_destroy_window(CP_Window* window)
{
    window = NULL;
    (void)window;
}

#endif // __linux__
