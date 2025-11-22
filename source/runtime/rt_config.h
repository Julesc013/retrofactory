#pragma once

#include "system/types.h"

/* Fixed-size buffers to avoid heap/STL. */
enum
{
    kRfMaxPath = 260,
    kRfMaxName = 32
};

struct RuntimeConfig
{
    char config_path[kRfMaxPath];
    char data_path[kRfMaxPath];
    char save_path[kRfMaxPath];
    char mods_path[kRfMaxPath];
    char logs_path[kRfMaxPath];
    char edition[kRfMaxName];
    char presenter[kRfMaxName];
    char render_backend[kRfMaxName];
    char audio_backend[kRfMaxName];
};

bool rt_config_load(RuntimeConfig *out_config, const char *path);
bool rt_config_merge(RuntimeConfig *base, const RuntimeConfig &override_config);
