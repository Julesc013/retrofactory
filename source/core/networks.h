#pragma once

#include "system/types.h"

struct NetworkState
{
    u32 power_networks;
    u32 fluid_networks;
};

void networks_init(NetworkState &state);
void networks_shutdown(NetworkState &state);
