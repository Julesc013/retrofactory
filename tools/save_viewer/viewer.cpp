#include "viewer.h"

#include <cstdio>
#include <cstdlib>

#include "core/core.h"
#include "saveload/saveload.h"
#include "saveload/hash.h"

int save_view_main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::printf("Usage: save_viewer <save_path>\n");
        return 1;
    }

    const char *path = argv[1];

    CoreConfig cfg;
    CoreState state;
    if (!core_init(state, cfg))
    {
        std::printf("Core init failed\n");
        return 1;
    }

    if (!load_core_state(state, path))
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
    std::printf(" networks: P=%u F=%u\n", state.networks.power.size, state.networks.fluid.size);
    std::printf(" hash: %llu\n", static_cast<unsigned long long>(hash));

    core_shutdown(state);
    return 0;
}

int main(int argc, char **argv)
{
    return save_view_main(argc, argv);
}
