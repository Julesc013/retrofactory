#include "mods/registry.h"

#include <cstring>

static void clear_summary(ModPrototypeSummary &summary)
{
    std::memset(&summary, 0, sizeof(ModPrototypeSummary));
}

void mod_registry_init(ModRegistry &registry)
{
    registry.mod_count = 0u;
    clear_summary(registry.totals);
}

bool mod_registry_add(ModRegistry &registry, const ModManifest &manifest)
{
    if (registry.mod_count >= kMaxMods)
    {
        return false;
    }
    ModRecord &record = registry.mods[registry.mod_count];
    record.manifest = manifest;
    clear_summary(record.summary);
    registry.mod_count += 1u;
    return true;
}

void mod_registry_recalculate(ModRegistry &registry)
{
    clear_summary(registry.totals);
    u32 i;
    for (i = 0u; i < registry.mod_count; ++i)
    {
        const ModPrototypeSummary &summary = registry.mods[i].summary;
        registry.totals.entities += summary.entities;
        registry.totals.networks += summary.networks;
        registry.totals.recipes += summary.recipes;
        registry.totals.research += summary.research;
        registry.totals.worldgen += summary.worldgen;
    }
}
