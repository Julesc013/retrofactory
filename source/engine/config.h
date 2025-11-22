#pragma once

#include "runtime/rt_config.h"
#include "mods/registry.h"

struct EngineConfig
{
    RuntimeConfig runtime;
    ModRegistry mods;
};

bool engine_config_init(EngineConfig &config, const char *config_path);
