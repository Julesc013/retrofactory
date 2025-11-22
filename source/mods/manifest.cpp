#include "mods/manifest.h"

#include <cstring>

bool mod_manifest_load(const char *path, ModManifest &out_manifest)
{
    /* Stub: derive a simple name from the path. */
    std::memset(out_manifest.name, 0, sizeof(out_manifest.name));
    std::memset(out_manifest.version, 0, sizeof(out_manifest.version));

    if (path == 0)
    {
        return false;
    }

    const char *last_slash = path;
    const char *iter = path;
    while (*iter != 0)
    {
        if (*iter == '/' || *iter == '\\')
        {
            last_slash = iter + 1;
        }
        ++iter;
    }
    std::strncpy(out_manifest.name, last_slash, kModNameMax - 1);
    std::strncpy(out_manifest.version, "0.0.0", kModVersionMax - 1);
    return true;
}

bool mod_manifest_validate(const ModManifest &manifest)
{
    return manifest.name[0] != 0;
}
