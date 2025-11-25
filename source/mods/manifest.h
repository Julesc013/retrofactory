#pragma once

#include "system/types.h"

enum
{
    kModNameMax = 64,
    kModVersionMax = 32,
    kModPathMax = 260,
    kModMaxDeps = 8
};

struct ModManifest
{
    char name[kModNameMax];
    char version[kModVersionMax];
    char path[kModPathMax];
    u32 priority;
    bool is_pack;
    char dependencies[kModMaxDeps][kModNameMax];
    char conflicts[kModMaxDeps][kModNameMax];
    u32 dependency_count;
    u32 conflict_count;
};

bool mod_manifest_load(const char *path, ModManifest &out_manifest);
bool mod_manifest_validate(const ModManifest &manifest);
