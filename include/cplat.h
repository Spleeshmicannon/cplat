#ifndef CPLAT_H
#define CPLAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cplat/macros.h"
#include "cplat/logger.h"
#include "cplat/asserts.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct priv_cp_window CP_Window;

typedef struct 
{
    uint16_t width, height, x, y;
} 
CP_WindowConfig;

typedef enum 
{
    CP_ERROR_SUCCESS
} 
CP_ERROR;

CP_ERROR CP_create_window(CP_Window* window, const CP_WindowConfig* const config);
void CP_destroy_window(CP_Window* window);

#ifdef __cplusplus
}
#endif

#endif // CPLAT_H
