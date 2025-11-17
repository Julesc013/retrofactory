#ifndef PLATFORM_PLAT_API_H
#define PLATFORM_PLAT_API_H

#include "system/system_base.h"

struct plat_services
{
    void (*log_message)(const char *message);
    void (*sleep_ms)(uint32 milliseconds);
};

struct PlatSurface
{
    void *pixels;
    int width;
    int height;
    int pitch;
};

struct PlatWindow;

bool plat_init(const plat_services &services);
void plat_shutdown();

PlatWindow *plat_window_create(int width, int height);
void plat_window_destroy(PlatWindow *window);
bool plat_window_pump_events(PlatWindow *window);
PlatSurface plat_window_get_surface(PlatWindow *window);
void plat_window_present(PlatWindow *window, const PlatSurface &surface);
void plat_window_request_close(PlatWindow *window);
void plat_sleep_ms(uint32 milliseconds);

#endif /* PLATFORM_PLAT_API_H */
