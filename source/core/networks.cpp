#include "core/networks.h"

void networks_init(NetworkState &state)
{
    state.power_networks = 0u;
    state.fluid_networks = 0u;
}

void networks_shutdown(NetworkState &state)
{
    state.power_networks = 0u;
    state.fluid_networks = 0u;
}
