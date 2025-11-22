#pragma once

#include "runtime/rt_config.h"
#include "mods/registry.h"
#include "mods/prototypes.h"

struct EngineConfig
{
    RuntimeConfig runtime;
    ModRegistry mods;
    PrototypeStore prototypes;
};

bool engine_config_init(EngineConfig &config, const char *config_path);
