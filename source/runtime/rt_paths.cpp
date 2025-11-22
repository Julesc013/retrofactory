#include "runtime/rt_paths.h"

#include <cstring>

bool rt_paths_resolve(RuntimeConfig &config)
{
    /* Ensure required fields have defaults. */
    if (config.data_path[0] == 0)
    {
        std::strncpy(config.data_path, "data", kRfMaxPath - 1);
    }
    if (config.save_path[0] == 0)
    {
        std::strncpy(config.save_path, "saves", kRfMaxPath - 1);
    }
    if (config.mods_path[0] == 0)
    {
        std::strncpy(config.mods_path, "data/mods", kRfMaxPath - 1);
    }
    if (config.logs_path[0] == 0)
    {
        std::strncpy(config.logs_path, "logs", kRfMaxPath - 1);
    }
    return true;
}
