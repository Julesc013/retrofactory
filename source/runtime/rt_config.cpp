#include "runtime/rt_config.h"

#include <cstring>

bool rt_config_load(RuntimeConfig *out_config, const char *path)
{
    if (out_config == 0)
    {
        return false;
    }

    /* Default to something safe; ignore file for now. */
    std::memset(out_config, 0, sizeof(RuntimeConfig));

    if (path != 0)
    {
        std::strncpy(out_config->config_path, path, kRfMaxPath - 1);
    }
    std::strncpy(out_config->data_path, "data", kRfMaxPath - 1);
    std::strncpy(out_config->save_path, "saves", kRfMaxPath - 1);
    std::strncpy(out_config->mods_path, "data/mods", kRfMaxPath - 1);
    std::strncpy(out_config->logs_path, "logs", kRfMaxPath - 1);
    return true;
}

bool rt_config_merge(RuntimeConfig *base, const RuntimeConfig &override_config)
{
    if (base == 0)
    {
        return false;
    }

    if (override_config.config_path[0] != 0)
    {
        std::strncpy(base->config_path, override_config.config_path, kRfMaxPath - 1);
    }
    if (override_config.data_path[0] != 0)
    {
        std::strncpy(base->data_path, override_config.data_path, kRfMaxPath - 1);
    }
    if (override_config.save_path[0] != 0)
    {
        std::strncpy(base->save_path, override_config.save_path, kRfMaxPath - 1);
    }
    if (override_config.mods_path[0] != 0)
    {
        std::strncpy(base->mods_path, override_config.mods_path, kRfMaxPath - 1);
    }
    if (override_config.logs_path[0] != 0)
    {
        std::strncpy(base->logs_path, override_config.logs_path, kRfMaxPath - 1);
    }
    return true;
}
