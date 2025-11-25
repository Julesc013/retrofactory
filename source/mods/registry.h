#pragma once

#include "mods/manifest.h"

enum
{
    kMaxMods = 64
};

struct ModPrototypeSummary
{
    u32 entities;
    u32 networks;
    u32 recipes;
    u32 research;
    u32 worldgen;
};

struct ModRecord
{
    ModManifest manifest;
    ModPrototypeSummary summary;
};

struct ModRegistry
{
    ModRecord mods[kMaxMods];
    u32 mod_count;
    ModPrototypeSummary totals;
};

void mod_registry_init(ModRegistry &registry);
bool mod_registry_add(ModRegistry &registry, const ModManifest &manifest);
void mod_registry_recalculate(ModRegistry &registry);
