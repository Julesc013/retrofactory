#pragma once

#include "system/types.h"

struct RtProfileScope
{
    const char *name;
    u64 start_ticks;
};

void rt_profile_begin(RtProfileScope &scope, const char *name);
void rt_profile_end(RtProfileScope &scope);
