#pragma once

#include "system/types.h"

enum
{
    kModNameMax = 64,
    kModVersionMax = 32
};

struct ModManifest
{
    char name[kModNameMax];
    char version[kModVersionMax];
};

bool mod_manifest_load(const char *path, ModManifest &out_manifest);
bool mod_manifest_validate(const ModManifest &manifest);
