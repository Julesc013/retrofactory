#include "mods/registry.h"

void mod_registry_init(ModRegistry &registry)
{
    registry.mod_count = 0u;
}

bool mod_registry_add(ModRegistry &registry, const ModManifest &manifest)
{
    if (registry.mod_count >= kMaxMods)
    {
        return false;
    }
    registry.mods[registry.mod_count] = manifest;
    registry.mod_count += 1u;
    return true;
}
