#include <cstdio>

#include "core/core_api.h"
#include "save/save.h"
#include "save/state_hash.h"

namespace
{
    const uint64 kTestSeed = 0xCAFEBABEULL;
    const uint32 kTestTicks = 500u;
    const char kTestPath[] = "test_state.rfs";
}

int main()
{
    CoreConfig config;
    config.initial_seed = kTestSeed;

    CoreState original_state;
    if (!core_init(original_state, config))
    {
        return 1;
    }

    uint32 tick_index;
    for (tick_index = 0u; tick_index < kTestTicks; ++tick_index)
    {
        if (!core_tick(original_state))
        {
            return 1;
        }
    }

    const uint64 hash_before = compute_state_hash(original_state);

    if (!save_core_state(original_state, kTestPath))
    {
        return 1;
    }

    CoreState loaded_state;
    if (!load_core_state(loaded_state, kTestPath))
    {
        std::remove(kTestPath);
        return 1;
    }

    const uint64 hash_after = compute_state_hash(loaded_state);
    std::remove(kTestPath);

    if (original_state.tick != loaded_state.tick)
    {
        return 1;
    }

    if (original_state.rng.state != loaded_state.rng.state)
    {
        return 1;
    }

    if (hash_before != hash_after)
    {
        return 1;
    }

    return 0;
}
