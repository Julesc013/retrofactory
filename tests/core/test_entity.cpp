#include "core/core.h"

int main()
{
    CoreConfig cfg;
    cfg.initial_seed = 42u;

    CoreState state;
    if (!core_init(state, cfg))
    {
        return 1;
    }

    const EntityId id = entities_spawn(state.entities, state.world, 2u, 1u, 1u);
    if (id == 0u)
    {
        core_shutdown(state);
        return 1;
    }

    const u8 before = state.entities.entities.data[0].state;
    entities_tick(state.entities, state.world, state.tick);
    const u8 after = state.entities.entities.data[0].state;
    if (before == after)
    {
        core_shutdown(state);
        return 1;
    }

    core_shutdown(state);
    return 0;
}
