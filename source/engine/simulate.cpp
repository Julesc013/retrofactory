#include "engine/simulate.h"

bool simulate_headless(EngineContext &ctx, u32 ticks)
{
    u32 i;
    for (i = 0u; i < ticks; ++i)
    {
        if (!engine_tick(ctx))
        {
            return false;
        }
    }
    return true;
}
