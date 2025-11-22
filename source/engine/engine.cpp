#include "engine/engine.h"

bool engine_init(EngineContext &ctx, const char *config_path)
{
    engine_state_init(ctx.state);
    if (!engine_config_init(ctx.config, config_path))
    {
        return false;
    }

    ctx.core_config.initial_seed = 1u;
    if (!core_init(ctx.core_state, ctx.core_config))
    {
        return false;
    }

    engine_state_set_phase(ctx.state, EnginePhase_Running);
    return true;
}

bool engine_tick(EngineContext &ctx)
{
    if (ctx.state.phase != EnginePhase_Running)
    {
        return false;
    }
    return core_tick(ctx.core_state);
}

void engine_shutdown(EngineContext &ctx)
{
    core_shutdown(ctx.core_state);
    engine_state_set_phase(ctx.state, EnginePhase_Shutdown);
}
