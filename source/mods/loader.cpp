#include "mods/loader.h"

#include "mods/manifest.h"
#include "mods/depgraph.h"

bool mod_loader_scan(ModRegistry &registry, const RuntimeConfig &config)
{
    mod_registry_init(registry);

    /* Stub: add a single base mod using data path. */
    ModManifest manifest;
    if (!mod_manifest_load(config.data_path, manifest))
    {
        return false;
    }
    return mod_registry_add(registry, manifest);
}

bool mod_loader_resolve(ModRegistry &registry)
{
    ModRegistry resolved;
    if (!depgraph_resolve(registry, resolved))
    {
        return false;
    }
    registry = resolved;
    return true;
}

bool mod_loader_apply(const ModRegistry &/*registry*/)
{
    /* Stub: nothing to load yet. */
    return true;
}
