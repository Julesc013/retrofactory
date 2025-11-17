#ifndef PLATFORM_PLAT_API_H
#define PLATFORM_PLAT_API_H

#include "system/system_base.h"

struct plat_services
{
    void (*log_message)(const char *message);
    void (*sleep_ms)(uint32 milliseconds);
};

bool plat_init(const plat_services &services);
void plat_pump_events();
void plat_shutdown();

#endif /* PLATFORM_PLAT_API_H */
