#include "engine/eng_app.h"

#include "core/core_api.h"
#include "save/state_hash.h"

namespace
{
    const uint64 kEngineSeed = 0x123456789ULL;
    const uint32 kEngineTicks = 600u;
}

int engine_main_loop()
{
    CoreConfig config;
    config.initial_seed = kEngineSeed;

    CoreState state;
    if (!core_init(state, config))
    {
        return 1;
    }

    uint32 tick_index;
    for (tick_index = 0u; tick_index < kEngineTicks; ++tick_index)
    {
        if (!core_tick(state))
        {
            core_shutdown(state);
            return 1;
        }
    }

    const uint64 hash = compute_state_hash(state);
    (void)hash;

    core_shutdown(state);
    return 0;
}
