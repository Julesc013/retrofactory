#include "viewer.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "core/core.h"
#include "saveload/saveload.h"
#include "saveload/hash.h"

int save_view_main(int argc, char **argv)
{
    const bool demo_mode = (argc < 2) || (std::strcmp(argv[1], "--demo") == 0);
    const char *path = demo_mode ? 0 : argv[1];

    CoreConfig cfg;
    CoreState state;
    if (!core_init(state, cfg))
    {
        std::printf("Core init failed\n");
        return 1;
    }

    if (!demo_mode && !load_core_state(state, path))
    {
        std::printf("Failed to load save: %s\n", path);
        core_shutdown(state);
        return 1;
    }

    const u64 hash = compute_state_hash(state);
    std::printf("Save summary:\n");
    std::printf(" tick: %u\n", state.tick);
    std::printf(" tiles: %u\n", state.world.tile_count);
    std::printf(" entities: %u\n", state.entities.entities.size);
    if (state.entities.entities.size > 0u)
    {
        const u32 sample = state.entities.entities.size < 3u ? state.entities.entities.size : 3u;
        u32 i;
        for (i = 0u; i < sample; ++i)
        {
            const EntityInstance &inst = state.entities.entities.data[i];
            std::printf("  entity[%u] id=%u proto=%u state=%u at %u,%u\n",
                        i,
                        inst.id,
                        inst.proto_id,
                        inst.state,
                        inst.x,
                        inst.y);
        }
    }
    std::printf(" networks: P=%u F=%u\n", state.networks.power.size, state.networks.fluid.size);
    if (state.networks.power.size > 0u)
    {
        const NetworkNode &n = state.networks.power.data[0];
        std::printf("  power head load=%u/%u\n", n.load, n.capacity);
    }
    std::printf(" research topics: %u active=%u progress=%u\n",
                state.research.topics.size,
                state.research.active,
                state.research.progress);
    std::printf(" hash: %llu\n", static_cast<unsigned long long>(hash));

    core_shutdown(state);
    return 0;
}

int main(int argc, char **argv)
{
    return save_view_main(argc, argv);
}
