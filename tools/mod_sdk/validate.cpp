#include "validate.h"

#include <cstdio>

#include "runtime/rt_config.h"
#include "runtime/rt_paths.h"
#include "mods/loader.h"

int mod_validate_main(int argc, char **argv)
{
    RuntimeConfig config;
    const char *config_path = argc > 1 ? argv[1] : 0;
    if (!rt_config_load(&config, config_path) || !rt_paths_resolve(config))
    {
        std::printf("Failed to load runtime config\n");
        return 1;
    }

    ModRegistry registry;
    if (!mod_loader_scan(registry, config))
    {
        std::printf("Scan failed for mods\n");
        return 1;
    }
    if (!mod_loader_resolve(registry) || !mod_loader_apply(registry))
    {
        std::printf("Resolve/apply failed\n");
        return 1;
    }

    std::printf("Validated %u mods\n", registry.mod_count);
    u32 i;
    for (i = 0u; i < registry.mod_count; ++i)
    {
        const ModRecord &rec = registry.mods[i];
        std::printf(" - %s %s (E:%u N:%u R:%u Res:%u W:%u)\n",
                    rec.manifest.name,
                    rec.manifest.version,
                    rec.summary.entities,
                    rec.summary.networks,
                    rec.summary.recipes,
                    rec.summary.research,
                    rec.summary.worldgen);
    }
    return 0;
}

int main(int argc, char **argv)
{
    return mod_validate_main(argc, argv);
}
