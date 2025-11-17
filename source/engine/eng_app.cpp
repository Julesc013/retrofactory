#include "engine/eng_app.h"

#include "core/core_api.h"
#include "runtime/rt_config_paths.h"
#include "runtime/rt_log.h"
#include "save/state_hash.h"

namespace
{
    const uint64 kEngineSeed = 0x123456789ULL;
    const uint32 kEngineTicks = 600u;

    void log_runtime_paths(const RuntimePaths &paths)
    {
        log_info("Runtime paths (POSIX defaults):");
        log_info(paths.base_data_dir);
        log_info(paths.user_config_dir);
        log_info(paths.user_mods_dir);
        log_info(paths.user_saves_dir);
    }
}

int engine_main_loop()
{
    log_info("Engine main loop starting");

    RuntimePaths paths = compute_runtime_paths();
    log_runtime_paths(paths);

    CoreConfig config;
    config.initial_seed = kEngineSeed;

    CoreState state;
    if (!core_init(state, config))
    {
        log_error("core_init failed");
        return 1;
    }

    uint32 tick_index;
    for (tick_index = 0u; tick_index < kEngineTicks; ++tick_index)
    {
        if (!core_tick(state))
        {
            log_error("core_tick failed");
            core_shutdown(state);
            return 1;
        }
    }

    const uint64 hash = compute_state_hash(state);
    (void)hash;

    core_shutdown(state);
    log_info("Engine main loop exiting");
    return 0;
}
