#include "core/networks.h"

int main()
{
    NetworkState state;
    networks_init(state);

    NetworkId a = networks_create_power(state, 50u);
    NetworkId b = networks_create_fluid(state, 25u);
    if (a == 0u || b == 0u)
    {
        networks_shutdown(state);
        return 1;
    }

    networks_tick(state, 10u);
    if (state.power.size == 0u || state.fluid.size == 0u)
    {
        networks_shutdown(state);
        return 1;
    }

    if (state.power.data[0].load > state.power.data[0].capacity ||
        state.fluid.data[0].load > state.fluid.data[0].capacity)
    {
        networks_shutdown(state);
        return 1;
    }

    networks_shutdown(state);
    return 0;
}
