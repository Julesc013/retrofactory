#include "packer.h"

#include <cstdio>

#include "runtime/rt_config.h"
#include "runtime/rt_paths.h"
#include "mods/loader.h"

namespace
{
    bool write_bundle(const ModRecord &rec)
    {
        char out_path[260];
        std::snprintf(out_path, sizeof(out_path), "dist/%s.bundle", rec.manifest.name);
        std::FILE *out = std::fopen(out_path, "wb");
        if (out == 0)
        {
            return false;
        }

        std::fprintf(out, "name=%s\nversion=%s\npriority=%u\npack=%s\n",
                     rec.manifest.name,
                     rec.manifest.version,
                     rec.manifest.priority,
                     rec.manifest.is_pack ? "true" : "false");
        std::fprintf(out, "prototypes entities=%u networks=%u recipes=%u research=%u worldgen=%u\n",
                     rec.summary.entities,
                     rec.summary.networks,
                     rec.summary.recipes,
                     rec.summary.research,
                     rec.summary.worldgen);

        char pack_path[260];
        std::snprintf(pack_path, sizeof(pack_path), "%s/pack.json", rec.manifest.path);
        std::FILE *pack = std::fopen(pack_path, "rb");
        if (pack != 0)
        {
            std::fseek(pack, 0, SEEK_END);
            const long size = std::ftell(pack);
            std::fseek(pack, 0, SEEK_SET);
            if (size > 0 && size < 16 * 1024)
            {
                char *buffer = new char[static_cast<size_t>(size) + 1u];
                const size_t read = std::fread(buffer, 1u, static_cast<size_t>(size), pack);
                buffer[read] = '\0';
                std::fprintf(out, "pack_json=%s\n", buffer);
                delete[] buffer;
            }
            std::fclose(pack);
        }

        std::fclose(out);
        return true;
    }
}

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
    PrototypeStore prototypes;
    prototypes_init(prototypes);
    if (!mod_loader_scan(registry, config) || !mod_loader_resolve(registry) || !mod_loader_apply(registry, prototypes))
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
    std::printf(" Bundled entities: %u, recipes: %u\n",
                prototypes.entities.size,
                prototypes.recipes.size);
    u32 bundled = 0u;
    for (u32 i = 0u; i < registry.mod_count; ++i)
    {
        const ModRecord &rec = registry.mods[i];
        if (rec.manifest.is_pack && write_bundle(rec))
        {
            bundled += 1u;
        }
    }
    std::printf("Bundled %u packs into dist/\n", bundled);
    prototypes_free(prototypes);
    return 0;
}

int main(int argc, char **argv)
{
    return pack_assets(argc, argv);
}
