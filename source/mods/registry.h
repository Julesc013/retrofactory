#pragma once

#include "mods/manifest.h"

enum
{
    kMaxMods = 64
};

struct ModRegistry
{
    ModManifest mods[kMaxMods];
    u32 mod_count;
};

void mod_registry_init(ModRegistry &registry);
bool mod_registry_add(ModRegistry &registry, const ModManifest &manifest);
