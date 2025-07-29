#include "cplat.h"
#ifdef CP_WIN32

#include <windows.h>

bool CP_init()
{
    return true;
}

void CP_deinit()
{
}

CP_ERROR CP_create_window(CP_Window* window, const CP_WindowConfig* const config)
{
    


    return CP_ERROR_SUCCESS;
}

void CP_destroy_window(CP_Window* window)
{

}




#endif // CP_WIN32
