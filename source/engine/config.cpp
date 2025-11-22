#include "engine/config.h"

#include "mods/loader.h"
#include "runtime/rt_paths.h"

bool engine_config_init(EngineConfig &config, const char *config_path)
{
    if (!rt_config_load(&config.runtime, config_path))
    {
        return false;
    }
    if (!rt_paths_resolve(config.runtime))
    {
        return false;
    }
    if (!mod_loader_scan(config.mods, config.runtime))
    {
        return false;
    }
    if (!mod_loader_resolve(config.mods))
    {
        return false;
    }
    if (!mod_loader_apply(config.mods))
    {
        return false;
    }
    return true;
}
