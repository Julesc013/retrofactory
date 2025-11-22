#include "mods/depgraph.h"

#include <cstring>

static int compare_mods(const ModRecord &a, const ModRecord &b)
{
    if (a.manifest.priority != b.manifest.priority)
    {
        return a.manifest.priority < b.manifest.priority ? -1 : 1;
    }
    return std::strcmp(a.manifest.name, b.manifest.name);
}

bool depgraph_resolve(const ModRegistry &registry, ModRegistry &resolved)
{
    resolved = registry;
    /* Stable insertion sort for determinism. */
    u32 i;
    for (i = 1u; i < resolved.mod_count; ++i)
    {
        u32 j = i;
        while (j > 0u && compare_mods(resolved.mods[j], resolved.mods[j - 1u]) < 0)
        {
            ModRecord tmp = resolved.mods[j - 1u];
            resolved.mods[j - 1u] = resolved.mods[j];
            resolved.mods[j] = tmp;
            j -= 1u;
        }
    }
    mod_registry_recalculate(resolved);
    return true;
}
