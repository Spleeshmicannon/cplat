#include "cplat.h"
#ifdef CP_LINUX

#include <stdio.h>
#include <stdlib.h>

static struct 
{
    
} platformState;

bool CP_init()
{
    return true;
}

void CP_deinit()
{

}

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



#endif // CP_LINUX
