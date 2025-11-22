#pragma once

#include "mods/registry.h"
#include "runtime/rt_config.h"

bool mod_loader_scan(ModRegistry &registry, const RuntimeConfig &config);
bool mod_loader_resolve(ModRegistry &registry);
bool mod_loader_apply(const ModRegistry &registry);
