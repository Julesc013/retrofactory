#include "core/core_api.h"

bool core_init(CoreState &state, const CoreConfig &config)
{
    state.reserved = config.reserved;
    return true;
}

bool core_tick(CoreState &state)
{
    ++state.reserved;
    return true;
}

void core_shutdown(CoreState &state)
{
    state.reserved = 0u;
}
