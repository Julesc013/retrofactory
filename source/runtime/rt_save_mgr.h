#pragma once

#include "runtime/rt_config.h"
#include "system/types.h"

struct SaveDescriptor
{
    char path[kRfMaxPath];
    u32 slot;
};

bool rt_save_build_descriptor(const RuntimeConfig &config, u32 slot, SaveDescriptor &out_desc);
