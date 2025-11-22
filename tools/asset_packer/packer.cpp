#include "packer.h"

#include <cstdio>

#include "runtime/rt_config.h"
#include "runtime/rt_paths.h"
#include "mods/loader.h"

int pack_assets(int argc, char **argv)
{
    RuntimeConfig config;
    const char *config_path = argc > 1 ? argv[1] : 0;
    if (!rt_config_load(&config, config_path) || !rt_paths_resolve(config))
    {
        std::printf("Failed to load runtime config\n");
        return 1;
    }

    ModRegistry registry;
    if (!mod_loader_scan(registry, config) || !mod_loader_resolve(registry) || !mod_loader_apply(registry))
    {
        std::printf("Mod scan/resolve failed\n");
        return 1;
    }

    std::printf("Asset packer loaded %u mods/packs from %s\n", registry.mod_count, config.data_path);
    std::printf("Prototypes: entities=%u networks=%u recipes=%u research=%u worldgen=%u\n",
                registry.totals.entities,
                registry.totals.networks,
                registry.totals.recipes,
                registry.totals.research,
                registry.totals.worldgen);
    return 0;
}

int main(int argc, char **argv)
{
    return pack_assets(argc, argv);
}
