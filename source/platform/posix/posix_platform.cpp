#include "platform/plat_api.h"
#include "platform/plat_ids.h"

bool plat_init(const plat_services &services)
{
    (void)services;
    return true;
}

void plat_pump_events()
{
}

void plat_shutdown()
{
}
