#pragma once

#include "launcher/common/launcher_model.h"
#include "runtime/rt_config.h"

struct LauncherConfig
{
    LauncherModel model;
    RuntimeConfig runtime;
};

bool launcher_config_load(LauncherConfig &config, LauncherEdition edition, const char *config_path);
